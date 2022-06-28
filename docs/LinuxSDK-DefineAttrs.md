# Defining Your Own Attributes

The Afero Profile Editor allows you to create a custom set of attributes that you can monitor and manipulate with your application via the Attribute Daemon Client. User-defined attributes set via Profile Editor are numbered from 1 through 1023. You can define the type, default value, and flags for any attribute your application needs. By default, all attributes from 1–1023 are owned by your Attribute Client application and will always be writeable by your application.

## Obtaining the Afero Profile Editor

The Profile Editor is available to all Afero partner-developers and can be downloaded for either Windows or macOS:

<ul class="af-ul">
	<li><a id="1560890585.92" href="http://cdn.afero.io/latest-ape/win" target="_blank">Windows <img src="../img/windows.svg" alt="Download for Windows" width="25"></a></li>
	<li><a id="1501187220.85" href="http://cdn.afero.io/latest-ape/mac" target="_blank">macOS <img src="../img/macos.svg" alt="Download for Mac OS" width="25"></a></li>
</ul> 

For all the details, refer to the [Afero Profile Editor User Guide](../Projects).

### Sample Profile for Attribute Daemon Clients

In this section, we will create a small sample profile to create some attributes you can manipulate through Attribute Daemon Client. These attributes will be referenced in the examples in the following sections.

<ol class="af-ol">
	<li>Open the Profile Editor and sign in using your Afero developer account email and password.</li>
	<li>Select <span class="UIText">New</span> to open the New Project dialog box. For <span class="UIText">Module Type</span>, select the module for your Linux hub. In the example, we’ll use “Potenco”, but ensure that the Module Type here matches your Linux hub product. Give your Device Type a name, which will also be used for your default project name. Click the <span class="UIText">Create</span> button to save your new project.</li>
	define two attributes for this project.</li>
	<ol class="af-ol-lower-alpha">
		<li>Select the network interfaces available in your Linux hub hardware to enable or disable support for that network interface in your device. For Potenco devices, ensure Wi-Fi is checked.</li>
		<li>Still in the Attributes window, click <span class="UIText">+Device Attribute</span>. Create a new attribute using:
		<ul class="af-ul">
			<li>ID = 1</li>
			<li>Name = Output</li>
			<li>Data type = Boolean</li>
			<li>Attribute Options = Select both Read and Write checkboxes.</li>
		</ul>
		<li>Click <span class="UIText">+Device Attribute</span> again. Create a new attribute using:
		<ul class="af-ul">
			<li>ID = 2</li>
			<li>Name = Input</li>
			<li>Data type = Boolean</li>
			<li>Attribute Options = Select Read checkbox <strong>only</strong>, leaving Write unchecked.</li>
		</ul>
		</li>
		<li>Click <span class="UIText">Save</span> in the upper-right of the window to save your attribute definitions.</li>
	</ol>
	<li>Click <span class="UIText">UI Controls</span> in the left-hand navigation bar. You’ll now start defining the user interface.</li>
	<ol class="af-ol-lower-alpha">
		<li>Click <span class="UIText">+Attribute Option</span> to open the Attribute Option Details dialog box for the Output attribute. Complete the fields as shown:
<br><img src="../img/LinuxSDK-APE-1.png" alt="Output Attribute Option Details" width="500"><br>
Click <span class="UIText">OK</span> to save the Output attribute details.</li>
		<li>Click <span class="UIText">+Attribute Option</span> again to open the Attribute Option details for the Input attribute. Complete the fields as shown:
<br><img src="../img/LinuxSDK-APE-2.png" alt="Input Attribute Option Details" width="500"><br>
Click <span class="UIText">OK</span> to save the Input attribute details.</li>
		<li>Still on the UI Controls window, under <span class="UIText">Define the UI Controls</span> heading, click <span class="UIText">+Control</span> to assign a UI control to the Output attribute. Select <span class="UIText">Menu</span> as the control type. Click <span class="UIText">Add</span> to assign the control. On the pane that opens, select “Output” for the <span class="UIText">Attribute Option</span> and “Inline” as the <span class="UIText">View Style</span>.</li>
<img src="../img/LinuxSDK-APE-3.png" width="350" alt="Output Menu Control">
		<li>Click <span class="UIText">+Control</span> again to assign a UI control to the Input attribute. Select <span class="UIText">Menu</span> as the control type as well. Click <span class="UIText">Add</span> to assign it. On the pane that opens, select “Input” as the <span class="UIText">Attribute Option</span> and “Inline” as the <span class="UIText"> View Style</span>.</li>
		<li>Click <span class="UIText">Save</span> in the upper-right of the window to save your UI Controls.</li>
	</ol>
		<li>Click <span class="UIText">UI Control Groups</span> in the navigation bar. You’ll now define the last part of the user interface: grouping your two menu controls.</li>
	<ol class="af-ol-lower-alpha">
		<li>Click the <span class="UIText">+</span> button to create a new group and give the group a name, like “IO”. Drag both UI Controls from the far-right pane into the group.</li>
<img src="../img/LinuxSDK-APE-4.png" alt="UI Control Groups">
		<li>Click <span class="UIText">Save</span> in the upper-right of the window to save your UI Control Group.</li>
	</ol>
	<li>Click <span class="UIText">Publish</span> in the left-hand navigation bar. If you have a Linux hub device on your Afero account, you can publish this profile to that device now from the Publish window. This will update the device profile that is visible to Attribute daemon and you can then access and modify these attributes via client applications described elsewhere in this Afero Linux SDK Guide.</li>
</ol>

### Attribute Definition Reference

In the directory where you saved your profile, there is a file called **`device-description.h`**. This file is a C-format header file that defines the names, data types, and other information in the profile you just created. If you create Attribute Client applications using the available C API, this file is handy to include in your application to access attribute information. If you make any changes to your profile in the Profile Editor, this file will be updated, so as you develop your profile remember to copy it to your application.