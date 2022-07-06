# INSPECTOR USER GUIDE

The Afero Inspector provides a real-time, in-depth view of the state of each device associated with a given Afero account. The tool is useful for developers when testing and debugging device behavior.

You can open the Inspector by going to [https://inspector.afero.io](https://inspector.afero.io/) then signing in with your Afero developer account.

## The Inspector Home Window

After you sign in, the Inspector Home window appears:

<img src="../img/InspectorMain.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

This window offers you the following information and actions:

<ul class="af-ul">
	<li>Directly under the Welcome banner is the number of total devices associated with your account and the currently active (online) devices.</li>
	<li>A “card” is shown for each of your associated devices. Within each card you’ll see:</li>
	<ul class="af-ul-2">
		<li>Device icon - Can be changed on the Profile Editor’s Device Type window.</li>
		<li>Device’s friendly name - Can be changed in the <span class="UIText">Device Information</span> pane of Inspector, on the mobile app, and on the Profile Editor’s Publish window.</li>
		<li>Device ID (alpha-numeric string under the device name) - Click the Device ID to copy the ID to your clipboard.</li>
		<li>Connectivity status - Online or offline.</li>
	</ul>
	<li>View details for any of your devices by selecting the associated card; click the friendly name directly, not the Device ID. (Read more in <a id="1541707823.53" href="#InspectorDeviceDetails">Device Details Window</a>.)</li>
	<li>The icons in the upper-right of the window are links to the following:
		<ul class="af-ul no-bullet">
			<li><img class="img-inline" src="../img/Inspector-HomeIcon.png" width="25" alt="Inspector Home Window Icon" >- Returns you to this Home window from the Device Details or Account Information windows.</li>
			<li><img class="img-inline" src="../img/Inspector-AccountIcon.png" width="25" alt="Inspector Account Icon">- Opens a window with your account information. (Read more in <a id="1491415625.45" href="#UserInfo">Developer Account Information</a>.)</li>
			<li><img class="img-inline" src="../img/Inspector-UserGuideIcon.png" width="25" alt="Inspector User Guide Icon">- Opens this Inspector User Guide in a new window.</li>
			<li><img class="img-inline" src="../img/Inspector-SignOutIcon.png" width="25" alt="Inspector Sign Out Icon">- Signs you out from Inspector.</li>
		</ul>
	</li>
</ul>

## Device Details Window

After you select a device card in the Home window, that device’s Details window opens. To look at the details for a different device, use the left-hand Navigation pane, where all the devices associated with your account are listed, identified by their friendly names. Devices with an orange dot are online; devices with a greyed-out dot are offline.

<img src="../img/Inspector-DeviceDetails.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

The selected device details are presented in the following panes:

- [Attribute Tabs](../Inspector#attribute-tabs)
- [Connection - Wi-Fi/Bluetooth Signal Strength](../Inspector#connection-wi-fibluetooth-signal-strength)
- [Device Log](../Inspector#device-log)
- [Device Information](../Inspector#device-information)

### Attribute Tabs

The attributes relevant to the selected device are displayed in two tabs: [Device Attributes](../Inspector#DeviceAttr) and [Afero System Attributes](../Inspector#SystemAttr).

You can download the information and even edit the <span class="UIText">READ/WRITE</span> attributes from these windows.

<ul class="af-ul">
	<li>Click<img class="img-inline" src="../img/Inspector-DownloadIcon.png" width="25" alt="Inspector Download Icon">(there is one on each tab) to download the attribute information to a file in JSON format.</li>
	<li>You can sort on the Attribute ID, Attribute Name, or Last Update columns. Click the associated arrow to select that column for sort (the arrow will turn orange), plus reverse the sort order.</li>
	<li>You can edit READ/WRITE attribute values in real-time when the device is online; these values are indicated with a pencil icon. Click the pencil to edit and update a value:<br>
	<img src="../img/Inspector-EditValue.png" alt="Edit Attribute Value" width="400"><br>
	Click<img class="img-inline" src="../img/Inspector-SaveIcon.png" alt="Save Icon">to save an edit and update the value, or<img class="img-inline" src="../img/Inspector-DismissIcon.png" alt="Dismiss Icon">to dismiss the edit box with no update. If the value you type is invalid or if your device is offline, you’ll see an error message (in orange text) below the edit value field.</li>
</ul>


#### Device Attributes

The <span class="UIText">DEVICE ATTRIBUTES</span> tab details the activity of your device attributes:

<div class="af-table-borders">
	<table class="af-table-borders">
		<thead>
			<tr>
				<th>Field</th>
				<th>Description</th>
				<th>Example</th>
			</tr>
		</thead>
		<tbody>
			<tr>
				<td>Attribute ID</td>
				<td>Attributes are identified by a 2-byte ID and can contain a variable amount of information based on the type of attribute. Attributes can come in many types and have specific ID ranges.</td>
				<td>1024</td>
			</tr>
			<tr>
				<td>Attribute Name</td>
				<td>Name assigned to this attribute. You assign device attributes in the  Afero Profile Editor.</td>
				<td>LED</td>
			</tr>
			<tr>
				<td>Value</td>
				<td>Current value of the attribute.</td>
				<td>1</td>
			</tr>
			<tr>
				<td>Operations</td>
				<td>Operations that have been set for the attribute in the device Profile.</td>
				<td>READ/WRITE</td>
			</tr>
			<tr>
				<td>Last Update</td>
				<td>Timestamp of when the attribute was last changed.</td>
				<td>Jun 21st 2018, 9:55 AM</td>
			</tr>
		</tbody>
	</table>
</div>


#### Afero System Attributes

The attributes shown in the <span class="UIText">AFERO SYSTEM ATTRIBUTES</span> tab are generally set by the Cloud and read by the ASR module, but they can also be set by ASR. For a description of these attributes, refer to the [Device Attribute Registry](../AttrRegistry).

### Connection - Wi-Fi/Bluetooth Signal Strength

For the selected device, this pane displays either the Received Signal Strength Indicator (RSSI) signal status for Wi-Fi or, if the device is connected via Bluetooth and a hub, the signal strength (in dBm), as seen by the hub(s).

Click<img src="../img/Inspector-CollapseIcon.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none">to show/hide this pane.

#### Wi-Fi Connection

For devices connected via Wi-Fi, the following information is displayed (see above for illustration):

<div class="af-table-borders">
	<table>
		<thead>
			<tr>
				<th>Field</th>
				<th>Description</th>
				<th>Example</th>
			</tr>
		</thead>
		<tbody>
			<tr>
				<td>Wi-Fi SSID</td>
				<td>Name of the Wi-Fi network the device is connected to.</td>
				<td>SHAW-66E150</td>
			</tr>
			<tr>
				<td>Wi-Fi Bars</td>
				<td>Wi-Fi signal strength for UI purposes.</td>
				<td>-48</td>
			</tr>
			<tr>
				<td>Wi-Fi Steady State</td>
				<td><p>Wi-Fi connection state when last stable. The value is used to communicate the Wi-Fi state to the applications outside the Wi-Fi setup.</p>
				<p>Possible values are:</p>
				<ul class="af-ul no-bullet">
					<li>0 = Not Connected</li>
					<li>1 = Pending</li>
					<li>2 = Connected</li>
					<li>3 = Unknown Failure</li>
					<li>4 = Association Failed</li>
					<li>5 = Handshake Failed</li>
					<li>6 = Echo Failed</li>
					<li>7 = SSID (Network Name) Not Found</li>
					<li>8 = NTP (Network Time Protocol) Failed
				</ul>
				</td>
				<td>2&nbsp;(Connected)</td>
			</tr>
		</tbody>
	</table>
</div>

#### Bluetooth Connection

For devices connected via Bluetooth and a hub, the RSSI is primarily a function of distance and battery power; but of course there are other interferences, such as refractions, reflections, and scattering. RSSI is expressed in decibels from 0 (zero) to -120db. (Zero being the strongest signal.) Typical values will be between -25 (a few inches away) and -100 (~50 meters away).

<div class="af-table-borders">
	<table class="af-table-borders">
		<thead>
			<tr>
				<th>Field</th>
				<th>Description</th>
				<th>Example</th>
			</tr>
		</thead>
		<tbody>
			<tr>
				<td>RSSI</td>
				<td>Signal strength at hub.</td>
				<td>SHAW-66E150</td>
			</tr>
			<tr>
				<td>Timestamp</td>
				<td>Time of last signal strength reading.</td>
				<td>14:56:06.143</td>
			</tr>
		</tbody>
	</table>
</div>

### Device Log

This section lists the activity (events) between this device and Afero in real-time.

<ul class="af-ul">
	<li>Filter the log contents by typing a search string in the search box<img class="img-inline" src="../img/Inspector-SearchBox.png" width="150" alt="Inspector Log Search Box">.</li>
	<li>Clear the search results by clicking<img class="img-inline" src="../img/Inspector-ClearFilterIcon.png" width="25" alt="Inspector Clear Filter Icon">.</li> 
	<li>Click<img class="img-inline" src="../img/Inspector-DownloadIcon.png" width="25" alt="Inspector Download Icon">to download the log contents to a file. Note that this file also contains the current RSSI connectivity data.</li>
	<li>Click<img class="img-inline" src="../img/Inspector-ClearLogIcon.png" width="25" alt="Inspector Clear Log Icon">to remove the current log contents.</li>
</ul>

### Device Information

This section contains details that you have defined for the device in the Afero Profile Editor, as well as system-assigned information, such as device location. Click<img src="../img/Inspector-DownloadIcon.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none">to download the device information to a file.

<div class="af-table-borders">
	<table class="af-table-borders">
		<thead>
			<tr>
				<th>Field</th>
				<th>Description</th>
				<th>Example</th>
			</tr>
		</thead>
		<tbody>
			<tr>
				<td>Device ID</td>
				<td>Unique alphanumeric string assigned to every Afero Platform device. </td>
				<td>01231732d62ca571</td>
			</tr>
			<tr>
				<td>Friendly Name</td>
				<td>A descriptive name for your device that makes it easy to identify. You can edit the friendly name by clicking the pencil icon to the right of the name.</td>
				<td>Cat Tracker</td>
			</tr>
			<tr>
				<td>Profile ID</td>
				<td>Unique alphanumeric string assigned to a device Profile.</td>
				<td>ED2B11B7&#8209;1597&#8209;42E7&#8209;8E0A&#8209;3F1AF92E347D</td>
			</tr>
			<tr>
				<td>Partner ID</td>
				<td>Unique alphanumeric string assigned to the partner with which the developer is associated.</td>
				<td>3BFFBED9&#8209;D443&#8209;4962&#8209;89D1&#8209;59B4B06E3864</td>
			</tr>
			<tr>
				<td>Device Type ID</td>
				<td>Universally unique identifier (UUID) value that maps to a specific Device Type (thermostat, washing machine, camera, etc.). Device Type IDs are unique across the Platform and do not change even if you change a device name.</td>
				<td>0B84D736&#8209;240D&#8209;4D2D&#8209;9696&#8209;34B9DF138F49</td>
			</tr>
			<tr>
<!--			<td>Last Reported</td>
				<td>Date when device was last online.</td>
				<td>September 19th 2017, 5:03 PM</td>
			</tr> -->
			<tr>
				<td>Latitude/Longitude</td>
				<td>Latitude/longitude of the device’s last reported location.</td>
				<td>48.415876094949 , -123.318191214954</td>
			</tr>
		</tbody>
	</table>
</div>


## Developer Account Information

To view your account information, click<img src="../img/Inspector-AccountIcon.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none">in the upper-right of the window. Most of the information in this section was provided by you when registering for an Afero developer account:

<div class="af-table-borders">
	<table class="af-table-borders">
		<thead>
			<tr>
				<th>Field</th>
				<th>Description</th>
				<th>Example</th>
			</tr>
		</thead>
		<tbody>
			<tr>
				<td colspan="3" bgcolor="lightgrey"><strong>USER</strong></td>
			</tr>
			<tr>
				<td>Name</td>
				<td>The given name and surname you used when registering your Afero developer account.</td>
				<td>Cera Smith</td>
			</tr>
			<tr>
				<td>User ID</td>
				<td>Unique alphanumeric string assigned to you.</td>
				<td>47622660&#8209;D8BE&#8209;40CB&#8209;9F7B&#8209;XXXXXXXXXXXX</td>
			</tr>
			<tr>
				<td>Username (email)</td>
				<td>Email address that you used when registering your Afero developer account.</td>
				<td>csmith@example.io</td>
			</tr>
			<tr>
				<td colspan="3" bgcolor="lightgrey"><strong>ACCOUNTS</strong></td>
			</tr>
			<tr>
				<td>&lt;<em>Account&nbsp;Name</em>&gt;</td>
				<td>For each Afero account you have, lists the name and ID of the account.</td>
				<td>Cera Enterprise - 43facfc9&#8209;aae3&#8209;4fb4&#8209;9ed1&#8209;9c5cfxxxxxxx</td>
			</tr>
			<tr>
				<td>Type</td>
				<td>The type of account you have.</td>
				<td>Customer</td>
			</tr>
			<tr>
				<td colspan="3" bgcolor="lightgrey"><strong>PARTNERS</strong></td>
			</tr>
			<tr>
				<td>Partner Name</td>
				<td>Lists any partner names with which you are associated.</td>
				<td>Smart &amp; E-Z Corp</td>
			</tr>
			<tr>
				<td>Partner ID</td>
				<td>The associated partner ID.</td>
				<td>91812ca6&#8209;69fc&#8209;45b2&#8209;bbbb&#8209;d7112xxxxxxx</td>
			</tr>						
		</tbody>
	</table>
</div>
			