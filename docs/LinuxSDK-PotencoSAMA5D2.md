# Afero Potenco Setup Guide for SAMA5D2 Board

This page contains instructions for setting up Afero Potenco, a secure Linux device that can also function as a hub for Afero enabled Bluetooth® low energy devices.

The instructions on this page explain how to flash the image to the internal eMMC memory slot on a Microchip SAMA5D2 board, and connect the Afero Hardware Security Module (HSM). The OS image can be provided by Afero, or you can build it yourself to include your own customizations, depending on your needs.

This page contains the following sections:

- [Requirements](../LinuxSDK-PotencoSAMA5D2#requirements)
- [Install and Boot the Potenco Operating System](../LinuxSDK-PotencoSAMA5D2#install-and-boot-the-potenco-operating-system)
- [Build the SAMA5D2 Linux OS Image](../LinuxSDK-PotencoSAMA5D2#build-the-sama5d2-linux-os-image)
- [Flash the SAMA5D2 Image](../LinuxSDK-PotencoSAMA5D2#flash-the-sama5d2-image)
- [Connect the Peaberry Security Module](../LinuxSDK-PotencoSAMA5D2#connect-the-peaberry-security-module)
- [Build the Potenco SDK](../LinuxSDK-PotencoSAMA5D2#build-the-potenco-sdk)
- [Use Potenco as an Afero Hub](../LinuxSDK-PotencoSAMA5D2#use-potenco-as-an-afero-hub)
- [Implement Your IoT Project](../LinuxSDK-PotencoSAMA5D2#implement-your-iot-project)
- [Support for Questions and Issues](../LinuxSDK-PotencoSAMA5D2#support-for-questions-and-issues)

## Requirements

### Operating System Requirements

Afero Potenco devices run on a Yocto Linux platform; specifically:

- The Potenco software stack relies on several common Linux components, including the TCP/IP stack, iptables, the I2C device driver, and Unix Domain Sockets.
- We use Yocto Linux to simplify the build and distribution of the sample software.
- The software is not sensitive to the Linux kernel version.
- Some of the software writes to the root filesystem.

### Hardware Requirements

You will need the following pieces of hardware:

- SAMA5D2 development board - From Microchip.
- Peaberry board - Afero ATECC508A-MAHKN security chip on a small adapter board.
- FTDI cable - Used for serial console access.
- Power supply - Micro-USB cable supplying 5VDC at 500mA. A standard PC USB is sufficient; a separate power adapter is needed to power the SAMA5D2 board.

### Firmware Requirements

You will need to download the firmware image for your development board from Afero. Follow the instructions below to get the correct image:

**1**&nbsp;&nbsp;Contact the Afero Customer Enablement (ACE) team at ace@afero.io for the FTP login instructions.

**2**&nbsp;&nbsp;Use the instructions to go to the FTP site and download the pre-built SAMA5D2 image.

## Install and Boot the Potenco Operating System

To install and boot a Potenco Operating System image on the SAMA5D2 development board, follow the instructions below.

### Create the eMMC Image

**1**&nbsp;&nbsp;Obtain the image files, either from the Afero FTP site, or from the output of your Potenco build environment (see [Build the SAMA5D2 Linux OS Image](../LinuxSDK-PotencoSAMA5D2#build-the-sama5d2-linux-os-image)).
- u-boot: at91bootstrap-sama5d2_xplained.bin
- secondary bootloader: u-boot-sama5d2-xplained-emmc.bin
- image: core-image-minimal-sama5d2-xplained-emmc.wic

**2**&nbsp;&nbsp;Obtain/download Microchip’s flash tool, SAM-BA, available for Windows, macOS, and Linux. Microchip recommends using version 3.2.3. Download from https://github.com/atmelcorp/sam-ba/releases.

**3**&nbsp;&nbsp;Follow the instructions in [Flash the SAMA5D2 Image](../LinuxSDK-PotencoSAMA5D2#flash-the-sama5d2-image) to flash the image on the eMMC.

### Boot from the eMMC Image

**1**&nbsp;&nbsp;Ensure the SAMA5D2 board is unplugged from power.

**2**&nbsp;&nbsp;Connect an FTDI cable or USB-to-FTDI Serial adapter to the serial console/DEBUG port on the SAMA5D2 board. It’s a 6-pin male header with label DEBUG J1. The pins are labeled to help you orient the FTDI cable correctly. The ground pin on the FTDI cable should be closest to the Ethernet port on the board.

**3**&nbsp;&nbsp;Connect the FTDI cable to your computer.

**4**&nbsp;&nbsp;Using a serial terminal program, connect to the FTDI serial connection (115200 baud). At the log-in prompt, log in as root with no password to access the system.

**5**&nbsp;&nbsp;Locate the RESET button on the board. It’s a small pushbutton near the A5-USB-A port.

**6**&nbsp;&nbsp;Hold the RESET button down for few seconds and release it. The board will power on.

## Build the SAMA5D2 Linux OS Image

Follow the instructions below to build the OS image.

### Host Machine Environment Setup

**1**&nbsp;&nbsp;You‘ll need access to an Ubuntu Linux system to build the SAMA5D2 Linux OS image. Any X86-compatible system running a 64-bit Ubuntu 16.04 LTS Desktop or Server operating system should suffice.

**2**&nbsp;&nbsp;On the Ubuntu system, ensure that basic GCC build tools and other required tools are installed:

```
$ sudo apt-get install gawk wget git-core diffstat unzip texinfo gcc-multilib \
build-essential chrpath socat cpio python python3 python3-pip python3-pexpect \
xz-utils debianutils iputils-ping python3-git python3-jinja2 libegl1-mesa libsdl1.2-dev \
xterm
```

**3**&nbsp;&nbsp;You will need to get the `repo` command. On Ubuntu 16.04, type:

```
$ sudo apt-get install repo -y
```

### Image Build Environment Setup Information

Afero Yocto builds an image with the following settings:

- BUILD_TYPE=prod
- BUILD_TARGET=debug or release
- BUILD_PROFILE=potenco (predefined and typically doesn't change)

These build environment variables are defined in the `conf/local.conf` file but can be overridden in the bitbake command line.

### Build Procedure

Follow the build steps below:

**1**&nbsp;&nbsp;Create a directory for the project:

```
$ mkdir sama5
$ cd sama5
```

**2**&nbsp;&nbsp;Clone the `yocto/poky` GitHub repository with the proper branch ready:

```
$ git clone git://git.yoctoproject.org/poky -b sumo
```

**3**&nbsp;&nbsp;Clone `meta-openembedded` GitHub repository with the proper branch ready:

```
$ git clone git://git.openembedded.org/meta-openembedded -b sumo
```

**4**&nbsp;&nbsp;Clone the `meta-qt5` GitHub repository with the proper branch ready:

```
$ git clone git://code.qt.io/yocto/meta-qt5.git
$ cd meta-qt5
$ git checkout v5.9.6
$ cd ..
```

**5**&nbsp;&nbsp;Clone the `meta-atmel` layer with the proper branch ready:

```
$ git clone git://github.com/linux4sam/meta-atmel.git -b sumo
```

**6**&nbsp;&nbsp;Clone the Afero repositories using the manifest file for SAMA5: `sama5-hub-manifest`. This clones meta-afero and various sources (hubby, attrd, etc.). The source repos will be put into `src_afero`.

```
$ repo init -u ssh://git@github.com/AferoCE/sama5-hub-manifest.git
$ repo sync -j8
```

**7**&nbsp;&nbsp;Enter the `poky` directory to configure the build system and start the build process:

```
$ cd poky
```

**8**&nbsp;&nbsp;Customize the build environment and initialize the build directory:

```
$ cp ~/sama5/meta-afero/scripts/oe-buildenv-internal  ~/sama5/poky/scripts/oe-buildenv-internal
$ source oe-init-build-env build-microchip
```

**9**&nbsp;&nbsp;Customize the build to use the desired meta layer. For example, meta-atmel, meta-afero, etc. Make sure the path is to the proper directory.

```
$ cp ~/sama5/meta-afero/conf/bblayers.conf  ~/sama5/poky/build-microchip/conf/bblayers.conf
$ cp ~/sama5/meta-afero/conf/local.conf     ~/sama5/poky/build-microchip/conf/local.conf
```

<mark>**&check; Note:**  Any changes to bblayers.conf or local.conf need to be saved to the meta-afero version. Specifically…<br>If you change the following files:</mark>

`~/sama5/poky/build-microchip/conf/local.conf~/sama5/poky/build-microchip/conf/bblayers.conf`

<mark>You must copy them and commit the changes to repo:</mark>

`~/sama5/meta-afero/conf/bblayers.conf~/sama5/meta-afero/conf/local.conf`

**10**&nbsp;&nbsp;Workaround for now, until we have a fix for a bug in Microchip at91bootstrap v3.8.11: Use v3.8.10 by deleting the at91bootstrap_3.8.11.bb:

```
$ rm ~/sama5/meta-atmel/recipes-bsp/at91bootstrap/at91bootstrap_3.8.11.bb
```

**11**&nbsp;&nbsp;Build and compile the Yocto distribution. Note that the first time you do this, compiling will take a while; depending on your CPU, it might take a couple of hours.

```
$ BUILD_TYPE=prod BUILD_TARGET=debug BUILD_PROFILE=potenco bitbake core-image-minimal
```

**12**&nbsp;&nbsp;You can clean the Afero source and build with a different build type, or target. The example below shows `bitbake` cleaning the tmp dir for the Afero source packages. Then compile and build the image for `BUILD_TYPE=prod` :

```
$ bitbake -c cleansstate beetle af-conn af-edge af-ipc af-sec af-util attrd hubby otamgr
$ BUILD_TYPE=prod BUILD_TARGET=debug BUILD_PROFILE=potenco bitbake core-image-minimal
```

## Flash the SAMA5D2 Image

The SD card slot on the board is used to connect the Wi-Fi+BT combo chip so we don’t build the image for an SD card. We use the eMMC-based image as we must flash the image to the internal eMMC. The image target we build is **`sama5d2-xplained-emmc`**.

### The SAM-BA In-System Programmer

You will need to obtain the SAM-BA software from Microchip to flash the SAMA5D2 image to the eMMC on your SAMA5D2 Potenco. You download this tool, version 3.2.3 (on Linux) from GitHub: https://github.com/atmelcorp/sam-ba/releases.

Make sure you add the path of the executable to your $PATH.

### Debug Tool

The SAMA5D2 board provides a FTDI-based debug port; namely, the six (6) pins adjacent to the Ethernet port.

**1**&nbsp;&nbsp;Plug in the FTDI-USB cable.

**2**&nbsp;&nbsp;You should see the port as follows (on Linux):

```
/dev/ttyUSBx
```

Where *`x`* is a number such as 0, 1, etc.

**3**&nbsp;&nbsp;Now you can use picocom, screen, or minicom to access the console; for example:

```
$ picocom -b 115200 /dev/ttyUSB0
```

### 

### Steps for Flashing

**1**&nbsp;&nbsp;Prepare for flashing:

***1a*** Disconnect/remove all the USB and power cables.

***2b*** Remove the VDDBU jumper for 10-15 seconds.

**2**&nbsp;&nbsp;Connect the USB to the board before launching SAM-BA:

***2a*** Close the JP9 (BOOT_DIS) jumper to prevent booting from eMMC or serial Flash by disabling Flash Chip Selects.

***2b*** Power the board by connecting J14 to power.

***2c*** Connect a USB micro-A cable (J23 A5-USB-A) to the board.

**3**&nbsp;&nbsp;After you power on and plug in J23, check that you can see the J23 port on your terminal:

```
$ ll /dev/ttyA*
crw-rw---- 1 root dialout 166, 0 Jun  8 17:52 /dev/ttyACM0
```

**4**&nbsp;&nbsp;Make sure the user (“tina” in the example below) is in the dialout group:

```
$ cat /etc/group | grep dialout
dialout:x:20:tina
```

**5**&nbsp;&nbsp;Open the JP9 (BOOT_DIS) jumper to allow write to the flash, then flash the bootloader. You need to do this only the first time or when the bootloader has changed:

```
$ sam-ba -p usb -b sama5d2-xplained -a serialflash -c erase::0x3000 -c writeboot:at91bootstrap-sama5d2_xplained.bin
```

**6**&nbsp;&nbsp;Flash the u-boot, only the first time or when the u-boot has changed:

```
$ sam-ba -p usb -b sama5d2-xplained -a serialflash -c erase:0x8000:0x70000 -c write:u-boot-sama5d2-xplained-emmc.bin:0x8000
```

**7**&nbsp;&nbsp;Flash this image, which contains the rootfs, kernel, and our code:

```
$ sam-ba -p usb -b sama5d2-xplained -a sdmmc -c write:core-image-minimal-sama5d2-xplained-emmc.wic
```

**8**&nbsp;&nbsp;After flashing, set the BUREG to disable a boot attempt from emmc1:

```
$ sam-ba -p usb -d sama5d2 -a bootconfig -c writecfg:bscr:valid,bureg0 -c writecfg:bureg0:QSPI0_IOSET1,QSPI1_IOSET1,SPI0_IOSET1,SPI1_IOSET1,NFC_IOSET1,SDMMC0,SDMMC1_DISABLED,UART1_IOSET1,JTAG_IOSET1
```

**9**&nbsp;&nbsp;To read it back, type:

```
$ sam-ba -p usb -d sama5d2 -a bootconfig -c readcfg:bscr -c readcfg:bureg0 -c readcfg:bureg1 -c readcfg:bureg2 -c readcfg:bureg3 -c readcfg:fuse
```

**10**&nbsp;&nbsp;After flashing, the image boots into the bootloader prompt because it doesn’t have the `u-boot.env` file. Set the `bootcmd` and save the boot env:

```
=> setenv bootcmd 'fatload mmc 0:1 0x21000000 sama5d2_xplained.itb; bootm 0x21000000#kernel_dtb#'
=> saveenv
Saving Environment to FAT... writing uboot.env
OK
```

**11**&nbsp;&nbsp;Reboot to kernel by pushing the RESET button or by typing the `boot` command.

### Debugging

The error shown below means you need to open the BOOT_DIS jumper, and try again:

```
$ sam-ba -p usb -b sama5d2-xplained -a serialflash -c erase::0x3000 -c writeboot:at91bootstrap-sama5d2_xplained.bin
Opening serial port 'ttyACM0'
Connection opened.
/home/user/Downloads/myTools/sam-ba_3.2.3/qml/SAMBA/Applet.qml:232: Error: Could not initialize applet (status: 1)
Connection closed.
```

## Connect the Peaberry Security Module

The SAMA5D2 board has a different I/O connector pinout than the Peaberry board, so it must be manually wired to connector J18 on the board.

**1**&nbsp;&nbsp;Use four (4) short jumper wires (such as https://www.adafruit.com/product/1956 or anything similar) to connect the four (4) pins shown on the Peaberry board to pins 1, 2, 7, and 8 on connector J18 on the SAMA5D2 board.

**2**&nbsp;&nbsp;In the first diagram below, the Peaberry board is shown with the connector facing up. Connect the four pins shown to connector J18 as follows:

    Power VDD to J18 pin 1
    GND to J18 pin 2
    I2C_SCL to J18 pin 7
    I2C_SDA to J18 pin 8

**3**&nbsp;&nbsp;The two (2) unmarked pins on the Peaberry board must remain unconnected.

<img src="../img/PeaberryPinDiag.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

<img src="../img/CapExpHeaders-SAMA5D2.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

## Build the Potenco SDK

The Potenco SDK is a set of binary packages that you can supply to your customers to develop applications for the Potenco OS, without having access to all the repositories needed to build the entire OS image.

Installation and use of the Potenco SDK is covered in a separate document.

**1**&nbsp;&nbsp;Set up the environment.

```
$ cd ~/sama5/
$ source oe-init-build-env build-microchip
```

**2**&nbsp;&nbsp;Now you can build the SDK:

```
$ bitbake -c populate_sdk core-image-minimal
```

**3**&nbsp;&nbsp;The SDK (.sh file) can be found at:

```
/poky/build-microchip/tmp/deploy/sdk
```

## Use Potenco as an Afero Hub

Download the Afero mobile app, if you haven’t already, and sign in. The Peaberry board should have come with a couple of QR Code stickers, including one on the back of the board. Scan the QR Code in the Afero mobile app to add the Potenco hub to your account. Once online, any Afero BLE devices on your account will use the Potenco hub to connect to the Afero Cloud.

## Implement Your IoT Project

Potenco also acts as an Afero edge device and can communicate with the Afero Cloud as a connected device in addition to acting as a hub. You can create a Potenco profile in the Afero Profile Editor, and attributes that you define in that profile can be accessed via scripts running on the Potenco board.

## Support for Questions and Issues

If you have any questions or issues with Potenco please use the project's [GitHub Project Issue Tracker](https://github.com/AferoCE/potenco/issues).

 **&#8674;** *Next:* [Add an afLib C-Based App to Potenco Build for SAMA5D2](../LinuxSDK-PotencoSAMA5D2App)