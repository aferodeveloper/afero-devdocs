# afPro SPI Protocol

To communicate with the MCU, ASR uses Afero Serial Protocol (afPro) over a Serial Peripheral Interface (SPI) bus. Note that we require use of SPI Mode 0 (CPOL and CPHA are both ‘0’; data is sampled at the leading rising edge of the clock).

To illustrate, we’ve provided some examples below. These examples assume:

- a standalone MCU as the master
- an Afero dev board as the slave

We use timing diagrams from a logic analyzer to show the traffic.

## The Approach

Because all SPI transactions are driven by the master, and because afPro supports bi-directional communications, two problems must be solved:

1. Flow control to allow the slave to prepare for transactions as well as handling collisions (i.e., when the master and the slave both have something to send).
2. Ability for the slave to request the master to read from it.

To solve problem 1, we use a sync message to communicate the number of bytes to be sent from the master or the slave. This message can be sent at any time to make sure the master stays in sync with the slave. It is also used to recognize collisions (i.e., both master and slave want to send at the same time).

To solve problem 2, we add an interrupt line to the standard SPI signals. The slave can pulse the interrupt line when it wants the master to communicate with it. The master can then send a sync message to begin the transaction.

## Resetting ASR When the MCU Reboots

Whenever the MCU reboots, it must then force ASR to reboot as well, thus canceling any unresolved transactions between master and slave. The MCU does this by sending a Reset signal to ASR. But first the MCU must be ready to receive a request from ASR:

1. MCU prepares the interface for communication with ASR.
2. MCU asserts the Reset signal for a minimum of 250 ms.

This sequence forces ASR to start from a known (initial) state. ASR will initiate the zero sync right after it reboots, as described below in [Example 1](../afPro-SPI/#example-1-zero-sync).

## Sync Messages

A sync message is the first thing sent for every afPro transaction. There are two types of sync messages: Sync Request and Sync Acknowledge. Both messages have the same format; the only difference is the message type.

| UINT8_T      | UINT16_T        | UINT16_T        | UINT8_T  |
| :----------- | :-------------- | :-------------- | :------- |
| Message Type | MOSI byte count | MISO byte count | Checksum |

The message type is 0x30 for a Sync Request and 0x31 for a Sync Acknowledge. The checksum is simply the unsigned sum of all the other bytes in the message.

The simplest form of sync is when the master sends a Sync Request with both byte counts set to zero. If the slave also has no bytes to send, the master sends a Sync Acknowledge and the transaction is complete.

## Example Transactions

For all the timing diagrams in the examples below, the byte counts are little-endian and the signal order is:

- MOSI - Master Out, Slave In; the master line used to send data to the devices
- MISO - Master In, Slave Out; the slave line used to send data to the master
- CLK - Serial Clock; the master line used to keep data signals in sync
- CS - Chip Select; the master line used to select one or more devices
- INT - Interrupt; the slave line used to interrupt the signal to the master to get the master’s attention

Let’s look at these examples:

- [Example 1: Zero Sync](../afPro-SPI/#example-1-zero-sync) - Occurs when the master sends a Sync Request to the slave, but the data payload is empty. For example, when the master boots, it sends a zero Sync Request to the slave.
- [Example 2: Set Attribute](../afPro-SPI/#example-2-set-attribute) - Occurs when the master sets the value of an attribute.
- [Example 3: Update Attribute](../afPro-SPI/#example-3-update-attribute) - Occurs when the MCU changes the value of an attribute and sends the update back to ASR.
- [Example 4: Set Attribute with Collision](../afPro-SPI/#example-4-set-attribute-with-collision) - Occurs when the master and slave both want to send a request.

### Example 1. Zero Sync

The INT line shows the ASR module starting transactions with the MCU:

[*Click image to enlarge:*
<img src="../img/SPI-ZeroSync-Overview.jpg" style="vertical-align:middle;margin:0px 0px;border:none">](img/SPI-ZeroSync-Overview.jpg)

It follows this order:

1. ASR pulses INT.
2. MCU sends Sync Request.
3. ASR pulses INT.
4. MCU sends Sync Acknowledge.
5. ASR pulses INT.

Now let’s look at the individual messages.

After the MCU receives an interrupt from ASR, the MCU sends a Sync Request message:

[*Click image to enlarge:*
<img src="../img/SPI-ZeroSync-Request.jpg" style="vertical-align:middle;margin:0px 0px;border:none">](img/SPI-ZeroSync-Request.jpg)

As the message is being clocked out, data from ASR is being clocked in. In the example above, the data clocked in exactly matches the data clocked out. The message type and checksum bytes are 0x30, and the two 16-bit byte counts are 0.

Since neither the MCU nor ASR has any bytes to send, the MCU can respond with a Sync Acknowledge.

It first waits for an interrupt from ASR, then sends the Sync Acknowledge message:

[*Click image to enlarge:*
<img src="../img/SPI-ZeroSync-Ack.jpg" style="vertical-align:middle;margin:0px 0px;border:none">](img/SPI-ZeroSync-Ack.jpg)

The Sync Acknowledge looks just like the Sync Request except the message type is 0x31. The MCU ignores the data returned from ASR at this point.

Finally, the MCU waits for a final interrupt from ASR to know the transaction is complete.

At this point the master and slave are in sync. Neither one has anything to send.

Now let’s look at a more realistic example. Let’s look at a Set Attribute transaction, which sets the value of an attribute, then the subsequent Update Attribute transaction.

### Example 2: Set Attribute

In this example, the Afero Cloud is requesting that the MCU change the value of one of its attributes using a Set Attribute transaction. The MCU makes the change and then responds with an Update Attribute transaction. Let’s start with an overview of the Set/Update Attribute transaction as a whole:

[*Click image to enlarge:*
<img src="../img/SPI-SetMCUFromService-Overview.jpg" style="vertical-align:middle;margin:0px 0px;border:none">](img/SPI-SetMCUFromService-Overview.jpg)

The transaction starts with ASR pulsing the interrupt line to let the MCU know there is something to send. The transaction ends with ASR pulsing the interrupt line to signal the operation has completed. There are 13 distinct events here:

**Set Attribute Transaction**

1. ASR pulses INT.
2. MCU sends Sync Request.
3. ASR pulses INT.
4. MCU sends Sync Acknowledge.
5. ASR pulses INT.
6. MCU receives data from ASR.
7. ASR pulses INT.

**Update Attribute Transaction**

1. MCU sends Sync Request.
2. ASR pulses INT.
3. MCU sends Sync Acknowledge.
4. ASR pulses INT.
5. MCU sends data to ASR.
6. ASR pulses INT.

Let’s look at each event in more detail.

**Receive Request**

The transaction begins with ASR pulsing the interrupt to signal the MCU it has something to send. The MCU responds by sending a Sync Request message:

[*Click image to enlarge:*
<img src="../img/SPI-SetMCUFromService-RecvRequest.jpg" style="vertical-align:middle;margin:0px 0px;border:none">](img/SPI-SetMCUFromService-RecvRequest.jpg)

As the MCU is clocking out the Sync Request saying that it has 0 bytes to send, it is clocking in the ASR response, which says it has 9 bytes to send. The MCU sees that ASR wants to send data and responds with a Sync Acknowledge.

**Receive Acknowledge**

The MCU now sends a Sync Acknowledge message that includes the byte count from the ASR Sync Request. The MCU is telling ASR it is ready to receive 9 bytes:

[*Click image to enlarge:*
<img src="../img/SPI-SetMCUFromService-RecvAck.jpg" style="vertical-align:middle;margin:0px 0px;border:none">](img/SPI-SetMCUFromService-RecvAck.jpg)

**Receive Data**

Next, the MCU clocks out 9 bytes of zeros and clocks in the actual Set Attribute command from ASR:

[*Click image to enlarge:*
<img src="../img/SPI-SetMCUFromService-RecvData.jpg" style="vertical-align:middle;margin:0px 0px;border:none">](img/SPI-SetMCUFromService-RecvData.jpg)

Finally, ASR pulses the interrupt line to let the MCU know it is ready to receive data again.

### Example 3: Update Attribute

Now the MCU changes the attribute value locally and then starts an Update Attribute transaction to ASR. As always, it begins with a Sync Request. The MCU message is 11 bytes, so it sends 0x0B in the Sync Request message:

[*Click image to enlarge:*
<img src="../img/SPI-SetMCUFromService-SendRequest.jpg" style="vertical-align:middle;margin:0px 0px;border:none">](img/SPI-SetMCUFromService-SendRequest.jpg)

Next is the Sync Acknowledge message:

[*Click image to enlarge:*
<img src="../img/SPI-SetMCUFromService-SendAck.jpg" style="vertical-align:middle;margin:0px 0px;border:none">](img/SPI-SetMCUFromService-SendAck.jpg)

The Sync Acknowledge message confirms that we are going to send 11 bytes. On the next interrupt from ASR we send the actual data:

[*Click image to enlarge:*
<img src="../img/SPI-SetMCUFromService-SendData.jpg" style="vertical-align:middle;margin:0px 0px;border:none">](img/SPI-SetMCUFromService-SendData.jpg)

After the final interrupt from ASR, the transaction is complete.

### Example 4: Set Attribute with Collision

So now let’s look at what happens when both sides want to send something. Let’s say that at the exact same time the MCU wants to send something to ASR, ASR wants to send something to the MCU. We call this a “collision”. The protocol is designed to handle this case.

The first thing that happens is the MCU sends its Sync Request:

[*Click image to enlarge:*
<img src="../img/SPI-Collision.jpg" style="vertical-align:middle;margin:0px 0px;border:none">](img/SPI-Collision.jpg)

In the example above, the MCU wants to send 10 bytes to ASR. As it clocks out its request, it clocks in the request from ASR that wants to send the MCU 12 bytes. At this point there is a collision. The protocol dictates that whenever there is a collision, the MCU wins. Both the MCU and ASR notice the collision. ASR queues its request and prepares to handle the MCU request. The MCU just waits for the next interrupt from ASR and then resends the Sync Request:

[*Click image to enlarge:*
<img src="../img/SPI-CollisionResync.jpg" style="vertical-align:middle;margin:0px 0px;border:none">](img/SPI-CollisionResync.jpg)

At this point, ASR has queued its request and we are back in sync. After this message, the MCU sends the Sync Acknowledge followed by the data, just like in the examples above.

In general, the protocol is designed so that whenever there is an unexpected result, the MCU can resend the Sync Request to get things back in sync. You may need to send the Sync Request a couple of times, but ASR will eventually respond with the correct result.
