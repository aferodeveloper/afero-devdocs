# Device Attribute Registry

Each Afero device will have a number of attributes associated with it. Attributes are identified by a 2-byte ID and can contain a variable amount of information based on the type of attribute. Attributes can come in many types and have specific ID ranges.

<div>
<table class="af-table-borders">
<thead>
<tr>
<th>ID</th>
<th>Description</th>
<th>Data&nbsp;Type</th>
<th>Usage Notes</th>
</tr>
</thead>
<tbody>
<tr>
<td>1&#8209;1023</td>
<td>Cloud, Device MCU</td>
<td></td>
<td>An attribute residing on an external MCU defined by the third-party developer. The size and meaning of these attributes are defined by the developer using the Afero Profile Editor. To get the default value of one of these attributes, the MCU must call <code>af_lib_get_attribute</code>.</td>
</tr>
<tr>
<td>1024</td>
<td>I/O 0 Value Attribute</td>
<td></td>
<td>LED on Afero dev board.</td>
</tr>
<tr>
<td>1026</td>
<td>I/O 1 Value Attribute</td>
<td></td>
<td>Configurable</td>
</tr>
<tr>
<td>1028</td>
<td>I/O 2 Value Attribute</td>
<td></td>
<td>Configurable</td>
</tr>
<tr>
<td>1030</td>
<td>I/O 3 Value Attribute</td>
<td></td>
<td>Button on Afero dev board</td>
</tr>
<tr>
<tr>
<td>1201</td>
<td>UTC Time</td>
<td>SINT32</td>
<td>Local UTC time on ASR should the MCU need/want it. If present in the Profile, ASR will deliver it to the MCU periodically.</td>
</tr>
<tr>
<td>1202</td>
<td>Device ID</td>
<td>BYTES</td>
<td>If the MCU requires the ASR Device ID, the MCU can do a GET on this attribute to retrieve the ID.</td>
</tr>
<tr>
<td>1203</td>
<td>Association ID</td>
<td>BYTES</td>
<td>Should the MCU require the Association ID of the ASR it can do a GET on this attribute to retrieve it.</td>
</tr>
<tr>
<td>1204</td>
<td>Company Code</td>
<td>SINT8</td>
<td>Should the MCU require the Company Code of the ASR it can do a GET on this attribute to retrieve it.</td>
</tr>
<tr>
<td>1205</td>
<td>Online Status</td>
<td>BYTES</td>
<td>This three-byte array describes the online status of the device according to the following formula:
<ul class="af-ul no-bullet">
<li>First byte: 0 = Offline, 1 = Online</li>
<li>Second byte: 1 = BLE, 2 = Wi-Fi</li>
<li>Third byte: Number of Wi-Fi bars (if second byte is 2)</li>
</ul>
</td>
</tr>
<tr>
<td>1206</td>
<td>Device Capabilities</td>
<td>BYTES</td>
<td>A bit field in which the device states to the MCU what capabilities it supports. Bits are indexed from left to right, allowing additional bytes to be appended to support additional capabilities. The bits are:
<ul class="af-ul no-bullet">
<li>Bit 0: MCU OTA Support</li>
</ul>
</td>
</tr>
<tr>
<td>1207</td>
<td>afLib Capabilities</td>
<td>BYTES</td>
<td>A bit field in which afLib states to ASR what capabilities it supports. Bits are indexed from left to right, allowing additional bytes to be appended to support additional capabilities.</td>
</tr>
<tr>
<td>1301</td>
<td>OTA MCU Info</td>
<td>BYTES</td>
<td>N-byte array used to hold state and other information related to the OTA.</td>
</tr>
<tr>
<td>1302</td>
<td>OTA MCU Transfer</td>
<td>BYTES</td>
<td>N-byte array used for transmitting the image data to the MCU. By default this will be 510 bytes for Modulo-1 and 2046 bytes for Modulo-2, but the MCU can limit the amount of data it receives each time by lowering the size of this attribute.</td>
</tr>
<!-- Two attributes for media upload to Cloud
<tr>
<td>1303</td>
<td>Media Upload Request</td>
<td>BYTES</td>
<td>Contains the mimetype, local ID, SHA-256 of a data file that the MCU wants to upload to the Cloud.</td>
</tr>
<tr>
<td>1304</td>
<td>Media Upload Response</td>
<td>BYTES</td>
<td>Contains the response status code, SHA-256 of the data file to upload, URL, and byte offset (for resuming a failed upload).</td>
</tr> -->
<tr>
<td>2001</td>
<td>Bootloader Version</td>
<td>SINT64</td>
<td>The bootloader is always the first thing to be upgraded as it is responsible for installing firmware images. This firmware type is used for the Afero Secure Hub and Afero Secure Radios.</td>
</tr>
<tr>
<td>2002</td>
<td>BLE Stack Version</td>
<td>SINT64</td>
<td>This is the Bluetooth stack for the radio module. This will likely never be updated, but  is included here for completeness. If the stack is updated, the application must always be updated at the same time, before the device is rebooted.</td>
</tr>
<tr>
<td>2003</td>
<td>FW Application Version</td>
<td>SINT64</td>
<td>This is the radio module application responsible for reading the device description and handling attributes. This firmware type is used for the Afero Secure Hub and Afero Secure Radios.</td>
</tr>
<tr>
<td>2004</td>
<td>Device Description</td>
<td>SINT64</td>
<td>The description of the attributes for a particular Afero application.</td>
</tr>
<tr>
<td>2005</td>
<td>Hub</td>
<td>SINT64</td>
<td>This is the entire firmware package for the hub including the software that runs on the Freescale processor. The Freescale package is signed separately using the same firmware type id, and then is included in the OpenWrt package. At boot time, only one version number is reported for the entire firmware package.</td>
</tr>
<tr>
<td>2006</td>
<td>Wi-Fi</td>
<td>SINT64</td>
<td>This is the version for the Wi-Fi chip (e.g., WINC3400).</td>
</tr>
<tr>
<td>2007</td>
<td>Wi-Fi Certificates</td>
<td>SINT64</td>
<td>This is the package of Wi-Fi certificates used by the Wi-Fi chip to talk to the Cloud. This includes specific server certificates for Conclave and OTA servers as well as their root certificates. There is only one slot for this image type and the binary image is signed on the way to the device.</td>
</tr>
<tr>
<td>2008</td>
<td>WAN APN List</td>
<td>SINT64</td>
<td>This package contains a single file that comprises all the different WAN APNs the device supports.</td>
</tr>
<tr>
<td>65001</td>
<td>UTC offset data</td>
<td>BYTES</td>
<td>Contains the current UTC offset in minutes, the next timestamp to change the UTC offset, and the new UTC offset to apply at the previous timestamp, in that order (2-bytes signed short, 4-bytes int, 2-bytes signed short).</td>
</tr>
<tr>
<td>65004</td>
<td>Configured SSID</td>
<td>UTF8S</td>
<td>The SSID the Afero Secure Hub is currently configured to use.</td>
</tr>
<tr>
<td>65005</td>
<td>Wi-Fi Bars</td>
<td>SINT8</td>
<td>Integer, read-only attribute for Wi-Fi signal strength, for UI purposes.</td>
</tr>
<tr>
<td>65006</td>
<td>Wi-Fi Steady State</td>
<td>SINT8</td>
<td>Wi-Fi steady state: 
<ul class="af-ul no-bullet">
<li>0 = Not connected</li>
<li>1 = Pending</li>
<li>2 = Connected</li>
<li>3 = Unknown failure</li>
<li>4 = Association failed</li>
<li>5 = Handshake failed</li>
<li>6 = Echo failed</li>
<li>7 = SSID (Network Name) Not found</li>
<li>8 = NTP (Network Time Protocol) failed
</ul>
<p>This value is used to communicate the Wi-Fi state to the apps outside the Wi-Fi setup.</p></td>
</tr>
<tr>
<td>65012</td>
<td>Command</td>
<td>BYTES</td>
<td><p>This value will have a one-byte command followed by parameters, if any. The format of the last three bytes are dictated by the first byte. The device will return a two-byte array indicating the result of the command to the server. The first byte will be the command and the second byte will be the result:</p>
<ul class="af-ul">
<li>NO_ERROR = 0</li>
<li>COMMAND_FAILED = 0xFF</li>
<li>NOT_SUPPORTED = 0xFE, PARSE_ERROR = 0xFD</li>
</ul> 
<p>Current commands are listed below. Note that not all commands are available on all devices.</p>
<ul class="af-ul no-bullet">
<li>1 = Reboot - Last three bytes are ignored.</li>
<li>2 = Clear user data - Factory reset; last three bytes are ignored.</li>
<li>3 = Enter factory test mode (i.e., softAP).</li>
<li>4 = Enter factory mode (i.e., profiprogo).</li>
<li>5 = Peripheral list change - The list of peripherals on the account has changed. The format of the parameters is [modification][device id length][device id], where:
<ul class="af-ul">
<li>[modification] (1 byte): 1 for added, 0 for removed</li>
<li>[device id length] (1 byte): Length of device ID</li>
<li>[device id] (device id length bytes): Device ID</li>
</ul>
</li>
<li>6 = Device view state change - Somebody has either started/stopped viewing a device. The format of the parameters is [viewing refresh time][viewer id length][viewer id][device id length][device id], where:
<ul class="af-ul">
<li>[viewing refresh time] (2 bytes): 0 for not viewing; otherwise, the time in seconds that viewing should be true</li>
<li>[viewer id length] (1 byte): Length of viewer ID</li>
<li>[viewer id] (viewer id length bytes): Viewer ID</li>
<li>[device id length] (1 byte): Length of device ID</li>
<li>[device id] (device id length bytes): Device ID</li>
</ul>
<li>7 = Claim state change - Somebody has either started/stopped claiming a device. The format of the parameters is [claim refresh time][claimant id length][claimant id][device id length][device id], where:
<ul class="af-ul">
<li>[claim refresh time] (2 bytes): 0 for disclaim; otherwise, the time in seconds that the claim should be true</li>
<li>[claimant id length] (1 byte): Length of claimant ID</li>
<li>[claimant id] (claimant id length bytes): Claimant ID</li>
<li>[device id length] (1 byte): Length of device ID</li>
<li>[device id] (device id length bytes): Device ID</li>
</ul>
<li>8 = Start/stop peripheral mode: Either start/stop advertising over BLE. The format of the parameters is [mode], where:
<ul class="af-ul">
<li>[mode] (1 byte): 1 to start advertising, 0 to stop advertising</li>
</ul>
<li>9 = Enable/disable Wi-Fi: Either enable/disable the use of Wi-Fi on a device. The format of the parameters is [mode], where:
<ul class="af-ul">
<li>[mode] (1 byte): 1 to enable, 0 to disable</li>
</ul>
</li>
<li>A = Upload hub logs to the media service.</li>
</ul>
</td>
</tr>
<tr>
<td>65013</td>
<td>ASR State (AF_SYSTEM_ASR_STATE)</td>
<td>SINT8</td>
<td>This byte will hold ASR state information, including:
<ul class="af-ul no-bullet">
<li>0 = Rebooted (AF_MODULE_STATE_REBOOTED)</li>
<li>1 = Linked (AF_MODULE_STATE_LINKED)</li>
<li>2 = Updating (AF_MODULE_STATE_UPDATING)</li>
<li>3 = Update ready to apply (reboot requested) (AF_MODULE_STATE_UPDATE_READY)</li>
<li>4 = Initialized (AF_MODULE_STATE_INITIALIZED)</li>
<li>5 = Re-linked (AF_MODULE_STATE_RELINKED)</li>
<li>6 = Factory Reset (MODULE_STATE_FACTORY_RESET)</li>
</ul>
</td>
</tr>
<td>65014</td>
<td>Low Battery Warning</td>
<td>SINT8</td>
<td><p><em>Applies to ASR-1 Only</em></p>
<p>This value will be updated by ASR-1 when the battery gets low. It will track the battery level as:</p>
<ul class="af-ul no-bullet">
<li>0: voltage &gt;2.7V</li>
<li>1: 2.5V &lt;voltage&gt; 2.7V</li>
<li>2: 2.3V &lt;voltage&gt; 2.5V</li>
<li>3: 2.3V &lt;voltage&gt; 2.1V</li>
</ul>
<p>At 2.1 volts, the ASR-1 will power down to avoid problems such as flash corruption.</p>
</td>
</tr>
<tr>
<td>65015</td>
<td>Linked Timestamp</td>
<td>SINT32</td>
<td>The timestamp when the peripheral linked with the Cloud (as set by the Cloud upon successful linking). This is a Unix Epoch timestamp, which is the number of seconds since 1/1/1970.</td>
</tr>
<tr>
<td>65018</td>
<td>Attribute ACK</td>
<td>SINT16</td>
<td>Reserved. <!--Latch attribute update is ACKed by sending a SET to this attribute ID with a value of the latch attribute’s ID.--></td>
</tr>
<tr id="RebootReason">
<td>65019</td>
<td>Reboot Reason</td>
<td>UTF8S</td>
<td><p>A comma-delimited string that always has the same number of fields: 23 in the current version (V1). The reboot reason code appears in the second field; possible code  values are explained below:</p>
<ul class="af-ul no-bullet">
<li>0 = Power on</li>
<li>1 = Watchdog reset</li>
<li>2 = Software reset</li>
<li>3 = Reset pin asserted</li>
<li>4 = Crystal failure</li>
<li>5 = Brownout</li>
<li>240 = Other</li>
<li>255 = Unknown</li>
</ul>
</td>
</tr>
<tr>
<td>65020</td>
<td>BLE Comms</td>
<td>BYTES</td>
<td>Reserved. <!--Byte array of BLE communication parameters. Format of each parameter is little endian hex.
<ul class="af-ul no-bullet">
<li>Bytes 0-1: Advertising Interval: Range 20-10240, Default 0xFA00 (250)</li>
<li>Bytes 2-3: Advertising Timeout: Range 0-16383, Default 0x0000 (0)</li>
<li>Bytes 4-5: Min Conn Interval: Range 8 - 4000, Default 0x1800 (24)</li>
<li>Bytes 6-7: Max Conn Interval: Range 8 - 4000, Default 0x1800 (24)</li>
<li>Bytes 8-9: Slave Latency: Range 0 - 1000, Default 0x0C00 (12)</li>
<li>Bytes 10-11: Conn Sup Timeout: Range 100-32000, Default 0xA00F (4000)</li>
</ul>-->
</td>
</tr>
<tr>
<td>65021</td>
<td>MCU Interface</td>
<td>SINT8</td>
<td>This attribute is required in the profile. The default value for this attribute is set by APE and any changes to the value at runtime are ignored. The following values are valid:
<ul class="af-ul no-bullet">
<li>0 = No MCU</li>
<li>1 = SPI SLAVE</li>
<li>2 = UART</li>
<!-- for fw r2.3				<li>3 = afLib</li> -->
</ul>
</td>
</tr>
<tr>
<td>65066</td>
<td>Device Capability</td>
<td>BYTES</td>
<td><p>A bit field in which the device states what capabilities it supports. The bits are:</p>
<div class="af-table-nostyle">
<table>
<tr class="underline">
<td><strong>Bit&nbsp;Index</strong></td>
<td><strong>Name</strong></td>
<td><strong>Description</strong></td>
</tr>
<tr>
<td>0</td>
<td>COMMAND_ACK</td>
<td>Command Attribute sets are acknowledged with command response.</td>
</tr>
<tr>
<td>1</td>
<td>VIEW</td>
<td>Accepts view command attribute values.</td>
</tr>
<tr>
<td>2</td>
<td>ADD_REMOVE_DEVICE</td>
<td>Accepts add/remove peripheral command attribute values.</td>
</tr>
<tr>
<td>3</td>
<td>PERIPHERAL_MODE</td>
<td>Accepts peripheral mode enable/disable command attribute values.</td>
</tr>
<tr>
<td>4</td>
<td>WIFI_MODE</td>
<td>Accepts Wi-Fi enable/disable command attribute values.</td>
</tr>
<tr>
<td>5</td>
<td>CLAIM</td>
<td>Accepts claim command attribute values.</td>
</tr>
<tr>
<td>6</td>
<td>ROUTER</td>
<td>Can route messages to other devices when visible.</td>
</tr>
<!-- for fwR2.2						<tr>
<td>7</td>
<td>ENTERPRISE</td>
<td>Device supports Enterprise functionality.</td>
</tr> -->
</table>
</div>
</td>
</tr>
<tr>
<td>65067</td>
<td>Rate Limit Config</td>
<td>BYTES</td>
<td>Optional list of rate limit configs used by the attribute store to rate limit how often a given attribute is updated when being either viewed or not viewed. An individual config is six (6) bytes long and has the following format: [attribute][viewing update interval][non viewing update interval], where:
<ul class="af-ul">
<li>[attribute] (2 bytes): Attribute to which the following rate limits apply</li>
<li>[viewing update interval] (2 bytes): Viewing update interval in seconds</li>
<li>[non viewing update interval] (2 bytes): Non viewing update interval in seconds</li>
</ul>
<p>The behavior is as follows: For a given viewing state, the attribute store will enforce that the given attribute will not be updated more frequently than the value specified in the config. The first update after a reboot and any responses to server SET messages will be exempt from this policy.</p></td>
</tr>
<tr>
<td>65068</td>
<td>Queued Attributes Config</td>
<td>BYTES</td>
<td>This is an optional list of queued attribute configs used by the attribute store to queue a given attribute instead of overwriting when offline. An individual config is five (5) bytes long and has the following format: <code>[attribute][queue policy][queue size]</code>, where:
<ul class="af-ul">
<li>[attribute] (2 bytes): Attribute to which the following rate limits apply.</li>
<li>[queue policy] (1 byte): Queue order and replacement policy. (See <a id="1555364880.52" href="/AttrDef#all-about-queuing">All About Queuing</a>.)</li>
<li>[queue size] (2 bytes): Maximum number of elements.</li>
</ul>
</tr>
<tr>
<td>65069</td>
<td>MCU SPI Config</td>
<td>BYTES</td>
<td>Binary byte array with the following formatted values (in little endian hex):
<ul class="af-ul">
<li>Bytes 0-1:  Maximum Tx/Rx SPI transfer size: Range 1-2048, Default 0xFF (255).</li>
<li>Bytes 2-5: Start transfer delay in microseconds: range 1-1,000,000, Default: 0x0FA0(4000).</li>
<li>Bytes 6-9: Pulse width (or Interrupt pulse interval) in microseconds: Range 1 - 1,000, 000, Default 0x2710 (10000).</li>
</ul>
</td>
</tr>
</tbody>
</table>
</div>
