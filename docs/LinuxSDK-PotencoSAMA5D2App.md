# Add an afLib C-Based App to the Potenco Build for SAMA5D2

Now that you have successfully built Potenco using the instructions in the Linux SDK documentation, you may want to add your own recipe and application. In the sections below, we will lay out both the instructions for how to do this, and the reasoning behind the instructions.

- [About BitBake Layers](../LinuxSDK-PotencoSAMA5D2App#about-bitbake-layers)
- [Prerequisites](../LinuxSDK-PotencoSAMA5D2App#prerequisites)
- The Steps
    - [Step 1. Have Your Potenco Build Image](../LinuxSDK-PotencoSAMA5D2App#step-1-have-your-potenco-build-image)
    - [Step 2. Create af-app Directory](../LinuxSDK-PotencoSAMA5D2App#step-2-create-af-app-directory)
    - [Step 3. Add Source File](../LinuxSDK-PotencoSAMA5D2App#step-3-add-source-file)
    - [Step 4. Create the makefile](../LinuxSDK-PotencoSAMA5D2App#step-4-create-the-makefile)
    - [Step 5. Create the BitBake Layer](../LinuxSDK-PotencoSAMA5D2App#step-5-create-the-bitbake-layer)
    - [Step 6. Create the BitBake Recipe for the App](../LinuxSDK-PotencoSAMA5D2App#step-6-create-the-bitbake-recipe-for-the-app)
    - [Step 7. Create the layer.conf File](../LinuxSDK-PotencoSAMA5D2App#step-7-create-the-layerconf-file)
    - [Step 8. Modify bblayers.conf File](../LinuxSDK-PotencoSAMA5D2App#step-8-modify-bblayersconf-file)
    - [Step 9. Build Application](../LinuxSDK-PotencoSAMA5D2App#step-9-build-application)
    - [Step 10. Flash or Copy App Image to Target Device](../LinuxSDK-PotencoSAMA5D2App#step-10-flash-or-copy-app-image-to-target-device)
    - [Step 11. Create and Publish Device Profile](../LinuxSDK-PotencoSAMA5D2App#step-11-create-and-publish-device-profile)
    - [Step 12. Open the App on the Device](../LinuxSDK-PotencoSAMA5D2App#step-12-open-the-app-on-the-device)
- [Using the Ready-Made Example App](../LinuxSDK-PotencoSAMA5D2App#using-the-ready-made-example-app)

We are assuming some general familiarity with Yocto and BitBake so will not be defining the terminology used.

## About BitBake Layers

BitBake uses the concept of “layers” to control the flow for downloading, compiling, linking, staging, and building the final root filesystem. We are going to add our own layer, which we will call “`app`”. You will be using the command “`bitake app`” to create the application binary file. It is important to note that exactly how you do this will depend entirely on the starting Yocto build system that you use for your target processor.

Every manufacturer of hardware who supplies such a build system will typically put things in different places. For the SAMA5D2 board, you will find the source code in several different locations depending on which part of the system you are trying to build. Afero has also chosen to place all our components in the top-level directory. This is also where we will be putting your application program.

Before we get started, let’s get some preliminaries out of the way.

## Prerequisites

- You have completed the instructions for building the Atmel SAMA5D2 based Potenco project, contained in the section [Build the SAMA5D2 OS Image](../LinuxSDK-PotencoSAMA5D2#build-the-sama5d2-linux-os-image).
- You have named the directory in which the Potenco project exists as “`sama5/src_afero`” and placed it in your home directory. If you choose to use a different path or directory name, you must adjust the instructions below accordingly.
- You must have at least 64GB of disk space and 4GB of memory in your Ubuntu 16.04 system to successfully build the Potenco image.

## The Steps

### Step 1. Have Your Potenco Build Image

Again, be sure you have created a Potenco build image per the instructions in [Build the SAMA5D2 OS Image](../LinuxSDK-PotencoSAMA5D2#build-the-sama5d2-linux-os-image). This will be both the core image that is used on the Atmel SAMA5D2 and the image to which you will add your own application code.

The first-time build of the Potenco image can take up to several hours, depending on the speed of your computer. In the instructions below, you will create your own BitBake layer so you can create your application by simply typing “`bitake app`”, avoiding having to rebuild from scratch. Also, when building the root filesystem, the time required will be much shorter once the Potenco image has been built vs. having to build it from scratch; that is, it will take minutes rather than hours.

### Step 2. Create af-app Directory

We will need to create a few directories, the first will be `af-app`. Create this at the root of the `sama5` directory:

```
$ cd sama5
$ cd src_afero
$ mkdir af-app
```

This is where your application will go, along with your makefile and any header files that you create. Your source as well as your object and final app binary files will be placed here during the build process.

### Step 3. Add Source File

Now that we have a place to put your source file, let’s add it. We can call it “`test_aflib_edge.c`”. The purpose of this program is to:

- Wait for a change in the Wi-Fi signal strength, then…
- Set attribute ID 2 to the value of 8, and finally…
- Send that new value to the Afero Cloud.

The `test_aflib_edge.c` file contents look like this:

```
/**
   Copyright 2019 Afero, Inc.
   This program is a demonstration program that shows you how to capture
   an event from the Afero Cloud to set an edge attribute, and do something with
   the data that is sent to you. It also includes the ability to send a data
   item from the edge device up to the Cloud. This basic functionality is
   the basis of all communications with the Afero Cloud and the device.
   Receiving an attribute is typically used for some kind of control
   on the device itself. And sending an attribute to the Cloud is typically used
   for reporting some kind of operational status from the device back to the
   Cloud and mobile app.
*/
 
// This is a simple unit test 'EDGED' app, simulating the handling of
// edge attributes (in the hub world).
 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <event2/event.h>
#include <event2/thread.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
 
#include "af_attr_def.h"
#include "af_attr_client.h" // AF_ATTR_MAX_LISTEN_RANGES
#include "af_log.h"
#include "af_rpc.h"
#include "af_ipc_server.h"
#include "aflib.h"
#include "aflib_mcu.h"
 
af_lib_t          *sAf_lib    = NULL;  // Currently not used in hub afLib.
struct event_base *sEventBase = NULL;  // We definitely need this.
 
// ##############
//
// EDGE ATTRIBUTES (or your APP attributes)
//
// EXAMPLE of handle edge/MCU attributes (which should have been defined in the
// loaded Profile on the device):
//
// attribute with ID 1 : led_button, has value on/off or (1/0)
// attribute with ID 2 : represents some kind of state value (uint32)
 
uint8_t   attr_id_1_led_button = 0;  // attributeId = 1
uint32_t  attr_id_2_state      = 0;  // attributeId = 2
 
// END EDGE ATTRIBUTES
// ##############
 
// EDGED (or your app): This is what you need to program.
//
// This callback is executed any time ASR has information for the MCU.
// The name of this event is defined in the afLib initialization in setup().
void attrEventCallback(const af_lib_event_type_t eventType,
                       const af_lib_error_t error,
                       const uint16_t attributeId,
                       const uint16_t valueLen,
                       const uint8_t* value)
{
    char hexBuf[80];
    bool set_succeeded = 1;
    uint8_t  ret;
 
   
    memset(hexBuf, 0, sizeof(hexBuf));
    if (value != NULL) {
        af_util_convert_data_to_hex_with_name("value", (uint8_t *)value,
                                          valueLen, hexBuf, sizeof(hexBuf));
    }
 
    AFLOG_INFO("attrEventCallback:attrid:%d, %s, len=%d", attributeId, hexBuf, valueLen);
    switch (eventType) {
        // Unsolicited notification when a non-MCU attribute changes state.
        case AF_LIB_EVENT_ASR_NOTIFICATION: // non-edge attribute notify
            AFLOG_INFO("NOTIFICATION EVENT: for attr=%d", attributeId);
 
            // TESTING: using the notification of the WIFI_RSSI as trigger to
            //   test af_lib_set_attribute_xx call.
            //
            // We are interested in attribute Wi-Fi RSSI (65005).
            if (attributeId == AF_ATTR_WIFISTAD_WIFI_RSSI) {
               // TESTING: send attributeId =2, with value=8 up
               //   (simulating an edge attribute is changed and need to do a af_lib_set_xx).
               AFLOG_INFO("EDGED: set_attribute_32() for id=2, value=8");
               attr_id_2_state = 8;
               ret = af_lib_set_attribute_32(sAf_lib, 2,
                                             attr_id_2_state, AF_LIB_SET_REASON_LOCAL_CHANGE);
               if (ret != AF_SUCCESS) {
                   AFLOG_ERR("af_lib_set_attribute: failed set for the test attributeId=2");
               }
            }
            break;
 
 
        case AF_LIB_EVENT_ASR_SET_RESPONSE:
            AFLOG_INFO("ASR_SET_RESPONSE EVENT:");
            break;
 
        case AF_LIB_EVENT_MCU_SET_REQUEST: // edge attribute set request
            AFLOG_INFO("MCU_SET_REQUEST EVENT: for attr=%d", attributeId);
            if (attributeId == 1) {
                attr_id_1_led_button = *value;
                af_lib_send_set_response(sAf_lib, attributeId, set_succeeded, 1, (const uint8_t *)&attr_id_1_led_button);
            }
            else if (attributeId == 2) {
                attr_id_2_state = *value;
                AFLOG_INFO("EDGED: new value=%d", attr_id_2_state);
                af_lib_send_set_response(sAf_lib, attributeId, set_succeeded, 1, (const uint8_t *)&attr_id_2_state);
            }
            else {
                // Don't know anything about these attributes, reject.
                set_succeeded = 0;  // failed the set
                af_lib_send_set_response(sAf_lib, attributeId, set_succeeded, valueLen, value);
            }
            break;
 
        case AF_LIB_EVENT_MCU_DEFAULT_NOTIFICATION: // Edge attribute changed.
            AFLOG_INFO("EDGE ATTR changed: for attr=%d", attributeId);
            // Your code to handle whatever needs to be done if you are interested in a particular attribute.
            break;
 
        case AF_LIB_EVENT_ASR_GET_REQUEST: {
            AFLOG_INFO("EDGE ATTR get_reqeust: for attr=%d", attributeId);
            // Attribute_store asks for the current value of attribute
            // (belongs to edged or MCU). Responding with attribute and its value
            // Note: af_lib_set_attribute_xx, where xx depends on the type of attributes.
 
            // Note the following in the call: AF_LIB_SET_REASON_GET_RESPONSE indicates
            //  it is a reply for the get_request.
            if (attributeId == 1) {
                ret = af_lib_set_attribute_8(sAf_lib, attributeId,
                                             attr_id_1_led_button, AF_LIB_SET_REASON_GET_RESPONSE);
            }
            else if (attributeId == 2) {
                AFLOG_INFO("af_lib_set_attriubte_32, with AF_LIB_SET_REASON_GET_RESPONSE");
                ret = af_lib_set_attribute_32(sAf_lib, attributeId,
                                             attr_id_2_state, AF_LIB_SET_REASON_GET_RESPONSE);
            }
            else {
                ret = AF_ERROR_NOT_SUPPORTED;
            }
 
            if (ret != AF_SUCCESS) {
                AFLOG_ERR("af_lib_set_attribute: failed for request id:%d", attributeId);
            }
            } // AF_LIB_EVENT_ASR_GET_REQUEST
            break;
 
 
        default:
           AFLOG_INFO("EVENT: %d, received but not handled", eventType);
           break;
 
    } // End switch.
}
 
/* This is the test program to verify afLib implementation */
int main(int argc, char *argv[])
{
    int retVal = AF_SUCCESS;
 
   /* Enable pthreads. */
    evthread_use_pthreads();
 
    /* Get an event_base. */
    sEventBase = event_base_new();
    /* And make sure we actually got one! */
    if (sEventBase == NULL) {
        /* var/log/messages entry to show where we are in this app. */
        AFLOG_ERR("main_event_base_new::can't allocate event base");
        retVal = -1;
        return (retVal);
    }
 
    AFLOG_INFO("EDGE: start");
 
    /* The event we want to register is the Afero library's getting data. */
    retVal = af_lib_set_event_base(sEventBase);
    if (retVal != AF_SUCCESS) {
        AFLOG_ERR("main_set_event_base::set event base failed");
        goto err_exit;
    }
    AFLOG_INFO("EDGE: call af_lib_create_with_unified_callback");
    /*
       Now create the event and point to our callback function when that event
       occurs. Note that this function, af_lib_create_with_unified_callback, will
       automatically subscribe you to attributes 1 - 1023, and several Wi-Fi, WAN, and Ethernet
       attributes as well as the Profile change attribute.
    */
    sAf_lib = af_lib_create_with_unified_callback(attrEventCallback, NULL);
    if (sAf_lib == NULL) {
        AFLOG_ERR("main_event_base_new::can't allocate event base");
        retVal = -1;
        goto err_exit;
    }
    AFLOG_INFO("EDGE: dispatching event base");
    /* Start it up! This will not return until
       either there are no more events or the loop exit
       or kill functions are called.
    */
    event_base_dispatch(sEventBase); 
err_exit:
    // Done, let's close/clean up.
    AFLOG_INFO("EDGED:  shutdown");
    af_lib_shutdown();
    return (retVal);
}
```

Now that we have the code ready, we need a way to compile it. This is where things get interesting with Yocto and BitBake. The makefile we create cannot be run directly; we can only invoke it through BitBake. This is because BitBake provides several paths that are necessary to find all the needed library elements. So before we can compile and test, we need to create the makefile and add the BitBake layer that will then invoke the makefile with the correct system paths.

### Step 4. Create the makefile

Start by opening your favorite text editor and create “`makefile`” in the `af-app` directory with contents as follows:

```
#  Copyright (c) 2019 Afero, Inc. All rights reserved.
APP_LIBS_NEEDED :=   -lpthread -levent_pthreads -levent -laf_util -laf_edge -laf_ipc -laf_attr
default: all
all: app
app: test_aflib_edge.c
            $(CC) $(CFLAGS)  -L $(APP_LIBS_NEEDED) -L $(APP_LIBS_NEEDED) -o app test_aflib_edge.c
```

Now that we have the makefile, we’re ready to create the BitBake layer.

### Step 5. Create the BitBake Layer

There are BitBake commands that create a BitBake layer for you; however, we will do this manually.

The Afero stack keeps its layers in the `~/sama5/meta-afero` directory. Our app will live in its own layer and will be named “`meta-app`”. Create it at the same directory level as `meta-afero` and the other layers; that is, in the directory `~/sama5`:

```
$ cd ~/sama5
$ mkdir meta-app
```

Now we need two subdirectories in `meta-app`: `conf` and `recipes-app`. Type the following:

```
$ cd meta-app
$ mkdir conf
$ mkdir recipes-app
```

### Step 6. Create the BitBake Recipe for the App

The recipe provides the structure we need to add our recipes and some BitBake configuration. Let’s start with the BitBake recipe for the app:

```
$ cd recipes-app
```

We will create our recipe in this `recipes-app` directory. The recipe name is not important, but usually reflects the application being built and always ends with `.bb`. We will call ours “`app_1.0.bb`”. Using your favorite editor, create the file and add the following contents to it:

```
# Copyright (C) 2019 Afero, Inc. All rights reserved
# human friendly description
DESCRIPTION = "Afero example app"
SECTION = "examples"
# A few things that this recipe will depend on follow.
DEPENDS = "libevent af-util af-ipc attrd af-edge"
# License is unimportant here for us, as is the checksum.
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""
# Bring in a few tools for the build, such as being able to define where we want the build to occur and the systemd libraries.
inherit externalsrc systemd 
# Point to where our source code is.
EXTERNALSRC = "${TOPDIR}/../af-app"
 
PARALLEL_MAKE = ""
# This is what we want it to do to compile the code, cd into the source directory, and run the makefile found there. 
do_compile(){
            cd ${S}
            oe_runmake
}
# For installing, we will copy the app created to the /usr/bin/staging directory for this layer and set its permissions to rwxr-x-r-x after first making sure the target directory exists.
do_install_append() {
    install -d ${D}/usr/bin
    install -m 755 ${EXTERNALSRC}/app ${D}/usr/bin
}
```

### Step 7. Create the layer.conf File

Now that we have the BitBake recipe, we need to create a configuration file that informs BitBake what to do with that file, the path, where the recipes are located, and what they are called.

The `.conf` file also tells BitBake what the target is called; in this case it’s called “`app`”. You can also assign a “priority”. We are using the same priority as all other Afero BitBake recipes, which is **6**.

So let’s get out the editor again and create the `layer.conf` file that BitBake will look for. It belongs in the `meta-app/conf` directory and contains the following:

```
# We have a conf and classes directory, add to BBPATH
BBPATH := "${BBPATH}:${LAYERDIR}"
# We have a recipes directory, add to BBFILES
BBFILES := "${BBFILES} ${LAYERDIR}/recipes-*/*.bb \
        ${LAYERDIR}/recipes-*/*.bbappend"
BBFILE_COLLECTIONS += "app"
BBFILE_PATTERN_app := "^${LAYERDIR}/"
BBFILE_PRIORITY_app = "6"
```

### Step 8. Modify bblayers.conf File

Okay! Now that we have the BitBake recipe and the `layer.conf` file created, we need to tell BitBake to look for it. There is a single file that tells BitBake about all the layers called “`bblayers.conf`”. It is always located in the `conf` directory under the build directory; i.e., the directory in which you invoke BitBake.

In our tree it is located in `~/sama5/poky/buildmicrochip/conf`.Take a look at the contents of and you will see a listing of directories. This listing was created when you first created the build directory during the configuration phase of the Potenco build. Example contents are shown below:

```
# POKY_BBLAYERS_CONF_VERSION is increased each time build/conf/bblayers.conf
# changes incompatibly
POKY_BBLAYERS_CONF_VERSION = "2"
BBPATH = "${TOPDIR}"

BBFILES ?= ""
BSPDIR := "${@os.path.abspath(os.path.dirname(d.getVar('FILE', True)) + '/../../..')}"

BBLAYERS ?= " \
  ${BSPDIR}/poky/meta \
  ${BSPDIR}/poky/meta-poky \
  ${BSPDIR}/poky/meta-yocto-bsp \
  ${BSPDIR}/meta-atmel \
  ${BSPDIR}/meta-openembedded/meta-oe \
  ${BSPDIR}/meta-openembedded/meta-networking \
  ${BSPDIR}/meta-openembedded/meta-python \
  ${BSPDIR}/meta-openembedded/meta-multimedia \
  ${BSPDIR}/meta-qt5 \
  ${BSPDIR}/meta-afero \
  "
BBLAYERS_NON_REMOVABLE ?= " \
  ${BSPDIR}/poky/meta \
  ${BSPDIR}/poky/meta-poky \
  "
```

We must modify this file so it includes the directory that is the top of the `meta-app` layer we added. It’s easily created by copying the last line (“`${BSPDIR}/meta-app \`”), pasting it below, then changing `meta-afero` to `meta-app` as shown in the `bblayers.conf` file below:

```
# POKY_BBLAYERS_CONF_VERSION is increased each time build/conf/bblayers.conf
# changes incompatibly
POKY_BBLAYERS_CONF_VERSION = "2"

BBPATH = "${TOPDIR}"
BBFILES ?= ""

BSPDIR := "${@os.path.abspath(os.path.dirname(d.getVar('FILE', True)) + '/../../..')}"

BBLAYERS ?= " \
  ${BSPDIR}/poky/meta \
  ${BSPDIR}/poky/meta-poky \
  ${BSPDIR}/poky/meta-yocto-bsp \
  ${BSPDIR}/meta-atmel \
  ${BSPDIR}/meta-openembedded/meta-oe \
  ${BSPDIR}/meta-openembedded/meta-networking \
  ${BSPDIR}/meta-openembedded/meta-python \
  ${BSPDIR}/meta-openembedded/meta-multimedia \
  ${BSPDIR}/meta-qt5 \
  ${BSPDIR}/meta-afero \
  ${BSPDIR}/meta-app \
  "

BBLAYERS_NON_REMOVABLE ?= " \
  ${BSPDIR}/poky/meta \
  ${BSPDIR}/poky/meta-poky \
  "
```

### Step 9. Build Application

We can now build the application; it will automatically be deposited in the staging directory. Type the following:

```
$ cd ~/sama5/poky/build_microchip
$ bitbake app
```

### Step 10. Flash or Copy App Image to Target Device

You are almost ready to either use `scp` to copy the app image to the target device, **or** flash the app image to the device’s internal eMMC. Both options are explained below.

#### Option 1: Flash Image to Target Device eMMC Memory

Before we can flash the image, we must tell the `core-image-minimal` BitBake recipe that there is one more thing to include in the final image. This inclusion is not done automatically for a new layer and means you must edit the BitBake recipe to add it. Note that this BitBake recipe is appended to the `core-image-minimal.bb` file located elsewhere in the build tree.

Follow the instructions below to include your app in the final image:

**1**&nbsp;&nbsp;Go to the following directory:

```
$ cd ~/sama5/meta-afero/recipes-core/images/
```

**2**&nbsp;&nbsp;Find the file named “`core-image-minimal.bbappend`” and open it for editing.

**3**&nbsp;&nbsp;You will find a line beginning with “`IMAGE_INSTALL`” that lists a large number of items:

```
IMAGE_INSTALL += "beetle hubby af-sec af-ipc af-util af-edge attrd af-conn bluez5 bluez5-noinst-tools wpa-supplicant iptables iw linux-firmware-bcm43430 curl openssl ntp vim-common otamgr i2c-tools dropbear ntp"
```

**4**&nbsp;&nbsp;This list identifies the items that are to be included in the final rootfs. Just add “`app`” to the list after the other Afero items (bolded below so you can find it easily):

```
IMAGE_INSTALL += "beetle hubby af-sec af-ipc af-util af-edge attrd af-conn bluez5 bluez5-noinst-tools app wpa-supplicant iptables iw linux-firmware-bcm43430 curl openssl ntp vim-common otamgr i2c-tools dropbear ntp"
```

**5**&nbsp;&nbsp;Save the file. Now, after you use the command “`bitbake core-image-minimal`”, the app will be deposited in `/usr/bin` in the root filesystem.

**6**&nbsp;&nbsp;Take the resulting `afimg` file, and flash it onto the SAMA5D2 board using the method outlined in the [Afero Potenco Setup Guide for SAMA5D2 Board > Flash the SAMA5D2 Image](../LinuxSDK-PotencoSAMA5D2#flash).

#### Option 2: Copy Image Using SCP

You can also securely copy the application image to the SAMA5D2 board, which is substantially faster than burning in the entire root filesystem. For this to work, the SAMA5D2 board must already be connected. The first time you do this you have to flash the entire root filesystem on the SAMA5D2; after that, if you simply want to test your app's functionality, you can use `scp`. Here’s how you do that:

**1**&nbsp;&nbsp;Get the IP address from the device by typing:

```
$ ifconfig
```

The following should return:

```
lo       Link encap:Local Loopback 
		 inet addr:127.0.0.1  Mask:255.0.0.0
		 inet6 addr: ::1%132976/128 Scope:Host
		 UP LOOPBACK RUNNING  MTU:65536  Metric:1
		 RX packets:163 errors:0 dropped:0 overruns:0 frame:0
		 TX packets:163 errors:0 dropped:0 overruns:0 carrier:0
		 collisions:0 txqueuelen:1
		 RX bytes:12376 (12.0 KiB)  TX bytes:12376 (12.0 KiB)

wlan0     Link encap:Ethernet  HWaddr 40:BD:32:77:FE:84 
		 inet addr:10.177.227.122  Bcast:10.177.227.255  Mask:255.255.255.0
		 UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
		 RX packets:15462 errors:0 dropped:0 overruns:0 frame:0
		 TX packets:18874 errors:0 dropped:0 overruns:0 carrier:0
		 collisions:0 txqueuelen:1000
		 RX bytes:1585679 (1.5 MiB)  TX bytes:2183887 (2.0 MiB)
```

**2**&nbsp;&nbsp;The device you are interested in is **wlan0**. The IP address of the device is identified in this internet address value: “`inet addr: 10.177.227.122`”.

**3**&nbsp;&nbsp;Go to the directory where the app is located, then type the following on your Ubuntu build system, using the inet address:

```
$ scp app root@10.177.227.122:/usr/bin `/usr/bin` on the SAMA5D2 board.
```

Now we will create the Potenco device Profile and publish it to the device.

### Step 11. Create and Publish Device Profile

After the system boots, you can type `**app**` at the command prompt and the application will run. However, it won’t **do** anything until you have pushed a new profile to your Potenco device that includes two new attributes. Use the Afero Profile Editor to create a new Profile that defines these attributes. You can read details on using the Profile Editor in the [Profile Editor User Guide](../Projects), but the specific instructions follow:

**1**&nbsp;&nbsp;Open the latest version of the Afero Profile Editor and sign in with your Afero developer account. (Download for either [Windows](http://cdn.afero.io/latest-ape/win) or [macOS](http://cdn.afero.io/latest-ape/mac).)

**2**&nbsp;&nbsp;On the Startup window, select NEW. On the New Project dialog, complete the fields as follows:

- **Module Type**: Potenco
- **Device Type Name**: MyApp (or any name you prefer)
- **Project Folder**: Default location is fine
- **Create Subfolder using Device Type Name**: Leave selected

Select the CREATE button.

**3**&nbsp;&nbsp;On the DEFINE THE DEVICE TYPE window that appears, add a DESCRIPTION if you wish then select the SAVE button in the upper-right.

**4**&nbsp;&nbsp;Select  ATTRIBUTES in the left-hand Navigation pane. On the DEFINE THE MCU ATTRIBUTES window, complete the fields as follows:

- **Supported Network Interfaces**: Select Wi-Fi and WAN

- **Device Configuration**, **Receive UTC Time**: Leave deselected

- **Device Attribute** - Create two MCU attributes using the +DEVICE ATTRIBUTE button:
     - **MCU attribute ID 1** - Name: int8, Data Type: SINT8, Default Value: 0, Operations: Read and Write
     - **MCU attribute ID 2** - Name: int32, Data Type: SINT32, Default Value: 0, Operations: Read and Write

Select the SAVE button in the upper-right.

**5**&nbsp;&nbsp;Select UI CONTROLS in the left-hand Navigation pane. On the DEFINE THE UI CONTROLS window, define two Value controls that will simply store and show the values to the app:

***5a***&nbsp;&nbsp;First, define two attribute options using the +ATTRIBUTE OPTION button in the right-hand pane:

- **int8** - Attribute: int8, Label: int8, leave remaining fields as shown
- **int32** - Attribute: int32, Label: int32, leave remaining fields as shown

***5b***&nbsp;&nbsp;Next, define two controls using the +CONTROL button:

- **Value control 1** - Attribute Option: int8, Control Type: Value, leave remaining fields as shown
- **Value control 2** - Attribute Option: int32, Control Type: Value, leave remaining fields as shown

Select the SAVE button in the upper-right.

**6**&nbsp;&nbsp;Select UI CONTROL GROUPS in the left-hand Navigation pane. On the DEFINE THE UI CONTROL GROUPS window:

***6a***&nbsp;&nbsp;Select the + button to create a new group.

***6a***&nbsp;&nbsp;Name your control group as you wish by first selecting “New Group” then editing the name.

***6a***&nbsp;&nbsp;Drag both Value controls from the right-hand pane into the group box.

Select the SAVE button in the upper-right.

**7**&nbsp;&nbsp;Select PUBLISH in the left-hand Navigation pane. On the PUBLISH window:

***7a***&nbsp;&nbsp;Select the checkbox adjacent to your Potenco device name.

***7a***&nbsp;&nbsp;Select the PUBLISH button. Your Profile will be published to your Potenco; monitor progress by watching the DEVICE ACTIVITY pane.

***7a***&nbsp;&nbsp;Once published, you can close the Afero Profile Editor.

### Step 12. Open the App on the Device

On the Potenco device, first go to the directory where the app is located:

```
$ cd /usr/bin
```

From here you can invoke the app directly by typing:

```
$ ./app
```

**or**, to force the app to run in the background, type:

```
$ ./app &
```

It will look like the program is hanging, but it is waiting for the Wi-Fi signal strength to change. When it changes, an event is generated that the code will see and respond to by setting attribute 2 value to 8. To make this happen, you can simply cup your hands over the Wi-Fi antenna. What you will see is the app suddenly showing a value of 8 for the attribute ID 2 value.

## Using the Ready-Made Example App

The example above was fairly straightforward and used a type of system event, RSSI change, to signal a developer-defined attribute change. A more robust example is available on GitHub, and it will give you a starting point for a more useful application that includes how to manage strings, respond to attribute set requests, as well as having a more extensive explanation of what’s going on within the code itself. In order to get and use this package, follow the instructions below.

Note that these instructions are very similar to those above and use identical concepts in creating and maintaining your own BitBake layer.

### Prerequisites

- You have completed the instructions for building the Atmel SAMA5D2-based Potenco project, contained in the section [Build the SAMA5D2 OS Image](../LinuxSDK-PotencoSAMA5D2#build-the-sama5d2-linux-os-image).
- You have named the directory in which the Potenco project exists as “`sama5/src_afero`” and placed it in your home directory. If you choose to use a different path or directory name, you must adjust the instructions below accordingly.
- You have at least 64GB of disk space and 4GB of memory in your Ubuntu system to successfully build the Potenco image.
- You have a GitHub account and have been given access to the am335x-demo-app GitHub repository. Contact the Afero Customer Enablement (ACE) team at “ace@afero.io” for access and the GitHub repo URL.

### Steps

**1.** Let’s start with getting the GitHub package. You should put it into the root of your home directory. Use the following command:

```
$ cd
```

**2**&nbsp;&nbsp;Download the package from the GitHub repository using the URL given to you by the ACE team:

```
$ git clone <GitHub repo URL> am335x-demo-app.git
```

The package will clone to a directory named “am335x-demo-app”. 

**3**&nbsp;&nbsp;Within this `am335x-demo-app` directory, you will find the directories and directory trees listed below. To install them, simply copy them into your own instance of the Potenco reference build.

- `af-app` - Holds your makefile and your application code.

- `meta-app` - Top-level BitBake recipe directory that defines when and how your application is built.

     - `conf` - Resides under `meta-afero` and holds the layer configuration file for the meta-app BitBake layer that describes how to build your application.
     
     - `recipes-app` - Resides under the `meta-afero` directory and holds the actual BitBake recipe for your application.

- `APEProject` - Holds directory containing the Afero Profile Editor project that defines the attributes used in this demo, as well as the UI definition for the Afero mobile app.

**4**&nbsp;&nbsp;Enter the directory that was created when you cloned the GitHub repo. The directory should be named `am335x-demo-app`:

```
$ cd am335x-demo-app
```

**5**&nbsp;&nbsp;List the contents:

```
$ ls
af-app meta-app APEProject
```

**6**&nbsp;&nbsp;Now copy the application source code that’s in the `af-app` directory to the top level of the `~/sama5/src_afero` directory tree:

```
$ cp -r af-app ~/sama5/src_afero
```

**7**&nbsp;&nbsp;List the directory contents to verify:

```
$ ls ~/sama5/src_afero
af-app af-conn af-edge af-ipc af-sec af-util attrd beetle hubby otamgr
```

**8**&nbsp;&nbsp;Copy the `meta-app` directory to the `~/sama5` directory tree so that it can be found by BitBake and the recipes extracted from it, then verify the contents:

```
$ cp -r meta-app ~/sama5
$ ls ~/sama5
meta-afero meta-app meta-atmel meta-openembedded meta-qt5 poky src_afero
```

**9**&nbsp;&nbsp;Enter the `~/sama5/poky/build-microchip/conf` directory, then look at the contents of the `bblayers.conf` file:

```
$ cd ~/sama5
$ ls
meta-afero  meta-app  meta-atmel  meta-openembedded  meta-qt5  poky  src_afero
$ cd conf
$ ls
bblayers.conf  local.conf  sanity_info  templateconf.cfg
$ more bblayers.conf

# POKY_BBLAYERS_CONF_VERSION is increased each time build/conf/bblayers.conf
# changes incompatibly

POKY_BBLAYERS_CONF_VERSION = "2"

BBPATH = "${TOPDIR}"
BBFILES ?= ""
BSPDIR := "${@os.path.abspath(os.path.dirname(d.getVar('FILE', True)) + '/../../..')}"

BBLAYERS ?= " \
  ${BSPDIR}/poky/meta \
  ${BSPDIR}/poky/meta-poky \
  ${BSPDIR}/poky/meta-yocto-bsp \
  ${BSPDIR}/meta-atmel \
  ${BSPDIR}/meta-openembedded/meta-oe \
  ${BSPDIR}/meta-openembedded/meta-networking \
  ${BSPDIR}/meta-openembedded/meta-python \
  ${BSPDIR}/meta-openembedded/meta-multimedia \
  ${BSPDIR}/meta-qt5 \
  ${BSPDIR}/meta-afero \
  "

BBLAYERS_NON_REMOVABLE ?= " \
  ${BSPDIR}/poky/meta \
  ${BSPDIR}/poky/meta-poky \

  "
```

**10**&nbsp;&nbsp;Open this `bblayers.conf` file for edit. Add the path to the `meta-app` directory after the `meta-afero` line. As an example, here are the last three lines defining BBLAYERS with the `meta-app` path added:

```
⋮
${BSPDIR}/meta-afero \
${BSPDIR}/meta-app \
"
```

**11**&nbsp;&nbsp;Enter the `~/sama5/poky/build-microchip` directory and use the following command to build your own application.

```
$ cd ~/sama5/poky/build-microchip
$ bitbake app
```

**12**&nbsp;&nbsp;At this point you have created an image named “app”. You can `scp` this image to the SAMA5D2 board.

**13**&nbsp;&nbsp;If you wish to have the file added to the final root filesystem image, we must tell the controlling BitBake recipe how to do so by editing the `~/sama5/meta-afero/recipes-core/images/core-image-minimal.bbappend` file, as explained below:

***13a***&nbsp;&nbsp;Go to the following directory:

```
$ cd ~/sama5/meta-afero/recipes-core/images/
```

***13b***&nbsp;&nbsp; Find the file named “`core-image-minimal.bbappend`” and open it for editing.

***13c***&nbsp;&nbsp;You will find a line beginning with “`IMAGE_INSTALL`” that lists a large number of items:

```
IMAGE_INSTALL += "beetle hubby af-sec af-ipc af-util af-edge attrd af-conn bluez5 bluez5-noinst-tools wpa-supplicant iptables iw linux-firmware-bcm43430 curl openssl ntp vim-common otamgr i2c-tools dropbear ntp"
```

***13d.***&nbsp;&nbsp;This list identifies the items that are to be included in the final rootfs. Just add “`app`” to the list after the other Afero items (bolded below so you can find it easily):

```
IMAGE_INSTALL += "beetle hubby af-sec af-ipc af-util af-edge attrd af-conn bluez5 bluez5-noinst-tools app wpa-supplicant iptables iw linux-firmware-bcm43430 curl openssl ntp vim-common otamgr i2c-tools dropbear ntp"
```

***13e***&nbsp;&nbsp;Save the file. Then execute the following command:

```
bitbake core-image-minimal
```

The app will be deposited in `/usr/bin` in the root filesystem.

Now you are ready to flash the image to the internal eMMC. Take the resulting `afimg` file and flash it onto the SAMA5D2 board using the method outlined in the [Potenco Setup Guide for SAMA5D2](../LinuxSDK-PotencoSAMA5D2#flash-the-sama5d2-image).

 *Next:*&nbsp;&nbsp;[**&#8674;** Secure Linux SDK Frequently-Asked Questions](../LinuxSDK-FAQ)