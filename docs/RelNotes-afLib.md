# afLib Release Notes

Download the latest afLib library from here: http://github.com/aferodeveloper/afLib.

## Afero afLib4 R1.0

Release date: October 14, 2019

- Updated and simplified example apps, including addition of `full_api_example` app, which documents every event afLib generates and how to handle each event.
- Updated handling of device Profile default values. AF_LIB_EVENT_MCU_DEFAULT_NOTIFICATION is now purely informational, and the new AF_LIB_EVENT_MCU_GET_REQUEST event will request that MCU send current value – including default value if desired – of each MCU attribute. Read more in [Callbacks and Events](../afLibCallbacks).
- Added **`reason`** parameter to `af_lib_set_attribute()`function to indicate the reason the MCU is setting the attribute at the time. Read more about this function in [Getting/Setting Attributes](../afLibAttributes#Func-setAttribute).
- Added Support for Plumo-2D devices.
- Platforms now supported include Arduino, Linux, and STM32.

## Afero afLib3 R1.2

Release date: July 17, 2019

- Restored afLib3 support for Modulo-1. afLib3 hadn’t been talking to a properly-connected ASR.
- Renamed example profiles so “device name + device type” are a unique combination. The Profile Editor complains if they’re not.
- Profiles for Modulo-1B had LED on GPIO0 as Active Low when it’s Active High.
- Re-added missing LICENSE.txt file.
- Re-added `arduino_transport_create_spi(CS_PIN)` API, inadvertently removed when adding `arduino_transport_create_spi(CS_PIN,frame_length).`
- 

## Afero afLib3 R1.1

Release date: January 7, 2019

- API change to `af_lib_asr_has_capability()` and `af_lib_send_set_response()` for consistency.
- Added ASR_STATE_RELINKED to indicate when ASR drops offline and reconnects.
- Simplified afBlink example app.
- Fixed issue in example apps that caused Modulo-1 to not work properly.
- Updated example apps for new “MCU update all” behavior in Firmware R2.1.1.
- Removed reference to undefined RESET pin when using Arduino Uno.
- Created example app Profiles for Modulo-1B.

## Afero afLib3 R1.0

Release date: August 9, 2018

***General changes:\***

- Atmel SAMD21 support (UART only).
- Linux support for connectivity to ASR-1 and ASR-2 via UART.

***When used in conjunction with Firmware R2.0 and later:\***

- New, more logical API.

- Better handling of MCU attributes with default values. These are now differentiated in the callback when they’re sent to the MCU.

- More granular event-handling in callback to simplify usage.

- MCU OTA support. You can upload and deploy new firmware for your connected MCU via the Afero OTA Manager. Read more in [Handling MCU OTA Updates](../MCU-OTA).

- New system attributes:

- - AF_ONLINE_STATUS will tell you if ASR is connected to the Afero Cloud.
  - AF_DEVICE_ID enables retrieval of the device ID using `af_lib_get_attribute()`.
  - AF_ASSOCIATION_ID enables retrieval of the association ID using `af_lib_get_attribute()`.

- New Modulo states:

- - AF_MODULE_STATE_INITIALIZED indicates initialization is complete, and it’s safe for MCU to `call af_lib_set_attribute*()`.
  - AF_MODULE_STATE_RELINKED can be used to differentiate a link state at reboot from a link state after the device may have dropped offline.

- Added function `af_lib_asr_has_capability()` to allow the MCU to query the ASR firmware for supported features.

## Afero afLib R1.3 and afLib2 R2.0

Release date: April 2, 2018

### afLib R1.3

- Deprecation of C++ afLib.
- Added defines around debug prints in afBlink example so we don’t run out of memory on Arduino Uno.

### afLib3 R2.0

- Initial public release of C afLib, called afLib2.
- Updated all example apps to use afLib2.