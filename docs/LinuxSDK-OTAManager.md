# OTA Manager Daemon Implementation

The OTA Manager is designed to facilitate the OTA system update, but is not responsible for the system update itself. The Afero Cloud OTA service enables binary blobs to be sent to all devices in the field, such as new firmware or new configuration files, in a way that ensures the integrity of the delivery. The OTA service signs the firmware and sends it to a pool of devices. On each Linux device, the hubby application receives the firmware and validates the signature using the Afero Hardware Security Module before informing the OTA Manager (via an Afero attribute) the location of the validated firmware in the filesystem.

## OTA Manager Responsibilities

The OTA Manager is implemented as a daemon, called “otamgr”, and is responsible for the following tasks:

1. Triggering the software update process using the image at the image file path.
2. Putting the header (found at the header file path) onto the new filesystem in the `/etc` directory, **if and only if** the update has finished successfully.
3. Setting the OTA Update Path Prefix attribute, if you don’t want the image and header files to be stored in the `/tmp` directory.

The OTA Manager source code is provided as part of the Linux Device SDK. It can be used as is, or customized as you see fit. The otamgr daemon picks up the OTA execution thread from hubby when the OTA image has been downloaded completely. We discuss the requirements in more detail below.

### Triggering the Software Update Using the Image File

After hubby has verified the OTA image, it informs you of the update via a notification on the OTA Update Path attribute (51612). The value of this attribute is a null-terminated string, and it tells you the path of two files: the header file and the image file.

- The header file path is constructed by adding the extension “.hdr” to the end of the OTA update path.
- The image file path is constructed by adding the extension “.img” to the end of the OTA update path.

For example, if the OTA Update Path attribute value were `/tmp/ota_type_5`, the header file would be found at `/tmp/ota_type_5.hdr`, and the image file would be found at `/tmp/ota_type_5.img`.

This is the core responsibility of the OTA Manager. No specific implementation is provided by Afero because the update process is usually highly dependent on the system design of the device.

### Putting the Header onto the New Filesystem

Assuming the update finishes successfully, the OTA update header must be placed in the /etc directory. For example, if the header file is at `/tmp/ota_type_5.hdr`, when the update completes, the header in `/tmp/ota_type_5.hdr` must be copied to the `/etc` directory so that the contents of `/etc/ota_type_5.hdr` file matches the original header file at `/tmp/ota_type_5.hdr`.

The reason for the second requirement is that the Afero Cloud needs to know the correct version number of the software on the device. If the header file is missing from the `/etc` directory, the Afero OTA service will push the update again. Therefore, if the file is not copied correctly, the result may be an endless repetition of updates.

This requirement is tricky to implement if you can’t perform the update while running the OS from the root filesystem. In such a case you need to pivot the root to a different filesystem before flashing the “normal” root filesystem. Then you need to mount the “normal” root filesystem, copy the header file to `/etc`, and then reboot.

### Setting the OTA Update Path Prefix

By default, hubby stores the image and header files in the `/tmp` directory. This directory is almost always a RAM disk. The directory must be big enough to hold the entire contents of the OTA image and header plus some overhead. If the `/tmp` directory is not big enough to hold this data you can use a different directory.

For example, if you have an SD card mounted at `/media/sdcard` you can have hubby download the OTA data there. To do this you must override the default OTA update path prefix, which is specified using the OTA Update Path Prefix attribute (51618).

The OTA Update Path Prefix attribute is owned by the OTA Manager. If the OTA Manager does not respond to hubby’s `get` request for the attribute (for example, if there were no OTA Manager to respond to the request), hubby will use the default prefix of “`/tmp`”.

## The otamgr Daemon Implementation

Like other Afero Linux SDK daemons, otamgr is implemented using the event loop and callback mechanisms. The implementation is relatively simple; it listens for events that support OTA functionality via system commands. These events are in the form of system attribute SET and GET notifications:

- AF_ATTR_HUBBY_STATE (system attribute 61611)
- AF_ATTR_HUBBY_OTA_UPGRADE_PATH (system attribute 51612)
- AF_ATTR_HUBBY_COMMAND (system attribute 65012)

Below is the otamgr `on_notify` callback function that shows the implementation of these attributes:

```
static void on_notify(uint32_t attributeId, uint8_t *value, int length, void *context)
{
    switch (attributeId) {
        case AF_ATTR_HUBBY_STATE :
            on_hubby_state(value, length);
            break;
        case AF_ATTR_HUBBY_OTA_UPGRADE_PATH :
            on_ota_upgrade_path(value, length);
            break;
        case AF_ATTR_HUBBY_COMMAND :
            on_hubby_command(value, length);
            break;
        default :
            AFLOG_WARNING("notification_unknown:attributeId=%d", attributeId);
            break;
    }
}
```

As the code snippet above shows, otamgr passively listens to the system notifications. When a supported notification is received, otamgr acts on it. The AF_ATTR_HUBBY_OTA_UPGRADE_PATH notification from hubby triggers the actual system update process, provided the “sysupgrade” functionality has been implemented by the third-party developer.

 **&#8674;** *Next:* [Linux Device Full Image OTA Update](../LinuxSDK-FullImageUpdate)