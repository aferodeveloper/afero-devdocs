# DEVELOPER HUB SETUP


Any connected device created with an Afero ASR requires an Afero Hub to communicate with the Cloud. Options for an Afero Hub include a dedicated Afero Secure Hub, a smartphone running the Afero mobile app, or the Afero Hub Software package, which allows you to create your own standalone hub. We refer to the latter as an Afero Developer Hub, or Developer Hub for short. Our first release is for Raspberry Pi, or any other ARM-based computer system running Debian Linux.

This Raspberry Pi release includes the following:

- **Afero Hub Software** - Software daemon that provides communication between Afero powered devices and the internet (via the Linux system’s internet connectivity).
- **Beetle** - Software daemon that provides communication between the Afero Hub Software daemon and BlueZ, the Linux Bluetooth stack.

To set up an Afero Developer Hub, please use the information in the sections below:

- [Caveats and Considerations](../StandaloneHub#caveats-and-considerations)
- [Hardware Requirements](../StandaloneHub#hardware-requirements)
- [Set Up the Raspberry Pi](../StandaloneHub#set-up-the-raspberry-pi)
- [Install the Afero Hub Software](../StandaloneHub#install-the-afero-hub-software)
    - [Install the Hub Software via APT Repository (Recommended)](../StandaloneHub#install-the-hub-software-via-apt-repository-recommended)    
    - [Install the Hub Software Manually](../StandaloneHub#install-the-hub-software-manually)
- [Add the Virtual-Hub Device to Your Account](../StandaloneHub#add-the-virtual-hub-device-to-your-account)
- [Manage the Afero Hub Software](../StandaloneHub#manage-the-afero-hub-software)
    - [Start, Stop, and Restart the Hub Software](../StandaloneHub#start-stop-and-restart-the-hub-software)  
    - [Manage the Virtual-Hub Device](../StandaloneHub#manage-the-virtual-hub-device)  
    - [Remove, Reinstall, and Update the Hub Software](../StandaloneHub#remove-reinstall-and-update-the-hub-software)    
- [Get Support](../StandaloneHub#get-support)

## Caveats and Considerations

- We recommend running the Developer Hub on the latest Raspbian “Buster” OS, however any version of Buster, and any version of Stretch (version 2017-08-16 or newer) are supported. Raspian can be downloaded from https://www.raspberrypi.org/downloads/raspbian/.
- The Developer Hub should not be used in a production environment, because the hub's encrytion keys are stored in the filesystem and not in a Hardware Security Module. If you need a hub device for a production product, several options are available. Please [contact us.](https://www.afero.io/html/home/contact-afero.html)

## Hardware Requirements

We require ARM-based systems running Debian Linux or any Debian variant:

- **Raspberry Pi 4** - No additional hardware requirements.

- **Raspberry Pi 3** - No additional hardware requirements.

- **Raspberry Pi Zero W** - No additional hardware requirements.

- **Raspberry Pi Zero (non-wireless)** - This configuration will work as a hub but is not recommended because of these additional hardware requirements:

    - USB Wi-Fi adapter ([such as this product on Amazon.com](https://www.amazon.com/Edimax-EW-7811Un-150Mbps-Raspberry-Supports/dp/B003MTTJOY)).
    
    - USB Bluetooth 4.0 USB adapter ([such as this product on Amazon.com](https://www.amazon.com/Plugable-Bluetooth-Adapter-Raspberry-Compatible/dp/B009ZIILLI/)).
    
    - Micro-USB “OTG” cable and USB hub to connect adapters to the Pi.

- **Raspberry Pi 1 or 2 Model B or B+** - This configuration will work as a hub but is not recommended because of these additional hardware requirements:

    - USB Wi-Fi adapter ([such as this product on Amazon.com](https://www.amazon.com/Edimax-EW-7811Un-150Mbps-Raspberry-Supports/dp/B003MTTJOY)).
    
    - USB Bluetooth 4.0 USB adapter ([such as this product on Amazon.com](https://www.amazon.com/Plugable-Bluetooth-Adapter-Raspberry-Compatible/dp/B009ZIILLI/)).
    
    - External power supply; USB power will not be sufficient.

- **Other ARM-based Debian Linux systems** - These have not been tested, but should also work…

    - As long as you have Bluetooth hardware supported by BlueZ.
    - Any internet connectivity is supported.

We will be adding support for other small computing systems as demand and time permits.

The Afero Hub Software is not particularly platform- or architecture-specific and should work on comparable systems. We have tested most extensively with Raspberry Pi Models 3, 4, and Zero W; we recommend using those systems for your Developer Hub.

## Set Up the Raspberry Pi

The Raspbian OS allows you to configure the Wi-Fi network from your local PC before booting it in the Raspberry Pi.

**1**&nbsp;&nbsp;Install the Raspbian OS to an appropriate SD card (8GB or larger) via the instructions at https://www.raspberrypi.org/documentation/installation/installing-images/.

**2**&nbsp;&nbsp;Remove and re-insert the SD card into your computer.

**3**&nbsp;&nbsp;Open the SD card image, which will have the volume name of “boot”.

**4**&nbsp;&nbsp;Using a text editor like Notepad for Windows, TextEdit for macOS, or your favorite plain text editor program (do not use a document program like Word or Pages), do the following:

***4a*** Create a file on the SD card named “wpa_supplicant.conf”.

***4b*** Paste the text shown below into this file, replacing the Wi-Fi network SSID (network name) and password with the proper values for your Wi-Fi network. If you reside outside the US, replace the “country” setting with the proper two-letter country code.

```
  country=US
  ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
  update_config=1
  
  network={
	  ssid="your_wifi_ssid"
	  scan_ssid=1
	  psk="your_wifi_password"
	  key_mgmt=WPA-PSK
  }
```

***4c*** Save the file.

**5**&nbsp;&nbsp;(Optional) If you do not have a keyboard and monitor to attach to the Pi, you can use a secure shell connection to the Pi to connect over your network: Using your text editor, create a file named “ssh” or “ssh.txt”. The contents of this file don’t matter, as long as the file exists on the “boot” SD card and is named “ssh”. This allows you to use the “ssh” command to connect to the Pi over your network.

**6**&nbsp;&nbsp;Unmount the SD card and remove it from your computer. Insert it into the Raspberry Pi, and power up the Pi. In subsequent steps, you will work on the Pi.

**7**&nbsp;&nbsp;And finally, please take these important security steps after your Pi is set up and working:

***7a*** Change the default password for the “pi” user using the `passwd` command.

***7b*** Replace the plaintext Wi-Fi password in the file `/etc/wpa_supplicant/wpa_supplicant.conf` with an encrypted version using the `wpa_passphrase` utility.

## Install the Afero Hub Software

You can install the Afero Hub Software in either of two ways. The first, [via APT repository](../StandaloneHub#HubbyInstallAPT), is recommended. The second, [manual install](../StandaloneHub#HubbyManPkgInstall), is there if you need it.

### Install the Hub Software via APT Repository (Recommended)

Afero provides a Debian Repository for the packages we publish. If you add our repository to your system’s Advanced Packaging Tool (APT) sources list, you can install and update packages through the normal Debian package tools, such as `aptitude` and `apt-get`.

**1**&nbsp;&nbsp;Run the following commands to download a script that will add Afero to your APT sources:

```
$ wget https://cdn.afero.io/repo/deb/addrepo.sh
$ bash ./addrepo.sh
```

   - This script adds the Afero repo information and signing key to your system configuration.
   - Select **y** to run `apt-get update` as part of the script. This will take a few minutes to run.

     ```
     $ sudo apt-get install afero-hub
     ```

   This will fetch the afero-ble and afero-hub packages as well as any other prerequisite packages you may need.

**2**&nbsp;&nbsp;During installation, you will be prompted to accept the [Afero Developer Terms of Service](https://www.afero.io/html/home/privacy.html) (scroll down on this page to read Developer Terms) before the packages will install.
, which is the license that covers your use of the Afero ble and hub packages.

**3**&nbsp;&nbsp;Skip down to [Add the Virtual-Hub Device to Your Account](../StandaloneHub#add-the-virtual-hub-device-to-your-account) to continue.

### Install the Hub Software Manually

**1**&nbsp;&nbsp;If you don’t wish to use our package repository, or if you’re installing these packages offline, you may download them manually from the following URLs:

   - https://cdn.afero.io/repo/deb/afero-ble_latest_armhf.deb
   - https://cdn.afero.io/repo/deb/afero-hub_latest_armhf.deb

If you intend to install these packages offline, you will also need to download the `html2text, qrencode, libqrencode3, and bluetooth` packages, either from a connected Debian system (with the command: `apt-get download html2text bluetooth qrencode libqrencode3 libpng12-0`), or fetch them online from https://packages.debian.org/stretch/allpackages/.

**2**&nbsp;&nbsp;Install these packages with the following commands:

```
$ sudo dpkg -i html2text*.deb libpng12-0*deb libqrencode3*.deb qrencode*deb bluetooth*deb
$ sudo reboot
$ sudo dpkg -i afero-ble*.deb
```

**3**&nbsp;&nbsp;You will be prompted to accept the [Afero Developer Terms of Service](https://www.afero.io/html/home/privacy.html) (scroll down on this page to read Developer Terms) before the packages will install.
, which is the license that covers your use of the Afero ble and hub packages.

```
$ sudo dpkg -i afero-hub*.deb
```

**4**&nbsp;&nbsp;Continue with [Add the Virtual-Hub Device to Your Account](../StandaloneHub#add-the-virtual-hub-device-to-your-account).

## Add the Virtual-Hub Device to Your Account

The installation of the Hub Software package will create a virtual-hub device that you can connect to your Afero account by scanning a QR code, just as with other devices such as Modulo. Follow the steps below:

**1**&nbsp;&nbsp;**Accept Terms of Service** - During installation, you must accept the [Afero Developer Terms of Service](https://www.afero.io/html/home/privacy.html) (scroll down on this page to read Developer Terms) before the packages will install.

**2**&nbsp;&nbsp;Scan QR code with Afero mobile app - The package installation will present a QR code on your screen. Launch the Afero mobile app on your smartphone, and tap ADD DEVICE to add a device to your account, then scan the QR code.

If the QR code won’t scan for some reason, tap MANUALLY ADD DEVICE and type the alphanumeric Association ID listed below the QR code.

**3**&nbsp;&nbsp;The software will then connect to the Afero Cloud to associate the Hub Software with your account.

**At this point, installation is complete, and your Developer Hub is ready to use!** For future reference, we’ve included instructions for managing your hub, directly below.

## Manage the Afero Hub Software

### Start, Stop, and Restart the Hub Software

The Afero Hub Software provides two daemons, **beetle** and **hubby** (the process name of Afero Hub Software), which are managed through the **systemd** init system. They automatically start when the package is installed and when the system boots; but they can also be controlled manually.

Use the following commands to display the state of the two daemons:

```
$ sudo systemctl status beetle
$ sudo systemctl status hubby
```

You will see output similar to the following for beetle:

```
$ sudo systemctl status beetle
● beetle.service - Afero BTLE Daemon
   Loaded: loaded (/lib/systemd/system/beetle.service; enabled)
   Active: active (running) since Fri 2017-09-08 12:17:12 EDT; 16min ago
     Docs: man:beetle
 Main PID: 715 (beetle)
   CGroup: /system.slice/beetle.service
           └─715 /usr/bin/beetle
```

If the status for **hubby** does not report “`active (running)`” but instead reports “`failed`”, it is possible that your Hub Software configuration has been damaged. If this happens, launch the mobile app, select your hub, and tap SETTINGS > REMOVE DEVICE. Then run the following command to re-create a QR code, which you will then re-scan to attach the Hub Software to your account again:

```
$ sudo dpkg-reconfigure afero-hub
```

Under normal circumstances, you should not have to manually stop and restart the Hub Software daemons; but if you must, the following commands will do the trick.

- To **manually stop** the daemons, use:

    ```
    $ sudo systemctl stop beetle
    ```

    Stopping the beetle daemon will automatically stop the hubby daemon.

- To **manually start** the daemons, use:

    ```
    $ sudo systemctl start hubby
    ```

    Starting the hubby daemon will automatically start the beetle daemon.

### Manage the Virtual-Hub Device

If for some reason you remove the virtual-hub device from your Afero account, or if the virtual-hub stops working, you can easily re-add it to the same or another Afero account without re-installing the software. To do this, run the following command on your system:

```
$ sudo dpkg-reconfigure afero-hub
```

You’ll be asked if you want to re-build the hub configuration:

- Answer **no** if you removed the virtual-hub device from your account, and simply want to associate it with another account. By answering **no**, the existing virtual-hub device’s QR code will be displayed. Simply scan the QR code to add it to an Afero account.

- Answer **yes** if your virtual-hub device has stopped working, especially if there are messages in `/var/log/syslog` that indicate the device’s configuration has been damaged. By answering **yes**, the configuration will rebuild and allow you to add a new hub-device by scanning a new QR code.

  If the old virtual hub remains in the mobile app even after you’ve added the new device, select the old device icon and tap REMOVE DEVICE to remove it from your account.

### Remove, Reinstall, and Update the Hub Software

You can remove, re-install, and update these packages using normal Debian package commands.

- To remove the software but keep your configuration intact (to preserve your virtual-hub device), run:

    ```
    $ sudo apt-get remove afero-hub afero-ble
    ```

- To remove the software along with your associated configuration, first remove the virtual-hub device from your Afero account via the mobile app, then run:

    ```
    $ sudo apt-get remove --purge afero-hub afero-ble
    ```

- If you installed the packages manually (rather than through `apt-get`), do one of the following.

    - To remove the software but leave the virtual-hub device intact, run:

        ```
        $ sudo dpkg -r afero-hub afero-ble
        ```

    - To remove both the package **and** the virtual-hub device, run:

        ```
        $ sudo dpkg -P afero-hub afero-ble
        ```

- When we release a new version of the Hub Software, you can update the packages through the normal `apt-get` process:

     ```
     $ sudo apt-get install --only-upgrade afero-ble   
     $ sudo apt-get install --only-upgrade afero-hub
     ```

Upgrading the packages will preserve the virtual-hub device attached to your account.

## Get Support

Support for the Afero Hub Software is provided through the project's [GitHub Issue Tracker](https://github.com/aferodeveloper/developerhub/issues). More sensitive questions (e.g., concerning your credentials) can be sent to [developer@afero.io](mailto:developer@afero.io).

The Afero Hub Software daemons write information to `/var/log/syslog` so if you have any issues with the operation of the Hub Software please be sure to save a copy of `/var/log/syslog` before you contact us.