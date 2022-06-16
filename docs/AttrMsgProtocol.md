# Device Attribute Message Protocol

The Afero architecture uses a messaging protocol that is independent of the wireless technology used. This protocol provides a simple way for the client application to SET or GET attributes on the ASR module.

You will need the information on this page only if you are implementing the protocol yourself, **not** using afLib API. If you **are** using the API to communicate with ASR, the information below will be implemented for you.



## Message Format

The format of the messages is defined below:

| MESSAGE |        | uint16_t | Message length including header, but not including the two bytes of the length itself|
| :------ | :------ | :---------- |--------------------------------------------|
|    | **Header**         | uint8_t  |Message type:<br>0x0B = Set<br>0x0C = Get<br>0x0D = Update<br>0x29 = Update Rejected<br>0x2A = Set Default 
|    |                    | uint8_t  | Request ID:<br>0 = Transaction initiated from peripheral<br>>0 = Transaction initiated from authenticator                                                             
|    |   **SET**         | uint16_t  | Attribute ID 
|    |                    | uint16_t | Value length                          |    
|    |                    | n bytes  | Variable-length attribute value       |    
|    |  **GET**           |uint16_t  | Attribute ID                          |
|    | **UPDATE**         | uint16_t | Attribute ID                          |                               |
|    |                    | uint8_t  | Update state:<br>0x00 = UPDATED<br>0x01 = INTERRUPTED; device-side UPDATE in progress or preempted by device-side UPDATE<br>0x02 = UNKNOWN_ATTRIBUTE<br>0x03 = LENGTH_EXCEEDED<br>0x04 = CONFLICT; Previous SET in progress<br>0x05 = TIMEOUT; SET operation timed out<br>0x06 = FORBIDDEN; SET not allowed<br>0x07 = Q_FULL; Queued attribute queue full failure<br>0xAA = INVALID_STATE; Cloud received unrecognized state value |
|    |                    | uint8_t  | Update reason:<br>0x00 = UNKNOWN<br>0x01 = MODULE; Unsolicited Afero module-initiated or MCU-initiated UPDATE; e.g., button press<br>0x02 = SERVICE; Response to Cloud-initiated SET<br>0x03 = MCU; Response to MCU-initiated SET<br>0x04 = LINK; Linking completed<br>0x05 = BOUND; A bound attribute was changed<br>0x06 = FORBIDDEN; SET not allowed<br>0x07 = NOTIFY_MCU_WE_REBOOTED; Notify MCU that ASR rebooted; not sent to Cloud<br>0x08 = LOCAL_SET; Response to local SET; e.g., when a scheduled event fires<br>0x09 = REBOOT0x0A = CRC_FAILURE; Cyclic redundancy check (CRC) failure; used to sync state between device and Cloud when attribute values no longer match<br>0x0B = GET_RESPONSE; Response to GET (Cloud or MCU-initiated)<br>0xAA = Invalid reason; SET when the Cloud receives an UPDATE with either no reason or an invalid reason |
|    |                    | uint16_t            | Value length                                                 |
|    |                    | n bytes             | Variable-length attribute value                              |
|    | **Update Rejected** | uint16_t           | Attribute ID                                                 |
|    |                    | uint8_t             | Update state: See "Update state" above. |
|    |                    | uint8_t             | Update reason: See "Update reason" above. |
|    | **Set Default**    | uint16_t            | Attribute ID                                                 | 
|    |                    | uint16_t            | Value length                                                 |
|    |                    | n bytes             | Variable-length attribute value                              |

## Protocol Rules

- A SET for a specific attribute can only be sent by a non-owner of the attribute.
- A GET for a specific attribute can only be sent by a non-owner of the attribute with the exception that an MCU can send a GET message to retrieve the value of its own attribute.
- An UPDATE for a specific attribute can only be sent by the owner of the attribute, except in the following case:
    - ASR is sending back the result of an MCU get message for its own attribute.
- Request IDs are always zero **unless** the transaction is a GET or a SET originally from the service, or the transaction is an UPDATE for a SET or a GET originally from the service.
- For every SET or GET message the owner of a specific attribute receives, it must send a corresponding UPDATE. The owner must parrot back the request ID from the incoming SET or GET message.
- Updates can also occur if the attribute change was initiated locally; for example, if the MCU changes one of its own attributes. In this case there is no corresponding SET or GET transaction.
- The receiver of an UPDATE message never sends a response to the message with one exception: ASR can send an unsolicited UPDATE REJECTED message if the UPDATE message is bad in one of the following ways:
    - Attribute ID is not owned by the sender (MCU is the only possible sender in this case).
    - Attribute ID is not in the Profile.
    - The value length is larger than the length of the attribute in the Profile.
- Only one SET or GET transaction for a given attribute can be in flight at a time. If a SET or GET transaction for an attribute has occurred and the corresponding UPDATE transaction has not yet occurred, further SETs or GETs for that attribute are rejected.
- GET transactions never originate from the service during normal use. However this could change in the future.
- During the initialization phase of the ASR-MCU interface after reboot:
    - ASR originates and sends a GET message for each MCU attribute to the MCU to get the initial value. The MCU responds with an UPDATE message containing the initial value for each.
    - ASR originates and sends a SET_DEFAULT message for each MCU attribute that has a default value to the MCU to give the MCU the default value. There is no reply to the SET_DEFAULT message.
- All transports are half duplex. There is collision control if both sides try to send at the same time (the MCU always wins); but once the winner has been chosen, the winner is guaranteed to send the entire message.
- Updates sent in either direction must only happen one at a time; specifically:
    - For the SPI transport, because ASR is the slave, it uses flow control to enforce this UPDATE rule.
    - For the SPI transport, because the MCU is the master, it can enforce this rule by delaying the start of the transaction. This is natural in a single-threaded event loop type of environment.
    - For the UART transport, both ASR and the MCU must be ready for a transaction to occur, making this rule easy to enforce.
- Updates are allowed to interrupt SET and GET operations according to the design of the attribute store.
- afLib uses the a single transmit queue for SET, GET, and UPDATE operations. Each operation must finish before the next one is initiated. Therefore, only one SET, GET, or UPDATE operation from the MCU can occur at any one time. ASR has no such limitations.

## Protocol Corollaries

- An UPDATE message from ASR must have a request ID of zero because:
    - The service owns no attributes, therefore it cannot initiate an UPDATE message.
    - When the MCU performs a SET or GET, it uses a request ID of zero.
- If the MCU forgets to send an UPDATE for a SET initiated from the service for a specific attribute, that attribute **cannot be SET by the service again**. The attribute will remain **stuck** until ASR reboots and forgets that the SET is still pending.

 *Next:*&nbsp;&nbsp;[**&#8674;** Device Attribute Registry](../AttrRegistry)