# Wi-Fi Station Daemon Implementation

The Wi-Fi Station daemon (wifistad) is event-based and designed to manage the Wi-Fi setup. This daemon allows the user to configure a Wi-Fi connection to an AP with a mobile app.

This page contains the following sections:

- [Platform Dependency](../LinuxSDK-WiFiStDaemon#platform-dependency)
- [Event-Driven Design](../LinuxSDK-WiFiStDaemon#event-driven-design)
- [Wi-Fi Station Daemon System Components](../LinuxSDK-WiFiStDaemon#wi-fi-station-daemon-system-components)

## Platform Dependency

wifistad has network-related, platform-specific dependencies, but wifistad is not responsible for proper network configuration or setup. In order for Wi-Fi networking to work properly, the following networking functionality must be enabled:

- wpa_supplicant
- DNS
- IP route management

In this section, we provide some examples of how this network functionality can be achieved; however, these examples are not meant to be exhaustive or comprehensive. It is important that the network functionality work seamlessly to provide the desired Wi-Fi connectivity.

It is expected that the wpa_supplicant be invoked by the system. Where and how this is done is platform-dependent. For example, on Raspberry Pi 3, you can use `/etc/network/interface` to automatically start the wpa_supplicant using the “auto” keyword:

```
#Wireless interfaces
auto wlan0
iface wlan0 inet dhcp
wireless_mode managed
wireless_essid any
wpa-driver nl80211
wpa-conf /etc/wpa_supplicant.conf
```

DNS functionality is essential for the device to work properly. Again, there are different ways to achieve the DNS functionality, such as dynamically leasing an IP or DNS lookup. For example, in Raspberry Pi 3, one can use udhcpc to provide the DNS functionality. Specify dhcp in the iface keyword statement in the `/etc/network/interface` file to automatically enable the interface to use DHCP.

When the device connects to Wi-Fi and dynamically gets an IP through DHCP, a route must be added to the route table.

## Event-Driven Design

The daemon has the following event types:

- daemon events - WIFISTAD_EVENT_XXX, to be used for wifistad, to manage Wi-Fi AP and station mode functionality. Events are handled in `prv_state_machine()`.
- wpa manager events - WPA_EVENT_ID_YYY, intended to be used for Wi-Fi setup and interaction related to/with the wpa_supplicant. The event handler is `prv_wpa_event_callback()`.

In addition to being driven by events, the wpa_manager and daemon maintain their own states. In each case, the state and event together decide how an event is handled.

The wpa_manager has the states shown below. The transition diagram depicts the events that drive its transition.

```
typedef enum {
         WPA_STATE_NOT_READY,
         WPA_STATE_READY,
         WPA_STATE_CONNECTING,
         WPA_STATE_CONNECTED, 
         WPA_STATE_MAX
} wpa_state_e;
```

When wpa_manager is in the WPA_STATE_NOT_READY state, all activities related to WPA are not allowed. This state indicates that wpa_supplicant cannot be connected to. Once we can connect to the wpa_supplicant, WPA_EVENT_ID_READY is generated and the state is transitioned to WPA_STATE_READY.

Once wpa_manager is in the WPA_STATE_READY state, activities related to Wi-Fi are allowed. We also have WPA_STATE_CONNECTING and WPA_STATE_CONNECTED states, which differentiate steps in the Wi-Fi set-up process: handshake, authentication, and connection. This set-up process takes some time, depending on the access point in use.

*WPA State Diagram:*<br>
<img src="../img/WPAStateDiagram.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">


Similarly, wifistad supports the states shown below. Only the first four states are currently supported; they mirror the wpa_manager’s state transition process, so we won’t go into the details.

```
typedef enum {
          WIFISTAD_STATE_UNINITIALIZED,
          WIFISTAD_STATE_SCANNING,
          WIFISTAD_STATE_WPA_CONNECTING,
          WIFISTAD_STATE_WPA_CONNECTED,
          WIFISTAD_STATE_BECOMING_MASTER,
          WIFISTAD_STATE_MASTER_READY,
          WIFISTAD_STATE_MAX
} wifistad_state_t;
```

## Wi-Fi Station Daemon System Components

The Wi-Fi Station daemon has the following components:

- [WPA manager](../LinuxSDK-WiFiStDaemon#wpa-manager) - Interfaces with wpa_supplicant and intercepts its events; performs Wi-Fi scan, Wi-Fi network setup, and terminations.
- [wifi_event.sh script](../LinuxSDK-WiFiStDaemon#wifi_eventsh-script) - Provides a hook for developers to modify/enhance Wi-Fi network functionality.
- [Attribute management](../LinuxSDK-WiFiStDaemon#attribute-management) - Works with attrd and hubby to assist users setting up Wi-Fi using the Afero mobile app for iOS or Android.

The implementation uses two event loops:

- main event loop - Handle wifistad-generated events.
- wpa supplicant loop – Thread that serves the wpa events from and to wpa_supplicant. Please note the thread servicing the wpa event needs to run to completion before a second event is scheduled because there is a queue for these events.

The sections below discuss each component in more detail with an emphasis on the motivation behind the design. If you must customize any sections for a particular component, the requirements are presented.

### WPA Manager

wpa_manager is the main component of the Wi-Fi Station daemon, wifistad. It is responsible for managing the interface to the WPA supplicant. This means the daemon is in charge of connecting to the wpa_suppliant socket, controlling the network enabling, etc. It provides the following essential services:

1. Sets up wpa_supplicant config for connecting to the user’s AP.
2. Provides SSID list to hubby for enabling the user’s AP.
3. Broadcasts the Wi-Fi state/status information so hubby can pass the information to the application and Cloud service.

At the time of writing, wifistad is not responsible for starting the wpa_supplicant. Although wifistad could start the wpa_supplicant, we choose to have the device OS start wpa_supplicant during system initialization.

The details of wpa_supplicant are out of scope for this document. However, wpa_manager makes heavy use of the WPA supplicant APIs. wpa_manager opens two sockets to interface with WPA supplicant:

- control channel - Used to send wpa commands to wpa_supplicant.
- monitor channel - Used to listen to unsolicited messages, including connect and disconnect from the Access Point (AP) events.

As mentioned earlier, a worker thread is created to interact with these two sockets. A set of asynchronized APIs (i.e., wpa_manager_[XXXX]_async) works as wrapper functions to the various wpa commands used. For example, to perform a Wi-Fi scan, you would use `wpa_manager_scan_async()`, which internally sends a SCAN command via the control channel. Because events need to be queued, two asynchronous calls should not be invoked one right after another.

Developers can enhance the asynchronized API functionality by adding the queuing of the commands. It is worthwhile to note that because there are two different threads, the main event handling thread and the wpa_supplicant worker thread (described above), any data variables that require sharing between the threads need to be access controlled or carefully handled, otherwise unpredictable behavior could result.

wpa_manager also implements wpa supplicant “close” event detection. It waits to “re-connect” to the wpa_supplicant sockets in the event that wpa_supplicant is terminated, either manually by the operators or as a result of a crash and restart.

#### wpa_supplicant Configuration

The Linux-based OS has a `.conf` file for setting up user-based configuration for application initialization at system startup. `/etc/wpa_supplicant.conf` is the configuration file for specifying the wpa_supplicant default configuration. Typically, this file is used to specify the user’s AP SSID and its credentials to enable Wi-Fi connectivity. Instead of enabling Wi-Fi this way, which requires the user modify this file (not user-friendly), we use wifistad and the mobile app together to enable the Wi-Fi setup.

Therefore, the Wi-Fi network configuration must be disabled at system startup in the `/etc/wpa_supplicant.conf` file by specifying a dummy AP:

```
network={
      key_mgmt=NONE
      ssid="dummy"
      disabled=1
}
```

<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> It is very important to disable the Wi-Fi network configuration so it doesn’t interfere with wifistad functionality.
</div>
</div>

#### WPA SCAN and SSID List

In order to set up the Wi-Fi connection, the user needs a list of APs that the device can find. The initial Wi-Fi setup request comes from the user through the Afero mobile app for iOS or Android. The user goes to the app’s device setting screen and selects the Wi-Fi Setup button. The soft dynamic hub running on the mobile app uses a Bluetooth low energy connection to talk to the soft dynamic hub (hubby) on the device to communicate the request for Wi-Fi setup. When hubby receives the setup request message, it passes the request to wifistad via an attribute `set` operation mechanism for attribute Wi-Fi SSID List (ID = 65009), and kickstarts the request for the AP list.

wifistad performs the scan list and extracts and formats the AP list information back to hubby. hubby then forwards the list to the mobile app. The user then can select the AP and type their Wi-Fi credentials to complete the Wi-Fi setup.

### wifi_event.sh Script

Depending on the platform, there may be a need to implement certain functionality when Wi-Fi is connected or disconnected. The `wifi_event.sh` script provides the hook to do that. wifistad is notified when Wi-Fi is connected, and calls `wifi_event.sh` with the connected parameter. When Wi-Fi disconnects, it invokes `wifi_event.sh` with “disconnected”. This gives a third-party developer a mechanism for modifying network functionality in the script, without having to change the daemon code.

#####Usage

```
./wifi_event.sh [connected | disconnected ]
```

Currently, calling `wifi_event.sh connected` terminates the udhcpc process and then restarts it. As the udhcpc process is not notified when an AP is changed, this provides a convenient way to obtain a new IP address.

#####Example

```
/usr/lib/af-conn/wifi_event.sh connected 
```

or

```
/usr/lib/af-conn/wifi_event.sh disconnected
```

### Attribute Management

On the device, each daemon is responsible for the attributes relevant to its own functionality. These attributes can hold statistical-related, operational state/status-related, or network management-related information. The supported/defined attributes are documented in the [Device Attribute Registry](../AttrRegistry). wifistad must register with the Attribute daemon (attrd) at initialization time, and must implement `get` and `set` functionality for each supported attribute.

The following Wi-Fi related attributes are supported by wifstad:

| ID    | NAME               | SIZE | TYPE  | DEFAULT | RANGE | FLAGS: RW=READ/WRITE PH=PASSTHROUGH/ HIDE FROM MCU | DESCRIPTION                                                  |
| :---- | :----------------- | :--- | :---- | :------ | :---- | :------------------------------------------------- | :----------------------------------------------------------- |
| 65010 | Wi-Fi Credentials  | 255  | BYTES | -       | -     | RW PH                                              | The encrypted (with the shared secret) Wi-Fi credentials.    |
| 65009 | Wi-Fi SSID List    | 2048 | BYTES | -       | -     | RW PH                                              | The encrypted (with the shared secret) list of Wi-Fi SSIDs that the hub can see. |
| 65008 | Network Type       | 1    | SINT8 | -       | -1–2  | -                                                  | -1 = NONE<br>0 = Ethernet<br>1 = WLAN<br>2 = WAN                   |
| 65007 | Wi-Fi Setup State  | 1    | SINT8 | 0       | 0–2   | RH                                                 | Wi-Fi set-up state:<br>0 - Not Connected<br>1 - Pending<br>2 - Connected<br>3 - Unknown Failure<br>4 - Association Failed<br>5 - Handshake Failed<br>6 - Echo Failed<br>7 - SSID Not Found<br>This attribute is used during the Wi-Fi setup process to let the apps know what state the Wi-Fi connection is in. |
| 65006 | Wi-Fi Steady State | 1    | SINT8 | 0       | 0–5   | R                                                  | Wi-Fi steady state:<br>0 - Not Connected<br>1 - Pending<br>2 - Connected<br>3 - Unknown Failure<br>4 - Association Failed<br>5 - Handshake Failed<br>6 - Echo Failed<br>7 - SSID Not Found<br>This attribute is used to communicate the Wi-Fi state to the apps outside of the Wi-Fi setup. |
| 65005 | Wi-Fi Bars         | 1    | SINT8 | 0       | -     | R                                                  | Integer read-only attribute: Wi-Fi signal strength for UI purposes. |
| 65004 | Configured SSID    | 33   | UTF8S | 0       | -     | R                                                  | The SSID the hub is currently configured to use.             |

Developers can add and support their own attributes by first adding the attributes into the attrd database, then implementing the `get` and `set` functionality of these attributes.

<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong>  These attributes must be part of the device Profile to work properly.
</div>
</div>
