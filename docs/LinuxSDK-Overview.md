# Overview of the Afero Secure Linux Device SDK

This section describes the overall architecture of the Afero Secure Linux Device SDK (“Afero Linux SDK” for short). It also includes an explanation of how the components communicate with each other. The Afero SDK enables Linux devices to operate as an Afero edge device and provide hub functionality for nearby Afero powered Bluetooth® low energy (BLE) devices on the same user account as the Linux device.

This page contains the following sections:

- [System Diagram](../LinuxSDK-Overview#system-diagram)
- [The Afero Secure Device on Linux](../LinuxSDK-Overview#the-afero-secure-device-on-linux)
- [Afero Linux SDK Components](../LinuxSDK-Overview#afero-linux-sdk-components)
- [Software Dependencies](../LinuxSDK-Overview#software-dependencies)

## System Diagram

<img src="../img/LinuxEdgeDeviceStrategy.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

## The Afero Secure Device on Linux

The Afero Secure Linux Device SDK relies on several Linux components, including the TCP/IP stack, iptables, the I2C device driver, and Unix Domain Sockets. The implementation is based on Yocto to simplify the build and distribution of the sample software; however, the Afero Linux SDK package can be adapted to almost any Linux distribution. The software is not sensitive to the Linux kernel version. Note that some of the software writes to the root filesystem.

## Afero Linux SDK Components

Each component in an Afero Linux device functions as follows:

| COMPONENT | DESCRIPTION |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Edge Device daemon (edged)](../LinuxSDK-Overview#edge-device-daemon-edged) | Provides edge device functionality for the Linux device.     |
| [Attribute daemon (attrd)](../LinuxSDK-Overview#LinuxSDK-Overview/#attribute-daemon-attrd)  | Provides interprocess communication based on Afero attributes. |
| [hubby](../LinuxSDK-Overview#hubby) | Provides core Afero device and Cloud service communication functionality for both hub and/or edge devices. |
| [beetle](../LinuxSDK-Overview#beetle) | Provides hubby with a Bluetooth low energy interface.        |
| [Afero Security daemon (afsecd)](../LinuxSDK-Overview#afero-security-daemon-afsecd) | Provides hubby with an interface to the Afero Hardware Security Module (HSM). |
| [Afero IPC Library (af-ipc)](../LinuxSDK-Overview#afero-ipc-library-af-ipc) | Provides a common, simple, low-level interprocess communication system based on Unix domain sockets. |
| [Wi-Fi Station daemon (wifistad)](../LinuxSDK-Overview#wi-fi-station-daemon-wifistad) | Controls the WPA Supplicant and allows the Afero mobile app to set up the user’s Wi-Fi using Bluetooth. |
| [WAN daemon (wand)](../LinuxSDK-Overview#wan-daemon-wand) | Maintains a WCDMA/LTE modem internet connection for hubs that have cellular connectivity. |
| [Connection Manager daemon (connmgr)](../LinuxSDK-Overview#connection-manager-daemon-connmgr) | Dynamically selects network interfaces to route data to the internet. It also maintains a very tight firewall using iptables. |
| [OTA Manager (otamgr)](../LinuxSDK-Overview#ota-manager-otamgr) | Updates the Afero software using OTA images signed with Afero keys and verified using the Afero Hardware Security Module, and delivers Linux update packages to the system. |

### Edge Device Daemon (edged)

The Edge Device daemon is responsible for edge device functionality; for example, controlling an alarm siren, or measuring power consumption on an electrical outlet. Such functionality can be controlled using Afero attributes. Edge devices have a special attribute space that is separate from hub functionality. The specific attributes can be encoded into the Afero device Profile, which is created using the Afero Profile Editor development tool.

We provide the following ways to implement the edge attributes for your device:

- Write a daemon in C that links with the latest afLib library. afLib implements the same API that is used by an MCU connected to an Afero ASR module.
- Write scripts that accept command-line parameters to respond to notifications, attribute `set` operations, and attribute `get` operations. These scripts can be written in Bash, Python, Perl, or any other scripting language that supports command-line parameters.

Implementing edge device functionality is the subject of [Edge Device Daemon Implementation](../LinuxSDK-EdgeDaemon).

### Attribute Daemon (attrd)

This daemon is based on the af-ipc layer and provides an attribute-sharing mechanism that works in a similar way to the Afero attribute system. Clients of the Attribute daemon can own attributes, read attributes, change attribute values, and be notified when attribute values change. All attributes are considered binary blobs by the Attribute daemon.

hubby uses the Attribute daemon to communicate Afero attributes between the Cloud and the daemons that own them. For example, the Wi-Fi Station daemon (described [below](../LinuxSDK-Overview#wifistadSummary)) provides the Wi-Fi SSID list attribute, which the Afero mobile app requests when it sets up Wi-Fi. When the value is updated, the Wi-Fi Station daemon sends the new value of the attribute to hubby using the client API of the Attribute daemon. The Attribute daemon then forwards the value to hubby, which in turn forwards the value to the Cloud, which then forwards the value to the application.

The Attribute daemon uses the IPC layer and therefore uses libevent2 for its main event loop. All Attribute daemon client daemons must also have a libevent2 main event loop.

Afero provides the Attribute daemon in source form. It builds into a daemon and a static client library. If you want to create an Attribute daemon client, you have two choices:

- You can write a C daemon that uses the Attribute daemon client API.
- You can use scripts to handle the client functionality.

Both methods are discussed in [Attribute Daemon Client Implementation.](../LinuxSDK-AttrDaemon)

### hubby

The hubby application provides the following functionality:

- Maintains an SSL connection to the Afero Cloud.
- Determines which of the user’s Afero BLE edge devices the hub can see.
- Connects to the user’s Afero BLE edge devices, as needed.
- Passes encrypted packets between the Afero Cloud and Afero BLE edge devices.
- Downloads firmware updates and forwards them to Afero BLE edge devices.
- Forwards device attribute data, including edge device attributes, to the Afero Cloud.
- Uses the Afero Hardware Security Module to identify itself as a trusted Afero device.
- Maintains the Afero attributes in the device Profile.
- Provides BLE peripheral connectivity for connecting the device to a Wi-Fi access point.

hubby has three main interfaces:

- Bluetooth low energy interface (beetle)
- Afero Security daemon interface
- Attribute daemon client interface

Afero provides hubby as a binary, logging its state to syslog. The verbosity of the log is defined in the device Profile.

One special feature of hubby is that it provides Bluetooth low energy connectivity when it is unable to connect to the Afero Cloud. This allows a user to set the Wi-Fi SSID and WPA passphrase. The credential transfer is done so that it is not possible to get the passphrase by sniffing the Bluetooth traffic and the Wi-Fi traffic. Additionally the Afero Cloud only sees an encrypted passphrase and does not have the key to decrypt it.

### beetle

The beetle application is a Bluetooth low energy interface abstraction that allows hubby to support any Bluetooth low energy stack. beetle currently supports the Linux BlueZ stack.

beetle communicates with hubby through a localhost socket. It supports both Bluetooth low energy central and peripheral modes; peripheral mode is used to set up the Linux device’s Wi-Fi.

Afero provides beetle in source form. For more information on implementing a version of beetle for your specific Bluetooth stack, contact Afero.

### Afero Security Daemon (afsecd)

The Afero Security daemon provides an interface to the Afero Hardware Security Module (HSM). The HSM is used for the following:

- Determines the device identity via the Device ID, which is a 64-bit number that uniquely identifies the device in the Afero system.
- Provides an alternate device identity called the Association ID that is printed on the device’s QR code.
- Proves the device is an official Afero edge device and not masquerading as one.
- Verifies that software updates have been signed using a key in the Afero chain of trust.

hubby talks to the Afero Security daemon using a static library and the Afero IPC shared library (described directly below). The Afero Security daemon talks to the hardware interface of the HSM (I2C) using the i2c-dev interface and takes ownership of the device, which must be passed on the command line.

afsecd can also use the hidraw interface to talk to versions of the Afero HSM connected over USB.

Afero provides afsecd in source form, which builds into a shared library called `libaf_ipc.so`.

### Afero IPC Library (af-ipc)

The Afero IPC library is a shared library providing inter-process communication (IPC) between all the daemons. The communication is based on named Unix domain sockets. There are two separate APIs for this library: the server library and the client library.

Please note that af-ipc is based on the [libevent2 event system](https://github.com/libevent/libevent). This has implications for the rest of the system; specifically, the entire API requires the application to set up an event loop. In addition, hubby itself is based on libevent2.

The af-ipc library also includes a Remote Procedure Call (RPC) layer that serializes integers, strings, and binary blobs so they can be sent to another process using the af-ipc layer. The IPC and RPC functionality are used heavily by the Attribute daemon.

Afero provides af-ipc in source form, which builds into a single static library.

### Wi-Fi Station Daemon (wifistad)

The Wi-Fi Station daemon (wifistad) supervises the Linux WPA supplicant and adds functionality to get the Wi-Fi SSID list, select an AP, and set the passphrase for the AP. The Wi-Fi Station daemon communicates these settings with the Afero mobile app through Afero system attributes. The Connection Manager also relies on the Wi-Fi Station daemon to keep it up-to-date on the Wi-Fi state. This state is also communicated using Afero system attributes.

Communication with the WPA supplicant is performed through the same Unix domain socket the wpa_cli application uses.

The Wi-Fi Station daemon is provided as sample code in source form. It builds into a standalone daemon that is also a client of the Attribute daemon, which implies that it uses libevent2 as its main event loop. Implementing a Wi-Fi Station daemon is the subject of the [Wi-Fi Station Daemon Implementation](../LinuxSDK-WiFiStDaemon).

### WAN Daemon (wand)

The WAN daemon maintains a network interface over a cellular network. It also communicates the cellular modem state via Afero attributes, both to the Connection Manager so it knows the state of the modem network, and to the Afero mobile app so the user can debug connectivity issues.

The WAN daemon sample code supports the Cinterion ELS61 wireless module and the USB-connected Sierra Wireless HL75xx modems based on the Intel/Infineon chipsets, but it can be adapted to other modems by modifying the modem-specific Radio Interface Layer (RIL). It is not particularly well suited to the Qualcomm modems that have built-in application processors like the MDM96xx series because the WAN daemon is designed around a USB-connected modem.

The WAN daemon is provided as sample code in source form. It builds into a standalone daemon that is also a client of the Attribute daemon, which implies that it uses libevent2. The modem power states are managed separately by a script called wancontrol. The network setup is performed by another script called wannetwork. Implementing a WAN daemon is the subject of the [WAN Daemon Implementation](../LinuxSDK-WANDaemon).

### Connection Manager Daemon (connmgr)

The Connection Manager daemon has two main responsibilities:

- Switching the IP network between the Wi-Fi, wireless modem, and Ethernet interfaces; and
- Maintaining firewalls that dynamically open and close IP addresses to allow only afero.io traffic.

<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong>  Firewall functionality is optional; you can disable the firewall by invoking connmgr with the -d option.
</div>
</div>

The Connection Manager chooses the network interface based on a fixed priority. Ethernet has highest priority, but if it’s down, the Connection Manager chooses Wi-Fi. If Wi-Fi is down, the Connection Manager falls back to wireless modem.

To determine if a connection is up, the Connection Manager relies on all of the following:

- Interface status as reported by netlink
- Interface status as reported by the Wi-Fi and WAN daemons
- Ability to ping the Google DNS server (8.8.8.8)
- Ability to talk to the Afero echo server (echo.*.afero.io)

The routing changes are done by changing the route metric of the default route. The Connection Manager currently does not change the DNS name servers when it switches between network interfaces.

The firewall code is distinctive because it maintains a whitelist of allowed servers based on host name and not IP address. This distinction is required to accommodate the dynamic load balancers in the Afero Cloud. The Connection Manager uses libpcap to monitor DNS lookups, and if the request is for a host name in the whitelist, it opens up the resolved IP addresses in the firewall. This feature is useful for devices that wish to act as an access point for Afero Wi-Fi devices. For devices that are Wi-Fi stations only, this special firewall feature may not be necessary; blocking all incoming connections may provide sufficient security.

The Connection Manager is released by Afero as sample code. It builds into a standalone daemon that is a client of the Attribute daemon, which implies that the main loop is based on libevent2. Implementing a Connection Manager is the subject of the [Connection Manager Implementation](../LinuxSDK-ConnMgrDaemon).

### OTA Manager (otamgr)

The otamgr daemon allows you to use the Afero Over-the-Air (OTA) Service to update the firmware in your Linux devices. The Afero Cloud OTA service enables binary blobs, such as new firmware or new configuration files, to be sent to all devices in the field in a way that ensures the integrity of the delivery. Specifically, the OTA service signs the firmware and sends it to a pool of devices. On each Linux device, the hubby application receives the firmware and validates the signature using the Afero Hardware Security Module before informing the otamgr daemon (via an Afero attribute) the location of the validated firmware in the filesystem so otamgr can install the image on the device.

Use of the Afero otamgr daemon is not required. If you do choose to write your own OTA Manager, because the hubby application takes responsibility for downloading the image and broadcasting an attribute-based event, you must implement comparable functionality on your own OTA Manager and listen for the event broadcast. If you **do** use the Afero otamgr, you must provide a script that serves as a trigger point to update the image. The script must be named `sysupgrade` and must be located in the `/sbin` directory.

Note that the OTA service and its tools are **not** part of the OTA Manager. The OTA service and tools are responsible for signing and securely transporting/downloading the image to the devices. The OTA Manager takes over and installs the image once it has been downloaded to the device. You, as the third-party, are responsible for generating the OTA image (the “blob”) and the mechanism (e.g., a script or daemon) that triggers the update.

The OTA Manager is released as sample source code with stubbed functionality that builds into a standalone daemon. The OTA Manager **must** be a client of the Attribute daemon. Implementing the OTA Manager is the subject of [OTA Manager Implementation](../LinuxSDK-OTAManager).

## Software Dependencies

The Afero Linux SDK software packages depend on several standard Linux libraries and packages, called out in the DEPENDS section of the Afero software package recipes in Yocto:

- libevent - Version 2.1.8 is recommended because it has the best support for monotonic time
- libevent-pthreads - Same version as libevent
- json-c - Version 0.11 or later
- zlib - Version 1.2.8
- openssl - Version 1.0.2h
- curl - Version 7.47.1 or later
- libpcap - Version 1.3.0 or later
- libstdc++ - Whatever version comes with GCC 4.8 and above

This lean set of dependencies allows you to run the SDK software on tiny Linux systems such as OpenWrt. There are no dependencies on GLib or D-Bus. There are also no dependencies on OpenWrt packages such as ubus or uevent.