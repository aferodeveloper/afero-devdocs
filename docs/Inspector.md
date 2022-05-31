# INSPECTOR USER GUIDE

The Afero Inspector provides a real-time, in-depth view of the state of each device associated with a given Afero account. The tool is useful for developers when testing and debugging device behavior.

You can open the Inspector by going to [https://inspector.afero.io](https://inspector.afero.io/) then signing in with your Afero developer account.

## The Inspector Home Window

After you sign in, the Inspector Home window appears:

<img src="../img/InspectorMain.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

This window offers you the following information and actions:

- Directly under the Welcome banner is the number of total devices associated with your account and the currently active (online) devices.

- A “card” is shown for each of your associated devices. Within each card you’ll see:

    - Device icon - Can be changed on the Profile Editor’s Device Type window.
  
    - Device’s friendly name - Can be changed in the DEVICE INFORMATION pane of Inspector, on the mobile app, and on the Profile Editor’s Publish window.
    
    - Device ID (alpha-numeric string under the device name) - Click the Device ID to copy the ID to your clipboard.
    
    - Connectivity status - Online or offline.

- View details for any of your devices by selecting the associated card; click the friendly name directly, not the Device ID. (Read more in [Device Details Window](../Inspector#InspectorDeviceDetails).)

- The icons in the upper-right of the window are links to the following:

    <img src="../img/Inspector-HomeIcon.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">- Returns you to this Home window from the Device Details or Account Information windows.
    
    <img src="../img/Inspector-AccountIcon.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">- Opens a window with your account information. (Read more in [Developer Account Information](../Inspector#UserInfo).)
    
    <img src="../img/Inspector-UserGuideIcon.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">- Opens this Inspector User Guide in a new window.
    
    <img src="../img/Inspector-SignOutIcon.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">- Signs you out of Inspector.

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

You can download the information and even edit the READ/WRITE attributes from these windows.

- Click<img src="../img/Inspector-DownloadIcon.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">(there is one on each tab) to download the attribute information to a file in JSON format.

- You can sort on the Attribute ID, Attribute Name, or Last Update columns. Click the associated arrow to select that column for sort (the arrow will turn orange), plus reverse the sort order.

- You can edit READ/WRITE attribute values in real-time when the device is online; these values are indicated with a pencil icon. Click the pencil to edit and update a value:<br>
<img src="../img/Inspector-EditValue.png" width="300" style="vertical-align:middle;margin:0px 0px;border:none">

    Click<img src="../img/Inspector-SaveIcon.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none">to save an edit and update the value, or<img src="../img/Inspector-DismissIcon.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none">to dismiss the edit box with no update. If the value you type is invalid or if your device is offline, you’ll see an error message (in orange text) below the edit value field.

#### Device Attributes

The DEVICE ATTRIBUTES tab details the activity of your device attributes:

| FIELD          | DESCRIPTION                                                  | EXAMPLE                |
| :------------- | :----------------------------------------------------------- | :--------------------- |
| Attribute ID   | Attributes are identified by a 2-byte ID and can contain a variable amount of information based on the type of attribute. Attributes can come in many types and have specific ID ranges. | 1024                   |
| Attribute Name | Name assigned to this attribute. You assign device attributes in the Afero Profile Editor. | LED                    |
| Value          | Current value of the attribute.                              | 1                      |
| Operations     | Operations that have been set for the attribute in the device Profile. | READ/WRITE             |
| Last Update    | Timestamp of when the attribute was last changed.            | Jun 21st 2018, 9:55 AM |

#### Afero System Attributes

The attributes shown in the AFERO SYSTEM ATTRIBUTES tab are generally set by the Cloud and read by the ASR module, but they can also be set by ASR. For a description of these attributes, refer to the [Device Attribute Registry](../AttrRegistry).

### Connection - Wi-Fi/Bluetooth Signal Strength

For the selected device, this pane displays either the Received Signal Strength Indicator (RSSI) signal status for Wi-Fi or, if the device is connected via Bluetooth and a hub, the signal strength (in dBm), as seen by the hub(s).

Click<img src="../img/Inspector-CollapseIcon.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none">to show/hide this pane.

#### Wi-Fi Connection

For devices connected via Wi-Fi, the following information is displayed (see above for illustration):

| FIELD              | DESCRIPTION                                                  | EXAMPLE       |
| :----------------- | :----------------------------------------------------------- | :------------ |
| Wi-Fi SSID         | Name of the Wi-Fi network the device is connected to.        | SHAW-66E150   |
| Wi-Fi Bars         | Wi-Fi signal strength for UI purposes.                       | -48           |
| Wi-Fi Steady State | Wi-Fi connection state when last stable. The value is used to communicate the Wi-Fi state to the applications outside the Wi-Fi setup.<br><br>Possible values are:<br>0 = Not Connected<br>1 = Pending<br>2 = Connected<br>3 = Unknown Failure<br>4 = Association Failed<br>5 = Handshake Failed<br>6 = Echo Failed<br>7 = SSID (Network Name) Not Found<br>8 = NTP (Network Time Protocol) Failed | 2 (Connected) |

#### Bluetooth Connection

For devices connected via Bluetooth and a hub, the RSSI is primarily a function of distance and battery power; but of course there are other interferences, such as refractions, reflections, and scattering. RSSI is expressed in decibels from 0 (zero) to -120db. (Zero being the strongest signal.) Typical values will be between -25 (a few inches away) and -100 (~50 meters away).

| FIELD     | DESCRIPTION                           | EXAMPLE      |
| :-------- | :------------------------------------ | :----------- |
| RSSI      | Signal strength at hub.               | SHAW-66E150  |
| Timestamp | Time of last signal strength reading. | 14:56:06.143 |

### Device Log

This section lists the activity (events) between this device and Afero in real-time.

- You can filter the log contents by typing a search string in the search box<img src="../img/Inspector-SearchBox.png" width="200" style="vertical-align:middle;margin:0px 0px;border:none">. Clear the search results by clicking<img src="../img/Inspector-ClearFilterIcon.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">
- Click<img src="../img/Inspector-DownloadIcon.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">to download the log contents to a file. Note that this file also contains the current RSSI connectivity data.
- Click<img src="../img/Inspector-ClearLogIcon.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">to remove the current log contents.

### Device Information

This section contains details that you have defined for the device in the Afero Profile Editor, as well as system-assigned information, such as device location. Click<img src="../img/Inspector-DownloadIcon.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">to download the device information to a file.

| FIELD              | DESCRIPTION                                                  | EXAMPLE                              |
| :----------------- | :----------------------------------------------------------- | :----------------------------------- |
| Device ID          | Unique alphanumeric string assigned to every Afero Platform device. | 01231732d62ca571                     |
| Friendly Name      | A descriptive name for your device that makes it easy to identify. You can edit the friendly name by clicking the pencil icon to the right of the name. | Cat Tracker                          |
| Profile ID         | Unique alphanumeric string assigned to a device Profile.     | ED2B11B7‑1597‑42E7‑8E0A‑3F1AF92E347D |
| Partner ID         | Unique alphanumeric string assigned to the partner with which the developer is associated. | 3BFFBED9‑D443‑4962‑89D1‑59B4B06E3864 |
| Device Type ID     | Universally unique identifier (UUID) value that maps to a specific Device Type (thermostat, washing machine, camera, etc.). Device Type IDs are unique across the Platform and do not change even if you change a device name. | 0B84D736‑240D‑4D2D‑9696‑34B9DF138F49 |
|                    |                                                              |                                      |
| Latitude/Longitude | Latitude/longitude of the device’s last reported location.   | 48.415876094949 , -123.318191214954  |

## Developer Account Information

To view your account information, click<img src="../img/Inspector-AccountIcon.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none">in the upper-right of the window. Most of the information in this section was provided by you when registering for an Afero developer account:

| FIELD            | DESCRIPTION                                                  | EXAMPLE                                                |
| :--------------- | :----------------------------------------------------------- | :----------------------------------------------------- |
| **USER**         |                                                              |                                                        |
| Name             | The given name and surname you used when registering your Afero developer account. | Cera Smith                                             |
| User ID          | Unique alphanumeric string assigned to you.                  | 47622660‑D8BE‑40CB‑9F7B‑XXXXXXXXXXXX                   |
| Username (email) | Email address that you used when registering your Afero developer account. | csmith@example.io                                      |
| **ACCOUNTS**     |                                                              |                                                        |
| <*Account Name*> | For each Afero account you have, lists the name and ID of the account. | Cera Enterprise - 43facfc9‑aae3‑4fb4‑9ed1‑9c5cfxxxxxxx |
| Type             | The type of account you have.                                | Customer                                               |
| **PARTNERS**     |                                                              |                                                        |
| Partner Name     | Lists any partner names with which you are associated.       | Smart & E-Z Corp                                       |
| Partner ID       | The associated partner ID.                                   | 91812ca6‑69fc‑45b2‑bbbb‑d7112xxxxxxx                   |