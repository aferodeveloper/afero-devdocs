# Tutorial 2: Creating a Device Profile

Before you start, we assume that you’ve already run through [Tutorial 1: Linking Modulo](../Lesson1). If not, please start there.

This tutorial is designed for the Modulo-1 and Modulo-2 dev boards, but not for the Modulo-1B.

**1&nbsp;&nbsp;If you haven’t already, download the Afero Profile Editor, for [Windows <img src="../img/windows.svg" alt="drawing" width="25" style="vertical-align:middle;margin:0px 0px;border:none">](http://cdn.afero.io/latest-ape/win) or [macOS <img src="../img/macos.svg" alt="drawing" width="25" style="vertical-align:middle;margin:0px 0px;border:none">](http://cdn.afero.io/latest-ape/mac).**

Open and sign in to the Profile Editor. The Startup window appears:

<img src="../img/APE-StartScreen-Tut.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

**2&nbsp;&nbsp;For the purposes of this tutorial, we’re going to save you some typing by providing a pre-configured Modulo project.**

***2a*** First, you will need the appropriate Sample Project. The easiest way to get this is from the Profile Editor system <span class="UIText">TOOLS</span> menu. From this menu, select <span class="UIText">SAMPLE PROJECTS</span> &gt; [<span class="UIText">GITHUB REPOSITORY</span>](http://github.com/aferodeveloper/APE-Project-Profiles). (All Modulo projects, and more, are available from this GitHub repository.)
	
***2b*** On the GitHub web page that opens, click the green <span class="UIText">CLONE OR DOWNLOAD</span> button then select <span class="UIText">DOWNLOAD ZIP</span>. Navigate to your Profile Editor project directory to save the zip file.
    
***2c*** Once saved, go to the Profile Editor project directory on your filesystem and double-click the file to decompress it. You can keep all the projects in the unzipped project profiles directory, or delete the projects you’re not interested in.

***2d*** Back in the Afero Profile Editor Startup window, select the <span class="UIText">OPEN</span> button, navigate to the directory where you unzipped the project, and select the directory that holds the Modulo project you want to use in this lesson.

**3&nbsp;&nbsp;Once the project is open, click <span class="UIText">DEVICE TYPE</span> in the Navigation pane in the left of the window.**

To be sure we’re all starting in the same view, your window should look something like this:

<img src="../img/Mod2_DeviceType.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

<mark>**Note:**&nbsp; Notice the <span class="UIText">MODULE TYPE</span> of the Profile is specified right at the top of the pane (in the example, we’re working with a Modulo-2.) This module type was specified in the pre-built Profile you downloaded; when creating a Profile from scratch, you must specify the module type right from the beginning. Doing so is the first step in the <span class="UIText">NEW PROFILE</span> window. It’s critical that the module type in the Profile match the hardware you’re using in your project!</mark>

In the <span class="UIText">DEVICE TYPE</span> view, you can set the device name, type a detailed device description, and set the device icon. Of these items, only the icon will be visible to the end-user. Notice the <span class="UIText">PREVIEW</span> panel on the right of the view – it shows the selected device icon as it will appear in the mobile app.

Since we’re working with a pre-configured Profile, you don’t need to make any changes, but if you like, feel free to do so. When you’re finished, click <span class="UIText">SAVE</span> to save any changes you might have made.

**4&nbsp;&nbsp;Click ATTRIBUTES in the left-hand Navigation pane.** 

You’ll see that the four Modulo GPIO pins are listed. Only two GPIO pins are needed for this project, pins 0 and 3. Click the On/Off switch to <span class="UIText">ON</span> for these two pins; you’ll see them highlighted in the right-hand <span class="UIText">PREVIEW</span> pane:

<img src="../img/AttributeDefinition-Tut.png" width="400" style="vertical-align:middle;margin:0px 0px;border:none">

**5&nbsp;&nbsp;For this project, we’ve defined two attributes.**

One attribute is for GPIO 0, which is connected to the LED on the Modulo; and one for GPIO 3, which is tied to the pushbutton. We’ll take a detailed look at the attributes now. Note that selected values are shown in white letters on an orange background.<br>

***5a*** Click <span class="UIText">LED ATTRIBUTE</span>, the attribute name for GPIO 0, to open its detailed view:

<img src="../img/Tut2_LED_Attr.png" width="400" style="vertical-align:middle;margin:0px 0px;border:none">     

 - <span class="UIText">ATTRIBUTE NAME</span> is a required field for all attributes. We’ve named this one <span class="UIText">LED ATTRIBUTE</span> but you can change it in this field.
 - To control the LED from the mobile app, <span class="UIText">OPERATION MODE</span> must be <span class="UIText">OUTPUT</span>. This is because Outputs are Read/Write, and sending a command from the mobile app requires writing to an attribute.
 - GPIO 0 is given a <span class="UIText">DEFAULT LOGIC LEVEL</span> of 0, which sets the default state of the LED to OFF.
 - Leave both <span class="UIText">OPTIONS</span>, <span class="UIText">PWM</span> (pulse-width modulation) and <span class="UIText">PULSE</span>, deselected.
 - Change the <span class="UIText">ACTIVE</span> selector set to <span class="UIText">LOW</span>. Because the cathode of the LED is connected to GPIO 0, the LED turns on when the pin goes <span class="UIText">LOW</span>.
 - Don’t select any of the <span class="UIText">BIND TO ATTRIBUTES</span> buttons.
      
***5b*** Click <span class="UIText">BUTTON ATTRIBUTE</span>, the attribute name for GPIO 3, to open its detailed view:

<img src="../img/Tut2_Button_Attr.png" width="400" style="vertical-align:middle;margin:0px 0px;border:none"><br>

 - <span class="UIText">ATTRIBUTE NAME</span> is a required field for all attributes. We’ve named this one <span class="UIText">BUTTON ATTRIBUTE</span> but you can change it here.
 - Leave GPIO3 <span class="UIText">OPERATION MODE</span> set to <span class="UIText">INPUT</span>. This will display the state of the button when pressed in the mobile app.
 - Change the <span class="UIText">ACTIVE</span> setting to <span class="UIText">LOW</span>. When pressed, the button connects GPIO 3 to ground so the pin is considered active when it goes <span class="UIText">LOW</span>.
 - The <span class="UIText">PULL UP</span> setting is required for to keep the GPIO pin inactive when the button is not pressed.
 - With <span class="UIText">IS TOGGLE</span> set, the status displayed by the mobile app will switch with every momentary button push. If <span class="UIText">IS TOGGLE</span> is not set, the state displayed will always reflect the current button state.
 - Leave the <span class="UIText">DEBOUNCE TIME</span> set to 0, the <span class="UIText">ACTIVE</span> selector set to <span class="UIText">HIGH</span>, and don’t select any of the <span class="UIText">BIND TO ATTRIBUTES</span> buttons.
      
***5c***&nbsp; Click <span class="UIText">SAVE</span> when you are finished defining project attributes.<br>

**6&nbsp;&nbsp;To move on to defining the mobile app UI for this project, click <span class="UIText">UI CONTROLS</span> in the left-hand Navigation pane.**

You will probably not be surprised to find that two UI controls have been defined since our project uses two attributes. It’s common, though not a rule, to have one UI control for every attribute. As examples: some attribute values will be used “behind-the-scenes” to calculate the values of other attributes, and you can assign the same control to multiple attributes and multiple controls to the same attribute.

There are two steps to defining the UI controls: a) we first define the “attribute options” for each attribute; and b) we then assign UI controls to each attribute.

<mark>**Note:**&nbsp; You might be wondering why defining attribute options is separate from assigning controls. The reason is so you can easily assign multiple controls to the same attribute without having to retype a lot of details (although that doesn’t apply to this Tutorial). To learn more about attribute options, read [Define the Mobile App UI > Define the Attribute Options](../AppUIDef#Options).</mark>

***6a*** In the far-right pane under the heading <span class="UIText">ATTRIBUTE OPTIONS</span>, click the pencil icon in the <span class="UIText">LED:LED ATTRIBUTE</span> box to open its Attribute Option Details dialog:

<img src="../img/Les2-LEDAttrOpt.png" width="400" style="vertical-align:middle;margin:0px 0px;border:none">

You will see that we have defined the following:

 - The label for this control will display as “LED”.
 - The LED control is selected as <span class="UIText">PRIMARY OPERATION</span>, which means you can control the LED from the Home screen of the mobile app by double-tapping the device icon.
 - Note that two <span class="UIText">VALUE OPTIONS</span> have been defined. These present the two different states the on-board LED can have: 1 is On, 0 is Off. You can edit the labels if you want, but keep the values as defined.
 - Check the <span class="UIText">RUNNING STATE</span> box next to the On state for the LED. This will change the color of the device in the mobile app when the LED is on.
 - We don’t need to complete the <span class="UIText">RANGE OPTIONS</span> for this attribute since we’ll be using a Menu control, which does not require range definitions.
 - Click <span class="UIText">OK</span> to close the Attribute Option Details dialog.

Now, again in the far-right pane, click the pencil icon in the <span class="UIText">BUTTON:BUTTON ATTRIBUTE</span> box to open its Attribute Option Details dialog:

<img src="../img/Les2-ButtonAttrOpt.png" width="400" style="vertical-align:middle;margin:0px 0px;border:none">

 - The label for this control will display as “Button”.
 - This attribute happens to have the same <span class="UIText">VALUE OPTIONS</span> as the LED attribute, but is not set as the Primary Operation.
 - Again, we don't need any <span class="UIText">RANGE OPTIONS</span> defined.
 - Click OK to close the Attribute Option Details dialog.

***6b*** Now we will assign a UI Control to each attribute. There are many types of UI Controls available, but in this project are using the <span class="UIText">MENU</span> control for both.

<img src="../img/Dfn_UI_Ctls.png" width="400" style="vertical-align:middle;margin:0px 0px;border:none">

Click the first attribute, <span class="UIText">LED ATTRIBUTE (I/O 0): MENU</span>, to open it for editing.

<img src="../img/AttributeDef-LED.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none"><br>
      
 - For <span class="UIText">ATTRIBUTE OPTION</span> we’ve selected <span class="UIText">LED</span>, and for <span class="UIText">CONTROL TYPE</span> we’ve selected Menu.
 - For <span class="UIText">VIEW STYLE</span>, we’ve selected <span class="UIText">INLINE</span>, which means all menu options will be shown. (<span class="UIText">POPUP</span> View Style means the end-user must tap the control to reveal all menu options.) You can see examples by expanding <span class="UIText">VIEW STYLE PREVIEW</span> (shown in the screenshot above).

Now click <span class="UIText">BUTTON ATTRIBUTE (I/O 3): MENU</span> to open it.

<img src="../img/AttributeDef-Button.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

Notice that we’ve also selected the <span class="UIText">INLINE</span> View Style.

***6c*** Click <span class="UIText">SAVE</span> when you are finished defining UI controls.

**7&nbsp;&nbsp;Click UI CONTROL GROUPS in the left-hand Navigation pane to group your controls.**

The left-hand pane, <span class="UIText">DEFINE THE UI CONTROL GROUPS</span>, displays a representation of the device mobile app UI. At the top of both the mobile app and the Profile Editor is the “groups ribbon”; below are the controls for the selected group. Note that device Settings is a selection only shown in the mobile app groups ribbon, not in the Profile Editor.

***7a***  We’ve already set up two groups, shown in the groups ribbon and labeled <span class="UIText">LED</span> and <span class="UIText">Button</span>. (We’ve already created groups for this project, but if we hadn’t, you would click <img src="../img/AddGroupIcon.png" style="vertical-align:middle;margin:0px 0px;border:none"> to create a new group.)

***7b*** On the right, you’ll see all the controls you defined on the UI Controls window, identified by their Default Label. These are the controls available for grouping.

***7c*** Click the group labeled <span class="UIText">LED</span> to make it active and ready for editing. The group will be highlighted in orange:

<img src="../img/Control_Groups_LED.png" width="400" style="vertical-align:middle;margin:0px 0px;border:none">

Below the selected group, you’ll see the <span class="UIText">LED</span> menu control. We’ve already moved the <span class="UIText">LED</span> menu into the <span class="UIText">LED</span>group, but if we hadn’t, you would add a control by dragging it from the <span class="UIText">AVAILABLE UI CONTROLS</span> pane on the right into the controls display on the left, below the groups ribbon.

***7d*** To remove a control from a group, simply drag it back to the <span class="UIText">AVAILABLE UI CONTROLS</span> pane.<br><mark>**Note:**&nbsp; A control can appear in multiple groups, but *must* appear in at least one group or it won’t appear at all in the mobile app UI. That means that even if you have just one UI control, you’ll need to create a UI control group to contain it.</mark>

***7e*** Click SAVE when you are finished defining UI control groups.

**8&nbsp;&nbsp;Now that your device Profile is complete, click <span class="UIText">PUBLISH</span> in the left-hand Navigation pane to install it on your Modulo.**

If you intend to perform the next steps yourself (and you should!), you’ll need to set up some hardware:

 - You should have your Modulo connected to USB (for power only at this time).
 - You should have the Afero app running and open on your smartphone.

Before we get started, take a look at the <span class="UIText">PUBLISH</span> window:

 - This window shows you all the developer devices on your account. For each available device, the Status displays the Bluetooth signal strength, e.g. -40dBm, in an orange badge.
 - The Device Activity log will show all attribute-related communication between device and service in real-time. It is initially populated by the latest cached values from the Cloud.

**OK, time to Publish:**

***8a*** Select the device(s) you want to update by selecting the corresponding orange checkbox(es).

***8b*** Click <span class="UIText">PUBLISH</span>.

***8c*** The update can take up to one minute. You’ll see an update message in the Device Activity and the SW Version will change once the update has completed.

**9&nbsp;&nbsp;Try out the new mobile app UI for your device!**

Notice that the LED is on, which is indicated on the Home screen. Because it’s the Primary Operation defined with a Boolean attribute, tapping the large device icon on the Home screen will toggle the LED on and off.

<img src="../img/ios_app.png" width="800" style="vertical-align:middle;margin:0px 0px;border:none"><br>

You should now be able to control the Modulo LED using the LED buttons on your smartphone, and the pushbutton on the Modulo board should toggle the On/Off Button control in the mobile app UI.

<strong>&#8674;</strong> <em>Next:</em>&nbsp;&nbsp;[Tutorial 3: Afero + Arduino](../Lesson3)