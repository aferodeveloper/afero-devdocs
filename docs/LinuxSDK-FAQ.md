# Secure Linux SDK Frequently-Asked Questions

The questions below have been raised during practical use of the Secure Linux SDK and when developing your own app to run on Potenco. Answers to these FAQs are provided in the two sections below.

## FAQ on Using the Secure Linux SDK


**Q: I’m pruning the Yocto build to the bare minimum to save space on the target device. Can I replace the available tools with BusyBox variants?**

A: Yes, you can replace the tools with BusyBox variants; however, you should test your application thoroughly if you choose to do so. Afero does not support this directly, but it should not cause any problems. Again, the motto here is test thoroughly.

<hr>
**Q: Can I get rid of the Linux IW tools?**

A: Yes you can. Afero does not rely on them; however, you might want to wait until the device is ready for production as the IW tools are very useful for debugging.

 

<hr>
**Q: Are there any patches performed during the Yocto build that are critical to Afero specific operations? Can I move to later minor builds with no patches applied?**

A: There are hundreds of patches performed during the Yocto build. We typically do not look into them unless we wind up with a specific bug. For example, having to delete the `at91bootstrap_3.8/11.bb` file was a result of such versioning issues caused by an update to `at91bootstrap`.

 

<hr>
**Q: Are there any transient files, such as logs or temporary files, created by the Afero stack that need to be periodically cleaned up?**

A: We have logs that we compress and put in `/afero_nv`. There is a script called “`logpush`”, which compresses the log files from `/var/log/` into .gz files. The oldest files are periodically removed based on the configuration definition in the `/etc/logpush.conf` file. In the logpush.conf file there is a limit of 288 .gz log compressed files that we want to keep, which is about three days’ of logging information. These log files can be uploaded to our media server if there is a bug, and these configuration parameters can be changed.

 

<hr>
**Q: Are there any directories that must be read-write for proper Afero stack operation?**

A: There are several partitions that we use for various operations; for example:

- `/afero_nv` is a directory where we save compressed log files and holds device configuration information and user information, and should be preserved across OTAs.
- `/etc` is where new device Profiles are written during a Profile update.
- `/tmp` is typically used for all kinds of temporary files so tends to be written to.

In general, Afero has not created a rootfs system that is unbreakable. The basic idea of the reference platform is to ascertain correct operation of the Afero stack. It was not designed to be a bombproof Linux system with features such as read-only partitions or a journaling filesystem used to recover from unexpected power loss. So, there are many changes that can and likely should be made to make a more robust hub device.

 

<hr>
**Q: During the development of my Afero hub, I can see the Peaberry and provisioning returns…<br>
`{ "devId":"012302e06a1b067f", "assId":"0244c164a5eaa574cb4a9c53", "compId":"5a" }`
(or something very similar).<br>Is there anything more I need to do to verify proper operation of the HSM?**

A: The provisioning shows that we can access the HSM, read the device, and access the related info. That is typically sufficient to access the 508. However, a full test requires Bluetooth to be working and communicating with hubby, and eventually linking with the Afero Cloud. However, for simple verification of the atacc508 connection, what you have shown is sufficient.

 

<hr>
**Q: I can configure and start the wpa_supplicant to connect to my local network. Is there anything else I need to do to pretest the Wi-Fi connectivity before I get to the point of end-to-end provisioning and Cloud-app connection?**

A: That should be sufficient. Typically, we make sure we have the wpa_supplicant running, that the interface is up, and that it is able to scan the BLE devices.

 

<hr>
**Q: Does any part of the Afero stack use DBUS to communicate to bluetoothd or wpa_supplicant? If not, is it ok to drop from the build/startup?**

A: Afero daemons don’t use DBUS directly; however, BlueZ uses it for inter-process communication.


## FAQ on Creating a Potenco App

<hr>
**Q: Should I wait a couple of hours every time I build the Potenco image and/or the application image?**

A: No. The first build of the Potenco root filesystem can take several hours. However, once built, the only parts that will typically change are the Afero stack and your own application. Since your application is built on its own layer, the build will only take a few minutes. Even when you decide to rebuild the entire filesystem, just the prebuilt images are packaged together into the root filesystem image. After that first build, the turnaround time from build to test is minutes instead of hours.

 

<hr>
**Q: Do I have to use an SD card to move my data from the development system to the target system?**

A: No, you can use SCP to copy the image from the development system to the target processor. You can SCP the image to `/usr/bin` with the following command “`scp app root@<devices IP address>:/usr/bin/`”. Note that you should be on the same network as the device.

 

<hr>
**Q: Are there any better examples of applications I can start with?**

A: We have two options that demonstrate an application using Afero stack: 1) the cut & paste solution described in [Add an afLib C-Based App to the Potenco Build for BeagleBone Green Wireless](../LinuxSDK-PotencoBBGWApp) and/or [Add an afLib C-Based App to the Potenco Build for SAMA5D2](../LinuxSDK-PotencoSAMA5D2App), or 2) the demo application in the GitHub repository, which is a more robust application, with more examples of interacting with attributes of different types, also described in the two pages referenced with Option 1 above.