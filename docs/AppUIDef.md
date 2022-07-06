# Define the Mobile App UI

Once you’ve created the Attribute definition, you’re ready to design how the end-user views and controls your device from the Afero mobile app. There are three parts to this task:

- [Define the Attribute Options](../AppUIDef#define-the-attribute-options) - We recommend you begin by providing the details needed to support *all* the UI controls for *all* of your attributes. Some of your attributes may not be suited for some of the controls, or you might have some attributes you don’t plan on exposing in a control; but in case you want to experiment and for maximum flexibility, it’s a good idea to prepare by completing the attribute option details for all your attributes.
- [Define the UI Controls](../AppUIDef#define-the-ui-controls) - In this step you assign attribute options to UI controls. Each control you use will correspond to a specific attribute, except for Battery Level and Temperature: Battery Level and Temperature are unique in that they combine information from more than one attribute. The UI control could provide only a graphic representation of the attribute state (a Read-Only attribute), or could be an active control that allows the end-user to set the attribute value (a Read/Write attribute).
- [Define the UI Control Groups](../AppUIDef#define-the-ui-control-groups) - For usability, you will organize your UI controls into one or more control groups. The name of each group is displayed in a “ribbon” above the group’s controls in the mobile app UI. Control groups are organizational constructs and provide no additional functionality to the enclosed controls.

After you’ve defined the mobile app UI but before you move on to publishing your Profile, check out your UI using a virtual “preview” device in the Afero mobile app. Read more about this feature in [Preview the Mobile App UI](../AppUIDef#preview-the-mobile-app-ui).

## Define the Attribute Options

For each of your attributes, you will define “attribute options”, which specify how a control will display the attribute’s values. Follow the instructions below:

**1**&nbsp;&nbsp;Click <span class="UIText">UI CONTROLS</span> in the left-hand Navigation pane to open the UI Controls definition window.

**2**&nbsp;&nbsp;In the far-right pane, click + <span class="UIText">ATTRIBUTE OPTION</span>. The Attribute Option Details dialog box opens:<br>
<img src="../img/APE-AttrOptDetails.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

**3**&nbsp;&nbsp;Complete each field, as relevant to the targeted UI control(s), using the table below as reference:

   | FIELD                                                        | DESCRIPTION                                                  | RELEVANT UI CONTROL(S)                     |
   | :----------------------------------------------------------- | :----------------------------------------------------------- | :----------------------------------------- |
   | Attribute                                                    | Click the <span class="UIText">ATTRIBUTE</span> drop-down menu and select your first attribute. You will repeat for each of your attributes, but if you know **for a fact** that you won’t be displaying an attribute value in the mobile app UI, then you can skip that attribute. | All Controls                               |
   | Default Label                                                | This is the label that will be displayed in the mobile app UI; it is what the end-user will see. Type a label that clearly identifies the device characteristic that is being displayed or controlled. | All Controls                               |
   | Primary Operation                                            | On/Off checkbox, **only** available for write attributes that toggle between two values, and for controls that support writing to an attribute. Only one of your UI controls can be set as Primary Operation within a given device Profile.With this option **selected**, you’ll be able to control the attribute from the mobile app Home screen by double-tapping the device icon. Double-tapping will toggle between the attribute values. | &bull; Menu<br>&bull; Slider<br>&bull; Temperature<br>&bull; Battery Level<br>&bull; Switch

<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> If you're defining <strong>both</strong> Value and Range Options (below) for the same attribute option, <strong>make the number of value options equal to the number of range steps</strong>. For example, if your Range is <em>0-50</em>, with step size <em>10</em>, make sure you have <em>6</em> Value Options defined.
</div>
</div>

   | FIELD                                                        | DESCRIPTION                                                  | RELEVANT UI CONTROL(S)                     |
   | :----------------------------------------------------------- | :----------------------------------------------------------- | :----------------------------------------- |
   | Value Options                                                | Select the <span class="UIText">VALUE OPTIONS</span> checkbox before entering values. Note that Switch controls **require** that you select this checkbox and define two value options.<br><br>To define the options, type the set of attribute values along with their corresponding labels in the table. In the mobile app, value options are often rendered as a menu of buttons, only one of which can be selected at any one time. Using the menu example, after the end-user taps any of the buttons, the associated attribute would be set to the corresponding value.<br><br>If you select the <span class="UIText">RUNNING STATE</span> box for a given value option, when the associated attribute has the selected value, your device will be marked as “Running” in the mobile app UI — this means the Home screen device icon will be colored orange.<br><br>Note that the value options you define will appear in the UI in the order listed in the table. They won't automatically reorder in the UI on the basis of value or label. This gives you complete control over presentation, regardless of underlying implementation.<br><br>**To reorder a defined value option**, move your cursor to the left of the option to reveal the hand cursor and a textured handle. Click and hold the handle; without letting go, reposition the option, then let go to drop it into place.<br><img src="../img/MovingValueOptions.png" width="200" style="vertical-align:middle;margin:0px 0px;border:none"> | &bull; Menu<br>&bull; Slider<br>&bull; Value<br>&bull; Switch (*required*)<br>&bull; Text Box |
   | Range Options                                                | Select the <span class="UIText">RANGE OPTIONS</span> checkbox before entering values.<br><br>Min, Max - All values must be numbers (can use decimals with Q15_16 data type). Min must be less than Max. The Min value is both the minimum limit of the slider range and the numeric label shown on the left. Max value is the maximum of the range, on the right.<br><br>Number or Size of Steps - You must define the set points (steps) you want the control to use between Min and Max values. The way you do this depends on the underlying data type of the attribute:<p><ul><li>If your attribute is a fixed-point integer (Q_15_16 data type), you will set the number of steps. The size of each step will be calculated for you.</li><li>For all other data types, you must type the minimum granularity of the slider. For example, if you define Min as 0, Max as 1000, and Step as 2, there are 500 valid positions on the slider.</li></ul>Unit Label - This label describes the units that the control is displaying and is limited to eight characters. The label is drawn above the control adjacent to the current value of the slider. As a Slider example, if you define the label as “Lumens”, as you drag the slider, it changes from “25 Lumens” to “40 Lumens”, and so on. | &bull; Slider<br>&bull; Temperature<br>&bull; Battery Level             |

**4**&nbsp;&nbsp;When you have finished defining the details for a given attribute, click OK to save and dismiss the dialog box. Your saved attribute options will all be listed in the far-right column labeled <span class="UIText">ATTRIBUTE OPTIONS</span>. Each attribute option is identified with its *Default Label:Attribute* in the upper-left of each pane:<br>

<img src="../img/APE-AttrOptionsPane.png" width="200" style="vertical-align:middle;margin:0px 0px;border:none">

You can take the following actions on any of your saved attribute options:

- View a summary of your attribution option details by hovering over the info icon<img src="../img/APE-InfoIcon.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">.
- Edit saved attribute option details by selecting the pencil icon<img src="../img/PencilIconWhite.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none"> in the relevant Attribute Option pane: The Attribute Option Details dialog box will open for edit.
- You can delete an attribute option by selecting the trash icon<img src="../img/TrashIconWhite.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">.

**6**&nbsp;&nbsp;Select <span class="UIText">+ ATTRIBUTE OPTION</span> to open a new Attribute Option Details dialog box; add the details for your next attribute and save. Repeat for each attribute.

Now that you’ve defined all your attribute options, you can move on to assigning controls for your attributes.

## Define the UI Controls

Use this window to define which attribute option(s) will be displayed using which UI controls(s).

You can assign a single attribute option to multiple UI controls. Each control assigned will use the attribute option details that are relevant to that control.

**1**&nbsp;&nbsp;If you haven’t defined any controls for your device yet, or to create a new one, click <span class="UIText">+ CONTROL</span> to open the Select Control Type window. You’ll be able to select from the following controls (read below, step 2, for details).

   | [MENU](../AppUIDef#MenuControl)      |      | [SLIDER](../AppUIDef#SliderControl)  |
   | ------------------------------------------------------------ | ---- | ------------------------------------------------------------ |
   | <img src="../img/MenuControl.png" width="200" style="vertical-align:middle;margin:0px 0px;border:none"> |      | <img src="../img/SliderControl.png" width="200" style="vertical-align:middle;none"> |
   | [VALUE](../AppUIDef#ValueControl)    |      | [TEMPERATURE](../AppUIDef#TempControl) |
   | <img src="../img/ValueControl.png" width="200" style="vertical-align:middle;margin:0px 0px;border:none"> |      | <img src="../img/TempControl.png" width="200" style="vertical-align:middle;margin:0px 0px;border:none"> |
   | [BATTERY LEVEL](../AppUIDef#BatteryControl) |      | [SWITCH](../AppUIDef#SwitchControl)  |
   | <img src="../img/BatteryLevelControl.png" width="200" style="vertical-align:middle;margin:0px 0px;border:none"> |      | <img src="../img/SwitchControl.png" width="200" style="vertical-align:middle;margin:0px 0px;border:none">|
   | [TEXT BOX](../AppUIDef#TextBoxControl) |      |                                                              |
   | <img src="../img/TextBoxControl.png" width="200" style="vertical-align:middle;margin:0px 0px;border:none"> |      |                                                              |

**2**&nbsp;&nbsp;Use the following guidelines when selecting and defining a control. Note that if you have not defined attribute options for a given attribute, that attribute won’t be available in the <span class="UIText">ATTRIBUTE OPTION</span> drop-down menu. You must do that by selecting the <span class="UIText">+ATTRIBUTE OPTION</span> button in the right-hand pane. Instructions for completing those fields are described above in [Define the Attribute Options](../AppUIDef#Options).

**MENU** - Use when the function has a small number of discrete value states and when each value state is short (can only be one line of text). When you select this control you must define the fields described below:

   | FIELD            | DESCRIPTION                                                  |
   | :--------------- | :----------------------------------------------------------- |
   | Attribute Option | Select the attribute option you want to associate with this Menu control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+ATTRIBUTE OPTION</span> button in the right-hand pane. |
   | Control Type     | Should be predefined with Menu control.                      |
   | View Style       | Select <span class="UIText">INLINE</span> or <span class="UIText">POPUP</span>. Inline style exposes each menu item as a selectable button. Popup style shows the current value in a selectable circle, which expands to the full menu on tap. Select <span class="UIText">VIEW STYLE PREVIEW</span> to see what that control will look like in the mobile app. |

    

**SLIDER** - Use when the attribute supports a range of values. The attribute associated with a Slider must have a data type with sufficient range to support the full range of values and will need to be defined as Output (GPIO) or Writeable (MCU).

   When you select this control you must define the fields described below:

   | FIELD            | DESCRIPTION                                                  |
   | :--------------- | :----------------------------------------------------------- |
   | Attribute Option | Select the attribute option you want to associate with this Slider control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+ATTRIBUTE OPTION</span> button in the right-hand pane. |
   | Control Type     | Should be predefined with Slider control.                    |
   | View Style       | Select <span class="UIText">INLINE</span> or <span class="UIText">POPUP</span>. Inline style displays the full slider. Popup style shows the current value in a selectable circle, which expands to a full vertically-oriented slider on tap. Select <span class="UIText">VIEW STYLE PREVIEW</span> to see what that control will look like in the mobile app. |

**VALUE** - Use when you need to display status, as text. You will probably associate a Value control with an MCU attribute that has a data type of UTF8, which would allow your MCU to set the Value control display dynamically. If the attribute is read/write, the user will be able to change the value on tap.

When you select this control you must define the fields described below:

   | FIELD            | DESCRIPTION                                                  |
   | :--------------- | :----------------------------------------------------------- |
   | Attribute Option | Select the attribute option you want to associate with this Value control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+ATTRIBUTE OPTION</span> button in the right-hand pane. |
   | Control Type     | Should be predefined with Value control.                     |
   | View Style       | Select <span class="UIText">INLINE</span> or <span class="UIText">POPUP</span>. Inline style displays the current value in a rectangle. Popup style shows the current value in a selectable circle. When tapped, both view styles can allow a selection by the end-user. Select <span class="UIText">VIEW STYLE PREVIEW</span> to see what that control will look like in the mobile app. |
  
**TEMPERATURE** - Use when the function specifically requires the user set a temperature level. This control uses up to three attributes:

   - (Required) The first attribute is read/write. It represents the value of a thermostat set point, or Target Temperature, which the user will set using the specialized slider.
   - (Optional) The second represents the Current Temperature, so is a read-only value displayed on a scale, which you define in Celsius degrees.
   - (Optional) The last attribute represents the Current Status; it is read-only and can be used to display the status of a heating/cooling device, such information as “Heating” or “Cooling”. Current status can also display a numeric value.

When you select this control you must define the fields described below:

   | FIELD                 | DESCRIPTION                                                  |
   | :-------------------- | :----------------------------------------------------------- |
   | Attribute Option      | Select the attribute option you want to associate with this Temperature control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+ATTRIBUTE OPTION</span> button in the right-hand pane. |
   | Control Type          | Should be predefined with Temperature control.               |
   | View Style            | Select <span class="UIText">INLINE</span> or <span class="UIText">POPUP</span>. Inline style displays the full slider. Popup style shows the current value in a selectable circle, which expands to a full vertically-oriented slider on tap. Select <span class="UIText">VIEW STYLE PREVIEW</span> to see what that control will look like in the mobile app. |
   | Additional Attributes | You can optionally set two more attributes if you want to show the end-user the current temperature and a heating “status” (as a number or a string):<p><ul><li><span class="UIText">CURRENT TEMPERATURE</span> - Select a defined attribute that will report the current temperature, then in the Attribute Options for this attribute, define the <span class="UIText">CURRENT TEMP RANGE</span>: Min, Max, and Number or Size of Steps.</li><li><span class="UIText">CURRENT STATUS</span> - Select a defined attribute that will report the thermostat “status”, which will appear next to the slider (below for Inline; on the left for Popup).</li></ul> |

**BATTERY LEVEL** - Use when you need to display battery level to the end-user. This control is like the Slider but gets information from two attributes:

   - One attribute is a range attribute, intended to display the battery charge level as a percentage.
   - The second attribute is a Boolean attribute, used as the Is Charging indicator. A value of “true” is used to indicate active charging. This is a Read-Only control type.

   When you select this control you must define the fields described below:

   | FIELD                 | DESCRIPTION                                                  |
   | :-------------------- | :----------------------------------------------------------- |
   |                       |                                                              |
   | Attribute Option      | Select the attribute option you want to associate with this Battery Level control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+ATTRIBUTE OPTION</span> button in the right-hand pane. |
   | Control Type          | Should be predefined with Battery Level control.             |
   | View Style            | Select <span class="UIText">INLINE</span> or <span class="UIText">POPUP</span>. Inline style displays the battery charge in a rectangular horizontal display. Popup style shows the current value in a selectable circle, which expands to a rectangular vertical display on tap. Select <span class="UIText">VIEW STYLE PREVIEW</span> to see what that control will look like in the mobile app. |
   | Additional Attributes | You have the option to select an attribute (Boolean data type) that will indicate whether the device <span class="UIText">IS CHARGING</span>. If set to “true”, a charging indicator will appear in the UI below the charge value. If set to “false”, no indicator will display in the UI. |

**SWITCH** - Use when you want to give the user two mutually-exclusive choices. When you select this control you must define the fields described below:

   | FIELD            | DESCRIPTION                                                  |
   | :--------------- | :----------------------------------------------------------- |
   | Attribute Option | Select the attribute option you want to associate with this Switch control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+ATTRIBUTE OPTION</span> button in the right-hand pane. |
   | Control Type     | Should be predefined with Switch control.                    |
   | View Style       | Select <span class="UIText">INLINE</span> or <span class="UIText">POPUP</span>. Inline style displays the full switch; popup style shows the current value in a selectable circle that toggles the value on tap. Select <span class="UIText">VIEW STYLE PREVIEW</span> to see what that control will look like in the mobile app. |

**TEXT BOX** - Use the Text Box control for any of the following cases:

   - To present menu options that have labels running more than one line. (Menu control options must fit on one line.) The attribute must be Read/Write.
   - To hold a multi-line block of static text. Attribute should be Read-Only. This is useful for product or feature descriptions.
   - To hold a multi-line block of text that the end-user can edit. This is useful when, for example, you have a sign with a display message that changes. You can use the mobile app to update this message text.

When you select this control you must define the fields described below:

   | FIELD            | DESCRIPTION                                                  |
   | :--------------- | :----------------------------------------------------------- |
   | Attribute Option | Select the attribute option you want to associate with this Text Box control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+ATTRIBUTE OPTION</span> button in the right-hand pane. |
   | Control Type     | Should be predefined with Text Box control.                  |
   | View Style       | <span class="UIText">INLINE</span> is the only View Style option for the Text Box control. Select <span class="UIText">VIEW STYLE PREVIEW</span> to see what that control will look like in the mobile app. |

**3**&nbsp;&nbsp;When you are finished defining the UI controls, click SAVE in the upper-right corner of the window.

## Define the UI Control Groups

You are now ready to take your UI controls and decide how you want to present them to the end-user in the mobile app. You’ll do that in the UI Control Groups window; it simulates what the end-user will see in the mobile app, and provides some editing tools for you to use.

UI control “groups” are simply organizational devices, like folders, for logically grouping and presenting your controls to the end-user. If your device were a humidifier, you could group Temp and Humidity controls in a Climate group. The end-user could then easily find all their climate controls in a single group, improving ease-of use.

Every UI control must belong to at least one UI control group; orphan controls are not displayed in the mobile app UI. This means that even if you’re planning to use a single UI control, you must create a group and place your loner in it.

Read through and follow the steps below:

**1**&nbsp;&nbsp;Click <span class="UIText">UI CONTROL GROUPS</span> in the left-hand Navigation pane to open the UI Control Groups definition window. We’ll look the example below before starting.

The left-hand pane, <span class="UIText">DEFINE THE UI CONTROL GROUPS</span>, displays a representation of the device mobile app UI (both shown below). At the top of both the mobile app and the Profile Editor is the “groups ribbon”; below are the controls for the selected group. Note that device Settings and Automation are selections only shown in the mobile app groups ribbon, not in the Profile Editor.

You’ll see two groups have been defined: **LED** and **Button**. These are shown at the top in the “groups ribbon”. The **LED** group is centered and highlighted, which means it’s “active” and ready for editing. Below the groups ribbon you’ll see that the **LED** control has been added to the **LED** group. In the right-hand pane, <span class="UIText">AVAILABLE UI CONTROLS</span>, the **Button** control is shown as still “available” to the **LED** group.<br>
<img src="../img/APE-ControlGroups.png" width="800" style="vertical-align:middle;margin:0px 0px;border:none">

**2**&nbsp;&nbsp;You’ll define your groups in the groups ribbon:

 ***2a***&nbsp;&nbsp;To **add** a group, click<img src="../img/AddGroupIcon.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none">.
   
 ***2b***&nbsp;&nbsp;To **edit** a group name, click the name to make the group active and editable, then make your text edits. Note that group names cannot be longer than 39 characters.
   
 ***2c***&nbsp;&nbsp;To **reposition** a group in the ribbon, click the group to make it active, then hover your cursor to the left of the group name (outlined in orange) to reveal the textured move handle<img src="../img/APE-MoveGroup.png" width="130" style="vertical-align:middle;margin:0px 0px;border:none">then drag & drop the group – left or right – to the new position.
   
***2d***&nbsp;&nbsp;To **delete** a group, click the group to make it active, then hover your cursor to the right of the group name to reveal the trash icon<img src="../img/APE-GroupWithTrashCan.png" width="130" style="vertical-align:middle;margin:0px 0px;border:none">then click the trash.

**3**&nbsp;&nbsp;Now that you’ve defined your control groups, you can organize them as you’d like them to appear to the end-user on the mobile app screen. Remember you can include a given UI control in multiple groups.

***3a***&nbsp;&nbsp;In the groups ribbon, click the group you want to work on. If you need to scroll the groups ribbon, hover your cursor over the line separating the groups ribbon from controls display and drag the horizontal scroll bar that appears under your cursor. The group you select will center and be outlined in orange. The group is active and ready to receive controls.

***3b***&nbsp;&nbsp;The controls you defined in the UI Controls window appear in the right-hand column of the window, under the heading <span class="UIText">AVAILABLE UI CONTROLS</span>. To add a control to the active group, simply drag it from the column of available controls to the controls display below the groups ribbon. Repeat for all the controls you want to add to the active group.

***3c***&nbsp;&nbsp;To reposition a control within the group, simply drag & drop it.

***3d***&nbsp;&nbsp;To remove a control from a group, drag it back to the <span class="UIText">AVAILABLE UI CONTROLS</span> pane.

**4**&nbsp;&nbsp;To quickly switch a control’s View Style or remove it from the group, hover your cursor to the right of a control that’s been added to a group, then click the pencil icon that appears. Editing tools open:<br>
<img src="../img/APE-EditControl.png" width="300" style="vertical-align:middle;margin:0px 0px;border:none">

**5**&nbsp;&nbsp;When you’re finished defining your groups, click SAVE.

## Preview the Mobile App UI

It’s possible to preview the UI as you’re creating it, right in the Afero mobile app on your own device. To do this:

**1**&nbsp;&nbsp;Click the <span class="UIText">PREVIEW UI</span> button in the upper-left of the window.

**2**&nbsp;&nbsp;Check the mobile app UI. You will see that a new device icon has been added to your devices: this is the Preview Device.

<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> The Preview Device is a virtual device. Changing controls in the Preview UI will not affect the state of any actual device.
</div>
</div>

**3**&nbsp;&nbsp; In the mobile app, tap the Preview Device to open it and see the UI that you defined. You can test the controls, see changes to the Primary Operation, Running State, and so on.

**4**&nbsp;&nbsp;If you make changes in the Profile Editor, click the <span class="UIText">PREVIEW UI</span> button again to update the mobile app UI; you don’t need to Save to see changes.

**5**&nbsp;&nbsp;You can also exercise your attributes using the Attribute Tester, which opens after you click PREVIEW UI.<br>
<img src="../img/APE-AttributeTester.png" width="300" style="vertical-align:middle;margin:0px 0px;border:none">

To use the Attribute Tester, click any group icon to reveal the associated Attribute(s), type a desired value, and click <span class="UIText">UPDATE></span>. Changes will be visible in the mobile app UI.

**6**&nbsp;&nbsp;When you’re done testing your UI, remove the Preview Device by doing this: In the mobile app, go to the Preview Device > Settings screen, then tap the Remove Device button.
