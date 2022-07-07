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

<ul class="af-ul">
	<li><p>We recommend running the Developer Hub on the latest Raspbian “Buster” OS, however any version of Buster, and any version of Stretch (version 2017-08-16 or newer) are supported. Raspian can be downloaded from <a id="024-BzJulqb" href="https://www.raspberrypi.org/downloads/raspbian/" target="_blank">https://www.raspberrypi.org/downloads/raspbian/</a>.</p>
<!--
<div class="af-callout">
<div class="callout-text">
	<p>If you are currently using a Developer Hub running Release 1.0,	your most efficient update path to R1.1 is: 1) Download the latest Raspbian OS to another SD card. 2) Install R1.1 Hub Software as a new device per the instructions below. 3) Remove the old Developer Hub device from your Afero account using the mobile app.</p>
</div>
<br class="af-clear">
</div>
-->
	</li>
	<li>The Developer Hub should not be used in a production environment, because the hub's encrytion keys are stored in the filesystem and not in a Hardware Security Module. If you need a hub device for a production product, several options are available. Please <a id="1580148643.00" href="https://www.afero.io/html/home/contact-afero.html" target="_blank">contact us.</a></li>
</ul>

## Hardware Requirements

We require ARM-based systems running Debian Linux or any Debian variant:

<ul class="af-ul">
	<li><strong>Raspberry Pi 4</strong> - No additional hardware requirements.</li>
	<li><strong>Raspberry Pi 3</strong> - No additional hardware requirements.</li>
	<li><strong>Raspberry Pi Zero W</strong> - No additional hardware requirements.</li>
	<li><strong>Raspberry Pi Zero (non-wireless)</strong> - This configuration will work as a hub but is not recommended because of these additional hardware requirements:</li>
		<ul class="af-ul-2">
			<li>USB Wi-Fi adapter (<a id="1512602670.7" href="https://www.amazon.com/Edimax-EW-7811Un-150Mbps-Raspberry-Supports/dp/B003MTTJOY" target="_blank">such as this product on Amazon.com</a>).</li>
			<li>USB Bluetooth 4.0 USB adapter (<a id="1512602670.8" href="https://www.amazon.com/Plugable-Bluetooth-Adapter-Raspberry-Compatible/dp/B009ZIILLI/" target="_blank">such as this product on Amazon.com</a>).</li>
			<li>Micro-USB “OTG” cable and USB hub to connect adapters to the Pi.</li>
		</ul>
	<li><strong>Raspberry Pi 1 or 2 Model B or B+</strong> - This configuration will work as a hub but is not recommended because of these additional hardware requirements:</li>
		<ul class="af-ul-2">
			<li>USB Wi-Fi adapter (<a id="1512602670.91" href="https://www.amazon.com/Edimax-EW-7811Un-150Mbps-Raspberry-Supports/dp/B003MTTJOY" target="_blank">such as this product on Amazon.com</a>).</li>
			<li>USB Bluetooth 4.0 USB adapter (<a id="1512602671.01" href="https://www.amazon.com/Plugable-Bluetooth-Adapter-Raspberry-Compatible/dp/B009ZIILLI/" target="_blank">such as this product on Amazon.com</a>).</li>
			<li>External power supply; USB power will not be sufficient.</li>
		</ul>
	<li><strong>Other ARM-based Debian Linux systems</strong> - These have not been tested, but should also work&hellip;</li>
	<ul class="af-ul-2">
		<li>As long as you have Bluetooth hardware supported by BlueZ.</li>
		<li>Any internet connectivity is supported.</li>
	</ul>
</ul>

We will be adding support for other small computing systems as demand and time permits.

The Afero Hub Software is not particularly platform- or architecture-specific and should work on comparable systems. We have tested most extensively with Raspberry Pi Models 3, 4, and Zero W; we recommend using those systems for your Developer Hub.

## Set Up the Raspberry Pi

The Raspbian OS allows you to configure the Wi-Fi network from your local PC before booting it in the Raspberry Pi.

<ol class="af-ol">
	<li>Install the Raspbian OS to an appropriate SD card (8GB or larger) via the instructions at <a id="1512602671.12" href="https://www.raspberrypi.org/documentation/installation/installing-images/" target="_blank">https://www.raspberrypi.org/documentation/installation/installing-images/</a>.</li>
	<li>Remove and re-insert the SD card into your computer.</li>
	<li>Open the SD card image, which will have the volume name of “boot”.</li>
	<li>Using a text editor like Notepad for Windows, TextEdit for macOS, or your favorite plain text editor program (<em>do not</em> use a document program like Word or Pages), do the following:
		<ol class="af-ol-lower-alpha">
			<li>Create a file on the SD card named “wpa_supplicant.conf”.</li>
			<li>Paste the text shown below into this file, replacing the Wi-Fi network SSID (network name) and password with the proper values for your Wi-Fi network. If you reside outside the US, replace the “country” setting with the proper two-letter country code.
<pre>
 country=US
 ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
 update_config=1

 network={
 ssid="<em>your_wifi_ssid</em>"
 scan_ssid=1
 psk="<em>your_wifi_password</em>"
 key_mgmt=WPA-PSK
 }
</pre>
			</li>
			<li>Save the file.</li>
		</ol>
	</li>
	<li>(Optional) If you do not have a keyboard and monitor to attach to the Pi, you can use a secure shell connection to the Pi to connect over your network: Using your text editor, create a file named “ssh” or “ssh.txt”. The contents of this file don’t matter, as long as the file exists on the “boot” SD card and is named “ssh”. This allows you to use the “ssh” command to connect to the Pi over your network. </li>
	<li>Unmount the SD card and remove it from your computer. Insert it into the Raspberry Pi, and power up the Pi. In subsequent steps, you will work on the Pi.</li>
	<li>And finally, please take these important security steps after your Pi is set up and working: 
		<ol class="af-ol-lower-alpha">
			<li>Change the default password for the “pi” user using the <code>passwd</code> command.</li>
			<li>Replace the plaintext Wi-Fi password in the file <code>/etc/wpa_supplicant/wpa_supplicant.conf</code> with an encrypted version using the <code>wpa_passphrase</code> utility.</li>
		</ol>
	</li>
</ol>
## Install the Afero Hub Software

You can install the Afero Hub Software in either of two ways. The first, [via APT repository](../StandaloneHub#install-the-hub-software-via-apt-repository-recommended), is recommended. The second, [manual install](../StandaloneHub#install-the-hub-software-manually), is there if you need it.

### Install the Hub Software via APT Repository (Recommended)

Afero provides a Debian Repository for the packages we publish. If you add our repository to your system’s Advanced Packaging Tool (APT) sources list, you can install and update packages through the normal Debian package tools, such as `aptitude` and `apt-get`.

<ol class="af-ol">
	<li><p>Run the following commands to download a script that will add Afero to your APT sources:</p>
<pre>
 $ wget https://cdn.afero.io/repo/deb/addrepo.sh
 $ bash ./addrepo.sh
</pre>
<ul class="af-ul">
	<li>This script adds the Afero repo information and signing key to your system configuration.</li>
	<li>Select <strong>y</strong> to run <code>apt-get update</code> as part of the script. This will take a few minutes to run.</li>
</ul>
<pre>
 $ sudo apt-get install afero-hub
</pre>
	<p>This will fetch the afero-ble and afero-hub packages as well as any other prerequisite packages you may need.</p>
</li>
<li><p>During installation, you will be prompted to accept the <a id="1518220410.67" href="https://www.afero.io/html/home/privacy.html" target="_blank">Afero Developer Terms of Service</a> (scroll down on this page to find the Developer Terms), which is the license that covers your use of the Afero ble and hub packages.</p>
</li>
<li><p>Skip down to <a id="024-vLeNRYF" href="/StandaloneHub#add-the-virtual-hub-device-to-your-account">Add the Virtual-Hub Device to Your Account</a> to continue.</p>
</li>
</ol>

### Install the Hub Software Manually

<ol class="af-ol">
	<li><p>If you don’t wish to use our package repository, or if you’re installing these packages offline, you may download them manually from the following URLs:</p>
		<ul class="af-ul">
			<li><a id="024-Zbudshw" href="https://cdn.afero.io/repo/deb/afero-ble_latest_armhf.deb" target="_blank">https://cdn.afero.io/repo/deb/afero-ble_latest_armhf.deb</a></li>
			<li><a id="024-Gflpysu" href="https://cdn.afero.io/repo/deb/afero-hub_latest_armhf.deb" target="_blank">https://cdn.afero.io/repo/deb/afero-hub_latest_armhf.deb</a></li>
		</ul>
		<p>If you intend to install these packages offline, you will also need to download the <code>html2text, qrencode, libqrencode3, and bluetooth</code> packages, either from a connected Debian system (with the command: <code>apt-get download html2text bluetooth qrencode libqrencode3 libpng12-0</code>), or fetch them online from <a id="024-3P5bYWx" href="https://packages.debian.org/stretch/allpackages/" target="_blank">https://packages.debian.org/stretch/allpackages/</a>.</p>
	</li>
	<li><p>Install these packages with the following commands:</p>
<pre>
 $ sudo dpkg -i html2text*.deb libpng12-0*deb libqrencode3*.deb qrencode*deb bluetooth*deb
 $ sudo reboot
 $ sudo dpkg -i afero-ble*.deb
</pre>
	</li>
	<li><p>You will be prompted to accept the <a id="1518220410.68" href="https://www.afero.io/html/home/privacy.html" target="_blank">Afero Developer Terms of Service</a> (scroll down on this page to find the Developer Terms), which is the license that covers your use of the Afero ble and hub packages.</p>
<pre>
 $ sudo dpkg -i afero-hub*.deb</pre>
	</li>
	<li><p>Continue with <a id="024-KfyMmM1" href="/StandaloneHub#add-the-virtual-hub-device-to-your-account">Add the Virtual-Hub Device to Your Account</a>.</p>
	</li>
</ol>

## Add the Virtual-Hub Device to Your Account

The installation of the Hub Software package will create a virtual-hub device that you can connect to your Afero account by scanning a QR code, just as with other devices such as Modulo. Follow the steps below:

<ol class="af-ol">
	<li><strong>Accept Terms of Service</strong> - During installation, you must accept the <a href="https://www.afero.io/html/home/privacy.html" target="_blank">Afero Developer Terms of Service</a> (scroll down on this page to find the Developer Terms), before the packages will install.</li>
	<li><strong>Scan QR code with Afero mobile app</strong> - The package installation will present a QR code on your screen. Launch the Afero mobile app on your smartphone, and tap <span class="UIText">Add Device</span> to add a device to your account, then scan the QR code.
		<p>If the QR code won’t scan for some reason, tap <span class="UIText">Manually Add Device</span> and type the alphanumeric Association ID listed below the QR code.</p>
	</li>
	<li>The software will then connect to the Afero Cloud to associate the Hub Software with your account.</li>
</ol>
	
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

<ul class="af-ul">
	<li><p>To <strong>manually stop</strong> the daemons, use:</p>
		<pre>
$ sudo systemctl stop beetle</pre>
		<p>Stopping the beetle daemon will automatically stop the hubby daemon.</p>
	</li>
	<li><p>To <strong>manually start</strong> the daemons, use:</p>
		<pre>
$ sudo systemctl start hubby</pre>
		<p>Starting the hubby daemon will automatically start the beetle daemon.</p>
	</li>
</ul>


### Manage the Virtual-Hub Device

If for some reason you remove the virtual-hub device from your Afero account, or if the virtual-hub stops working, you can easily re-add it to the same or another Afero account without re-installing the software. To do this, run the following command on your system:

```
$ sudo dpkg-reconfigure afero-hub
```

You’ll be asked if you want to re-build the hub configuration:

<ul class="af-ul">
	<li>Answer <strong>no</strong> if you removed the virtual-hub device from your account, and simply want to associate it with another account. By answering <strong>no</strong>, the existing virtual-hub device’s QR code will be displayed. Simply scan the QR code to add it to an Afero account.</li>
	<li>
		<p>Answer <strong>yes</strong> if your virtual-hub device has stopped working, especially if there are messages in <code>/var/log/syslog</code> that indicate the device’s configuration has been damaged. By answering <strong>yes</strong>, the configuration will rebuild and allow you to add a new hub-device by scanning a new QR code.</p>
		<p>If the old virtual hub remains in the mobile app even after you’ve added the new device, select the old device icon and tap <span class="UIText">Remove Device</span> to remove it from your account.</p></li>
</ul>


### Remove, Reinstall, and Update the Hub Software

You can remove, re-install, and update these packages using normal Debian package commands.

<ul class="af-ul">
	<li><p>To remove the software but keep your configuration intact (to preserve your virtual-hub device), run:</p>
<pre>
$ sudo apt-get remove afero-hub afero-ble</pre>
	</li>
	<li><p>To remove the software along with your associated configuration, first remove the virtual-hub device from your Afero account via the mobile app, then run:</p>
<pre>
 $ sudo apt-get remove --purge afero-hub afero-ble</pre>
	</li>
	<li>If you installed the packages manually (rather than through <code>apt-get</code>), do one of the following.
		<ul class="af-ul-2">
			<li><p>To remove the software but leave the virtual-hub device intact, run:</p>
<pre>
 $ sudo dpkg -r afero-hub afero-ble</pre>
			</li>
			<li><p>To remove both the package <strong>and</strong> the virtual-hub device, run:</p>
<pre>
 $ sudo dpkg -P afero-hub afero-ble</pre>
			</li>
		</ul>
	</li>
	<li><p>When we release a new version of the Hub Software, you can update the packages through the normal <code>apt-get</code> process:</p>
<pre>
 $ sudo apt-get install --only-upgrade afero-ble
 $ sudo apt-get install --only-upgrade afero-hub</pre>
		<p>Upgrading the packages will preserve the virtual-hub device attached to your account.</p>
	</li>
</ul>

## Get Support

Support for the Afero Hub Software is provided through the project's <a id="1574355920.01" href="https://github.com/aferodeveloper/developerhub/issues" target="_blank">GitHub Issue Tracker</a>. More sensitive questions (e.g., concerning your credentials) can be sent to <a id="1501865590.75" href="mailto:developer@afero.io" target="_blank">developer@afero.io</a>.

The Afero Hub Software daemons write information to `/var/log/syslog` so if you have any issues with the operation of the Hub Software please be sure to save a copy of `/var/log/syslog` before you contact us.
