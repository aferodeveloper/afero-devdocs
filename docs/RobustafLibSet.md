# Robust af_lib_set_attribute*() Calls

All forms of `af_lib_set_attribute()` provide a return value to indicate whether the set request has been successfully enqueued. Checking this return value and re-trying can make your use of `af_lib_set_attribute*()` more robust. Here’s a code snippet from the afBlink example sketch that demonstrates some variations of this:

```
...SNIP...
bool rebootPending = false;		// True if reboot needed, e.g. if we received an OTA firmware
...SNIP...

// Callback executed any time ASR has information for the MCU
void attrEventCallback(const af_lib_event_type_t eventType,
            const af_lib_error_t error,
            const uint16_t attributeId,
            const uint16_t valueLen,
            const uint8_t* value) {

  switch (eventType) {
    case AF_LIB_EVENT_UNKNOWN:
      break;

...SNIP...

    case AF_LIB_EVENT_ASR_NOTIFICATION:
      // Unsolicited notification of non-MCU attribute change
      switch (attributeId) {

// EXAMPLE 1:
        case AF_MODULO_BUTTON:
          // curButtonValue is checked in loop(). If changed, will toggle blinking state.
          curButtonValue = *(uint16_t*) value;
          break;

        case AF_SYSTEM_ASR_STATE:
          switch (value[0]) {
            case AF_MODULE_STATE_REBOOTED:
              break;

            case AF_MODULE_STATE_LINKED:
              break;

            case AF_MODULE_STATE_UPDATING:
              break;

EXAMPLE 2:
            case AF_MODULE_STATE_UPDATE_READY:
              // rebootPending is checked in loop(). If true, will send reboot command.
              rebootPending = true;
              break;

            case AF_MODULE_STATE_INITIALIZED:
              break;

            default:
              break;
          }
          break;

        default:
          break;
      }
      break;

...SNIP...
  }
}

...SNIP...

void setModuloLED(bool on) {
  if (moduloLEDIsOn != on) {
    int16_t attrVal = on ? LED_ON : LED_OFF; // Modulo LED is active low

// EXAMPLE 3:
    int timeout = 0;
    while (af_lib_set_attribute_16(af_lib, AF_MODULO_LED, attrVal, AF_LIB_SET_REASON_LOCAL_CHANGE) != AF_SUCCESS) {
      delay(10);
      af_lib_loop(af_lib);
      timeout++;
      if (timeout > 500) {
        // If we haven't been successful after 5 sec (500 tries, each after 10 msec delay) 
        // we assume we're in some desperate state, and reboot
        pinMode(RESET, OUTPUT);
        digitalWrite(RESET, 0);
        delay(250);
        digitalWrite(RESET, 1);
        return;
      }
    }

    moduloLEDIsOn = on;
  }
}

void loop() {
  // If we were asked to reboot (e.g. after an OTA firmware update), make the call here in loop().
  // In order to make this fault-tolerant, we'll continue to retry if the command fails.
  if (rebootPending) {
    int retVal = af_lib_set_attribute_32(af_lib, AF_SYSTEM_COMMAND, AF_MODULE_COMMAND_REBOOT, AF_LIB_SET_REASON_LOCAL_CHANGE);
    rebootPending = (retVal != AF_SUCCESS);
  }

  // Modulo button toggles 'blinking'
  if (prevButtonValue != curButtonValue) {
    if (af_lib_set_attribute_bool(af_lib, AF_BLINK, !blinking, AF_LIB_SET_REASON_LOCAL_CHANGE) == AF_SUCCESS) {
      blinking = !blinking;
      prevButtonValue = curButtonValue;
    }
  }

  // Give the afLib state machine some time.
  af_lib_loop(af_lib);
}
```

**Notes:**

- In all three examples, we check the return value from `af_lib_set_attribute()`, and retry if the call was not successful. Doing this is essential to robust afLib usage.

- While retrying unsuccessful afLib calls, it is important to call `af_lib_loop()` to ensure that afLib gets time to handle requests. If, for example, we called `set_attribute()` and the call failed due to a full request queue, retrying the call would be pointless unless afLib got time to service the queue.

- It’s important to avoid calling `af_lib_loop()` within `attrEventCallback()`; doing so can have unexpected results. But we know that to make afLib calls robust we should confirm-and-retry, and we know that we must call `af_lib_loop()` *while* we retry. So if the callback tells us we need to call set_attribute, how do we do that robustly?<br><br>
Examples #1 and #2 demonstrate a useful pattern: Code in the callback is restricted to setting a flag to indicate a `af_lib_set_attribute()` call is required. Then in the main `loop()`, the flag is checked, `set_attribute()` is called if indicated, and retried as needed until success. This pattern is robust, easy to read, and can reduce redundant code.

- Example #3 shows a variation in which retrying is limited by a timeout: as above, the code retries `af_lib_set_attribute()`, waiting for AF_SUCCESS. But here a timeout prevents an indefinite cycle of re-trying in the face of some serious condition that is blocking us. If the timeout is exceeded, we assume that communication with afLib is fatally obstructed, so we trigger a reboot by directly manipulating the reset pin.

 **&#8674;** *Next:* [Useful Debugging Methods](../DebugMethods)