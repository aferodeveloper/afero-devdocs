# Defining Your Own Attributes

The Afero Profile Editor allows you to create a custom set of attributes that you can monitor and manipulate with your application via the Attribute Daemon Client. User-defined attributes set via Profile Editor are numbered from 1 through 1023. You can define the type, default value, and flags for any attribute your application needs. By default, all attributes from 1–1023 are owned by your Attribute Client application and will always be writeable by your application.

## Obtaining the Afero Profile Editor

The Profile Editor is available to all Afero partner-developers and can be downloaded for either Windows or macOS:

- [Windows <img src="../img/windows.svg" width="25" style="vertical-align:middle;margin:0px 0px;border:none">](http://cdn.afero.io/latest-ape/win)
- [macOS <img src="../img/macos.svg" alt="drawing" width="25" style="vertical-align:middle;margin:0px 0px;border:none">](http://cdn.afero.io/latest-ape/mac)

For all the details, refer to the [Afero Profile Editor User Guide](../Projects).

### Sample Profile for Attribute Daemon Clients

In this section, we will create a small sample profile to create some attributes you can manipulate through Attribute Daemon Client. These attributes will be referenced in the examples in the following sections.

**1**&nbsp;&nbsp;Open the Profile Editor and sign in using your Afero developer account email and password.

**2**&nbsp;&nbsp;Select NEW to open the New Project dialog box. For MODULE TYPE, select the module for your Linux hub. In the example, we’ll use “Potenco”, but ensure that the Module Type here matches your Linux hub product. Give your Device Type a name, which will also be used for your default project name. Click the CREATE button to save your new project.

**3**&nbsp;&nbsp;Click ATTRIBUTES in the left-hand navigation bar. You’ll define two attributes for this project.

***3a***&nbsp;&nbsp;Select the network interfaces available in your Linux hub hardware to enable or disable support for that network interface in your device. For Potenco devices, ensure Wi-Fi is checked.

***3b***&nbsp;&nbsp;Still in the Attributes window, click +DEVICE ATTRIBUTE. Create a new attribute using:

- ID = 1
- Name = Output
- Data type = Boolean
- Attribute Options = Select both Read and Write checkboxes.

***3c***&nbsp;&nbsp;Click +DEVICE ATTRIBUTE again. Create a new attribute using:

- ID = 2
- Name = Input
- Data type = Boolean
- Attribute Options = Select Read checkbox **only**, leaving Write unchecked.
      
***3d***&nbsp;&nbsp;Click SAVE in the upper-right of the window to save your attribute definitions.
   
**4**&nbsp;&nbsp;Click UI CONTROLS in the left-hand navigation bar. You’ll now start defining the user interface.

***4a***&nbsp;&nbsp;Click +ATTRIBUTE OPTION to open the Attribute Option Details dialog box for the Output attribute. Complete the fields as shown:

<img src="../img/LinuxSDK-APE-1.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

Click OK to save the Output attribute details.

***4b***&nbsp;&nbsp;Click +ATTRIBUTE OPTION again to open the Attribute Option details for the Input attribute. Complete the fields as shown:

<img src="../img/LinuxSDK-APE-2.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

Click OK to save the Input attribute details.

***4c***&nbsp;&nbsp;Still on the UI Controls window, under DEFINE THE UI CONTROLS heading, click +CONTROL to assign a UI control to the Output attribute. Select MENU as the control type. Click ADD to assign the control. On the pane that opens, select “Output” for the ATTRIBUTE OPTION and “Inline” as the VIEW STYLE.

<img src="../img/LinuxSDK-APE-3.png" width="400" style="vertical-align:middle;margin:0px 0px;border:none">

***4d***&nbsp;&nbsp;Click +CONTROL again to assign a UI control to the Input attribute. Select MENU as the control type as well. Click ADD to assign it. On the pane that opens, select “Input” as the ATTRIBUTE OPTION and “Inline” as the VIEW STYLE.

***4e***&nbsp;&nbsp;Click SAVE in the upper-right of the window to save your UI Controls.
   
**5**&nbsp;&nbsp;Click UI CONTROL GROUPS in the navigation bar. You’ll now define the last part of the user interface: grouping your two menu controls.

***5a***&nbsp;&nbsp;Click the + button to create a new group and give the group a name, like “IO”. Drag both UI Controls from the far-right pane into the group.

<img src="../img/LinuxSDK-APE-4.png" width="400" style="vertical-align:middle;margin:0px 0px;border:none">

***5b***&nbsp;&nbsp;Click SAVE in the upper-right of the window to save your UI Control Group.

**6**&nbsp;&nbsp;Click PUBLISH in the left-hand navigation bar. If you have a Linux hub device on your Afero account, you can publish this profile to that device now from the Publish window. This will update the device profile that is visible to Attribute daemon and you can then access and modify these attributes via client applications described elsewhere in this Afero Linux SDK Guide.

### Attribute Definition Reference

In the directory where you saved your profile, there is a file called **`device-description.h`**. This file is a C-format header file that defines the names, data types, and other information in the profile you just created. If you create Attribute Client applications using the available C API, this file is handy to include in your application to access attribute information. If you make any changes to your profile in the Profile Editor, this file will be updated, so as you develop your profile remember to copy it to your application.