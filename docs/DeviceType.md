# Define the Device Type

After selecting a project on the Startup window, the Profile Editor opens to the **Device Type definition** window:

<img src="../img/APE-DefineDvcType.png" width="900" style="vertical-align:middle;margin:0px 0px;border:none">

## Add Your Information

**1**&nbsp;&nbsp;In the <span class="UIText">DEFINE THE DEVICE TYPE</span> section, complete the fields using the information in the table below:

   | FIELD              | DESCRIPTION                                                  |
   | :----------------- | :----------------------------------------------------------- |
   | Device Type Name   | This name is a string; by default it assumes the name you gave it when creating the project, but you can change it here. Note that if you do change it here, the project name displayed in Profile Editor will also update to reflect the new name. Here are some important things to know about Device Type:<p><ul><li>In the mobile app, the default name given the device is the Device Type Name. This name will be used only initially as it can be changed to a “friendly name” by the end-user when adding the device to their account, or by editing the device name on the device Settings screen. You, as the developer, can also change this friendly name on the Publish window. So use a descriptive name here that makes sense to you and your end-users. Also, the friendly name is what appears in the Inspector tool to identify your devices.<p><p>Note that if the end-user does change the friendly name on the mobile app, the Device Type Name itself will not change. The only place you can change the Device Type Name is in this field; and only after the Profile has been published does any change you make reflect on the Publish window and in the Afero system.</li><p><li>The Device Type is an important identifier. Once you publish a Profile with a given Device Type, an ID is associated with the Device Type that remains unchanged as long as the Device Type is active. As pointed out above, you can always rename your Device Type using this field, but the `devicetypeID` will not change.</li><p><li>Device Type names must be unique across your projects. (Read more in [Before You Proceed](../SelectProject#before-you-proceed).)</li><p><li>The Device Type is used by the OTA Manager to associate an MCU firmware Image Type with devices.</li></ul>|
   | Description        | This description is a string, internal only and not displayed to the end-user. Type a description that is useful to you. |
   | Project Folder     | The directory on your local filesystem where your project is saved. You can’t edit the location; it’s displayed for information purposes only. |
   | Header File Output | If you are using a device that has a separate MCU, a header file containing the device description (attributes) will be generated when you publish your project. By default, the header file will be saved to your project directory, but you can use this setting to save the header file to a different directory. For example, you might wish to save the file directly into the project directory for the associated MCU code (which will rely on the header). |
   | Device Icon        | This icon will be visible to the end-user. Select a meaningful image to represent your device. Click <span class="UIText">SELECT</span> to see all available icons; highlight the icon you want then click <span class="UIText">USE THIS ONE!</span> Your selected icon will be displayed in the Preview pane to the right, to give you an idea of how it will look on a smartphone. It will also be displayed to the left of your project name in the Navigation pane. |

**2**&nbsp;&nbsp;The <span class="UIText">POWER OPTIMIZATION</span> section provides controls and information regarding battery power consumption.

<mark>**Note:**&nbsp; The Power Optimization calculations apply only to projects using either the Modulo-1 or Modulo-1B modules.</mark>

- Select an <span class="UIText">ADVERTISING INTERVAL</span> (in seconds) for Bluetooth. If you’re running on battery power, the shorter the interval, the more battery power is consumed. However, at higher intervals the responsiveness of the device will be negatively affected due to lag time.
- Once you’ve selected an interval, the <span class="UIText">DAILY AVERAGE CURRENT</span> field will display the projected average current required by Modulo-1 or Modulo-1B for that interval while idle (the module is said to be idle when performing no function other than advertising).
- Click the <span class="UIText">SHOW ADVANCED POWER CALCULATOR</span> link at the bottom of the pane to open the <span class="UIText">APPROXIMATE BATTERY LIFETIME</span> estimator. Use this calculator to estimate the runtime of an Modulo-1 or Modulo-1B based on the battery type you’re using. For a more accurate forecast of the battery life, type the number of connections per day and additional power the device may be using. You can also check what the runtimes might be for different battery capacities.
 
    <img src="../img/APE-ApproxBatteryLife.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

**3**&nbsp;&nbsp;Once you’ve defined the Device Type, you’ll want to save the definition.

- Clicking <span class="UIText">CANCEL</span> reverts any changes you made on the page.
- Clicking <span class="UIText">SAVE</span> initiates validation of the new data and, if validation is successful, the data is saved to your local project directory.

<mark>**Note:**&nbsp;  Every time you save, your data is validated. If validation fails for any reason, an error message will appear then move to the left. Click the error description in the message to highlight the field that needs correcting.</mark>


 **&#8674;** *Next:*&nbsp;&nbsp;[Define the Device Attributes](../AttrDef)