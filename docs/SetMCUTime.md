# Using the Afero Cloud to Keep Time on the MCU

ASR provides two different facilities for setting and keeping time on an MCU:

<ul class="af-ul">
	<li>When ASR links to the Afero Cloud, a timestamp and UTC offset data are provided to the MCU via two attributes, and</li>
	<li>If enabled, a periodic timestamp is supplied to the MCU from ASR once it’s connected to the Afero Cloud, also via attribute.</li>
</ul>

The first two attributes sent at link time can be used to set a Real-Time Clock (RTC) on the MCU if there is support for one. If your MCU doesn’t support RTC, you can use the periodic timestamp updates to provide a reasonably accurate clock for your device.

## Time-of-Day Attributes Sent at Link Time

When ASR comes online, it will return two attributes to the MCU. These attributes contain a timestamp and UTC offset data and are always sent. No configuration is required to receive them, although you do have to listen for them in your application’s attribute handler. If you don’t need clock support in your MCU application, you can safely ignore these attributes.

The two attributes are described below:

<ul class="af-ul">
	<li><strong>65015 - <code>LINKED_TIMESTAMP</code> (signed 32-bit long)</strong>
	<p>This is a UNIX Epoch (number of seconds since 00:00:00 1/1/1970) UTC timestamp marking when ASR successfully last linked to the Afero Cloud. The timestamp is returned shortly after ASR reboots and should be reasonably close to actual current time.</p>
	<p>The latency from the Cloud back to the MCU is typically less than 1-2 seconds, but don’t rely on this timestamp being more accurate than +/- 10 seconds (worst case). If your time of day (TOD) requirements are not critical, this is a very convenient way of getting TOD with little work.</p>
	</li>
	<li><strong>65001 - <code>UTC_OFFSET_DATA</code> (byte[8])</strong>
	<p>This byte array contains three pieces of information: the current local timezone offset from UTC, the next UTC offset, and a UNIX Epoch timestamp of when the “next” UTC offset is valid. Specifically:<p>
	<ul class="af-ul-2">
		<li>[0-1] - Little-endian signed int containing the local timezone offset from UTC in minutes</li>
		<li>[2-5] - Little-endian unsigned long containing an Epoch timestamp (UTC) for “next” offset validity</li>
		<li>[6-7] - Little-endian signed int containing the “next” local timezone offset from UTC in minutes</li>
	</ul>
</ul>

<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> UTC Offset is determined by the Location attributes set (by you) for ASR and <strong>are not dynamic</strong> in any way. The UTC Offset can and will be wrong if the Location in the device configuration is incorrect. You can set the correct Location for your Afero device using the Afero mobile app, or by using the Afero Inspector developer tool at <a href="https://inspector.afero.io" target="_blank"> https://inspector.afero.io</a>.</p>
</div>
</div>

Let’s look at examples of these attributes and the meanings of their values. These attribute values:

```
LINKED_TIMESTAMP=0x5a2b06c3
UTC_OFFSET_DATA=0xD4FEF0D3A45A10FF
```

Translate to:

```
Link Time: 0x5a2b06c3 (Fri 2017-12-08 21:40:19 GMT)
Current UTC Offset: -300 minutes (0xFED4) (Eastern Standard Time, in this example)
UTC Offset Change: Sun 2018-03-11 07:00:00 GMT (0x5AA4D3F0) (Spring 2018 USA DST Time Change, 2am local time)
Next UTC Offset: -240 minutes (0xFF10) (Eastern Daylight Time, in this example)
```

Only assume that `LINKED_TIMESTAMP` is current when the state of ASR transitions to a **connected** state. You can use `get_attribute` to return the Linked Timestamp at any point, but remember the result returned will be the time ASR first linked or re-linked to the Cloud, which could be significantly in the past if the device has been connected for a long time. We strongly suggest not querying the `LINKED_TIMESTAMP` attribute and only handle it when it is sent unsolicited to the MCU.

## Periodic Time-of-Day Attribute Updates

After ASR comes online, you can receive a periodic timestamp update from ASR via attribute. This attribute is sent to the MCU every 60 seconds *starting at the first full minute after link time*. This update is disabled by default but you can enable it for your device Profile using the Afero Profile Editor as follows: on the Attributes Definition window, [Define the MCU Attributes](../AttrDef#define-the-mcu-attributes) section, select the <span class="UIText">RECEIVE UTC TIME</span> checkbox. You must enable this attribute on a per-Profile basis, and the feature requires that your device be running firmware Release 2.0.1 or higher.

<ul class="af-ul">
	<li><strong>1201 - <code>ASR_UTC_TIME</code> (signed 32-bit long)</strong>
	<p>This is a UNIX Epoch (number of seconds since 00:00:00 1/1/1970) UTC timestamp, returned by ASR once per minute, starting the next minute (:00 seconds) after the <code>LINKED_TIMESTAMP</code>.</p>
	<p>This timestamp is sent once per minute approximately on the minute (:00 seconds). The current time is synced in ASR via NTP. The timestamp sent to the MCU may be as much as -0/+1 second off, based on processing time and queue handling time within afLib and the MCU.</p>
	</li>
</ul>

## afero_clock Example Code

In [afLib](http://github.com/aferodeveloper/afLib), we’ve included the **`afero_clock`** application, which provides a simple method for:

- Listening for the Linked Timestamp and UTC Offset attributes in `attrEventCallback` under `AF_LIB_EVENT_ASR_NOTIFICATION`.
- Listening for periodic `ASR_UTC_TIME` updates if enabled in your Profile.
- Setting a C time struct that can be manipulated within your application.

The example Profiles provided in the `afero_clock` project enable the `ASR_UTC_TIME` attribute and will display the value when received.

### Example Code Notes

<ul class="af-ul">
	<li>For best results, use an RTC chip and use the Linked Timestamp and UTC Offset attributes to set the RTC. By doing this you’ll have access to a reasonably accurate clock. Remember, the Linked Timestamp is only sent to the MCU when ASR links or re-links to the Afero Cloud.</li>
	<li>The device Profile created using the Afero Profile Editor doesn’t need any specific support for Linked Timestamp or UTC Offset attributes, as they are system attributes and will be presented to the MCU as long as the Afero device Profile has MCU support <strong>enabled</strong>. However, in order to receive the periodic <code>ASR_UTC_TIME</code> attribute updates, you must have selected the <span class="UIText">RECEIVE UTC TIME</span> checkbox for your device Profile (using the Profile Editor, Attribute Definition window, MCU Configuration section).</li>
	<li>The example code provided in afLib includes some code that protects against accepting <code>LINKED_TIMESTAMP</code> when an MCU application requests that data via <code>get_attribute</code>. Since the value returned is the timestamp of when ASR last linked to the Afero Cloud, this value will not update except when ASR re-links its connection. If this level of paranoia isn’t needed, all references and checks to “timestamp_read” can be removed.</li>
	<li>When ASR first boots, it will send an <code>SYSTEM_UTC_OFFSET_DATA</code> attribute update to the MCU; however, all data in this attribute will be zero. Since UTC Offset=0 is valid, you should check for the value of the UTC Offset Change Timestamp (the middle four bytes of the attribute value). If this timestamp is zero then the UTC Offset data is invalid and can be ignored. Once ASR links, you will receive the Linked Timestamp attribute and another UTC Offset attribute with valid data. In the case of a device with no Location or local timezone defined, the <code>UTC_OFFSET_DATA</code> attribute will remain all zeroes and can be safely ignored.</li>
	<li>It is possible to receive a new <code>SYSTEM_UTC_OFFSET_DATA</code> attribute update at any time (for example, the passing of a Daylight Saving Time boundary). As long as the Offset Change Time Timestamp is greater than the Linked Timestamp (or the Current Timestamp if you have an RTC), then the update data should be considered valid and the RTC updated accordingly (typically by subtracting the “old” offset and adding the “new” offset to re-adjust the clock to the new timezone).</li>
	<li>In C/C++, the <code>strftime()</code> call can be used to format either timestamp into human-readable forms.</li>
	<li>Data types for the <code>timestamp</code> attribute values are:
		<ul class="af-ul-2">
			<li>int32_t <code>linked_timestamp</code></li>
			<li>int16_t <code>utc_offset_now</code></li>
			<li>int16_t <code>utc_offset_next</code></li>
			<li>int32_t <code>utc_offset_change_time</code></li>
			<li>int32_t <code>asr_utc_time</code></li>
		</ul>
	</li>
</ul>
	
