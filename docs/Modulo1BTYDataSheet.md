# Modulo-1B-TY Data Sheet

Modulo-1B-TY is a development board for the ASR-1B Secure Bluetooth® low energy radio architecture, based on the nRF52832 BLE chipset. The board uses the pre-certified Taiyo Yuden EYSHCNZWZ module plus the Afero Hardware Security Module.

Modulo-1B-TY makes it easy to prototype and build connected products used in conjunction with the Afero IoT Platform. It enables ultra-low power connectivity for data communication. The product integrates a Bluetooth radio, RF front-end, a microcontroller running the Afero custom firmware, security IC, and crystal into a small package. This is an ideal solution for Internet of Things (IoT) applications.

This data sheet contains the following sections:

- [Overview](../Modulo1BTYDataSheet#overview)
- [Pin Definitions](../Modulo1BTYDataSheet#pindefinitions)
- [DC Electrical Specification](../Modulo1BTYDataSheet#dc-electrical-specification)
- [RF Specification](../Modulo1BTYDataSheet#rf-specification)
- [Environmental Specification](../Modulo1BTYDataSheet#environmental-specification)
- [Mechanical Specification](../Modulo1BTYDataSheet#mechanical-specification)
- [Where to Obtain](../Modulo1BTYDataSheet#where-to-obtain)
- [Compliance Notices](../Modulo1BTYDataSheet#compliance-notes)

This data sheet is also available as a [PDF file](../files/Modulo-1B-TYDataSheet.pdf).

## Overview

### Features

- Model name: Afero Modulo-1B Secure BLE Radio Development Board
- Part number: A-FGS-00017-00
- Afero model number: Modulo-1B-TY
- Bluetooth radio: Bluetooth low energy wireless technology (5.0)
- Number of Bluetooth antennas: One (1)
- Antenna configuration: On-module chip antenna
- Maximum transmit power: 4dBm
- Receive sensitivity: -96dBm
- Power consumption: 7.5mA(TX), 5.4mA(RX)
- Host interface: UART, SPI, 3.3V CMOS
- Security features: Cryptographic co-processor with secure, hardware-based key storage
- Interface pins: Four (4) x GPIO
- Power: Supplied via an input pin (VIN: 4.0-5.5V) or via µUSB connector
- Operating temperature: 0 - +85° C
- Storage temperature: -40 to +85° C
- Operating humidity: 10 to 80% RH, non-condensing
- Storage humidity: 5 to 90% RH, non-condensing
- LED indicators: One (1)
- Buttons: One (1)
- Dimensions: 62 x 18 x 12mm (L x W x H)
- Weight (Earth ABL): 7g
- Package: 600 mil DIP form factor
- Compliance: FCC/IC/TELEC 

### Block Diagram

<img src="../img/Mod1BTY-BlockDiagram.png" width="600" style="vertical-align:middle;margin:0px 0px;border:none">

## Pin Definitions

### Pinout

| PIN #  | NAME    | DESCRIPTION                  | PIN #  | NAME  | DESCRIPTION               |
| :----- | :------ | :--------------------------- | :----- | :---- | :------------------------ |
| **1**  | GND     | Ground                       | **19** | IO0   | General Purpose I/O       |
| **2**  | NC      |                              | **20** | IO1   | General Purpose I/O       |
| **3**  | NC      |                              | **21** | IO2   | General Purpose I/O       |
| **4**  | NC      |                              | **22** | IO3   | General Purpose I/O       |
| **5**  | NC      |                              | **23** | SCLK  | SPI Clock                 |
| **6**  | NC      |                              | **24** | INT   | Host Interrupt Active Low |
| **7**  | NC      |                              | **25** | NC    |                           |
| **8**  | NC      |                              | **26** | IO4   | RESERVED                  |
| **9**  | TX      | Host UART Transmit           | **27** | IO5   | RESERVED                  |
| **10** | RX      | Host UART Receive            | **28** | IO6   | RESERVED                  |
| **11** | NC      |                              | **29** | IO7   | RESERVED                  |
| **12** | CS      | SPI Chip Select              | **30** | NC    |                           |
| **13** | MOSI    | SPI Receive                  | **31** | RESET | Reset Active Low          |
| **14** | MISO    | SPI Transmit                 | **32** | NC    |                           |
| **15** | 3V3_OUT | Internal 3.3V LDO Power Rail | **33** | NC    |                           |
| **16** | NC      |                              | **34** | NC    |                           |
| **17** | NC      |                              | **35** | NC    |                           |
| **18** | GND     | Ground                       | **36** | VIN   | External Power Input      |
|        |         |                              |        |       |                           |

### Pinout Diagram

<img src="../img/Mod1BTY-Pinout.png" width="300" style="vertical-align:middle;margin:0px 0px;border:none">

### Configuration Options

| SOLDER BRIDGE # | DESCRIPTION                                                  |
| :-------------- | :----------------------------------------------------------- |
| **SB1**         | LED(D2) is connected to pin IO0 and is active high. LED will turn on when the pin is set high. User can disconnect LED from the pin by cutting the SB1. |
| **SB2**         | User can measure current on 3V3_OUT rail by cutting the SB2 and applying current meter to each end of SB2. |
| **SB3**         | Button(S1) is connected to pin IO3 and is active low. Pressing the button will indicate input value 0. User can disconnect the button from the pin by cutting the SB3. |

<img src="../img/Mod1BTY-SB.png" width="350" style="vertical-align:middle;margin:0px 0px;border:none">

## DC Electrical Specification

### Absolute Maximum Rating

| DESCRIPTION     | MIN  | MAX       | UNIT |
| :-------------- | :--- | :-------- | :--- |
| VIN             | -0.3 | 6.0       | V    |
| 3V3             | -0.3 | VIN + 0.3 | V    |
| I3V3 (3.3V Out) | 200  | 240       | mA   |

### Recommended Operating Conditions

| PARAMETER       | MIN  | MAX  | UNIT |
| :-------------- | :--- | :--- | :--- |
| VIN             | 4.0  | 5.5  | V    |
| 3V3             | 3.27 | 3.33 | V    |
| I3V3 (3.3V Out) |      | 200  | mA   |
|                 |      |      |      |

### Standard Test Conditions

Ambient Conditions: 25°C +/- 5°C, 65%RH +/- 10%

Power Supply: VIN 5V +/- 5%

### Typical Power Consumption

| POWER CONSUMPTION                    | MIN  | TYP  | MAX  | UNIT |
| :----------------------------------- | :--- | :--- | :--- | :--- |
| Sleep (No RF operations)             |      | 20   |      | uA   |
| TX only                              |      |      | 7.5  | mA   |
| RX only                              |      |      | 5.4  | mA   |
| Standby (250ms period advertisement) |      | 60   |      | uA   |

### GPIO DC Characteristics

| SYMBOL    | PARAMETER                 | CONDITIONS | MIN       | TYP  | MAX       | UNIT |
| :-------- | :------------------------ | :--------- | :-------- | :--- | :-------- | :--- |
| VIL       | Low-Level Input Voltage   | All Pins   | 0         |      | 0.3 x 3V3 | V    |
| VIH       | High-Level Input Voltage  | All Pins   | 0.7 x 3V3 |      | 3V3       | V    |
| VOL       | Low-Level Output Voltage  | All Pins   | 0         |      | .04       | V    |
| VOH       | High-Level Output Voltage | All Pins   | 3V3 - 0.4 |      | 3V3       | V    |
| IOL       | Sink Current              | VOL_Max    | 1         | 2    | 4         | mA   |
| IOH       | Source Current            | VOH_Min    | 1         | 2    | 4         | mA   |
| RPULLUP   | Pull-up Resistor          | All Pins   | 11        | 13   | 16        | kΩ   |
| RPULLDOWN | Pull-down Resistor        | All Pins   | 11        | 13   | 16        | kΩ   |

## RF Specification

Conditions: 25°C, VIN=5V, Parameters measured at RF connector.

### Bluetooth® Low Energy (BLE) Characteristics

| ITEMS               | MIN                   | TYP  | MAX    |
| :------------------ | :-------------------- | :--- | :----- |
| Specification       | Bluetooth® 5.0        |      |        |
| Mode                | Bluetooth® low energy |      |        |
| Frequency Range     | 2402 - 2480 MHz       |      |        |
| TX CHARACTERISTICS  |                       |      |        |
| Max Output Power    |                       | 4dBm | 6dBm   |
| RX CHARACTERISTICS  |                       |      |        |
| Receive Sensitivity |                       |      | -96dBm |

## Environmental Specification

### Absolute Maximum Rating

| DESCRIPTION           | MIN  | MAX  | UNIT |
| :-------------------- | :--- | :--- | :--- |
| Storage Temperature   | -40  | 85   | °C   |
| Storage Humidity      | 5    | 90   | %RH  |
| Operating Temperature | 0    | 85   | °C   |
| Operating Humidity    | 10   | 80   | %RH  |

### Recommended Operating Conditions

| PARAMETER                   | MIN  | MAX  | UNIT |
| :-------------------------- | :--- | :--- | :--- |
| Operating Temperature Range | 0    | 70   | °C   |

## Mechanical Specification

### Mechanical Drawing

<img src="../img/Mod1BTY-MechDrawing.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

### Antenna Keep-Out

To preserve antenna performance, keep materials away from the antenna element. The dimensioned box shows an example antenna keep-out region. Infringing on this keep-out should be done with direction from an RF engineer to best retain RF performance.

<img src="../img/Mod1BTY-AntennaKeepout.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

### Mechanical Characteristics

| ITEM       | TEST CONDITIONS                                 |
| :--------- | :---------------------------------------------- |
| Assembly   | No defects of wiring, soldering, and assembling |
| Appearance | No dirt, rust, corrosion, or foreign material   |
| Mass       | 7g +/- 1g                                       |
|            |                                                 |

## Where to Obtain

### Design Files

Complete Modulo-1B-TY design files are available at [this GitHub link](https://github.com/aferodeveloper/Modulo-1B-TY). The design files are open-sourced under the Apache 2.0 license.

### Ordering Information

| MODULE       | PART NUMBER    | NOTES                 |
| :----------- | :------------- | :-------------------- |
| Modulo-1B-TY | A-FGS-00017-00 | Header pins installed |

## Compliance Notices

### FCC

Contains FCC ID: RYYEYSHCN

This device complies with Part 15 of the FCC Rules. Operation is subject to the following two conditions:

1. This device may not cause harmful interference.
2. This device must accept any interference received, including interference that may cause undesired operation.

### IC(ISED) Canada

Contains IC: 4389B-EYSHCN

This device complies with Industry Canada’s licence-exempt RSS standard(s). Operation is subject to the following two conditions: (1) This device may not cause interference; and (2) This device must accept any interference, including interference that may cause undesired operation of the device.

Le présent appareil est conforme aux CNR d’Industrie Canada applicables aux appareils radio exempts de licence. L’exploitation est autorisée aux deux conditions suivantes : (1) l’appareil ne doit pas produire de brouillage, et (2) l’utilisateur de l’appareil doit accepter tout brouillage radioélectrique subi, même si le brouillage est susceptible d’en compromettre le fonctionnement.

### MIC Japan

Contains EYSHCN: 001-A10745

This product installs a radio system which has been approved as a radio station in a low power data communication system based on the Radio Law.

 *Next:*&nbsp;&nbsp;[**&#8674;** Modulo-2 Data Sheet](../Modulo2DataSheet)