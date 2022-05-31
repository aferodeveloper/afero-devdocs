# Attribute Update Reasons and States

Messages between the Afero powered device (or “peripheral”) and the Afero Cloud/mobile clients/partner databases contain, among other information, the message “type” and the message “data”. For message types that are UPDATE or UPDATE HISTORICAL, the data includes the Attribute Update State and Update Reason. Knowing how to interpret this data could help you when debugging.

The possible values for peripheral Attribute Update Reasons and States are described in the sections below.

## Peripheral Attribute Update Reasons

All Values are single-byte, hex encoded.

| VALUE | REASON                 | DESCRIPTION                                                  |
| :---- | :--------------------- | :----------------------------------------------------------- |
| 0x00  | UNKNOWN                |                                                              |
| 0x01  | MODULE                 | Unsolicited Afero module or MCU-initiated update (e.g., ASR button press or MCU button press). |
| 0x02  | SERVICE                | Response to Cloud-initiated Set.                             |
| 0x03  | MCU                    | Response to MCU-initiated Set.                               |
| 0x04  | LINK                   | Linking completed.                                           |
| 0x05  | BOUND                  | Attribute update was triggered by another attribute, to which this one is bound. |
| 0x06  | FAKE_UPDATE            | Afero module sent dummy update.                              |
| 0x07  | NOTIFY_MCU_WE_REBOOTED | Deprecated. (ASR now sends REBOOT (09) when ASR reboots.)    |
| 0x08  | LOCAL_SET              | Unsolicited Afero module or MCU-initiated update (e.g., ASR button press or MCU button press). |
| 0x09  | REBOOT                 | Update caused by device reboot.                              |
| 0x0A  | CRC_FAILURE            | Used to synchronize state between the device and the Cloud when attribute values no longer match. |
| 0x0B  | GET_RESPONSE           | Response to `get` (Cloud- or MCU-initiated).                 |
| 0xAA  | INVALID_REASON         | Set when the Cloud receives an update with no reason, or the reason is not valid. |

## Peripheral Attribute Update States

- All Values are visible to the MCU, and all but “07 Q_FULL” are visible to the Cloud.
- All Values are single-byte, hex encoded.

| VALUE | STATE             | DESCRIPTION                                                  |
| :---- | :---------------- | :----------------------------------------------------------- |
| 0x00  | UPDATED           |                                                              |
| 0x01  | INTERRUPTED       | Device-side update in progress or preempted by device-side update. |
| 0x02  | UNKNOWN_ATTRIBUTE |                                                              |
| 0x03  | LENGTH_EXCEEDED   |                                                              |
| 0x04  | CONFLICT          | Previous Set in progress.                                    |
| 0x05  | TIMEOUT           | Set timed out.                                               |
| 0x06  | FORBIDDEN         | Set not allowed.                                             |
| 0x07  | Q_FULL            | Queued attribute queue full failure.                         |
| 0xAA  | INVALID STATE     | Cloud received unrecognized state value                      |