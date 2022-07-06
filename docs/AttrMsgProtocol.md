# Device Attribute Message Protocol

The Afero architecture uses a messaging protocol that is independent of the wireless technology used. This protocol provides a simple way for the client application to SET or GET attributes on the ASR module.

You will need the information on this page only if you are implementing the protocol yourself, **not** using afLib API. If you **are** using the API to communicate with ASR, the information below will be implemented for you.

## Message Format

The format of the messages is defined below:

<div>
<table class="af-table-borders">
<colgroup>
<col class="c1">
<col class="c2">
<col class="c3">
<col class="c4">
</colgroup>
<tbody>
<tr>
<td rowspan="18"><strong>Message</strong></td>
<td></td>
<td>uint16_t</td>
<td>Message length including header, but <strong>not</strong> including the two bytes of the length itself</td>
</tr>
<tr>
<td rowspan="2"><strong>Header</strong></td>
<td>uint8_t</td>
<td><p>Message type:</p>
	<p>0x0B = Set</p>
	<p>0x0C = Get</p>
	<p>0x0D = Update</p>
	<!-- Used between service and ASR only<p>0x0F = Update Historical</p>-->
	<p>0x29 = Update Rejected</p>
	<p>0x2A = Set Default</p></td>
</tr>
<tr>
<td>uint8_t</td>
<td><p>Request ID:</p>
	<p>&nbsp; 0 = Transaction initiated from peripheral</p>
	<p>&gt;0 = Transaction initiated from authenticator</p></td>
</tr>
<tr>
<td rowspan="3"><strong>SET</strong></td>
<td>uint16_t</td>
<td>Attribute ID</td>
</tr>
<tr>
<td>uint16_t</td>
<td>Value length</td>
</tr>
<tr>
<td>n bytes</td>
<td>Variable-length attribute value</td>
</tr>
<tr>
<td><strong>GET</strong></td>
<td>uint16_t</td>
<td>Attribute ID</td>
</tr>
<tr>
<td rowspan="5"><strong>UPDATE</strong></td>
<td>uint16_t</td>
<td>Attribute ID</td>
</tr>
<tr>
<td>uint8_t</td>
<td id="UpdateState"><p>Update states:</p>
	<p>0x00 = UPDATED</p>
	<p>0x01 = INTERRUPTED; device-side UPDATE in progress or preempted by device-side UPDATE</p>
	<p>0x02 = UNKNOWN_ATTRIBUTE</p>
	<p>0x03 = LENGTH_EXCEEDED</p>
	<p>0x04 = CONFLICT; Previous SET in progress</p>
	<p>0x05 = TIMEOUT; SET operation timed out</p>
	<p>0x06 = FORBIDDEN; SET not allowed</p>
	<p>0x07 = Q_FULL; Queued attribute queue full failure</p>
	<p>0xAA = INVALID_STATE; Cloud received unrecognized state value</p></td>
</tr>
<tr>
<td>uint8_t</td>
<td id="UpdateReason"><p>Update reasons:</p>
	<p>0x00 = UNKNOWN</p>
	<p>0x01 = MODULE; Unsolicited Afero module-initiated or MCU-initiated UPDATE; e.g., button press</p>
	<p>0x02 = SERVICE; Response to Cloud-initiated SET</p>
	<p>0x03 = MCU; Response to MCU-initiated SET</p>
	<p>0x04 = LINK; Linking completed</p>
	<p>0x05 = BOUND; A bound attribute was changed</p>
	<p>0x06 = FORBIDDEN; SET not allowed</p>
	<p>0x07 = NOTIFY_MCU_WE_REBOOTED; Notify MCU that ASR rebooted; not sent to Cloud</p>
	<p>0x08 = LOCAL_SET; Response to local SET; e.g., when a scheduled event fires</p>
	<p>0x09 = REBOOT</p>
	<p>0x0A = CRC_FAILURE; Cyclic redundancy check (CRC) failure; used to sync state between device and Cloud when attribute values no longer match</p>
	<p>0x0B = GET_RESPONSE; Response to GET (Cloud or MCU-initiated)</p>
	<p>0xAA = Invalid reason; SET when the Cloud receives an UPDATE with either no reason or an invalid reason</p>
</td>
</tr>
<tr>
<td>uint16_t</td>
<td>Value length</td>
</tr>
<tr>
<td>n bytes</td>
<td>Variable-length attribute value</td>
</tr>
<tr>
<td rowspan="3"><strong>Update Rejected</strong></td>
<td>uint16_t</td>
<td>Attribute ID</td>
</tr>
<td>uint8_t</td>
<td>Update state: See <a id="1536862623.88" href="#UpdateState">state codes</a> above.</td>
<tr>
<td>uint8_t</td>
<td>Update reason: See <a id="1536862623.99" href="#UpdateReason">reason codes</a> above.</td>
</tr>
<tr>
<td rowspan="3"><strong>Set Default</strong></td>
<td>uint16_t</td>
<td>Attribute ID</td>
</tr>
<td>uint16_t</td>
<td>Value length</td>
<tr>
<td>n bytes</td>
<td>Variable-length attribute value</td>
</tr>
</tbody>
</table>
</div>

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
