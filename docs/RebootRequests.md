# Handling Reboot Requests

Your MCU code will use the `attrEventCallback()` callback to take action when attribute values change. Your primary concern will be changes to attributes you define, but your code must also watch for changes to the system attribute AF_SYSTEM_ASR_STATE. This attribute can have one of several values, including:

- Rebooted
- Linked
- Updating
- Update Ready
- Initialized
- Relinked
- Factory Reset

AF_SYSTEM_ASR_STATE will change to “Updating” whenever ASR receives an Over-the-Air (OTA) firmware or profile update. When the OTA is complete, ASR will change AF_SYSTEM_ASR_STATE to “Update Ready”, and your MCU code is responsible for recognizing this case and triggering a reboot.

Your code should initiate a controlled (“soft”) reboot by calling `af_lib_set_attribute()` for the attribute AF_SYSTEM_COMMAND, to the value AF_MODULE_COMMAND_REBOOT. Here’s a snippet:

```
bool initializationPending = true;  // If true, we're waiting on AF_MODULE_STATE_INITIALIZED
bool rebootPending = false;          // If true, a reboot is needed (e.g. if we received an OTA firmware update.)

// SNIP //

void attrEventCallback(const af_lib_event_type_t eventType,
                       const af_lib_error_t error,
                       const uint16_t attributeId,
                       const uint16_t valueLen,
                       const uint8_t* value) {

  switch (attributeId) {
// SNIP //

    case AF_SYSTEM_ASR_STATE:
      switch (value[0]) {

    case AF_MODULE_STATE_REBOOTED:
      // ASR has been rebooted
      initializationPending = true;
      break;

    case AF_MODULE_STATE_LINKED:
      break;

    case AF_MODULE_STATE_UPDATING:
      break;

    case AF_MODULE_STATE_UPDATE_READY:
      // ASR signals that an update has been received and a reboot is required to use it
      rebootPending = true;
      break;

    case AF_MODULE_STATE_INITIALIZED:
      // ASR signals that it's ready
      initializationPending = false;
      break;

    case AF_MODULE_STATE_RELINKED:
      break;

    case AF_MODULE_STATE_FACTORY_RESET:
      // ASR signals that is has been factory reset; MCU should handle any local housekeeping and then reboot ASR
      rebootPending = true;
      break;

    default:
      break;
    }
    break;

  default:
    break;
  }
}

void loop() {
  af_lib_loop(af_lib);    // Give the afLib state machine some time.

  if (initializationPending) {
    // If we're awaiting initialization, don't bother checking/setting attributes
  } else {
    
    if (rebootPending) {  // Someone wants us to reboot
      int retVal = af_lib_set_attribute_32(af_lib, AF_SYSTEM_COMMAND, AF_MODULE_COMMAND_REBOOT, AF_LIB_SET_REASON_LOCAL_CHANGE);
      rebootPending = (retVal != AF_SUCCESS);

      // Check for success; if not successful, we'll re-try next time around loop()
      if (!rebootPending) {
        // Reboot command sent; now awaiting AF_MODULE_STATE_INITIALIZED
        initializationPending = true;
      }

    }

  // Your application code here //
  
  }
}
```

Our `attrEventCallback()` checks the supplied attribute ID, looking for AF_SYSTEM_ASR_STATE. If we have received an update message for that attribute ID, we check the attribute value. If the value is 3 (AF_MODULE_STATE_UPDATE_READY), then we trigger a reboot by setting a global, `rebootPending`. In our main `loop()` function, any time `rebootPending` is true, we’ll call `af_lib_set_attribute_32()` for the AF_SYSTEM_COMMAND attribute, with value 1 (which is the value that signals a reboot). Our code resets `rebootPending` to false if it succeeds; if the `set_attribute()` call fails (for instance if the request queue is full), we’ll try again next time around the `loop()`.

 *Next:*&nbsp;&nbsp;[**&#8674;** Robust af_lib_set_attribute*() Calls](../RobustafLibSet)