# Software Tasks for Creating an Afero Secure Linux Device

There are a number of software tasks that must be finished to produce a fully-functional, production-ready device. The first section below goes over the [high-level software tasks](../LinuxSDK-SWTasks#high-level-software-tasks) that must be completed; the next section describes the [miscellaneous integration tasks](../LinuxSDK-SWTasks#miscellaneous-afero-integration-tasks).

The instructions in this section describe the process generically; you can use the instructions as a building block to create a working system for any architecture of your choice. See [Afero Potenco Setup Guide for BeagleBone Green Wireless Board](../LinuxSDK-PotencoBBGW) for one example.

## High-Level Software Tasks

At a high level, developing an Afero Linux device is like developing any other Linux-based embedded system. You must do the following:

1. [Define the Partition Scheme](../LinuxSDK-SWTasks#1-define-the-partition-scheme)
2. [Create a Build System](../LinuxSDK-SWTasks#2-create-a-build-system)
3. [Implement an OTA Scheme](../LinuxSDK-SWTasks#3-implement-an-ota-scheme)
4. [Implement Device-Specific Functionality](../LinuxSDK-SWTasks#4-implement-device-specific-functionality)
5. [Implement Factory Diagnostics](../LinuxSDK-SWTasks#5-implement-factory-diagnostics)
6. [Secure Your Hardware](../LinuxSDK-SWTasks#6-secure-your-hardware)
7. [Obtain Required Certifications](../LinuxSDK-SWTasks#7-obtain-required-certifications)

In the following sections we discuss specific requirements for each of these steps when you’re building an Afero secure Linux device.

### 1. Define the Partition Scheme

The partition scheme for a Linux device depends on the particular choices you make for:

- Bootloader requirements
- Nonvolatile data requirements
- Wi-Fi calibration data requirements
- System logs location
- Filesystem redundancy in case an OTA update fails

Afero Linux devices have only one specific requirement regarding the partition:

**Requirement** Devices must have a nonvolatile data partition that does not get erased or changed when an OTA is performed. This partition must be accessed from the `/afero_nv` directory. You may create a symlink residing on the root filesystem that points to the actual nonvolatile partition.

The reason for this requirement is that the Wi-Fi credentials must not be erased or removed when an OTA is performed.

### 2. Create a Build System

Often partners choose a build system based on the one that is used in the silicon vendor’s SDK. The Afero Linux Device SDK package is distributed with a Yocto layer, so it’s pretty easy to integrate into Yocto systems. However, the underlying packages are built using autotools, and the repositories are designed to be built in OpenWrt. We have successfully built on both OpenWrt and Yocto.

The Afero software is distributed in the following repos:

- binaries - Afero Security daemon and hubby in binary form.
- af-conn - Connectivity software in source form including the Connection Manager, Wi-Fi Station daemon, and the WAN daemon.
- af-ipc - Afero IPC layer in source form.
- af-utils - Some common utilities used by the Afero software in source form.
- attrd - Attribute daemon in source form.
- otamgr - Sample OTA Manager.
- logpush - Sample log rotation script.
- meta-afero - Yocto layer that builds the Afero software.

The last repo, meta-afero, is very helpful because it contains many of the glue files that allow the software to run on different hardware, for example, the TI BeagleBone Green Wireless board. It is easily modified for building in other Yocto-based build systems. We also have a Raspberry Pi meta-afero repo and soon will have a SAMA5 meta-afero repo.

**Requirement** Get the binaries, af-conn, af-ipc, af-utils, attrd, and otamgr code to build in your build system.

### 3. Implement an OTA Scheme

OTA updates are critical for the security of the device in the field. If you use the Afero Cloud to provide OTA updates, you will need to write an OTA Manager, which is a daemon that listens for OTA update attribute notifications. We provide a sample OTA Manager that works on OpenWrt. You will need to modify this code for your device. This daemon is described in detail in [OTA Manager Implementation](../LinuxSDK-OTAManager).

**Requirement** Write an OTA Manager based on the provided sample code to receive OTA updates from the Afero Cloud.

### 4. Implement Device-Specific Functionality

The device-specific code implements the features that make your device do something useful.

**Requirement** Write an Edge Device daemon to implement the device functionality. This is discussed in more detail in Edge Device Daemon Implementation.

**Requirement** If you're using a wireless modem (WAN), you must change or rewrite the WAN daemon. This is in the af-conn repository and details are given in [WAN Daemon Implementation](../LinuxSDK-WANDaemon).

**Requirement** If you have special Wi-Fi requirements – for example, if you’re building a Wi-Fi access point – you'll have to modify the Wi-Fi Station daemon and the Connection Manager. These components are also in the af-conn repository and more details are given in [Wi-Fi Station Daemon Implementation](../LinuxSDK-WiFiStDaemon) and the [Connection Manager Daemon Implementation](../LinuxSDK-ConnMgrDaemon).

In addition, there are integration tasks, discussed below in the [Miscellaneous Afero Integration Tasks](../LinuxSDK-SWTasks#miscellaneous-afero-integration-tasks) section.

### 5. Implement Factory Diagnostics

When you mass-produce devices in a factory, you must provide software that tests whether the hardware is functioning properly. These tests include:

- Initial system programming
- Voltage and current tests
- Peripheral testing
- Wireless peripheral over-the-air testing

To build an Afero powered device, you must fulfill requirements in addition to those needed for hardware validation. These additional requirements relate to getting data from the security chip to Afero and are discussed in the “Afero Secure Linux Device Factory Guide” (available soon).

**Requirement** Make sure your factory diagnostics perform the tasks defined in the soon-to-be-available “Afero Secure Linux Device Factory Guide”.

### 6. Secure Your Hardware

Many Linux IoT devices on the market have security problems. Linux provides powerful software development tools, but also provides tools and a known attack surface that can compromise the system. Afero requires the device be secured so attackers cannot easily gain access to the device; furthermore, ideally it should be impossible to access the device remotely.

**Requirement** Secure the device against basic wireless and physical attacks. There are some important steps you must take to secure any Linux system, including your device:

- Prevent users from accessing any serial console ports.
- Use a firewall to block all incoming TCP/IP connections, especially SSH.
- Disable root login.

### 7. Obtain Required Certifications

Like any wireless Linux device, a product will need to undergo certification from different governing bodies before it can be sold. These might include:

- FCC certification
- UL or CE certification
- Carrier certification if the device contains a wireless modem
- Wi-Fi certification
- Bluetooth Special Interest Group (SIG) certification

In addition, your device must undergo testing at Afero before you can sell it.

**Requirement** You must send your device to Afero for testing before you can sell it. At a high level, we will look at the following:

- The device must perform its primary function and be stable.
- The device’s security cannot be easily compromised.
- The device must allow Afero to perform OTA updates to the Afero Security daemon and hubby.

## Miscellaneous Afero Integration Tasks

This section captures some miscellaneous integration tasks required to get the Afero Linux SDK package running. We will discuss the following tasks:

- [Init Scripts](../LinuxSDK-SWTasks#init-scripts)
- [Logging](../LinuxSDK-SWTasks#logging)
- [Reporting hubby State Using LEDs](../LinuxSDK-SWTasks#reporting-hubby-state-using-leds)
- [Network Capabilities Script](../LinuxSDK-SWTasks#network-capabilities-script)
- [COMMAND Attribute](../LinuxSDK-SWTasks#command-attribute)
- [REBOOT_REASON Attribute](../LinuxSDK-SWTasks#reboot_reason-attribute)
- [Network Interface Names File](../LinuxSDK-SWTasks#network-interface-names-file)

### Init Scripts

To provide an init-agnostic system, we do not provide init scripts for the Afero daemons. Therefore you can use systemd, sysvinit, busybox init, upstart, or whatever init system you want. Just make sure that all the daemons are started by the same init system.

**Requirement** Write init scripts for the following daemons: afsecd, hubby, wifistad, wand (if applicable), connmgr, attrd, otamgr, edged.

**Requirement** Write watcher scripts for Afero daemons if you’re using sysvinit or busybox init.

If you’re using an init system that doesn’t monitor the processes it starts, we require that you have watcher scripts on these daemons to restart them if they exit for any reason. The exit code is always 1, indicating an error condition. The daemons won’t exit with 0 as they are meant to run indefinitely.

It is required that the Wi-Fi Station daemon watcher script restart wpa_supplicant if the Wi-Fi Station daemon exits. It is also required that the hubby watcher script restart Bluetooth because hubby exits if it detects a Bluetooth stack failure. There are no special requirements for restarting the Connection Manager or the WAN daemon.

### Logging

Logging is an optional feature but very useful.

All the Afero daemons send debug and error information to syslog. If you encounter problems with one of the daemons, the logs will provide the best clues of the underlying issue. If you want to store your logs on the device for later retrieval, have the logging system store the logs in a file on the `/tmp` filesystem and rotate those logs. For example, busybox can be configured to log to `/var/log/messages`.

Afero provides a script as an example for BBGW. It is called `logpush` and its function is to rotate the logs. It is triggered by a cron job that runs every 15 minutes:

```
/etc/crontabs/root:
*/15 * * * * /bin/nice /usr/bin/logpush
```

This script stores the current contents of /var/log/messages into a file on the `/data` partition (that’s easily changed) and gzips it. It maintains the last 288 compressed log files, approximately 72 hours’ worth.

The script has another benefit. If you run the script with the `-f` switch, the script will upload the logs to an Afero server for access. This is useful for debugging devices that are in the field. We are planning to provide partners with access to these logs for the devices they manufacture.

The logpush script is in the meta-afero repository. See the `recipes-afero/afero-binaries/files` directory.

### Reporting hubby State Using LEDs

Showing the hubby state using LEDs is an optional but highly recommended feature. hubby reports its state using an attribute (see Attribute Daemon Client Implementation for more information about attributes). This attribute is called HUBBY_STATE, and the attribute ID is 51611. If you register for notifications on this attribute you can update the state of the LEDs based on changes to this attribute.

hubby sets the HUBBY_STATE attribute to the following values:

- 0 - hubby has not yet determined if it can connect to the Afero Cloud.
- 1 - hubby is connected to the Afero Cloud and everything is working properly.
- 2 - hubby is not able to connect to the Afero Cloud at this time. hubby will continue to try to connect until it is successful.

The recommended way to handle this attribute is to set the LEDs to the 0 state (hubby has not yet determined if it can connect to the Afero Cloud) as soon as possible after boot. The bootloader is a good choice. Then use your init process to start up a daemon to receive notifications on this attribute.

When the daemon receives the HUBBY_STATE notification, the attribute value will have one of the above values. Set the LED state accordingly.

You can also `get` the attribute in case your daemon crashes and restarts.

Sample code for handing the HUBBY_STATE attribute can be found in the otamgr repository.

### Network Capabilities Script

The Connection Manager provides a NET_CAPABILITIES attribute (65022) that describes the network interfaces that can provide connectivity to the Afero Cloud. For example, the device may have Wi-Fi, WAN, and ethernet interfaces, all of which could provide a connection to the internet and therefore the Afero Cloud. The interfaces that are supported by the hub may be hardware-dependent.

Below is the definition of the NET_CAPABILITIES attribute:

- bit 0 - Set to 1 if the hub supports Wi-Fi.
- bit 1 - Set to 1 if the hub supports wireless modem (WAN).
- bit 2 - Set to 1 if the hub supports ethernet.

Because the Connection Manager cannot know the hardware capabilities of the device, it is a requirement that you provide a script, `/usr/lib/af-conn/net_capabilities`, that returns the network capabilities value.

**Requirement** Create a `/usr/lib/af-conn/net_capabilities` script to indicate the network interfaces your hardware supports.

Below is an example script for a device that always supports ethernet and Wi-Fi. It uses the serial number (stored in the /sys filesystem) to determine if the device has a cellular modem or not.

```
#!/bin/sh
#
# Network capabilities script used by the Connection Manager to determine
# what network interfaces are available based on the hardware configuration
# of the board.

# According to the Network Capabilities attribute definition:
# Bitfield indicating which network interfaces this hardware contains
#   bit 0 - Wi-Fi 1
#   bit 1 - WAN 1
#   bit 2 - Ethernet 1
#

set -e

WIFI_SUPPORTED=1
ETHERNET_SUPPORTED=1

VARIANT=`cat /sys/devices/platform/idme/device_serial_number | awk '{ print substr($1,6,2) }'`
case $VARIANT in
  01) CELLULAR_SUPPORTED=1 ;;
  02) CELLULAR_SUPPORTED=1 ;;
  03) CELLULAR_SUPPORTED=1 ;;
   *) CELLULAR_SUPPORTED=0 ;;
esac

cap=0
[ $WIFI_SUPPORTED     -ne "0" ] && cap=$(($cap | 0x01))
[ $CELLULAR_SUPPORTED -ne "0" ] && cap=$(($cap | 0x02))
[ $ETHERNET_SUPPORTED -ne "0" ] && cap=$(($cap | 0x04))

exit $cap
```

### COMMAND Attribute

The device Profile contains a system attribute called COMMAND (65012). Its value is a four-byte array, but only the first byte is currently used. The COMMAND attribute allows you to control the device using the API on the Afero Cloud. Specifically, the COMMAND attribute can be used to do the following:

- Reboot - The first byte of the COMMAND attribute is 0x01.
- Remove Wi-Fi credentials - The first byte of the COMMAND attribute is 0x02.
- Put the device into factory test mode - The first byte of the COMMAND attribute is 0x03.

The Afero Cloud automatically issues the second command (0x02) when a user removes a device from their account. If the device is online at the moment the user removes it, the service issues the command immediately. If the device is not online, the service will issue the command when the next user associates the device with their account. This ensures that the Wi-Fi credentials are erased before the next user attempts to associate the device with their AP.

The other two commands are never automatically issued by the service. The factory test mode command is useful for reverse logistics.

**Requirement** Implement the COMMAND attribute. The OTA Manager sample code has an example implementation.

### REBOOT_REASON Attribute

The device Profile contains a system attribute called REBOOT_REASON (65019). Its value is an ASCII string and contains the date and time the device rebooted and a human-readable reason why the reboot occurred. The Attribute daemon is responsible for responding to `get` requests for this attribute. It gets the information from a file on the Afero nonvolatile partition at: `/afero_nv/reboot_reason`

The date is determined from file’s modification date. The contents of the file are used for the human-readable reason for the reboot.

In general the device will reboot for the following reasons:

- The COMMAND attribute (see the previous section) was set to 0x01.
- There was a full OTA update.
- The triggering of some code that detects a catastrophic condition and reboots the device.

The OTA Manager sample code handles the first two reasons. Note that for any catastrophic condition handlers that reboot the device, you should write the reboot reason into the `/afero_nv/reboot_reason` file before rebooting.

**Requirement** Whenever you reboot to handle a catastrophic failure, write a human readable reboot reason into the `/afero_nv/reboot_reason` file.

### Network Interface Names File

By default the Afero Platform uses the following network interface device names for the network interfaces managed by the Connection Manager.

| NETWORK INTERFACE NAME | TYPE OF NETWORK INTERFACE                   |
| :--------------------- | :------------------------------------------ |
| eth0                   | Primary ethernet interface                  |
| wlan0                  | Primary Wi-Fi STA interface                 |
| wwan0                  | Primary WAN (cellular modem) interface      |
| wlan0-1                | Primary Wi-Fi AP interface                  |
| br-apnet               | Bridge interface (currently not being used) |

Your device may have different network interface names. For example, the wireless modem may be on eth0 instead of wwan0. If you can't use the default network interface names you can overwrite them by modifying the `/lib/afero_netif_names` file. This file looks like this:

```
# Network interface mappings
 
  ETH_INTERFACE_0='eth0'
  WAN_INTERFACE_0='wwan0'
  WIFISTA_INTERFACE_0='wlan0'
  WIFIAP_INTERFACE_0='wlan0-1'
  BRIDGE_INTERFACE_0='br-apnet'
```

All the Afero scripts and daemons that need to know the network interface names read this file. Of course if you use other non Afero scripts or config files that refer to network interface names (for example, `/etc/network/interfaces`), you will be responsible for modifying them to make sure that they use the correct names.