# Publish Your Profile

The final step in device Profile creation is publishing. When you publish your Profile using the Afero Profile Editor, several operations take place:

- The Device Type, Attribute, and Mobile App UI definitions are pushed to the Afero Cloud.
- The source you’ve created is used to generate a header file you will include in any MCU code you write that uses [afLib](../API-afLib).
- The device Profile is installed onto your Afero module on your device.

## Before You Publish

Do the following to prepare for publishing:

<ol class="af-ol">
	<li>Have your smartphone nearby with the Afero mobile app installed, running. Be signed in to your account.</li>
	<li>Have your Afero module connected to a micro-USB power source, either an AC adapter or your computer (not your smartphone).</li>
	<li>
		<p>With the Afero Profile Editor opened to your project, and with <span class="UIText">Publish</span> selected from the Navigation pane, your window should show a QR Code Generator button, a list of the devices associated with your account, and your device activity:</p>
		<img class="img-br" src="../img/Publish.png" width="600" alt="Publish Project">
	</li>
</ol>

   
### QR Code Generator

It might be convenient to generate a QR code on the fly. This would be helpful, for example, if you soldered your Afero module onto a board and made it inaccessible for subsequent scanning. To use the generator, click the <span class="UIText">QR CODE GENERATOR</span> button. In the window that appears, type the Association ID, click <span class="UIText">GENERATE</span>, and your code will appear. To save the code as a Scalable Vector Graphics (SVG) file, click the <span class="UIText">SAVE AS SVG</span> button. To generate your device’s QR code, you can also click the little QR code icon just to the right of your Association ID in the Developer Device table.

### Developer Devices

The first section lists all devices that belong to your account, including the devices shared with you. Note you can change the sort order of the information by clicking the column heading on which you want to control the sort.

For each device, you’ll see:
<div class="af-table-borders">
	<table class="af-table-borders">
		<tbody>
			<tr>
				<td>Checkbox</td>
				<td><p>Use the checkbox(es) in the first column to select the device(s) to which you want to publish the active Profile. If you see a red exclamation mark <img class="img-inline" src="../img/RedBang.png" alt="Exclamation Mark"> (as shown above), it could mean:</p>
				<ul class="af-ul">
					<li>The Module Type is not compatible with the Module Type of the active Profile.</li>
					<li>The device has a firmware update pending. Cycle the power on the device to trigger the firmware update. You’ll be able to see the update progress in the mobile app.</li>
				</ul>
			<p>Hover your cursor over the exclamation mark to reveal the popup with specific information.</td>
			</tr>
			<tr>
				<td>Friendly&nbsp;Name</td>
				<td>This is the friendly device name the end-user will see in the Afero mobile app (and which they can edit in the mobile app). You can edit the name here before publishing: hover your cursor over the name and click once the pencil icon appears. Type the new name then click the checkmark to save.</td>
			</tr>
			<tr>
				<td>Device Type</td>
				<td><p>The name and icon from the Device Type definition you created for this device. Note that if you change the Device Type Name on the Device Type window, the Publish window will not reflect the name change until after you publish the Profile to a device.</p>
				<p>You will need this name, after publishing, to submit a request to Afero for a factory programmer.</p></td>
			</tr>
			<tr>
				<td>Profile Version</td>
				<td><p>This device Profile version number is unique among all devices and users and is automatically incremented when any new Profile is published by any user. This means that your Profile Version numbers will increase every time you publish, but the specific number of your “next version” is not dependent exclusively on your current version. Hover over the version number to also view the Bootloader and Firmware version numbers.</p>
				<p>You will need this version number, after publishing, to submit a request to Afero for a factory programmer.</p></td>
			</tr>
			<tr>
				<td>Assn ID</td>
				<td>The Association ID, which is the number in the QR code that appears on the label of your Afero module. It is used to associate your device with your account. Click the QR code icon to open the QR Code Generator window. From there you can copy or edit the Association ID and regenerate the QR  code if necessary. Be careful if you do this as the Assn ID must match the ID on the actual device.</td>
			</tr>
			<tr>
				<td>Status</td>
				<td>The Bluetooth signal strength between the Afero module and your smartphone currently running the Afero mobile app.</td>
			</tr>
			<tr>
				<td>Test button</td>
				<td>Click the <span class="UIText">Test</span> button to open the Attribute Tester. From this window, click an attribute to open its value for editing. Any value changes you make are reflected in the Afero mobile app UI.
				</td>
			</tr>
		</tbody>
	</table>
</div>


Click a column heading to reorder or change the sort. Note that sorting works best on columns that display relatively static data, such as Name, Device Type, SW Version, and ID.

### Device Activity

Below the Developer Devices is a list of the activity (events) of all your devices with Afero in real-time. Note that this is not a history; the activity you see begins when you first go to the Publish window. To see activity for a specific device, use the DEVICE FILTER.

Activities listed include attribute changes (e.g., you can watch the effect of a button-tap in the Afero mobile app that turns on an LED on your device) as well as operations in the Publish sequence, which means you’ll be able to watch the publish process here.

For each event you will see:

<div class="af-table-borders">
	<table class="af-table-borders">
		<tbody>
			<tr>
				<td>Timestamp</td>
				<td>The time when the activity occurred.</td>
			</tr>
			<tr>
				<td>Attribute</td>
				<td>If an attribute was changed by the activity, it is identified here using its name and unique numeric identifier.</td>
			</tr>
			<tr>
				<td>Value</td>
				<td>If an attribute value was changed by the activity, the resulting value is shown here.</td>
			</tr>
			<tr>
				<td>Raw Value</td>
				<td>The hex string representation of the changed attribute value.</td>
			</tr>
		</tbody>
	</table>
</div>

## Now, Let’s Publish

<ol class="af-ol">
	<li>With the Afero mobile app active and your device powered up, the Status for this device should show a number like -30dBm. Specifying a specific numeric target is complex, but the point is that Status should not read “OFFLINE”. If it does, try restarting the mobile app, cycling power to the device, or restarting the Afero Profile Editor.</li>
	<li>Select the orange checkbox next to the ID of the device to which you intend to install.</li>
	<li>Click the <span class="UIText">Publish</span> button in the upper-right of the window.</li>
	<li>After a short interval, you should see a banner at the top of the window that tells you the Profile has been published and the Over-The-Air (OTA) update is in progress. During the update, you may see log info scroll past in the Device Activity, including lines such as “The Profile ID for this device was changed from &lt;old ID&gt; to &lt;new ID&gt;”. On the Afero dev boards, you may also see the LED flash.</li>
	<li>The update of your device is complete when the SW Version data in the Developer Devices table reads the new version number. This indicates the new Profile has been installed, and the device has restarted and reported its software version to the Afero Cloud.</li>
	<li>When the Profile is complete, the Afero mobile app should update to display the Control UI you defined for your device. If the update does not happen quickly, try restarting the Afero mobile app.</li>
</ol>

Once the new Control UI is in place, you should be able to use the controls to change attributes on the associated device.