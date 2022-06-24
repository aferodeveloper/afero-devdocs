# Afero Linux Device Full System Image OTA Update

The Afero Linux SDK supports Over-the-Air (OTA) image replacement of the entire (full) system on an Afero Linux-based device. This page describes the steps required for a third-party developer to manage the Linux device full OTA image update process.

This page contains the following sections:

- [Introduction](../LinuxSDK-FullImageUpdate#introduction)
    - [OTA Image Concepts](../LinuxSDK-FullImageUpdate#ota-image-concepts)
    - [The Journey of an OTA Image](../LinuxSDK-FullImageUpdate#the-journey-of-an-ota-image)
    - [Use Case](../LinuxSDK-FullImageUpdate#use-case)
    - [OTA Design and Best Practices](../LinuxSDK-FullImageUpdate#ota-design-and-best-practices)
- [The OTA Image Update Steps](../LinuxSDK-FullImageUpdate#the-ota-image-update-steps)
    - [1. Create the OTA Image](../LinuxSDK-FullImageUpdate#1-create-the-ota-image)
    - [2. Deploy the OTA Image to a Device](../LinuxSDK-FullImageUpdate#2-deploy-the-ota-image-to-a-device)
    - [3. Install Image on Device Using otamgr Daemon](../LinuxSDK-FullImageUpdate#3-install-image-on-device-using-otamgr-daemon)
- [OTA Python Tools Reference](../LinuxSDK-FullImageUpdate#ota-python-tools-reference)

## Introduction

The development of the Linux device software is a collaborative process between a third-party developer and Afero. Through the Afero Secure Linux Device SDK, Afero provides a Linux-based IoT platform and is responsible for the network connectivity management components. However, the development of the Linux device and its software is done by the third-party developer. This includes some of the work required for the OTA image update. As part of the platform, Afero provides a cloud-based OTA service and update tools that enable the third-party developer to upload an OTA image file to the Afero Cloud OTA service and download the image securely to the device.

### OTA Image Concepts

From the Afero Cloud point of view, an OTA image is an opaque binary image; that is, the meaning of the image contents is unknown and ignored. It is up to you to define what that data means and how you package it.

Behind the scenes, Afero adds a header to your OTA image. The header contains a signature that allows hubby to determine if someone has tampered with the image. If it has been tampered with, hubby will ignore the image. The header also contains the OTA Version, which is assigned by the Afero Cloud so it can determine if the firmware on the device is up-to-date. If the firmware is found not to be up-to-date, the OTA service pushes the OTA image to the device.

### The Journey of an OTA Image

The diagram below shows a high-level view of the journey of an OTA image from your system to the OTA service to the device:

<img src="../img/OTA-Journey.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

Following the illustrated flow:

1. The full image is uploaded to the OTA service using the OTA Python tools.
2. The image with header is stored on the OTA service. Because the new OTA version is higher than the one reported by the device, the OTA service notifies hubby of the new OTA image.
3. hubby downloads the image with header and verifies that the image has not been tampered with.
4. hubby stores the image without header on the filesystem. This image is guaranteed to be the same as the image uploaded.
5. hubby informs your software of the path to the OTA image so you can apply the update.

The image creation and deployment steps are detailed on this page; the steps taken by hubby are described in the Secure Linux SDK documentation, particularly the otamgr daemon section.

### Use Case

The solution described on this page assumes that the third-party developer has downloaded the Afero Secure Linux Device SDK, customized it for their development, then built a final software image. As part of the development work, they have also developed a system update strategy. Typically, this includes building an OTA update image and deciding how to manage the actual system update. Particularly on the embedded Linux system, the developer must decide whether to provide different partitions for system update or do a filesystem update. This decision dictates subsequent steps on how the device is updated. In the Yocto build environment, an entire filesystem is generated as the output of build system, which allows us to know the storage size of the image for a particular product. In a later section, we will discuss the update options and our suggestions; however, the final decision of how the OTA image is updated is left to the third-party developer.

### OTA Design and Best Practices

An OTA update is a mechanism for remotely updating internet-connected hardware with new settings, software, and/or firmware. The OTA update mechanism is a core part of a system’s architecture, with the remote hardware device being responsible for identifying and applying updates to itself, and the Cloud service responsible for distributing updates to its connected hardware clients. It is important that system update by the OTA mechanism works properly to avoid "bricking" the device. It is important that system update by the OTA mechanism works properly. In this section, we will outline an approach to OTA update capabilities.

The embedded Linux system consists of the following components: bootloader(s), kernel, root filesystem, application, and user data. The OTA update design falls into one of these categories:

- Single image that contains everything that must be updated: bootloader, kernel, rootf filesystem, and application; or
- Individual component update (e.g., kernel, application, rootfs) — that is, create separate partitions for different types of software.

**In both cases, the user data must be preserved and the image must be authenticated.**

The option selected depends on memory space availability, as well as other factors. Sometimes, a single OTA image may be the practical choice. However, if there is enough memory space, it may be desirable to build a fallback mechanism in case of an update failure; that is, create two non-volatile storage partitions for storing the software images - one active, and one for update. In this case, the second partition is updated with the OTA image, and then verified before the system is switched to use it. In case of failure, the system continues to use image in the previous working partition.

<img src="../img/OTAImagePartitions.png" width="250" style="vertical-align:middle;margin:0px 0px;border:none">

The advantage of using two partitions is that it allows the system update to be done as a whole and validated before use. The disadvantage is that it requires doubling the non-volatile storage size.

We strongly recommend the two partitions approach to the full OTA image update. Ultimately, the OTA design is a business decision, and as the third-party developer, you have full design control of the OTA architecture and how the system update is performed. However, there are few things you need to know about Afero Cloud OTA service in order to integrate the process effortlessly.

## The OTA Image Update Steps

Follow the instructions outlined below to perform a full OTA image update. (The steps include the use of the Python script tools Afero provides. These scripts are described in detail in the section, [OTA Python Tools Reference](../LinuxSDK-FullImageUpdate#ota-python-tools-reference)).

**Step 1**&nbsp;&nbsp;[Create the OTA image](../LinuxSDK-FullImageUpdate#step-1-create-the-ota-image), which includes:

- Customizing the configuration file
- Creating an OTA record
- Incorporating the OTA record into your build
- Uploading the image file to the OTA service

**Step 2**&nbsp;&nbsp;[Deploy the OTA image](../LinuxSDK-FullImageUpdate#step-2-deploy-the-ota-image-to-a-device) by pushing the OTA image file to the Linux device.

**Step 3**&nbsp;&nbsp;[Install the OTA image](../LinuxSDK-FullImageUpdate#step-3-install-image-on-device-using-otamgr-daemon) on the device using the otamgr daemon.

### Step 1. Create the OTA Image

This section focuses on how to create your OTA image, including integrating the OTA image creation with your build process.

#### Prerequisites

Set up the build environment using the following instructions, as appropriate:

- For the Atmel SAMA5D2, follow the instructions in [Build the SAMA5D2 Linux OS Image](../LinuxSDK-PotencoSAMA5D2#build-the-sama5d2-linux-os-image) to set up the SAMA5D2 Yocto build.
- For the BeagleBone Green Wireless, follow the instructions in [Build the BBGW Linux OS Image](../LinuxSDK-PotencoBBGW#build-the-potenco-os-image) to set up the BBGW Yocto build.


#### Set Up the Environment

**Note!** The instructions below assume you are developing on an Atmel SAMA5D2 board. If you are on a BeagleBone Green Wireless board, you can use the instructions below, but substitute the appropriate directory paths. If you have questions, contact the Afero Customer Enablement Team (“ace@afero.io”).

**1**&nbsp;&nbsp;Type the following:

```
$ cd ~/sama5/poky
$ source oe-init-build-env build-microchip
```

**2**&nbsp;&nbsp;Locate Yocto’s TMPDIR directory:

```
$ bitbake -e | grep ^TMPDIR
TMPDIR="/home/john/sama5/poky/build-microchip/tmp"
```

**3**&nbsp;&nbsp;Go to the Yocto build’s `tmp` directory; if it doesn’t already exist, create it:

```
$ mkdir -p tmp
$ cd tmp
```

Note that we are using the `tmp` directory as a workspace for the OTA update tools.

**4**&nbsp;&nbsp;Clone the Afero partner OTA tools repository. If you don’t delete the Yocto build’s `tmp` directory, then you only have to do this once:

```
$ git clone git@github.com:AferoCE/partner-hub-ota-tools.git
```

You should see the following files in the directory:

```
$ cd partner-hub-ota-tools
$ ls -la
drwxr-xr-x 8 john dialout  4096 Oct 28 13:58 .git
-rw-r--r-- 1 john dialout   622 Oct 16 16:03 partner-ota-conf.json
-rwxr-xr-x 1 john dialout  7731 Oct 28 13:29 partner-ota-hub-deploy.py
-rwxr-xr-x 1 john dialout 14244 Oct 28 13:51 partner-ota-hub-uploader.py
```

#### Customize the Configuration File

Read details in [About the Configuration File](../LinuxSDK-FullImageUpdate#about-the-configuration-file). Use your favorite editor to edit the configuration file `partner-ota-conf.json`. Fill in the following fields and save it:

```
$ vim partner-ota-conf.json
{
    "Comments": "Configuration file for partner Full OTA image upload tool",
 
    "description" : "<OTA Image description>", 
    "name"        : "<Your name for this product>",
    "version"     : "<x.x>",
    "url"         : "<dummy>",                        
 
    "username"    : "<your_username@your_company_url>",
    "userpw"      : "<account_password>",
    "auth-string" : "<auth string for your account>",
 
    "deviceTypeId": "<your_devicetypeId>",
    "partnerId"   : "<your_partnerID>",
 
    "imageFiles"  : {
       "A": "hub_update.bin"
    }
}
```

Contact the Afero Enablement team (“ace@afero.io”) if you have any problem with this configuration file.

#### Create the OTA Record

Read details in the reference sections below, [About the OTA Record](../LinuxSDK-FullImageUpdate#about-the-ota-record) and [Using the partner-ota-hub-uploader.py Script](../LinuxSDK-FullImageUpdate#using-the-partner-ota-hub-uploaderpy-script).

Run the `partner-ota-hub-uploader.py` tool with the option `--createOTARecord` to create an OTA record file using the above modified configuration file. Note: You must specify a build number with the -n option:

```
$ python partner-ota-hub-uploader.py -n <Your BuildNumber> --createOTARecord
```

Assuming success, the OTA record file is generated and saved; for example, as the following filename and in the following directory:

```
~home/sama5/poky/tmp/full_ota_record.json
```

#### Run and Package the Build

**1**&nbsp;&nbsp;Run the build. The example below shows BitBake compiling and building the image for
`BUILD_TYPE=prod` (as you normally would):

```
$ cd ../..
$ BUILD_TYPE=prod BUILD_TARGET=debug BUILD_PROFILE=potenco bitbake core-image-minimal
```

**2**&nbsp;&nbsp;In this step, package your build image as you like. Below is an example of copying the tar rootfs image to the `partner-hub-ota-tools` directory:

```
$ cd tmp/partner-hub-ota-tools
$ cp ../deploy/images/sama5d2-xplained-emmc/core-image-minimal-sama5d2-xplained-emmc.tar.gz   hub_update.bin
```

**3**&nbsp;&nbsp;Upload the image file to the Afero OTA service (i.e., `hub_update.bin`), using the same default configuration file and build number as used above:

```
$ python partner-ota-hub-uploader.py -n <your build number> --uploadOTAImage
You are done!
```
### Step 2. Deploy the OTA Image to a Device

Deploying an OTA image can be independent of the creating the image. In this section, it is assumed that you have already uploaded your OTA image to the Afero OTA service and now you want to deploy this uploaded image to an device. Currently, we only support deploying image one at a time using this tool.

**1**&nbsp;&nbsp;Make sure you have already cloned the Afero partner tools and edited the configuration file, as described above in [Set Up the Environment](../LinuxSDK-FullImageUpdate#set-up-the-environment) and [Customize the Configuration File](../LinuxSDK-FullImageUpdate#customize-the-configuration-file).

**2**&nbsp;&nbsp;Display your OTA images for the deviceTypeId and partnerId by typing the following:

```
$ python partner-ota-hub-deploy.py --list
```

Example output for the above command is shown below. Note that `imageId` is what you need for the OTA deployment:

```
$ python partner-ota-hub-deploy.py --list
 
----  List of HUB FULL OTA images ----

partnerId   : 4f7de484-cf23-478d-90a7-412104d5120b
deviceTypeId: a6542896-8464-48e1-b12f-664a57e4e703

Total Number of Images: 2
Image Id    Version          Name                  Description                  
----------  ---------------  --------------------- ------------------------------
49856       1.0.1            Potenco-test          OTA Image description 
49867       1.0.2d           Potenco-test          OTA Image description
```

**3**&nbsp;&nbsp;Look up your `deviceId` from your mobile app. Your `deviceId` should look something like this: 012359551bf18eba.

**4**&nbsp;&nbsp; Use the `partner-ota-hub-deploy.py` with the option `-i <imageId>` and `-d <deviceId>` to deploy your OTA images to the device with that ID:

```
$ python partner-ota-hub-deploy.py -i <imageId> and -d <deviceId>
```

For example:

```
$ python partner-ota-hub-deploy.py -i 49867 -d 012359551bf18eba
```

#### Example of Linux Device OTA: Tracing the Logs

These logs are based on the Atmel SAMA5D2 board. Other development boards should have very similar log entries.

```
hubby checked the OTA version, and decided that it is okay to start downloading the image:

root@sama5d2-xplained-emmc:~# Oct 30 21:11:55 sama5d2-xplained-emmc hubby[2014]: BentoProduct::LocalOtaAllowed(5, 7112): current version 0
Oct 30 21:11:55 sama5d2-xplained-emmc hubby[2014]: DeviceUpdater::DownloadFile(012359551bf18eba, https://otacdn.dev.afero.io/ota-image429612701945115078912d8ee80-b131-4da6-9814-89f9bf22d49c): adding easy 0x2c4410 to multi 0x1765a8
hubby ota function 0::012359551bf18eba::0::59282871

hubby completes the download and stores the image in /tmp/ directory:

Oct 30 21:12:55 sama5d2-xplained-emmc hubby[2014]: DeviceUpdater::CheckMultiInfos(): done: https://otacdn.dev.afero.io/ota-image429612701945115078912d8ee80-b131-4da6-9814-89f9bf22d49c => /tmp/ota.update.012359551bf18eba (59282871) 200:0:

Oct 30 21:12:55 sama5d2-xplained-emmc hubby[2014]: BentoProduct::LocalOtaComplete(5, /tmp/ota.update.012359551bf18eba)
Oct 30 21:12:55 sama5d2-xplained-emmc hubby[2014]: BentoProduct::LocalOtaComplete(/tmp/ota.update.012359551bf18eba): generating sha...
Oct 30 21:13:00 sama5d2-xplained-emmc hubby[2014]: BentoProduct::LocalOtaComplete(/tmp/ota.update.012359551bf18eba): sha generated d5dedf43b14c1c5ef390bc1b15f6e5ac3653c2c9d4997c7cc4fba05c86b6ec2d

hubby performs verification of the image using the header:

Oct 30 21:13:00 sama5d2-xplained-emmc hubby[2014]: 3 verify image

Validation is complete: status=0 (success), status=1 (failure):

Oct 30 21:13:00 sama5d2-xplained-emmc hubby[2014]: 3 done with OTA update verification
Oct 30 21:13:00 sama5d2-xplained-emmc hubby[2014]: BentoProduct::HandleValidateOTA(0)

hubby sends out notification (attribute 51612 or AF_ATTR_HUBBY_OTA_UPGRADE_PATH), using attribute daemon attrd:

client_set_own_attribute:attrId=51612,name=HUBBY_OTA_UPGRADE_PATH,owner=IPC.HUBBY,value[16]=2f746d702f6f74615f747970655f3500

otamgr received the notification, and starts to act on it:

Oct 30 21:13:01 sama5d2-xplained-emmc otamgr[475]: I ota_notification:path=/tmp/ota_type_5
Oct 30 21:13:01 sama5d2-xplained-emmc attrd[363]: 3 handle_receive_message:len=17,seqNum=0000000d:received client message
Oct 30 21:13:01 sama5d2-xplained-emmc otamgr[475]: 1 otamgr_image_src:path=/tmp/ota_type_5.img

And the script sysupgrade is invoked (if it is available), otherwise, NO actual OTA update occurs:

Oct 30 21:13:09 sama5d2-xplained-emmc otamgr[475]: 2 af_util_system::{ /sbin/sysupgrade /tmp/ota_type_5.img; } ; disown -a

If sysupgrade is not available, you will see:

sh: /sbin/sysupgrade: No such file or directory
```
### Step 3. Install Image on Device Using otamgr Daemon

Once the OTA image has been deployed to the device and hubby has informed the otamgr daemon (via an Afero attribute) the location of the validated firmware in the filesystem, the otamgr daemon can install the image on the device. Please refer to [OTA Manager (otamgr)](../LinuxSDK-Overview#ota-manager-otamgr) and [OTA Manager Daemon Implementation](../LinuxSDK-OTAManager) for details.

## OTA Python Tools Reference

The sections below describe the Afero OTA Python tools in detail.

### OTA Uploader and Deploy Tool Limitations

The uploader and deploy scripts have the following limitations:

- The deployment currently supports only one device at a time. Contact the Afero Customer Enablement team (“ace@afero.io”) if you want to configure an OTA update to a larger user base.
- It takes about approximately 20s for the OTA download to start after executing the `partner-ota-hub-deploy.py` script.
- In the event the upload is interrupted by a connectivity outage, or should the device should go offline, you must restart the OTA process manually.
- The device being updated must be in your account (i.e., the device must be associated with the account given in the OTA configuration file).
- Importantly, these tools are designed for development and testing only. They are not intended for managing production devices.

### Uploader Tool: partner-ota-hub-uploader.py

Afero provides a Python script to help with the following two tasks:

- Create an OTA record file, and then
- Upload the generated OTA image file to the Afero Cloud OTA service.

The script is called “`partner-ota-hub-uploader.py`” and is used to generate an OTA record for the full OTA image that you plan to upload, then is used to perform the upload as well. The script is used as a command line tool with input parameters and a JSON configuration file. This tool should be part of the build for the product release. Read more above in [Create the OTA Image](../LinuxSDK-FullImageUpdate#1-create-the-ota-image).

The script interacts with the backend of the Afero Cloud OTA service using the RESTful service API endpoints. The OTA service keeps records on the OTA images and each OTA record is used to evaluate whether a device is eligible for update by examining the version information, among other information. For convenience, some of the information required for the OTA record and access privileges are put in the JSON configuration file.

#### About the OTA Record

The Afero Cloud OTA service requires certain information about the OTA image to properly store it and for the OTA service to perform certain checks on it. Specifically, the following is required:

| REQUIRED FIELD   | DESCRIPTION                                                  |
| :--------------- | :----------------------------------------------------------- |
| createdTimestamp | Time creation in ms.                                         |
| description      | Easily-readable description of the OTA. Maximum length support is 500 characters. |
| id               | Unique record ID, used to identify the OTA record.           |
| name             | User-provided short name of the image, up to the maximum length of 35 characters. |
| partnerId        | Partner ID, created as part of the user account. Go to the Afero Profile Editor Profile Editor, Account Info for this ID. |
| tags             | Tag name for the release, up to maximum length of 35 characters. |
| type             | Firmware image type, currently the tool only support HUB (type "5"). |
| updatedTimestamp | Last updated time in ms. Same as createdTimestamp when first created. |
| url              | Storage URL after the OTA image is uploaded. It is an empty string initially. |
| version          | Friendly name you give this version of the OTA image being released. |
| versionNumber    | This is the version number that device reports.              |

Collectively, this information is referred to as the OTA record. The record contains information from difference sources, some provided by the third-party developer and other from the different subsystems in the Afero ecosystem. Afero provides a script tool that facilitates the creation of the OTA record as well as uploads the image to the OTA service. Read more below.

#### About the Configuration File

As part of creating the OTA image, you must customize the configuration file. This file provides the required credentials to access the Afero Cloud OTA service, as well as partner identification information for uploading the OTA images. The file must be in JSON format. A default configuration file is provided in the same directory as the Python script and is named `partner-ota-conf.json`.

An example configuration file is shown, with each field described below:

```
{  
    "Comments"    : "Configuration file for partner full OTA image upload tool",
 
    "description" : "OTA image description",
    "version"     : "1.0",
    "url"         : "dummy",
 
    "username"    : "john@company.io",
    "userpw"      : "12334",
    "auth-string" : "NGY3ZGU0ODQtY2YyMy00NzhkLTktYTctEREyMTA0ZDUxMjBiOjcxYTllODU5NDYxYjQ2ZjMhNTFhOTE3OWM0Z1234OTRk",
 
    "name"        : "Potenco-test",
    "deviceTypeId": "a6542896-8464-48f1-x12g-664a57e4e702",
    "partnerId"   : "4f7de484-b438-578d-90a7-422104d5120i",
 
 
    "imageFiles"   : {
        "A": "hub_update.bin"
    }
}
```

| FIELD        | DESCRIPTION                                                  |
| :----------- | :----------------------------------------------------------- |
| description  | User-friendly description of this OTA file, its purpose, etc. This is used to help the administrator and is not necessary for the OTA service. |
| name         | Short, user-friendly name that identifies the OTA image.     |
| version      | Friendly name you give this version of the OTA image being released. If the `-d` option is used in the command, then the letter “d” is appended to the version string, designating a debug build. |
| url          | Placeholder that eventually must be replaced by the URL to the Cloud storage where the OTA image is uploaded. |
| deviceTypeId | The device type ID generated by the Profile Editor for the Profile that identifies your device as a particular type. |
| imageFiles   | Provides the partition and the final full OTA image filename. Use `A` for the partition. **It is important that the final OTA image filename is the same filename string specified in this field.** |

The following fields are security credentials that are used to access the user’s account. The `username`, `auth-string`, and `partnerId` can be obtained from the Afero Profile Editor. Please refer to [Cloud APIs](../CloudAPIs) for more details.

| FIELD               | DESCRIPTION                                                  |
| :------------------ | :----------------------------------------------------------- |
| username and passwd | Username and password of the account that is used to manage the OTA. |
| auth-string         | Authorization header value (in string format).               |
| partnerId           | ID that assigned to you when your account is created. This is the "OAuth Client Id" from the Profile Editor, Account Info screen. |

#### Uploader Script Usage and Syntax

```
partner-ota-hub-uploader.py[-h] [-c <config_file>] [-d] [-s] -n <build number>
```

Option details are provided below:

| OPTION | DESCRIPTION                                                  |
| :----- | :----------------------------------------------------------- |
| -h     | Displays help text.                                          |
| -d     | Identifies this as a debug build.<br><br>`--d`: Use to append the letter “d” to the end of version string in the OTA record. |
| -s     | Skip search for BitBake’s TMPDIR. This option can be used for testing, or by advance users. The script integrates the OTA record into the BitBake build; the output OTA record is stored in the `$TMPDIR` (of the Yocto build). This option allows the user to skip using the `$TMPDIR`, and store the OTA record file in the current running directory. |
| -n     | The build number from the build server for this build. For example, Jenkins build server starts each build with build number one (1), and increment it by one for each subsequent build.<br><br>`--buildNum`: Use to append this build number to the end of version field to form the version string in the OTA record. The version string is used to keep track of the OTA build. |
| -c     | Specifies the use of the default JSON configuration filename and location: `partner-ota-conf.json`, residing in the same directory as the Python script.<br><br>`--conf`: Use to specify a different configuration filename and/or directory path from the defaults identified above.<br><br>`--createOTARecord`: Use to generate an OTA record. Upon success, an output file called “`full-ota-record.json`” is generated.<br><br>`--uploadOTAImage`: Use to upload the OTA file, with the filename designated in the configuration file, in the `imageFiles` field. |

#### Using the partner-ota-hub-uploader.py Script

This Python script is intended to be used in two different stages: 1) creating the OTA record, and 2) uploading the OTA image.

<mark>**&#x26A0; Caution!**  The same options should be used for both the `createOTARecord` and `uploadOTAImage` calls.<br><br>The OTA record created with `createOTARecord` call is intended to be subsequently used in `uploadOTAImage`; that is, `the full-ota-record.json` is used by `uploadOTAImage`.</mark>

##### **Stage 1. Create the OTA Record**

You must create an OTA record before building the actual image. An example of how to use the Python script is given below, where the build number is 10 and a user-provided JSON configuration file:

```
$ python partner-ota-hub-uploader.py -c dev-john-ota-conf.json -n 2 --createOTARecord
```

The output of this command is the OTA record, stored in a JSON file: `full-ota-record.json` in Yocto’s BitBake ${TMPDIR} (i.e., /home/poky/build-microchip/tmp).

The Afero recipe automatically looks for the `full-ota-record.json` file in ${TMPDIR} during the build. In this case, the `full-ota-record.json` is automatically picked up by the BitBake recipe to install the file in the device `/etc/` directory, *or* you can manually copy the file to the build image’s rootfs `/etc/` directory using the `-s` option.

An example of the OTA record in the `full_ota_record.json` file is given below:

```
$ cat full_ota_record.json
{
    "createdTimestamp": 1572280052532,
    "description": "OTA description",
    "id": "11561",
    "name": "Potenco-test",
    "partnerId": "4f7de484-cf23-478d-90a7-412104d5620b",
    "tags": [],
    "type": 5,
    "updatedTimestamp": 1572280052532,
    "url": "",
    "version": "1.0.2d",
    "versionNumber": "7112"
}
```

##### **Stage 2. Upload the OTA Image**

After you have built the image, injected the `full-ota-record.json` file in the appropriate place in the filesystem, and packaged the OTA image, the OTA image must be uploaded to the Afero OTA service using the command:

```
$ python partner-ota-hub-uploader.py -c dev-john-ota-conf.json  -n 2 --uploadOTAImage
  
Upload the OTA Image .....
Update OTA Record with the storage URL .....
Associate the Image with the deviceTypeId and ParnerId ....
Done!
```

### The Deploy Tool: partner-ota-hub-deploy.py

Afero provides the Python script, `partner-ota-hub-deploy.py`, that allows a third-party developer to deploy an uploaded OTA image from the Afero Cloud OTA service. To deploy an OTA image to a device, you need to know the ID of the OTA image that you uploaded to the OTA service. The `--list` option displays the images for a specified `partnerId` and a `deviceTypeId` (which is defined in the configuration file).

Note that you need the same configuration file that you used to create the OTA record and upload the OTA image to now deploy the image. Specifically, for deployment, the following configuration fields are required:

- username
- userpw
- auth-string
- deviceTypeId
- partnerId

To list the uploaded OTA images for a particular partner and a particular device type, use the script’s `--list` option, as shown in the example below:

```
$ python partner-ota-hub-deploy.py -c dev-john-ota-conf.json --list
----  List of DEVICE FULL OTA images ----
 
partnerId   : 4f7de484-cf23-478d-90a7-412104d8120b
deviceTypeId: a6542896-8464-48e1-b12f-664a57e6e703
 
Total Number of Images: 2
Image Id    Version          Name                    Description                  
----------  ---------------  ----------------------  ------------------------------
49856       1.0.1            Potenco-test            OTA image description 
49867       1.0.2d           Potenco-test            OTA image description
```

#### Script Usage and Syntax

```
$ partner-ota-hub-deploy.py[-h] [-c <config_file>] -d <deviceId> -i <imageId>
```

Option details are provided below:

| OPTION | DESCRIPTION                                                  |
| :----- | :----------------------------------------------------------- |
| -h     | Displays help text.                                          |
| -l     | Displays a list of OTA images on the OTA service. This shows the `imageId` that might be needed for OTA deployment.<br><br>`--list`: When this option is specified, no OTA deployment is enabled. |
| -c     | Specifies the use of the default JSON configuration filename and location: `partner-ota-conf.json`, residing in the same directory as the Python script.<br><br>`--conf`: Use to specify a different configuration filename and/or directory path from the defaults specified above. |
| -d     | Specifies the `deviceId` for the device that the full OTA image is to be applied to.<br><br>`--device`: Used in conjunction with `--imageId` option for deploying an OTA image to a device. |
| -i     | Specifies the uploaded OTA `imageId` that the full OTA image is to be applied to.<br><br>`--imageId`: Used in conjunction with `--device` option for deploying an OTA image to a device. |

Below is an example of deploying an OTA image, using `deviceId` = 012359551jr17eba, OTA imageId = 49712:

```
$ python partner-ota-hub-deploy.py -d 012359551jr17eba -i 49712 --conf dev-john-ota-conf.json
Request accepted for processing
```



<strong>&#8674;</strong> <em>Next:</em>&nbsp;&nbsp;[Afero Potenco Setup Guides](../LinuxSDK-PotencoSetupGds)