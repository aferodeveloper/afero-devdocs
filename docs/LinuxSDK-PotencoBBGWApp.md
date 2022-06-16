# Add an afLib C-Based App to the Potenco Build for BeagleBone Green Wireless

Now that you have successfully built Potenco using the instructions in the Linux SDK documentation, you may want to add your own recipe and application. In the sections below, we will lay out both the instructions for how to do this, and the reasoning behind the instructions.

- [About BitBake Layers](../LinuxSDK-PotencoBBGWApp#about-bitbake-layers)
- [Prerequisites](../LinuxSDK-PotencoBBGWApp#prerequisites)
- The Steps
    - [Step 1. Have Your Potenco Build Image](../LinuxSDK-PotencoBBGWApp#step-1-have-your-potenco-build-image)
    - [Step 2. Create af-app Directory](../LinuxSDK-PotencoBBGWApp#step-2-create-af-app-directory)
    - [Step 3. Add Source File](../LinuxSDK-PotencoBBGWApp#step-3-add-source-file)
    - [Step 4. Create the makefile](../LinuxSDK-PotencoBBGWApp#step-4-create-the-makefile)
    - [Step 5. Create the BitBake Layer](../LinuxSDK-PotencoBBGWApp#step-5-create-the-bitbake-layer)
    - [Step 6. Create the BitBake Recipe for the App](../LinuxSDK-PotencoBBGWApp#step-6-create-the-bitbake-recipe-for-the-app)
    - [Step 7. Create the layer.conf File](../LinuxSDK-PotencoBBGWApp#step-7-create-the-layerconf-file)   
    - [Step 8. Modify bblayers.conf File](../LinuxSDK-PotencoBBGWApp#step-8-modify-bblayersconf-file)
    - [Step 9. Build Application](../LinuxSDK-PotencoBBGWApp#step-9-build-application)
    - [Step 10. Copy App to Target Machine or Create SD Card Image](../LinuxSDK-PotencoBBGWApp#step-10-copy-app-to-target-machine-or-create-sd-card-image)
    - [Step 11. Create and Publish Device Profile](../LinuxSDK-PotencoBBGWApp#step-11-create-and-publish-device-profile)
    - [Step 12. Open the App on the Device](../LinuxSDK-PotencoBBGWApp#step-12-open-the-app-on-the-device)
- [Using the Ready-Made Example App](../LinuxSDK-PotencoBBGWApp#using-the-ready-made-example-app)

We are assuming some general familiarity with Yocto and BitBake so will not be defining the terminology used.

## About BitBake Layers

BitBake uses the concept of “layers” to control the flow for downloading, compiling, linking, staging, and building the final root filesystem. We are going to add our own layer, which we will call “`app`”. You will be using the command “`bitake app`” to create the application binary file. It is important to note that exactly how you do this will depend entirely on the starting Yocto build system that you use for your target processor.

Every manufacturer of hardware who supplies such a build system will typically put things in different places. For the BeagleBone Green Wireless board, you will find the source code in several different locations depending on which part of the system you are trying to build. Afero has also chosen to place all our components in the top-level directory. This is also where we will be putting your application program.

Before we get started, let’s get some preliminaries out of the way.

## Prerequisites

- You have completed the instructions for building the BeagleBone Green Wireless-based Potenco project, contained in the section [Build the Potenco OS Image](../LinuxSDK-PotencoBBGW#build-the-potenco-os-image).

- You have named the directory in which the Potenco project exists as “potenco” and placed it in your home directory. If you choose to use a different path or directory name, you must adjust the instructions below accordingly.
- You must have at least 64GB of disk space and 4GB of memory in your Ubuntu system to successfully build the Potenco image.

## The Steps

### Step 1. Have Your Potenco Build Image

Again, be sure you have created a Potenco build image per the instructions in [Build the Potenco OS Image](../LinuxSDK-PotencoBBGW#build-the-potenco-os-image). This will be both the core image that is used on the BeagleBone Green Wireless and the image to which you will add your own application code.

<mark>**&check; Note:**  The first-time build of the Potenco image can take up to several hours, depending on the speed of your computer. In the instructions below, you will create your own BitBake layer so you can create your application by simply typing “`bitake app`”, avoiding having to rebuild from scratch. Also, when building the root filesystem, the time required will be much shorter once the Potenco image has been built vs. having to build it from scratch; that is, it will take minutes rather than hours.</mark>

### Step 2. Create af-app Directory

We will need to create a few directories, the first will be `af-app`. Create this at the root of the `potenco` directory:

```
$ cd potenco
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
// EXAMPLE of handling Edge/MCU attributes (which should have been defined in the
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
// The name of this event is defined in the aflLib initialization in setup().
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
 
            // TESTING: Using the notification of the WIFI_RSSI as a trigger to
            //   test af_lib_set_attribute_xx call.
            //
            // We are interested in attribute Wi-Fi RSSI (65005).
            if (attributeId == AF_ATTR_WIFISTAD_WIFI_RSSI) {
               // TESTING: Send attributeId =2, with value=8 up
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
 
        case AF_LIB_EVENT_MCU_SET_REQUEST: // Edge attribute set request.
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
                set_succeeded = 0;  // Failed the set.
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
            // (belongs to edged or MCU). Responding with attribute and its value.
            // Note: af_lib_set_attribute_xx, where xx depends on the type of attributes.
 
            // Note the following in the call: AF_LIB_SET_REASON_GET_RESPONSE to indicate
            // it is a reply for the get_request.
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
       either there are no more events, or the loop exit
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

Start by opening your favorite text editor and create “`makefile`” in the `af-app` directory, with contents as follows:

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

The Afero stack keeps its layers in the `~/potenco/sources/meta-afero` directory. Our app will live in its own layer and will be named “`meta-app`”. Create it at the same directory level as `meta-afero` and the other layers; that is, in the directory ~/potenco/sources:

```
$ cd ~/potenco/sources
$ mkdir meta-app
```

Now we need two subdirectories in `meta-app: conf` and `recipes-app`. Type the following:

```
$ cd meta-app
$ mkdir conf
$ mkdir recipes-app
```

<mark>**&check; Note:**  The use of a separate layer for the application allows us to use “`bitake app`” (or whatever you might change it to as directed in the `layer.conf` file, in the “`BBFILE_COLLECTIONS +="app"`” label). This allows you to build the application very quickly without having to rebuild anything else… as long as you are using `scp` to copy the binary over to the BBGW. If you wish to include it in the root filesystem so that it’s part of the SD card, then you also need to use “`bitbake arag-afer-image`”, but this, again, will not take long, only a few minutes as the parts it requires are already built and it’s simply creating the final root filesystem to burn onto the SD card.</mark>

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
# A few things that this recipe will depend on .
DEPENDS = "libevent af-util af-ipc attrd af-edge"
# License is unimportant here for us as is the checksum.
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""
# Bring in a few tools for the build, such as being able to define where we want the build to occur and the systemd libraries.
inherit externalsrc systemd 
# point to where our source code is.
EXTERNALSRC = "${TOPDIR}/../af-app"
 
PARALLEL_MAKE = ""
#this is what we want it to do to compile the code, cd into the source directory and run the makefile found there. 
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

The `.conf` file also tells BitBake what the target is called; in this case it’s called “`app`”. You can also assign a “priority”. We are using the same priority as all the other Afero BitBake recipes, which is **6**.

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

Okay! Now that we have the BitBake recipe and the `layer.conf` file created, we need to tell BitBake to look for it. There is a single file that tells BitBake about all the layers. It’s called “`bblayers.conf`”. It is always located in the `conf` directory under the build directory; i.e., the directory in which you invoke BitBake.

In our tree it is located in `~/potenco/build/conf`.Take a look at the contents of `bblayers.conf` and you will see a listing of directories. This listing was created when you first created the build directory during the configuration phase of the Potenco build. Example contents are shown below:

```
# This template file was created by taking the oe-core/meta/conf/bblayers.conf
# file and removing the BBLAYERS section at the end.
 
# LAYER_CONF_VERSION is increased each time build/conf/bblayers.conf
# changes incompatibly
LCONF_VERSION = "7"
 
BBPATH = "${TOPDIR}"
BBFILES ?= ""
 
# Layers configured by oe-core-setup script
BBLAYERS += " \
           <your_full_path>/potenco/sources/meta-processor-sdk \
           <your_full_path>/potenco/sources/meta-arago/meta-arago-distro \
           <your_full_path>/potenco/sources/meta-arago/meta-arago-extras \
           <your_full_path>/potenco/sources/meta-qt5 \
           <your_full_path>/potenco/sources/meta-openembedded/meta-networking \
           <your_full_path>/potenco/sources/meta-openembedded/meta-ruby \
           <your_full_path>/potenco/sources/meta-openembedded/meta-python \
           <your_full_path>/potenco/sources/meta-openembedded/meta-oe \
           <your_full_path>/potenco/sources/meta-ti \
           <your_full_path>/potenco/sources/meta-linaro/meta-linaro-toolchain \
           <your_full_path>/potenco/sources/oe-core/meta \
           <your_full_path>/potenco/sources/meta-afero \
"
```

We must modify this file so it includes the directory that is the top of the `meta-app` layer we added. It’s easily created by copying the last line (“`${BSPDIR}/meta-app \`”), pasting it below, then changing `meta-afero` to `meta-app` as shown in the `bblayers.conf` file below:

```
# This template file was created by taking the oe-core/meta/conf/bblayers.conf
# file and removing the BBLAYERS section at the end.
 
# LAYER_CONF_VERSION is increased each time build/conf/bblayers.conf
# changes incompatibly
LCONF_VERSION = "7"
 
BBPATH = "${TOPDIR}"
BBFILES ?= ""
# Layers configured by oe-core-setup script
BBLAYERS += " \
           <your_full_path>/potenco/sources/meta-processor-sdk \
           <your_full_path>/potenco/sources/meta-arago/meta-arago-distro \
           <your_full_path>/potenco/sources/meta-arago/meta-arago-extras \
           <your_full_path>/potenco/sources/meta-qt5 \
           <your_full_path>/potenco/sources/meta-openembedded/meta-networking \
           <your_full_path>/potenco/sources/meta-openembedded/meta-ruby \
           <your_full_path>/potenco/sources/meta-openembedded/meta-python \
           <your_full_path>/potenco/sources/meta-openembedded/meta-oe \
           <your_full_path>/potenco/sources/meta-ti \
           <your_full_path>/potenco/sources/meta-linaro/meta-linaro-toolchain \
           <your_full_path>/potenco/sources/oe-core/meta \
           <your_full_path>/potenco/sources/meta-afero \
           <your_full_path>/potenco/sources/meta-app \
"
```

### Step 9. Build Application

We can now build the application; it will automatically be deposited in the staging directory. Type the following:

```
$ cd ~/potenco/build
$ bitbake app
```

### Step 10. Copy App to Target Machine or Create SD Card Image

At this point you can take the app and use `scp` to copy it to the target machine to run; alternatively, you can create a full SD card image.

To create a full SD card image, we need to tell the arago-afero-image BitBake recipe that there is one more thing to include in the final image. This is not done automatically for a new layer and means that you have to edit the BitBake recipe to add it. This recipe is located in `./sources/meta-afero/recipes-core/images/` and is called “`arago-afero-image.bb`”.

**1**&nbsp;&nbsp;Take a look at the `arago-afero-image.bb` file. You find the following section named IMAGE_INSTALL that lists a large number of items:

```
IMAGE_INSTALL = " \
   packagegroup-core-boot \
   kernel-modules \
   bash \
   udev-extraconf \
   base-files \
   module-init-tools \
   mtd-utils \
   mtd-utils-ubifs \
   curl \
   thermal-init \
   dbus \
   expat \
   glib-2.0 \
   libxml2 \
   libpcre \
   iptables \
   arago-gpl-notice \
   util-linux-fsck \
   i2c-tools \
   usbutils \
   zlib \
   libevent \
   json-c \
   libstdc++ \
   pru-icss \
   wl18xx-target-scripts \
   wpa-supplicant-wl18xx \
   bt-firmware \
   wl18xx-firmware \
   wlconf \
   iw \
   crda \
   backports \
   af-ipc \
   af-util \
   attrd \
   af-conn \
   af-sec \
   af-edge \
   otamgr \
   hubby \
   af-extras \
   dropbear \
   lrzsz \
"
```

**2**&nbsp;&nbsp;As you can see, it lists a large number of elements that are to be included in the final rootfs. Just add “`app \`” to this list, like in the following version. Place the addition near the bottom, as shown:

```
IMAGE_INSTALL = " \
   packagegroup-core-boot \
   kernel-modules \
   bash \
   udev-extraconf \
   base-files \
   module-init-tools \
   mtd-utils \
   mtd-utils-ubifs \
   curl \
   thermal-init \
   dbus \
   expat \
   glib-2.0 \
   libxml2 \
   libpcre \
   iptables \
   arago-gpl-notice \
   util-linux-fsck \
   i2c-tools \
   usbutils \
   zlib \
   libevent \
   json-c \
   libstdc++ \
   pru-icss \
   wl18xx-target-scripts \
   wpa-supplicant-wl18xx \
   bt-firmware \
   wl18xx-firmware \
   wlconf \
   iw \
   crda \
   backports \
   af-ipc \
   af-util \
   attrd \
   af-conn \
   af-sec \
   af-edge \
   otamgr \
   hubby \
   af-extras \
   app \
   dropbear \
   lrzsz \
"
```

**3**&nbsp;&nbsp;Save the file. Now, after you use the command “`bitbake arago-afero-image`”, the app will be deposited in `/usr/bin` in the root filesystem.

**4**&nbsp;&nbsp;You can take the resulting `afimg` file, flash it on to a micro SD card, insert it into the BBGW board, and boot up the system in the usual way by holding down the User button while the device is reset, then continue to hold it until the blue LEDs start to flash, usually more than 10 seconds.

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

**4**&nbsp;&nbsp;Select  ATTRIBUTES in the left-hand Navigation pane. On the  DEFINE THE MCU ATTRIBUTES window, complete the fields as follows:

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

***5b***&nbsp;&nbsp;Next, define two controls using the  +CONTROL button:

- **Value control 1** - Attribute Option: int8, Control Type: Value, leave remaining fields as shown
- **Value control 2** - Attribute Option: int32, Control Type: Value, leave remaining fields as shown

Select the SAVE button in the upper-right.

**6**&nbsp;&nbsp;Select UI CONTROL GROUPS in the left-hand Navigation pane. On the DEFINE THE UI CONTROL GROUPS window:

***6a***&nbsp;&nbsp;Select the + button to create a new group.

***6b***&nbsp;&nbsp;Name your control group as you wish by first selecting “New Group” then editing the name.

***6c***&nbsp;&nbsp;Drag both Value controls from the right-hand pane into the group box.

Select the SAVE button in the upper-right.

**7**&nbsp;&nbsp;Select PUBLISH in the left-hand Navigation pane. On the PUBLISH window:

***7a***&nbsp;&nbsp;Select the checkbox adjacent to your Potenco device name.

***7b***&nbsp;&nbsp;Select the PUBLISH button. Your Profile will be published to your Potenco; monitor progress by watching the DEVICE ACTIVITY pane.

***7c***&nbsp;&nbsp;Once published, you can close the Afero Profile Editor.

### Step 12. Open the App on the Device

On the Potenco device, invoke the app by typing **app** at the command prompt. It will look like the program is hanging, but it is waiting for the Wi-Fi signal strength to change. When it changes, an event is generated that the code will see and respond to by setting attribute 2 value to 8. To make this happen, you can simply cup your hands over the Wi-Fi antenna. What you will see is the app suddenly showing a value of 8 for the attribute ID 2 value.

## Using the Ready-Made Example App

The example above was fairly straightforward and used a type of system event, RSSI change, to signal a developer-defined attribute change. A more robust example is available on GitHub, and it will give you a starting point for a more useful application that includes how to manage strings, respond to attribute set requests, as well as having a more extensive explanation of what’s going on within the code itself. In order to get and use this package, follow the instructions below.

Note that these instructions are very similar to those above and use identical concepts in creating and maintaining your own BitBake layer.

### Prerequisites

- You have completed the instructions for building the BeagleBone Green Wireless based Potenco project, contained in the section [Build the Potenco OS Image](../LinuxSDK-PotencoBBGW#bldpotosimage).
- You have named the directory in which the Potenco project exists as “potenco” and placed it in your home directory. If you choose to use a different path or directory name, you must adjust the instructions below accordingly.
- You have at least 64GB of disk space and 4GB of memory in your Ubuntu system to successfully build the Potenco image.
- You have a GitHub account and have been given access to the am335x-demo-app GitHub repository. Contact the Afero Customer Enablement (ACE) team at “ace@afero.io” for access and the GitHub repo URL.

### Steps

**1**&nbsp;&nbsp;Let’s start with getting the GitHub package. You should put it into the root of your home directory. Use the following command:

```
$ cd
```

**2**&nbsp;&nbsp;Download the package from the GitHub repository using the URL given to you by the ACE team:

```
$ git clone <GitHub repo URL> am335x-demo-app.git
```

The package will clone to a directory named “`am335x-demo-app`”.

   
**3**&nbsp;&nbsp;Within this `am335x-demo-app` directory, you will find the directories and directory trees listed below. To install them, simply copy them into your own instance of the Potenco reference build.

- `af-app` - Holds your makefile and your application code.

- `meta-app` - Top-level BitBake recipe directory that defines when and how your application is built.

    - `conf` - Resides under `meta-afero` and holds the layer configuration file for the meta-app BitBake layer that describes how to build your application.
    
     - `recipes-app` - Resides under the `meta-afero` directory and holds the actual BitBake recipe for your application.

   - `APEProject` - Holds a directory containing the Afero Profile Editor project that defines the attributes used in this demo, as well as the UI definition for the Afero mobile app.

**4**&nbsp;&nbsp;Enter the directory that was created when you cloned the GitHub repo. The directory should be named `am335x-demo-app`:

```
$ cd am335x-demo-app
```

**5**&nbsp;&nbsp;List the contents:

```
$ ls
af-app meta-app APEProject
```

**6**&nbsp;&nbsp;Now copy the application source code that’s in the `af-app` directory to the top level of the `potenco` directory tree:

```
$ cp -r af-app ~/potenco
```

**7**&nbsp;&nbsp;List the directory contents to verify:

```
$ ls ~/potenco
af-app af-conn af-ipc af-util attrd build configs downloads oe-layertool-setup.sh otamgr sample-files sources
```

**8**&nbsp;&nbsp;Copy the `meta-app` directory to the `sources` directory in the `potenco` tree so that it can be found by BitBake and the recipes extracted from it, then verify the contents:

```
$ cp -r meta-app ~/potenco/sources/
$ ls ~/potenco/sources/
bitbake meta-afero meta-app meta-arago meta-linaro meta-openembedded meta-processor-sdk meta-qt5 meta-ti oe-core
```

**9**&nbsp;&nbsp;Enter the `~/potenco/build/conf` directory, then look at the contents of the `bblayers.conf` file:

```
$ cd potenco
$ ls
af-app af-conn af-ipc af-util attrd build configs downloads oe-layertool-setup.sh otamgr sample-files sources
$ cd build
$ ls
arago-tmp-external-linaro-toolchain bitbake.lock buildhistory cache conf sstate-cache
$ cd conf
$ ls
bblayers.conf local.conf sanity_info setenv
$ more bblayers.conf

# This template file was created by taking the oe-core/meta/conf/bblayers.conf
# file and removing the BBLAYERS section at the end.
# LAYER_CONF_VERSION is increased each time build/conf/bblayers.conf
# changes incompatibly
LCONF_VERSION = "7"
BBPATH = "${TOPDIR}"
BBFILES ?= "" # Layers configured by oe-core-setup script
BBLAYERS += " \
	<your_full_path>/potenco/sources/meta-processor-sdk \
	<your_full_path>/potenco/sources/meta-arago/meta-arago-distro \
	<your_full_path>/potenco/sources/meta-arago/meta-arago-extras \
	<your_full_path>/potenco/sources/meta-qt5 \
	<your_full_path>/potenco/sources/meta-openembedded/meta-networking \
	<your_full_path>/potenco/sources/meta-openembedded/meta-ruby \
	<your_full_path>/potenco/sources/meta-openembedded/meta-python \
	<your_full_path>/potenco/sources/meta-openembedded/meta-oe \
	<your_full_path>/potenco/sources/meta-ti \
	<your_full_path>/potenco/sources/meta-linaro/meta-linaro-toolchain \
	<your_full_path>/potenco/sources/oe-core/meta \
	<your_full_path>/potenco/sources/meta-afero \
"
```

**10**&nbsp;&nbsp;Open this `bblayers.conf` file for edit. First replace the instances of “`<your_full_path>`” with your local system’s fully-qualified path to the `potenco` tree (should be your home directory). Then add the path to the `meta-app` directory after the
`meta-afero` line. As an example, here are the last three lines with the `meta-app` path added:

```
⋮
<your_full_path>/potenco/sources/meta-afero \
<your_full_path>/potenco/sources/meta-app \
"
```

**11**&nbsp;&nbsp;Enter the `~/potenco/build` directory and use the following command to build your own application. Example output is shown below the command:

```
$ ~/potenco/build$ bitbake app

NOTE: Started PRServer with DBfile: <your_full_path>/potenco/build/cache/prserv.sqlite3, IP: 127.0.0.1, PORT: 45763, PID: 28648
Loading cache: 100% |###########################################################################################################| ETA: 00:00:00
Loaded 2844 entries from dependency cache.
Parsing recipes: 100% |#########################################################################################################| Time: 00:00:00
Parsing of 2257 .bb files complete (2251 cached, 6 parsed). 2849 targets, 477 skipped, 0 masked, 0 errors.
WARNING: No recipes available for:
<your_full_path>/potenco/sources/meta-arago/meta-arago-distro/recipes-multimedia/libav/libav_9.16.bbappend
<your_full_path>/potenco/sources/meta-processor-sdk/recipes-core/udev/udev_182.bbappend
NOTE: Resolving any missing task queue dependencies
Build Configuration:
BB_VERSION = "1.30.0"
BUILD_SYS = "x86_64-linux"
NATIVELSBSTRING = "Ubuntu-16.04"
TARGET_SYS = "arm-linux-gnueabi"
MACHINE = "am335x-evm"
DISTRO = "arago"
DISTRO_VERSION = "2016.05"
TUNE_FEATURES = "arm armv7a vfp thumb neon callconvention-hard cortexa8"
TARGET_FPU = "hard"
meta-processor-sdk = "HEAD:1686d851985872f34ca3a2c30d4a0726a20a5c99"
meta-arago-distro
meta-arago-extras = "HEAD:36598b78a6d9fbe442f93111f64d3413c04418a8"
meta-qt5 = "HEAD:d715f2c1d340fa38f8a9860acc73de5e14a38b75"
meta-networking
meta-ruby
meta-python
meta-oe = "HEAD:66174d8f1273e54f7d12e21ca4b4309706dceeee"
meta-ti = "HEAD:1ae0cadb1200d0007a779958e852a48379d42c26"
meta-linaro-toolchain = "HEAD:2f51d38048599d9878f149d6d15539fb97603f8f"
meta = "HEAD:b0b91490e4ede61a302eb547da2cc65aa7da87ff"
meta-afero = "master:8ae143eebf4f2c12fa00c1c01d7b9deb54c4c4ef"
meta-app = "master:e65daaecbf66e2220b4575cf0f376523af685db7"
NOTE: Preparing RunQueue NOTE: Executing SetScene Tasks
NOTE: Executing RunQueue Tasks app-1.0-r0 do_compile:
NOTE: app: compiling from external source tree <your_full_path>/potenco/build/../af-app
WARNING: app-1.0-r0 do_package_qa: QA Issue: No GNU_HASH in the elf binary: '<your_full_path>/potenco/build/arago-tmp-external-linaro-toolchain/work/cortexa8hf-neon-linux-gnueabi/app/1.0-r0/packages-split/app/usr/bin/app' [ldflags]
NOTE: Tasks Summary: Attempted 421 tasks of which 413 didn’t need to be rerun and all succeeded.
NOTE: Writing buildhistory
Summary: There were 2 WARNING messages shown.
```

**12**&nbsp;&nbsp;At this point you have created an image named “app”. You can `scp` this image to the BBGW board.

**13**&nbsp;&nbsp;If you wish to have the file added to the final root filesystem image, you must tell the controlling BitBake recipe how to do so by editing the `/sources/meta-afero/recipes-core/images/arago-afero-image.bb` file.

***13a***&nbsp;&nbsp;Look at the `arago-afero-image.bb` file; you will see a list of build target names that are part of the IMAGE_INSTALL= of the BitBake file:

- backports \
- af-ipc \
- af-util \
- attrd \
- af-conn \
- af-sec \
- af-edge \
- otamgr \
- hubby \
- af-extras \
- dropbear \

***13b***&nbsp;&nbsp;Add “app \” to this list, as shown:

- backports \
- af-ipc \
- af-util \
- attrd \
- af-conn \
- af-sec \
- af-edge \
- otamgr \
- hubby \
- af-extras \
- app \
- dropbear \

This adds the application as a target to the root filesystem. Therefore, when you execute `bitbake arago-afero-image`, you will have the target of the build “app” placed into the root filesystem according to where the `do_install_append` of the application tells it, which is the directory of the target root filesystem.

**14**&nbsp;&nbsp;Save the file, then execute the following command:

```
bitbake arago-afero-image
```

As explained above, the binary “`app`” is placed into the root filesystem as `/usr/bin/app`.

Now you have a full distribution image as per the [Potenco Setup Guide for BBGW](../LinuxSDK-PotencoBBGW). You can use those instructions to burn the image into an SD card and then install the image onto your BeagleBone Green Wireless board.

 *Next:*&nbsp;&nbsp;[**&#8674;** Afero Potenco Setup Guide for SAMA5D2 Board](../LinuxSDK-PotencoSAMA5D2)