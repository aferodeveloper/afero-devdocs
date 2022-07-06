# Afero Potenco Setup Guide for BeagleBone Green Wireless Board

This page contains instructions for setting up Afero Potenco, a secure Linux device that can also function as a hub for Afero enabled Bluetooth® low energy devices.

The instructions on this page explain how to download the software image, apply it to an SD card, install it on a BeagleBone Green Wireless board, and connect the Afero Hardware Security Module (HSM). The OS image can be provided by Afero, or you can build it yourself to include your own customizations, depending on your needs.

This page contains the following sections:

- [Requirements](../LinuxSDK-PotencoBBGW#requirements)
- [Install and Boot the Potenco Operating System](../LinuxSDK-PotencoBBGW#install-and-boot-the-potenco-operating-system)
- [Build the Potenco OS Image](../LinuxSDK-PotencoBBGW#build-the-potenco-os-image)
- [Connect the Peaberry Security Module](../LinuxSDK-PotencoBBGW#connect-the-peaberry-security-module)
- [Build the Potenco SDK](../LinuxSDK-PotencoBBGW#build-the-potenco-sdk)
- [Use Potenco as an Afero Hub](../LinuxSDK-PotencoBBGW#use-potenco-as-an-afero-hub)
- [Implement Your IoT Project](../LinuxSDK-PotencoBBGW#implement-your-iot-project)
- [Support for Questions and Issues](../LinuxSDK-PotencoBBGW#support-for-questions-and-issues)

## Requirements

### Operating System Requirements

Afero Potenco devices run on a Yocto Linux platform; specifically:

- The Potenco software stack relies on several common Linux components, including the TCP/IP stack, iptables, the I2C device driver, and Unix Domain Sockets.
- We use Yocto Linux to simplify the build and distribution of the sample software.
- The software is not sensitive to the Linux kernel version.
- Some of the software writes to the root filesystem.

### Hardware Requirements

You will need the following pieces of hardware:

- BeagleBone Green Wireless Single-Board Computer - From BeagleBoard.org and SeeedStudio.
- Peaberry board - Afero ATECC508A-MAHKN security chip on a small adapter board.
- FTDI cable - Used for serial console access.
- Power supply - Micro-USB cable supplying 5VDC at 500mA. A standard PC USB is sufficient; a separate power adapter will work but is not necessary.

### Firmware Requirements

You will need to download the firmware image for your development board from Afero. Follow the instructions below to get the correct image:

1. Contact the Afero Customer Enablement (ACE) team at ace@afero.io for the FTP login instructions.
2. Use the instructions to go to the FTP site and download the pre-built BeagleBone Green Wireless image.

## Install and Boot the Potenco Operating System

To install and boot a Potenco Operating System image on the BeagleBone Green Wireless development board, follow the instructions below.

### Create the SD Card Image

**1**&nbsp;&nbsp;Obtain the file `arago-afero-image-am335x-evm.afimg` either from the Afero FTP site, or from the output of your Potenco build environment (see [Build the Potenco OS Image](../LinuxSDK-PotencoBBGW#build-the-potenco-os-image)).

**2**&nbsp;&nbsp;Insert a MicroSD card into your PC. Use a card at least 4GB in size.

**3**&nbsp;&nbsp;Obtain software for your PC that can write an image to an SD card. We use a freeware tool called Etcher (https://www.balena.io/etcher/) that is available for Windows, MacOS, and Linux.

**4**&nbsp;&nbsp;Rename the `.afimg` file to use the extension “.img”. Write this .img file to the SD card, then eject the card from your PC. Etcher will unmount the SD card when it has successfully written.

<div class="af-callout-caution">
	<div class="callout-text">
	<p><img src="../img/Caution.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>CAUTION!</strong>&nbsp; Do not remove the SD card until all the data has been written to it and the device has been unmounted!</p>
	</div>
</div>

### Boot from the SD Card Image

**1**&nbsp;&nbsp;Ensure the BeagleBone board is unplugged from power.

**2**&nbsp;&nbsp;Insert the MicroSD card into the slot on the underside of the board. The label side of the MicroSD card should face the component side of the board.

**3**&nbsp;&nbsp;Connect an FTDI cable or USB to FTDI Serial adapter to the serial console port on the BeagleBone board. It’s a 6 pin male header near the center of the P9 I/O connector. 

The pins are labeled to help orient the FTDI cable correctly. The ground pin on the FTDI cable should be closest to the USB ports on the board.

**4**&nbsp;&nbsp;Connect the FTDI cable to your PC.

**5**&nbsp;&nbsp;Locate the USER button on the board. It's a small pushbutton near the FTDI connector.

**6**&nbsp;&nbsp;While holding the USER button down, connect the Micro-USB power cable to the board and it will power on.

**7**&nbsp;&nbsp;When the four (4) blue USER LEDs near the power connector start blinking, you can release the USER button.

**8**&nbsp;&nbsp;Using a serial terminal program, connect to the FTDI serial connection (115200 baud). At the log-in prompt, log in as root with no password to access the system.

## Build the Potenco OS Image

**1**&nbsp;&nbsp;You‘ll need access to an Ubuntu Linux system to build the Potenco OS image. For these instructions, you must use the Ubuntu 16.04 LTS Desktop image.

**2**&nbsp;&nbsp;On the Ubuntu system, ensure that basic GCC build tools and required tools are installed:

```
$ sudo apt-get install build-essential python diffstat texinfo chrpath libc6:i386 gawk git repo
```

**3**&nbsp;&nbsp;Begin by downloading the toolchain. You can do this in your home directory:

```
$ cd
$ wget https://releases.linaro.org/components/toolchain/binaries/5.3-2016.02/arm-linux-gnueabihf/gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf.tar.xz
```

**4**&nbsp;&nbsp;Unpack the toolchain and move it to the directory `ti-linaro-5.3` in your home directory. This path is required for the `oe-layertool-setup` script to work properly.

```
$ tar xf gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf.tar.xz
$ mv gcc-linaro-5.3-2016.02-x86_64_arm-linux-gnueabihf ~/ti-linaro-5.3
```

**5**&nbsp;&nbsp;Get the Arago setup repository and use it as the top-level directory for building Potenco:

```
$ git clone git@github.com:AferoCE/arago-afero-setup potenco
$ cd potenco
```

**6**&nbsp;&nbsp;Get the repositories needed for the build; you can modify these repos later if needed:

```
$ git clone git@github.com:AferoCE/tisdk-linux
$ git clone git@github.com:AferoCE/af-ipc
$ git clone git@github.com:AferoCE/af-util
$ git clone git@github.com:AferoCE/attrd
$ git clone git@github.com:AferoCE/otamgr
$ git clone git@github.com:AferoCE/af-conn
```

**7**&nbsp;&nbsp;Go into the `af-conn` directory, retrieve the various `af-conn` management files in the repo, then `cd` back up to the potenco directory:

```
$ cd af-conn
$ git clone git@github.com:AferoCE/af-conn-files files
$ cd ..
```

**8**&nbsp;&nbsp;Use `oe-layertool-setup.sh` script to retrieve the Yocto layers, including the meta-afero layer:

```
$ ./oe-layertool-setup.sh -f configs/aferoce/master.txt
```

**9**&nbsp;&nbsp;Set up the environment. For this to work properly you need to have put the toolchain in the `ti-linaro-5.3` directory in your home directory.

```
$ cd build
$ source conf/setenv
```

**10**&nbsp;&nbsp;Now you can build the image:

```
$ bitbake arago-afero-image
```

**11vTo flash the image, use the directions in [Install and Boot the Potenco Operating System](../LinuxSDK-PotencoBBGW#install-and-boot-the-potenco-operating-system). The SD image file you want is in the following file location:

```
arago-tmp-external-linaro-toolchain/deploy/images/am335x-evm/arago-afero-image-am335x-evm.afimg
```

## Connect the Peaberry Security Module

The BeagleBone Green Wireless board has a different I/O connector pinout than the Peaberry board, so it must be manually wired to connector P9 on the board.

**1**&nbsp;&nbsp;Use four (4) short jumper wires (such as https://www.adafruit.com/product/1956 or anything similar) to connect the four (4) pins shown on the Peaberry board to pins 1, 3, 19, and 20 on connector J9 on the BeagleBone Green Wireless board.

**2**&nbsp;&nbsp;In the first diagram below, the Peaberry board is shown with the connector facing up. Connect the four pins shown to connector J9 as follows:

   Power VDD to J9 pin 3
   GND to J9 pin 1
   I2C_SDA to J9 pin 20
   I2C_SCL to J9 pin 19

**3**&nbsp;&nbsp;The two (2) unmarked pins on the Peaberry board must remain unconnected.

<img src="../img/PeaberryPinDiag.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

**Cape Expansion Headers:**

<img src="../img/CapExpHeaders.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

## Build the Potenco SDK

The Potenco SDK is a set of binary packages that you can supply to your customers to develop applications for the Potenco OS, without having access to all the repositories needed to build the entire OS image.

**1**&nbsp;&nbsp;Set up the environment.

```
$ cd ~/potenco/build
$ source conf/setenv
```

**2**&nbsp;&nbsp;Now you can build the SDK:

```
$ bitbake -c populate_sdk arago-afero-image
```

**3**&nbsp;&nbsp;The SDK can be found at:

```
arago-tmp-external-linaro-toolchain/deploy/sdk/arago-2016.05-toolchain-2016.05.sh
```

## Use Potenco as an Afero Hub

Download the Afero mobile app, if you haven’t already, and sign in. The Peaberry board should have come with a couple of QR Code stickers, including one on the back of the board. Scan the QR Code in the Afero mobile app to add the Potenco hub to your account. Once online, any Afero BLE devices on your account will use the Potenco hub to connect to the Afero Cloud.

## Implement Your IoT Project

Potenco also acts as an Afero edge device and can communicate with the Afero Cloud as a connected device in addition to acting as a hub. You can create a Potenco profile in the Afero Profile Editor, and attributes that you define in that profile can be accessed via scripts running on the Potenco board.

## Support for Questions and Issues

If you have any questions or issues with Potenco please use the project's [GitHub Project Issue Tracker](https://github.com/AferoCE/potenco/issues).
