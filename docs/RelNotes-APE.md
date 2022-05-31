# Profile Editor Release Notes

## Afero Profile Editor R1.9

Release date: September 11, 2019

- Support added for Plumo-2C and Plumo-2D.
- Bug fixes and other improvements.

## Afero Profile Editor R1.8.8

Release date: July 18, 2019

- Bug fix related to Potenco attributes.

## Afero Profile Editor R1.8.6

Release date: June 7, 2019

- A couple bug fixes.

## Afero Profile Editor R1.8

Release date: May 30, 2019

- Support added for the Plumo-2C module, which contains the MT7697 chipset.

- Support added for the Plumo-2D module, which contains the ESP32 radio.

- New attribute features:

- - Attributes can be assigned to multiple controls. Read more in [Define the Mobile App UI](../AppUIDef).
  - Attribute updates can be tracked with a timestamp, now accurately reporting when the update occurred even if the device was offline at the time of update. Read more in [Define the Device Attributes > Use ASR Timestamps](../AttrDef#UseTimestamp).
  - Attributes updates can now be stored in a configurable queue. Read more in [Define the Device Attributes > Queue Configuration](../AttrDef/#QueueConfig) and [All About Queuing](../AttrDef#queuepolicy).
  - For projects using the Potenco module type, you can now specify supported network interfaces. Read more in [Define Potenco Attributes](../AttrDef#HubAttributes).

## Afero Profile Editor R1.7

Release date: March 27, 2019

- Support added for the Modulo-1B module.
- For developers associated with more than one partner, new feature for switching between partner accounts. You’ll find the Partner Selector on the Startup window.
- Control group titles limited to 39 characters, for optimal display on Android devices.
- Enforcement of unique Device Type name/module combination across projects. Developer is notified when creating, cloning, or opening projects. Read more in [Before You Proceed](../SelectProject#DOC-158).
- On the Publish window, in the Device Activity pane, the device attributes are now displayed with their names and numbers, for easy identification.
- For GPIO Input attributes, the COUNT flag is now available when MCU is enabled.
- Bug fixes and assorted improvements.

## Afero Profile Editor R1.6

Release date: October 1, 2018

- Bug fixes and other enhancements.

## Afero Profile Editor R1.5

Release date: August 9, 2018

- Support added for defining an “attribute event trigger” that results in a notification being sent to the end-user in the mobile app. Read more in [Define the Rules & Notifications](../RulesDef).
- Support added for updating the firmware on your MCU via OTA. Read more in [Configure the MCU](../AttrDef#ConfigMCU).
- The Afero Firmware R2.0 added a local attribute that will send the current UTC time to an MCU every minute as it changes, but only if it’s been enabled. We’ve added a checkbox to enable this UTC Time attribute and add it to your device Profile. Read more in [Define the MCU Attributes](../AttrDef#MCUAttrs) and [Using the Afero Cloud to Keep Time on the MCU](../SetMCUTime).
- On the UI Controls window, there is now dynamic rendering of UI controls for the different view styles.
- Assorted bug fixes.

## Afero Profile Editor R1.4

Release date: April 30, 2018

- Support added for new Text Box control.
- Two view styles now available for controls: inline and popup.
- Updated device icons and control previews to match the new Afero mobile app “panels” UI.
- Added Quick Links to relevant developer documentation from the Startup window.
- Improvements to validation error messages.

## Afero Profile Editor R1.3

Release date: January 23, 2018

- User interface facelift (new style and colors). Read more in [Profile Editor User Interface](../SelectProject#APE-UI).
- On the Publish screen, Device History has been replaced by Device Activity, a rolling log of events initially populated with the last values cached in the Cloud.

## Afero Profile Editor R1.2.23

Release date: December 6, 2017

- The GPIO attribute option STORE IN FLASH is now available for GPIO Inputs that have the ISTOGGLE option selected. In other words, if a GPIO Input does not have INTOGGLE selected, the STORE IN FLASH option will not be available.
- For GPIO attributes that are BOUND together, if one of the bound attributes has the STORE IN FLASH option selected, then all the bound attributes must have the STORE IN FLASH option selected.
- Various bug fixes.

## Afero Profile Editor R1.2

Release date: November 16, 2017

- Checkbox added to Attributes window for flagging a GPIO Output as STORE IN FLASH, which means an attribute’s value will persist through a power cycle.
- Improved presentation on small screens.
- Board type is now defined in the device-description.h file, generated when you publish a Profile that includes an MCU.
- Added hide/show button toggles![img](img/APE-Hide-Show-Arrows.png)to the Navigation pane on the left and the Preview pane on the right, for expanding your workspace when needed.
- Various bug fixes and other improvements.

## Afero Profile Editor R1.1.6

Release date: August 28, 2017

- Added new device icon for Modulo-2.
- Updated image on the right side of the Attributes screen to match the developer board used in the current project.
- Updated links to documentation to point to the updated Afero Developer Portal.
- Added RAM usage indicator on the Attributes window, for monitoring space available for developer-defined device attributes.
- Fixed bugs and made various small improvements.

## Afero Profile Editor R1.0

Release date: July 25, 2017

- Can reorder UI Control value options.
- Support added for UART MCU protocol.
- Can clone a Project from the OS File menu.
- Support added for both ASR-1 and Modulo-2 modules.
- Support added for FIXED data types.
- For FIXED data types, developer can enter the number of slider step values.
- Added a power use calculator for ASR-1 modules.
- Can apply a debounce interval to an associated input.
- Added count option for Input attributes.
- Added a QR Code Generator tool on the Publish pane.

### Important! Order of Operations

Because of dependencies between the Afero Profile Editor, afLib3, and Modulo firmware, upgrading to the latest Modulo release must be done in the correct order. This page documents the order and why it is the way it is. Please refer also to the [Afero Firmware Release Notes](../RelNotes-FW).

#### Step 1. OTA

The 1.2 release for Modulo will update the firmware on your Modulo. This is the first step and will be done in the background without you doing anything. Once the firmware has been updated, the Modulo will reboot. If you have an MCU connected, you will need to reboot the MCU. This problem is fixed in the 1.2 release but to get to the 1.2 you must manually reboot the MCU. We apologize for the inconvenience. It won‘t happen again.

At this point, your project should run just as it did before. If you want to continue using the same Profile and MCU application, they should continue to work with no modifications.

#### Step 2. Afero Profile Editor

Once you have updated the firmware, you can continue using the version of the Profile Editor that you have. However, if you want to use the new attributes in 1.2, you will need to update to the latest Profile Editor to get them.

If you update to the latest Profile Editor before your device has received the 1.2 firmware, you will have to wait for the OTA or go back to the previous release of the Profile Editor.

#### Step 3. Device Profile

The 1.2 release has added several new system attributes to the device Profile. You will only get these attributes once you have published your Profile using the latest version of the Profile Editor. So once you have the latest Profile Editor and Modulo firmware, load and re-publish your Profile. You don’t need to make any changes to it; the Profile Editor will automatically add the new attributes and update your device-description.h file.

#### Step 4. afLib3

Once you have performed all these updates, you are now ready to update to the latest version of afLib3. The latest afBlink example implements handlers for some of the new attributes. Pay particularly close attention to the handler for the AF_SYSTEM_ASR_STATE attribute in the attrNotifyHandler. This new attribute lets you know the state of the Modulo. It also let your sketch know when an update is pending so you can do house keeping before sending the command to the Modulo to tell it to reboot and apply the update.