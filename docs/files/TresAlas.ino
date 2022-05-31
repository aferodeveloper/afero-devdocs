#include <afErrors.h>
#include <afLib.h>
#include <afSPI.h>
#include <arduinoSPI.h>
#include <Command.h>
#include <iafLib.h>
#include <msg_types.h>
#include <StatusCommand.h>

#include <EEPROM.h>

#include <elapsedMillis.h>
elapsedMillis timeElapsed;        // control periodic reading of meter group

// ===============================================================================================
// Afero stuff

#include "TresAlas/device-description.h"

#define DEBUG_PRINT_BUFFER_LEN    256

#define INT_PIN                   14    // Modulo uses this to initiate communication
#define CS_PIN                    15    // ### Honeycomb has the SPI CS (SS) on pin 15, rather than the usual 10
#define RESET                     21    // This is used to reboot the Modulo when the Teensy boots

iafLib *aflib;


// ===============================================================================================
/*
 * Afero Honeycomb reference design pin assignments
 * 
 * Honeycomb is a single PCB containing:
 * 
 *   - An Afero Modulo dev board (any Modulo)
 *   - A Teensy 3.2 Arduino-compatible MCU board
 *   - A DE9 pinned as DTE, with true RS232 level signals and all handshake lines supported
 *   - A 3-conductor terminal block for RS485 communications
 *   
 * Arduino code is written to interface with the RS232 and RS485 ports often found on legacy
 * equipment.
 * 
 * That code then maps equipment-specific functions and features onto Afero Attributes, which
 * are then synchronized with the Afero Cloud.
 * 
 * 
 */

#define BTN_IN               2
#define LED_OUT              3
#define SER1_DTR             4
#define SER1_DSR             5
#define RS485_TX_EN          6
#define SER1_CTS             7    // also SER3_RX
#define SER1_RTS             8    // also SER3_TX

#define SPI_MOSI            11
#define SPI_MISO            12
#define SPI_SCK             13
#define SPI_INT             14
#define SPI_SS              15    // NOTE: different from usual position of pin 10

#define SER1_RI             16
#define SER1_CD             17    // can be strapped for power in on this pin

#define SPI_RESET           21    // reset Afero radio

#define SER1_ENABLE         22    // 0 = disable RS232 drivers
#define SER1_RX_DETECT      23    // 1 = some RS232 level signal(s) detected on DE9 connector


#define RS485_PORT          Serial2

#define RS232_PORT          Serial1
#define RS232_AUX           Serial3   // optionally this can be used instead of RTS/CTS


void setLED( bool state ) {
  if( state ) {
    pinMode( LED_OUT, OUTPUT );
    digitalWrite( LED_OUT, 0 );
  }
  else {
    pinMode( LED_OUT, INPUT );
  }
}

void set232_DTR( bool state ) {
  digitalWrite( SER1_DTR, state ? 0:1 );
}

void set232_RTS( bool state ) {
  digitalWrite( SER1_RTS, state ? 0:1 );
}


bool get232_RXActive() {
  return digitalRead( SER1_RX_DETECT );
}

// ===============================================================================================
// RS-485 bus driver direction helper functions

void pre485Tx()
{
  digitalWrite( RS485_TX_EN, 1 );
}

void post485Tx()
{
  digitalWrite( RS485_TX_EN, 0 );
}


void write_485( char *buf, int len ) {
  pre485Tx();

  for( int xxx = 0; xxx != len; xxx++ ) {
    RS485_PORT.write( buf[xxx] );
    //Serial.println( buf[xxx], HEX );
  }
  
  RS485_PORT.flush();
  post485Tx();
}



// ===============================================================================================
// Fuji Electric FD5000 Meter Communication Routines

#define STX     0x02
#define ETX     0x03
#define EOT     0x04
#define ENQ     0x05
#define ACK     0x06

char meter_select[5] =  { 0x05, '0', '0', 0x0d, 0x0a };     // fill in desired meter address
char meter_release[3] = { 0x04,           0x0d, 0x0a };

// 0105 is a precalculated checksum for the MES command
char meter_mes_cmd[] = { 0x02, 'M', 'E', 'S', 0x03, 0x38, 0x45, 0x0d, 0x0a };

#define METER_RX_MAX      512               // RS485 RX buffer

char meter_rx_buf[METER_RX_MAX];
int meter_rx_idx;


bool wait_lf_timeout() {
  int timeout = 5000;                       // ~5 sec
  char c;

  meter_rx_idx = 0;                         // reset rx buf index
  
  while( timeout ) {
    
    while( RS485_PORT.available() && (meter_rx_idx < METER_RX_MAX) ) {
      c = RS485_PORT.read();
      c &= 0x7F;                            // only 7 bits valid
      //Serial.println( c, HEX );
      meter_rx_buf[meter_rx_idx++] = c;
      if( c == 0x0a ) {                     // LF?
        meter_rx_buf[meter_rx_idx] = 0;
        return true;
      }
    }

    delay( 1 );
    timeout--;
  }

  return false;
}

bool fuji_select( int num ) {
  meter_select[2] = num+0x30;               // convert num -> ASCII
  write_485( meter_select, 5 );

  return wait_lf_timeout();
}


bool fuji_read( int num ) {

  //Serial.println("=== meter read start");
  
  if( fuji_select( num ) ) {                // try to select our meter
    //Serial.println("--- meter selected");
    
    write_485( meter_mes_cmd, 9 );          // send it a MES command (measurement)

    if( wait_lf_timeout() ) {               // did we get a response?
      int xxx = 0;
      char c;
      
      do {                                  // parse it
        c = meter_rx_buf[xxx++];
        if( (c == 0x03) || (c == 0x00) )
          break;
        //else
        //  Serial.println( c, HEX );
      }
      while( xxx != METER_RX_MAX );
    }
    else
      Serial.println("### Err, did not hear from meter");

    write_485( meter_release, 3 );

    //Serial.println("*** meter read complete");
    return true;
  }
  
  Serial.println("### meter read error");
  return false;
}


// ===============================================================================================
// Meter data variables
/* 
 * Tres Alas meters are as follows:
 * 
 * 1: Leg 1 VOLTS         integer
 * 2: Leg 2 VOLTS         integer
 * 3: Leg 1 AMPS          integer
 * 4: Leg 2 AMPS          integer
 * 5: Line Frequency HZ   xx.y (4 char + null term string)
 * 
 */

#define DEFAULT_POLLRATE          5     // 5 second default polling rate

uint8_t pollrate;

uint8_t leg1_volts;
uint8_t leg2_volts;
uint8_t leg1_amps;
uint8_t leg2_amps;
char line_freq[5];


// Data from the meter is a pain to parse. 
// It's a fixed length string with 3 leading spaces.
// The first char can be thousands, hundreds, tens, or ones: the number is LEFT JUSTIFIED.
// So we have to look for the first space AFTER each valid (ASCII) digit to figure out 
//  how many digits this number is.
// We assume only up to 3 digits valid for this application. Volts will be 3, amps will be 1 or 2.

int parse_int_response() {
  int r = 0;

  r = (meter_rx_buf[4]-0x30);         // this one will always be there, convert ASCII -> num

  if( meter_rx_buf[5] != 0x20 )       // 1 digit only?
  {
    r *= 10;                          // assume 2 digit
    r += (meter_rx_buf[5]-0x30);
    if( meter_rx_buf[6] != 0x20 )     // 2 digits only?
    {
      r *= 10;
      r += (meter_rx_buf[6]-0x30);    // it's 3 digits
    }
  }

  return r;
}

// ===============================================================================================
// EEPROM save/restore routines

void save_pollrate() {  
  EEPROM.put( 0, 0xbeefbeef );              // valid signature
  EEPROM.put( 4, pollrate );
  Serial.print("Saved pollrate ");
  Serial.print( pollrate );
  Serial.println(" seconds in EEPROM.");
}

void reload_pollrate() {    
  unsigned int r;

  EEPROM.get( 0, r );
  
  if ( r == 0xbeefbeef ) {
    EEPROM.get( 4, pollrate );
    Serial.println("Found pollrate in EEPROM: ");
    Serial.println( pollrate );
  }
  else {
    Serial.println("*** Did not find pollrate in EEPROM, reset");
    pollrate = DEFAULT_POLLRATE;
    save_pollrate();
  }
}


// ===============================================================================================
// setup/loop

void setup() {

  // ------------------------------------------
  // initialize hardware
    
  pinMode( RS485_TX_EN, OUTPUT );
  post485Tx();                          // ensure we're in Rx mode

  pinMode( BTN_IN, INPUT_PULLUP );
  
  setLED( false ) ;
  
  pinMode( SER1_DTR, OUTPUT );
  set232_DTR( true );
  
  pinMode( SER1_RTS, OUTPUT );
  set232_RTS( true );

  pinMode( SER1_CTS, INPUT );
  pinMode( SER1_DSR, INPUT );
  pinMode( SER1_RI, INPUT );
  pinMode( SER1_CD, INPUT );

  pinMode( SER1_RX_DETECT, INPUT );
  
  pinMode( SER1_ENABLE, OUTPUT );
  digitalWrite( SER1_ENABLE, 1 );      // enable RS232 drivers
  
  Serial.begin( 115200 );
  delay( 2000 );

  RS232_PORT.begin( 115200 );

  RS485_PORT.begin( 9600, SERIAL_7E1 );

  Serial.println("Hello from Honeycomb");


  // ------------------------------------------
  // Afero setup
  
  Serial.println("Resetting Modulo");
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, 0);
  delay(250);
  digitalWrite(RESET, 1);

  ArduinoSPI *arduinoSPI = new ArduinoSPI( CS_PIN );

  aflib = iafLib::create( digitalPinToInterrupt(INT_PIN), ISRWrapper, attrSetHandler, attrNotifyHandler, &Serial, arduinoSPI );

  // ------------------------------------------
  // initialize attributes
  
  reload_pollrate();
  aflib->setAttribute8( AF_POLLRATE, pollrate );

  timeElapsed = ((pollrate-3) * 1000);      // force initial read, 3 second delay to let everything settle
}



void loop() {
  uint8_t t1, t2, t3, t4;         // temps so we can read all meters in a burst
  char t5[5];
  
  if( timeElapsed >= (pollrate * 1000) ) {

    setLED( true );
    
    Serial.println("time to read meter group");
    timeElapsed = 0;

    // Burst read meters

    fuji_read( 1 );
    t1 = parse_int_response();
    delay( 10 );

    fuji_read( 2 );
    t2 = parse_int_response();
    delay( 10 );
  
    fuji_read( 3 );
    t3 = parse_int_response();
    delay( 10 );

    fuji_read( 4 );
    t4 = parse_int_response();
    delay( 10 );      
  
    fuji_read( 5 );
    t5[0] = meter_rx_buf[4];
    t5[1] = meter_rx_buf[5];
    t5[2] = meter_rx_buf[6];
    t5[3] = meter_rx_buf[7];
    t5[4] = 0;  


    // Print readings
    
    Serial.print("LEG 1: ");
    Serial.print( t1 );
    Serial.println(" VOLTS");

    Serial.print("LEG 2: ");
    Serial.print( t2 );
    Serial.println(" VOLTS");

    Serial.print("LEG 1: ");
    Serial.print( t3 );
    Serial.println(" AMPS");

    Serial.print("LEG 2: ");
    Serial.print( t4 );
    Serial.println(" AMPS");

    Serial.print("LINE FREQ: ");
    Serial.print( t5 );
    Serial.println(" HZ");
    
    
    // Update and report if values changed
    
    if( t1 != leg1_volts ) {
      leg1_volts = t1;
      Serial.println("Leg 1 Volts changed, reporting");
      aflib->setAttribute8( AF_LEG1VOLTS, leg1_volts );
    }

    if( t2 != leg2_volts ) {
      leg2_volts = t2;
      Serial.println("Leg 2 Volts changed, reporting");
      aflib->setAttribute8( AF_LEG2VOLTS, leg2_volts );
    }
    
    if( t3 != leg1_amps ) {
      leg1_amps = t3;
      Serial.println("Leg 1 Amps changed, reporting");
      aflib->setAttribute8( AF_LEG1AMPS, leg1_amps );
    }    

    if( t4 != leg2_amps ) {
      leg2_amps = t4;
      Serial.println("Leg 2 Amps changed, reporting");
      aflib->setAttribute8( AF_LEG2AMPS, leg2_amps );
    }    

    if( strcmp( t5, line_freq ) ) {
      strncpy( line_freq, t5, 5 );
      Serial.println("Line Frequency changed, reporting");
      aflib->setAttributeStr( AF_LINEFREQ, line_freq );
    }   
    
    setLED( false );
  }

  aflib->loop();
}


// ===============================================================================================
// Afero handlers

// -----------------------------------------------------------------------------------------------
// This is called when the service changes one of our attributes.

bool attrSetHandler(const uint8_t requestId, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    printAttribute("attrSetHandler", attributeId, valueLen, value);

    switch (attributeId) {
        // This MCU attribute tells us the number of seconds between polling the meter group
        case  AF_POLLRATE:
              pollrate = *value;
              Serial.print("Pollrate updated to " );
              Serial.print( pollrate );
              Serial.println(" seconds");
              save_pollrate();
              break;

        default:
              break;
    }

    // Return false here if your hardware was unable to perform the set request from the service.
    // This lets the service know that the value did not change.
    return true;
}


// -----------------------------------------------------------------------------------------------
// This is called when either an Afero attribute has been changed via setAttribute or in response
// to a getAttribute call.

void attrNotifyHandler(const uint8_t requestId, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    printAttribute("attrNotifyHandler", attributeId, valueLen, value);

    switch (attributeId) {

      /*
        // Update the state of the LED based on the actual attribute value.
        case AF_MODULO_LED:
            moduloLEDIsOn = (*value == 0);
            break;

            // Allow the button on Modulo to control our blinking state.
        case AF_MODULO_BUTTON: {
            uint16_t *buttonValue = (uint16_t *) value;
            if (moduleButtonValue != *buttonValue) {
                moduleButtonValue = *buttonValue;
                blinking = !blinking;
                if (aflib->setAttributeBool(AF_BLINK, blinking) != afSUCCESS) {
                    Serial.println("Could not set BLINK");
                }
            }
        }
            break;
      */

        default:
            break;
    }
}



// Define this wrapper to allow the instance method to be called
// when the interrupt fires. We do this because attachInterrupt
// requires a method with no parameters and the instance method
// has an invisible parameter (this).
void ISRWrapper() {
    if (aflib) {
        aflib->mcuISR();
    }
}



/****************************************************************************************************
 * Debug Functions                                                                                  *
 *                                                                                                  *
 * Some helper functions to make debugging a little easier...                                       *
 ****************************************************************************************************/
char *getPrintAttrHeader(const char *sourceLabel, const char *attrLabel, const uint16_t attributeId, const uint16_t valueLen) {
    char intStr[6];
    char *buffer = new char[DEBUG_PRINT_BUFFER_LEN];
    memset(buffer, 0, DEBUG_PRINT_BUFFER_LEN);

    sprintf(buffer, "%s id: %s len: %s value: ", sourceLabel, attrLabel, itoa(valueLen, intStr, 10));
    return buffer;
}

void printAttrBool(const char *sourceLabel, const char *attrLabel, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    char *buffer = getPrintAttrHeader(sourceLabel, attrLabel, attributeId, valueLen);
    strcat(buffer, *value == 1 ? "true" : "false");
    Serial.println(buffer);
    delete(buffer);
}

void printAttr8(const char *sourceLabel, const char *attrLabel, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    char *buffer = getPrintAttrHeader(sourceLabel, attrLabel, attributeId, valueLen);
    char intStr[6];
    strcat(buffer, itoa(*((uint8_t *)value), intStr, 10));
    Serial.println(buffer);
    delete(buffer);
}

void printAttr16(const char *sourceLabel, const char *attrLabel, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    char *buffer = getPrintAttrHeader(sourceLabel, attrLabel, attributeId, valueLen);
    char intStr[6];
    strcat(buffer, itoa(*((uint16_t *)value), intStr, 10));
    Serial.println(buffer);
    delete(buffer);
}

void printAttrHex(const char *sourceLabel, const char *attrLabel, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    char *buffer = getPrintAttrHeader(sourceLabel, attrLabel, attributeId, valueLen);
    for (int i = 0; i < valueLen; i++) {
        strcat(buffer, String(value[i], HEX).c_str());
    }
    Serial.println(buffer);
    delete(buffer);
}

void printAttrStr(const char *sourceLabel, const char *attrLabel, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    char *buffer = getPrintAttrHeader(sourceLabel, attrLabel, attributeId, valueLen);
    int len = strlen(buffer);
    for (int i = 0; i < valueLen; i++) {
        buffer[len + i] = (char)value[i];
    }
    Serial.println(buffer);
    delete(buffer);
}

void printAttribute(const char *label, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    switch (attributeId) {

        case AF_POLLRATE:
            printAttr8(label, "AF_POLLRATE", attributeId, valueLen, value);
            break;

            
        case AF_LEG1VOLTS:
            printAttr8(label, "AF_LEG1VOLTS", attributeId, valueLen, value);
            break;
    
        case AF_LEG1AMPS:
            printAttr8(label, "AF_LEG1AMPS", attributeId, valueLen, value);
            break;
            
            
        case AF_LEG2VOLTS:
            printAttr8(label, "AF_LEG2VOLTS", attributeId, valueLen, value);
            break;
    
        case AF_LEG2AMPS:
            printAttr8(label, "AF_LEG2AMPS", attributeId, valueLen, value);
            break;


        case AF_LINEFREQ:
            printAttrStr(label, "AF_LINEFREQ", attributeId, valueLen, value);
            break;

                        
        case AF_BOOTLOADER_VERSION:
            printAttrHex(label, "AF_BOOTLOADER_VERSION", attributeId, valueLen, value);
            break;

        case AF_SOFTDEVICE_VERSION:
            printAttrHex(label, "AF_SOFTDEVICE_VERSION", attributeId, valueLen, value);
            break;

        case AF_APPLICATION_VERSION:
            printAttrHex(label, "AF_APPLICATION_VERSION", attributeId, valueLen, value);
            break;

        case AF_PROFILE_VERSION:
            printAttrHex(label, "AF_PROFILE_VERSION", attributeId, valueLen, value);
            break;

      /*
        case AF_SYSTEM_REBOOT_REASON:
            printAttrStr(label, "AF_REBOOT_REASON", attributeId, valueLen, value);
            break;
       */

       default:
            printAttrHex(label, "unknown", attributeId, valueLen, value);
            break;

    }
}


