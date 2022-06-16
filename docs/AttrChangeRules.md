# Attribute Value Change Rules

What happens when an attribute value is changed is a function of its definition; specifically, the attribute ownership and flag(s). The rules around attribute value changes are described in the following sections:

- [Attribute Characteristics Affecting Change Behavior](../AttrChangeRules/#attribute-characteristics-affecting-change-behavior)
- [Change Rules for MCU Attributes](../AttrChangeRules/#change-rules-for-mcu-attributes)
- [Change Rules for ASR Attributes](../AttrChangeRules/#change-rules-for-asr-attributes)
- [SET, GET, UPDATE Messages](../AttrChangeRules/#set-get-update-messages)

Note that all mentions of “Cloud” refer to the Afero Cloud.

## Attribute Characteristics Affecting Change Behavior

Attributes are the simple pieces of data that form the language shared between ASR, the MCU, the Afero Cloud, and the Afero mobile app. Attributes have characteristics, including:

- **Type** - The data type (e.g., SINT8, SINT16, byte array).
- **Value** - The data value.
- **ID** - Integer identifying the attribute.
- **Owner** - Either the ASR or an associated MCU. (Using the Afero Profile Editor, you can create the ASR-owned GPIO attributes and MCU-owned attributes.)
- **Flags** - Metadata that control, for example, how an attribute can be changed.

Ownership and flags are looked at in more detail below.

### Attribute Ownership

Each attribute has an “owner”, which can be ASR or an associated MCU. It’s possible to quickly determine the owner by the attribute ID:

- Attributes owned by the MCU are assigned IDs in the range 1-1023.

- Attributes owned by ASR have IDs in the range 1024 - 65022.

All attribute IDs in a given device Profile are listed in the device-description.h file, which is created for you by the Afero Profile Editor.

Ownership affects how an attribute is treated; specifically:

- What happens when the attribute value is changed, and
- What message types (GET, SET, or UPDATE) are used to control the attribute.

At a simple level, SET messages can be thought of as the initiator of an attribute change, while UPDATE messages typically indicate that an attribute value has been changed and other parts of the system should be made aware of the change. Attributes are described in more detail below in [SET, GET, UPDATE Messages](../AttrChangeRules/#set-get-update-messages).

### Attribute Flags

Attributes have “flags”, which play a part in determining how attribute values are changed. You set the following flags in the Afero Profile Editor > Attribute Definitions window:

| STRING | FLAG, DEFINED IN PROFILE EDITOR   | DESCRIPTION                                                 |
| :----- | :-------------------------------- | :---------------------------------------------------------- |
| READ   | Writable checkbox (deselected)    | Attribute is readable by the Cloud and mobile app.          |
| WRITE  | Writable checkbox (selected)      | Attribute is writable by the Cloud and mobile app.          |
| LATCH  | Latch option for Input attributes | Attribute changes require an acknowledgment from the Cloud. |

## Change Rules for MCU Attributes

An MCU attribute value can change in response to:

- An action initiated by the MCU; e.g., a user adjusts a temperature dial connected to the MCU, then the MCU code sets the attribute, or
- A Cloud-relayed SET message; e.g., a user changes a temperature control on the mobile app, sending a message to the Afero Cloud, which is relayed to the MCU.

Both situations are described below.

### MCU-Initiated Changes

If the MCU changes the value of one of its attributes, the MCU notifies ASR with an UPDATE message. ASR caches the new value, then forwards the UPDATE to the Afero Cloud.

**For write-only attributes, the UPDATE is not forwarded to the Cloud.**

### Cloud-Relayed Changes

When the Cloud relays a request to change the value of an MCU attribute, it does so with a SET message. For example, when a user executes an action in the mobile app, this results in a SET sent to the Cloud. From the Cloud, the SET request is delivered to ASR. ASR forwards the SET from the Cloud to the MCU, and the change is made.

However, if any of the following conditions are met, the result will differ:

| CONDITION                                                    | CHANGE RESULT                   | NO RESPONSE GIVEN TO THE CLOUD; ILLEGAL OPERATION.           |
| :----------------------------------------------------------- | :------------------------------ | :----------------------------------------------------------- |
| Attribute is read-only                                       | SET is ignored                  | Attribute is readable by the Cloud and mobile app.           |
| MCU interface isn’t up                                       | SET fails                       | ASR sends back UPDATE with state = “unknown ID” (0x02).      |
| MCU has not yet replied to a previous SET                    | SET fails                       | ASR sends back UPDATE with state = “conflict” (0x04).        |
| Length of the requested value exceeds the attribute value’s length | SET fails                       | ASR sends back UPDATE with state = “length exceeded” (0x03). |
| Attribute is already set to the requested value              | SET is not forwarded to the MCU | ASR sends back UPDATE with state = “success” (0x00).         |

When the Cloud wants to get the value of an MCU attribute, it makes the request with a GET message. ASR immediately replies with an UPDATE message to the Cloud using the last value received from the MCU.

ASR currently has no method for initiating a change to an MCU attribute. ASR can only forward a request from the Cloud.

## Change Rules for ASR Attributes

Like the MCU, ASR can change the value of one of its attributes:

- By initiating the change, or
- In response to a Cloud-relayed change message.

Both situations are described below.

### ASR-Initiated Changes

When ASR changes the value of one of its attributes, it notifies the MCU and the Cloud with an UPDATE message.

However, if the following condition is met, the result will differ:

| CONDITION               | RESULT                                                       |
| :---------------------- | :----------------------------------------------------------- |
| Attribute is write-only | ASR cached value is updated, but the UPDATE is otherwise ignored and not forwarded to the MCU or the Cloud. |

### Cloud-Relayed Changes

When the Cloud wants to change the value of an ASR attribute, it requests the change with a SET message. ASR updates the attribute value and sends an UPDATE message to the MCU and to the Cloud.

However, if any of the following conditions are met, the result will differ:

| CONDITION                                                    | RESULT         | UPDATE STATE RESPONSE                                       |
| :----------------------------------------------------------- | :------------- | :---------------------------------------------------------- |
| Attribute is read-only                                       | SET is ignored | No response given to the Cloud; illegal operation           |
| ASR has not yet replied to a previous SET                    | SET fails      | ASR sends back UPDATE with state = “xconflict” (0x04)       |
| Length of the requested value exceeds the attribute value’s length | SET fails      | ASR sends back UPDATE with state = “length exceeded” (0x03) |
| Attribute is already set to the requested value              | SET succeeds   | ASR sends back UPDATE with state = “success” (0x00)         |

When the Cloud or MCU wants to get the value of an ASR attribute, it requests the value with a GET message. Then ASR sends an UPDATE message to the requestor (Cloud or MCU) containing the value.

## SET, GET, UPDATE Messages

SET, GET, and UPDATE are messages for communicating attribute state between endpoints. There are three endpoints: the MCU, ASR, and the Afero Cloud.

### SET Messages

SET messages are used when a non-owner requests an owner to change one of its attribute values, as follows:

| CHANGE REQUEST MADE BY | TO CHANGE ATTRIBUTE VALUE ON |
| :--------------------- | :--------------------------- |
| Cloud                  | ASR                          |
| Cloud                  | MCU (via ASR)                |
| MCU                    | ASR                          |

ASR currently does not support SET autonomously initiated by ASR to change an MCU attribute.

### GET Messages

GET messages are used when a non-owner wants to get an owner’s attribute value:

| GET REQUEST MADE BY: | TO GET ATTRIBUTE VALUE ON:                                   |
| :------------------- | :----------------------------------------------------------- |
| Cloud                | ASR                                                          |
| Cloud                | MCU (ASR immediately returns the cached value; it does not forward the GET to the MCU.) |
| MCU                  | ASR                                                          |

ASR currently does not support GET autonomously initiated by ASR to get an MCU attribute.

### UPDATE Messages

UPDATE messages are used by an owner to notify all non-owners that an attribute value has changed, regardless of the reason for the change (response to SET, autonomously updated, etc.), as follows:

| CHANGE MADE ON: | UPDATE NOTIFICATION SENT TO: |
| :-------------- | :--------------------------- |
| ASR             | MCU and Cloud                |
| MCU             | Cloud (via ASR)              |

An UPDATE is also sent in response to a GET, to return the current value.

### Summary of Operations

**ASR-owned attributes** (such as the Modulo button or LED) support these operations:

| MESSAGE TYPE | FROM  | TO          |
| :----------- | :---- | :---------- |
| SET          | MCU   | ASR         |
| GET          | MCU   | ASR         |
| SET          | Cloud | ASR         |
| GET          | Cloud | ASR         |
| UPDATE       | ASR   | MCU & Cloud |

**MCU-owned attributes** support these operations:

| MESSAGE TYPE | FROM  | TO                    | DETAILS                                                      |
| :----------- | :---- | :-------------------- | :----------------------------------------------------------- |
| SET          | Cloud | MCU (via ASR)         | ASR forwards to the MCU.                                     |
| GET          | Cloud | ASR                   | ASR returns UPDATE with cached value (ASR does not forward GET to the MCU). |
| GET          | MCU   | ASR                   | ASR returns UPDATE with cached value (or default value if never updated). |
| UPDATE       | MCU   | Cloud (via ASR).      | ASR caches and forwards to the Cloud.                        |
| UPDATE       | ASR   | MCU                   | Response to GET from the MCU. This operation is an exception to the model. Under ordinary circumstances, the MCU would already know the values of its own attributes. However, for convenience, the ASR allows this exception: it allows the MCU to use ASR as storage for MCU attributes. |
| UPDATE       |       | Cloud of cached value | This happens only at startup during the Update-All process.  |

Note that the following operations **do not** exist:

- SET/GET of an MCU attribute originating from ASR (not originating from the Cloud)
- UPDATE of an ASR attribute from MCU or the Cloud
- UPDATE of an MCU attribute from the Cloud

 **&#8674;** *Next:*&nbsp;&nbsp;[afLib API](../API-afLib)