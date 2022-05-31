# Firmware Release Notes

## Afero Firmware R2.1.2

Release date: March 11, 2019

Version number: 30503 for Modulo-1B

- Assorted bug fixes.

## Afero Firmware R2.1.1

Release date: December 21, 2018

Version numbers: 30362 for Modulo-1B

- Support for nRF52832, the Nordic multiprotocol Bluetooth low energy, ANT/ANT+ and 2.4GHz proprietary System-on-Chip.
- Support for attribute timestamp flag.
- Support for queued attributes, a subset of attributes that maintain a history of previous updates, as opposed to nonqueued attributes, which are overwritten when the device is offline.
- New reboot reason format. Read more in [Device Attribute Registry > Attribute 65019](../AttrRegistry#RebootReason).
- Bug fixes.

## Afero Firmware R2.0.1

Release date: August 29, 2018

Version number: 28036 for Modulo-2, 28036 for Abelo-2A

- Changed the MCU UTC time update to fire **on** the minute. Read more in [Using the Afero Cloud to Keep Time on the MCU](../SetMCUTime).
- Fixed a bug that could prevent some of the MCU default values from being sent to the MCU at startup.

## Afero Firmware R2.0

Release date: July 24, 2018

Version numbers: 27144 for Modulo-2, 27144 for Abelo-2A

- API added to support MCU over-the-air (OTA) firmware updates. Read more in [Handling MCU OTA Updates](../MCU_OTA).
- New version of [afLib](http://github.com/aferodeveloper/afLib) is available, called afLib3. Read more in [afLib Release Notes](../RelNotes-afLib) and [afLib3 API](../API-afLib).
- Better handling of MCU attribute default values.
- Changes to Update All.
- New local attributes that support the following:
  - The MCU can GET the UTC time pushed every minute.
  - The MCU can GET the device’s Association ID and Device ID.
  - The device’s online status will be pushed to the MCU when it changes.
- Support for STM + Cypress/Broadcom architecture.
- Many bug fixes and improvements.

## Afero Firmware R1.3

Release date: March 14, 2018

Version number: 23773 for Modulo-2

- Wi-Fi stability improvements.
- Added Update state and reasons to the [Device Attribute Message Protocol](../AttrMsgProtocol).
- New version of [afLib](http://github.com/aferodeveloper/afLib) is available, called afLib2. We’ve improved the sample code and included the example application, **aflib_time_check**. Read more in [Using the Afero Cloud to Keep Time on the MCU](../SetMCUTime).

## Afero Firmware R1.2.1.35

Release date: December 6, 2017

Version numbers: 20578 for Modulo-1; 20647 for Modulo-2

- Bug fix for GPIO attribute STORE IN FLASH option.

## Afero Firmware R1.2.1

Release date: November 14, 2017

Version numbers: 19876 for Modulo-1; 20032 for Modulo-2

- Persistent GPIO Attributes - An output attribute can be flagged as “Store in Flash” so that its value will persist through a power cycle.
- Various bug fixes.

## Afero Firmware R1.2

Release date: July 24, 2017

- New State Attribute - Allows the MCU to know the state of ASR, including linked, updating, and update-ready-to-apply.
- New Command Attribute - Lets the MCU reboot the Modulo.
- New Linked Timestamp Attribute - Modulo now knows what time it is.
- Count Support - A GPIO can be configured to count input pulses.
- FIXED_15 _16 Support - Support for a new fixed-point data type.
- Input Debouncing - A GPIO can be configured to debounce noisy input signals.
- UART Support - A MCU can now be connected via UART.
- Many other bug fixes and improvements.

### Important! Order of Operations

Because of dependencies between the Afero Profile Editor, afLib3, and Modulo firmware, upgrading to the latest Modulo release must be done in the correct order. This page documents the order and why it is the way it is. Please refer also to the [Afero Profile Editor Release Notes](../RelNotes-APE).

#### Step 1. OTA

The 1.2 release for Modulo will update the firmware on your Modulo. This is the first step and will be done in the background without your doing anything. Once the firmware has been updated, the Modulo will reboot. If you have an MCU connected, you will need to reboot the MCU. This problem is fixed in the 1.2 release but to get to the 1.2 you must manually reboot the MCU. We apologize for the inconvenience. It won’t happen again.

At this point, your project should run just as it did before. If you want to continue using the same Profile and MCU application, they should continue to work with no modifications.

#### Step 2. Afero Profile Editor

Once you have updated the firmware, you can continue using the version of the Profile Editor that you have. However, if you want to use the new attributes in 1.2, you will need to update to the latest Profile Editor to get them.

If you update to the latest Profile Editor before your device has received the 1.2 firmware, you will have to wait for the OTA or go back to the previous release of the Profile Editor.

#### Step 3. Device Profile

The 1.2 release has added several new system attributes to the device Profile. You will only get these attributes once you have published your Profile using the latest version of the Profile Editor. So once you have the latest Profile Editor and Modulo firmware, load and re-publish your Profile. You don’t need to make any changes to it; the Profile Editor will automatically add the new attributes and update your device-description.h file.

#### Step 4. afLib3

Once you have performed all these updates, you are now ready to update to the latest version of afLib3. The latest afBlink example implements handlers for some of the new attributes. Pay particularly close attention to the handler for the AF_SYSTEM_ASR_STATE attribute in the attrNotifyHandler. This new attribute lets you know the state of the Modulo. It also let your sketch know when an update is pending so you can do house keeping before sending the command to the Modulo to tell it to reboot and apply the update.