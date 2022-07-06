# Define the Mobile App UI

Once you’ve created the Attribute definition, you’re ready to design how the end-user views and controls your device from the Afero mobile app. There are three parts to this task:

- [Define the Attribute Options](../AppUIDef#define-the-attribute-options) - We recommend you begin by providing the details needed to support *all* the UI controls for *all* of your attributes. Some of your attributes may not be suited for some of the controls, or you might have some attributes you don’t plan on exposing in a control; but in case you want to experiment and for maximum flexibility, it’s a good idea to prepare by completing the attribute option details for all your attributes.
- [Define the UI Controls](../AppUIDef#define-the-ui-controls) - In this step you assign attribute options to UI controls. Each control you use will correspond to a specific attribute, except for Battery Level and Temperature: Battery Level and Temperature are unique in that they combine information from more than one attribute. The UI control could provide only a graphic representation of the attribute state (a Read-Only attribute), or could be an active control that allows the end-user to set the attribute value (a Read/Write attribute).
- [Define the UI Control Groups](../AppUIDef#define-the-ui-control-groups) - For usability, you will organize your UI controls into one or more control groups. The name of each group is displayed in a “ribbon” above the group’s controls in the mobile app UI. Control groups are organizational constructs and provide no additional functionality to the enclosed controls.

After you’ve defined the mobile app UI but before you move on to publishing your Profile, check out your UI using a virtual “preview” device in the Afero mobile app. Read more about this feature in [Preview the Mobile App UI](../AppUIDef#preview-the-mobile-app-ui).

## Define the Attribute Options

For each of your attributes, you will define “attribute options”, which specify how a control will display the attribute’s values. Follow the instructions below:

<ol class="af-ol">
	<li><p>Click <span class="UIText">UI Controls</span> in the left-hand Navigation pane to open the UI Controls definition window.</li>
	<li>In the far-right pane, click <span class="UIText">+Attribute Option</span>. The Attribute Option Details dialog box opens:
	<img class="img-br" src="../img/APE-AttrOptDetails.png" width="500" alt="Attribute Option Details Dialog Box"></li>
	<li>Complete each field, as relevant to the targeted UI control(s), using the table below as reference: 
		<div class="af-table-borders">
			<table>
			<caption></caption>
				<thead>
					<tr>
						<th><br>Field</th>
						<th><br>Description
						<th>Relevant UI Control(s)</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>Attribute</td>
						<td>Click the <span class="UIText">Attribute</span> drop-down menu and select your first attribute. You will repeat for each of your attributes, but if you know <strong>for a fact</strong> that you won’t be displaying an attribute value in the mobile app UI, then you can skip that attribute.</td>
						<td>All Controls</td>
					</tr>								
					<tr>
						<td>Default Label </td>
						<td>This is the label that will be displayed in the mobile app UI; it is what the end-user will see. Type a label that clearly identifies the device characteristic that is being displayed or controlled.</td>
						<td>All Controls</td>
					</tr>								
	<!--			<tr>
						<td>Input Type</td>
							<td>If the user will be typing only numbers into the text box, select <span class="UIText">Numeric Keypad</span> from the drop-down menu. Otherwise, select <span class="UIText">Full Keyboard</span>, which is the default.</td>
					</tr> -->								
					<tr>
						<td>Primary Operation</td>
						<td>
							<p>On/Off checkbox, <strong>only</strong> available for write attributes that toggle between two values, and for controls that support writing to an attribute. Only one of your UI controls can be set as Primary Operation within a given device Profile.</p>
							<p>With this option <strong>selected</strong>, you’ll be able to control the attribute from the mobile app Home screen by double-tapping the device icon. Double-tapping will toggle between the attribute values.</p>
						</td>

						<td>
							<ul class="af-ul-table">
								<li>Menu</li><li>Slider</li><li>Temperature</li><li>Battery Level</li><li>Switch</li>
							</ul>
						</td> 
					</tr>
					<tr class="hide_rightlefttopbottom">
						<td colspan="3">
							<div class="af-callout-caution">
								<div class="callout-text">
									<p><img src="../img/Note.svg" width="17" style="vertical-align:bottom;padding:0">&nbsp;&nbsp;<strong>NOTE:</strong>  If you're defining <strong>both</strong> Value and Range Options (below) for the same attribute option, <strong>make the number of value options equal to the number of range steps</strong>. For example, if your Range is <em>0-50</em>, with step size <em>10</em>, make sure you have <em>6</em> Value Options defined.</p></p>
								</div>
							</div>
						</td>
					</tr>
					<tr>
						<td>Value Options</td>
						<td>
							<p>Select the <span class="UIText">Value Options</span> checkbox before entering values. Note that Switch controls <strong>require</strong> that you select this checkbox and define two value options.</p>
							<p>To define the options, type the set of  attribute values along with their corresponding labels in the table. In the mobile app, value options are often rendered as a menu of buttons, only one of which can be selected at any one time. Using the menu example, after the end-user taps any of the buttons, the associated attribute would be set to the corresponding value.</p>
							<p>If you select the <span class="UIText">Running State</span> box for a given value option, when the associated attribute has the selected value, your device will be marked as “Running” in the mobile app UI — this means the Home screen device icon will be colored orange.</p>
							<p>Note that the value options you define will appear in the UI in the order listed in the table. They won't automatically reorder in the UI on the basis of value or label. This gives you complete control over presentation, regardless of underlying implementation.</p>
	<!--					<div class="af-callout">
								<div class="callout-img">
									<i class="af-icon-checkmark-circle"></i>
								</div>
								<div class="callout-text">
									<p class="callout-br">The value options you define will appear in the UI in the order listed in the table. They won't automatically reorder in the UI on the basis of value or label. This gives you complete control over presentation, regardless of underlying implementation.</p>
									<p class="callout-br">As an example, say you are building a fan that has four speeds (including OFF). High speed is represented by attribute value 3, Medium by attribute value 2, Low by value 1, and OFF by 0. Since you control the order of presentation by the order of the value option definitions, you can make a slider control that goes from OFF to Low to Medium to High as the user slides it left-to-right; or, just as easily, a slider that goes OFF to High to Medium to Low, which is another popular configuration for physical fan controls.</p>
								</div>
								<br class="af-clear">
							</div> -->
							<p><strong>To reorder a defined value option</strong>, move your cursor to the left of the option to reveal the hand cursor and a textured handle. Click and hold the handle; without letting go, reposition the option, then let go to drop it into place.<img class="img-br" src="../img/MovingValueOptions.png" alt="Moving Value Options"></p>
						</td>
						<td><ul class="af-ul-table">
								<li>Menu</li><li>Slider</li><li>Value</li><li>Switch&nbsp;(<em>required</em>)</li><li>Text Box</li>
							</ul>
						</td>
					</tr>
					<tr>
						<td>Range&nbsp;Options</td>
						<td><p>Select the <span class="UIText">Range Options</span> checkbox before entering values.</p>
							<p>Min, Max - All values must be numbers (can use decimals with Q15_16 data type). Min must be less than Max. The Min value is both the minimum limit of the slider range and the numeric label shown on the left. Max value is the maximum of the range, on the right.</p>
							<p></p>
							<p>Number&nbsp;or Size of Steps - You must define the set points (steps) you want the control to use between Min and Max values. The way you do this depends on the underlying data type of the attribute:
							<ul class="af-ul">
								<li>If your attribute is a fixed-point integer (Q_15_16 data type), you will set the number of steps. The size of each step will be calculated for you.</li>
								<li>For all other data types, you must type the minimum granularity of the slider. For example, if you define Min as 0, Max as 1000, and Step as 2, there are 500 valid positions on the slider.</li>
							</ul></p>
							<p></p>
							<p>Unit Label - This label describes the units that the control is displaying and is limited to eight characters. The label is drawn above the control adjacent to the current value of the slider. As a Slider example, if you define the label as “Lumens”, as you drag the slider, it changes from “25 Lumens” to “40 Lumens”, and so on.</p>
						</td>
						<td rowspan="3">
							<ul class="af-ul">
								<li>Slider</li><li>Temperature</li><li>Battery Level</li>
							</ul>
						</td>
					</tr>
				</tbody>
			</table>
		</div>
	</li>
	<li><p>When you have finished defining the details for a given attribute, click <span class="UIText">OK</span> to save and dismiss the dialog box. Your saved attribute options will all be listed in the far-right column labeled <span class="UIText">Attribute Options</span>. Each attribute option is identified with its <em>Default Label:Attribute</em> in the upper-left of each pane:</p>
	<img class="img-br" src="../img/APE-AttrOptionsPane.png" width="200" alt="Attribute Options Pane"></li>
	You can take the following actions on any of your saved attribute options:</li>
	<ul class="af-ul-2">
		<li>View a summary of your attribution option details by hovering over the info icon<img class="img-inline" src="../img/APE-InfoIcon.png" alt="Info Icon">.</li>
		<li>Edit saved attribute option details by selecting the pencil icon<img class="img-inline" src="../img/PencilIconWhite.png" alt="Pencil Icon">in the relevant Attribute Option pane:
		The Attribute Option Details dialog box will open for edit.</li>					
		<li>You can delete an attribute option by selecting the trash icon<img class="img-inline" src="../img/TrashIconWhite.png" alt="Trash Icon">.</li>
	</ul>
	<li>Select <span class="UIText">+Attribute Option</span> to open a new Attribute Option Details dialog box; add the details for your next attribute and save. Repeat for each attribute.</li>
</ol>

Now that you’ve defined all your attribute options, you can move on to assigning controls for your attributes.

## Define the UI Controls

Use this window to define which attribute option(s) will be displayed using which UI controls(s).

<div class="af-callout">
	<div class="callout-text">
	<p><img src="../img/Note.svg" width="17" style="vertical-align:bottom;padding:0">&nbsp;&nbsp;<strong>NOTE:</strong>  You can assign a single attribute option to multiple UI controls. Each control assigned will use the attribute option details that are relevant to that control.</p>
	</div>
</div>

<ol class="af-ol">
	<li><p>If you haven’t defined any controls for your device yet, or to create a new one, click <span class="UIText">+ Control</span> to open the Select Control Type window. You’ll be able to select from the following controls (read below, step 2, for details).</p>
	<div class="af-table-nostyle">
		<table>
			<thead></thead>
			<tbody>
				<tr>
					<td align="center"><a id="1523472235.63" href="#MenuControl"><span class="UIText">Menu</span></a></td>
					<td></td>
					<td align="center"><a id="1523472235.73" href="#SliderControl"><span class="UIText">Slider</span></a></td>
				</tr>
				<tr>
					<td bgcolor="252525" align="center"><img class="img-inline" src="../img/MenuControl.png" alt="Menu Control" style="vertical-align:middle"></td>
					<td></td>
					<td bgcolor="252525" align="center"><img class="img-inline" src="../img/SliderControl.png" alt="Slider Control" style="vertical-align:middle"><br>
					</td>
				</tr>
				<tr>
					<td align="center"><br><a id="1523472235.83" href="#ValueControl"><span class="UIText">Value</span></a></td>
					<td></td>
					<td align="center"><br><a id="1523472235.93" href="#TempControl"><span class="UIText">Temperature</span></a></td>
				<tr>
					<td bgcolor="252525" align="center"><img class="img-inline" src="../img/ValueControl.png" alt="Value Control" style="vertical-align:middle"></td>
					<td></td>
					<td bgcolor="252525" align="center"><img class="img-inline" src="../img/TempControl.png" alt="Temperature Control" style="vertical-align:middle"></td>
				</tr>
				<tr>
					<td align="center"><br><a id="1523472236.03" href="#BatteryControl"><span class="UIText">Battery Level</span></a></td>
					<td></td>
					<td align="center"><br><a id="1523472236.13" href="#SwitchControl"><span class="UIText">Switch</span></a></td>
				</tr>
				<tr>
					<td bgcolor="252525" style="text-align:center"><img class="img-inline" src="../img/BatteryLevelControl.png" alt="Battery Level Control" style="vertical-align:middle"></td>
					<td></td>
					<td bgcolor="252525" align="center"><img class="img-inline" src="../img/SwitchControl.png" alt="Switch Control" style="vertical-align:middle"></td>
				</tr>
				<tr>
					<td colspan="3" align="center"><br><a id="1523472236.23" href="#TextBoxControl"><span class="UIText">Text Box</span></a></td>
				</tr>
				<tr>
					<td colspan="3" align="center"><img class="img-inline" src="../img/TextBoxControl.png" width="275" alt="Text Box Control"></td>
				</tr>
			</tbody>
		</table>
	</div></li>
	<li><p>Use the following guidelines when selecting and defining a control. Note that if you have not defined attribute options for a given attribute, that attribute won’t be available in the <span class="UIText">Attribute Option</span> drop-down menu. You must do that by selecting the <span class="UIText">+Attribute Option</span> button in the right-hand pane. Instructions for completing those fields are described above in <a id="1557952685.46" href="#Options">Define the Attribute Options</a>.</p>
		<p id="MenuControl"><span class="UITextLarge">Menu</span> - Use when the function has a small number of discrete value states and when each value state is short (can only be one line of text). When you select this control you must define the fields described below:</p>
		<div class="af-table-borders">
			<table>
				<thead>
					<tr>
						<th>Field</th>
						<th>Description</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>Attribute Option</td>
						<td>Select the attribute option you want to associate with this Menu control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+Attribute Option</span> button in the right-hand pane.</td>
					</tr>
					<tr>
						<td>Control Type </td>
						<td><p>Should be predefined with Menu control.</p></td>
					</tr>
					<tr>
						<td>View Style</td>
						<td><p>Select <span class="UIText">Inline</span> or <span class="UIText">Popup</span>. Inline style exposes each menu item as a selectable button. Popup style shows the current value in a selectable circle, which expands to the full menu on tap. Select <span class="UIText">View Style Preview</span> to see what that control will look like in the mobile app.</p></td>
					</tr>
				</tbody>
			</table>
		</div>

		<p>&nbsp;</p>
		<p id="SliderControl"><span class="UITextLarge">Slider</span> - Use when the attribute supports a range of values. The attribute associated with a Slider must have a data type with sufficient range to support the full range of values and will need to be defined as Output (GPIO) or Writeable (MCU).</p>
		<p>When you select this control you must define the fields described below:</p>
		<div class="af-table-borders">
			<table>
				<thead>
					<tr>
						<th>Field</th>
						<th>Description</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>Attribute Option</td>
						<td>Select the attribute option you want to associate with this Slider control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+Attribute Option</span> button in the right-hand pane.</td>
					</tr>
					<tr>
						<td>Control Type </td>
						<td><p>Should be predefined with Slider control.</p></td>
					</tr>
					<tr>
						<td>View Style</td>
						<td>Select <span class="UIText">Inline</span> or <span class="UIText">Popup</span>. Inline style displays the full slider. Popup style shows the current value in a selectable circle, which expands to a full vertically-oriented slider on tap. Select <span class="UIText">View Style Preview</span> to see what that control will look like in the mobile app.</p></td>
					</tr>
				</tbody>
			</table>
		</div>
		<p>&nbsp;</p>
		<p id="ValueControl"><span class="UITextLarge">Value</span> - Use when you need to display status, as text. You will probably associate a Value control with an MCU attribute that has a data type of UTF8, which would allow your MCU to set the Value control display dynamically. If the attribute is read/write, the user will be able to change the value on tap.</p>
		<p>When you select this control you must define the fields described below:</p>
		<div class="af-table-borders">
			<table>
				<thead>
					<tr>
						<th>Field</th>
						<th>Description</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>Attribute Option</td>
						<td>Select the attribute option you want to associate with this Value control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+Attribute Option</span> button in the right-hand pane.</td>
					</tr>
					<tr>
						<td>Control Type </td>
						<td><p>Should be predefined with Value control.</p></td>
					</tr>
					<tr>
						<td>View Style</td>
						<td><p>Select <span class="UIText">Inline</span> or <span class="UIText">Popup</span>. Inline style displays the current value in a rectangle. Popup style shows the current value in a selectable circle. When tapped, both view styles can allow a selection by the end-user. Select <span class="UIText">View Style Preview</span> to see what that control will look like in the mobile app.</p></td>
					</tr>
				</tbody>
			</table>
		</div>
		<p>&nbsp;</p>
		<p id="TempControl"><span class="UITextLarge">Temperature</span> - Use when the function specifically requires the user set a temperature level. This control uses up to three attributes:</p>
		<ul class="af-ul">
			<li>(Required) The first attribute is read/write. It represents the value of a thermostat set point, or Target Temperature, which the user will set using the specialized slider.</li>
			<li>(Optional) The second represents the Current Temperature, so is a read-only value displayed on a scale, which you define in Celsius degrees.</li>
			<li>(Optional) The last attribute represents the Current Status; it is read-only and can be used to display the status of a heating/cooling device, such information as “Heating” or “Cooling”. Current status can also display a numeric value.</li>
		</ul>
		<p>When you select this control you must define the fields described below:</p>
		<div class="af-table-borders">
			<table>
				<thead>
					<tr>
						<th>Field</th>
						<th>Description</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>Attribute Option</td>
						<td>Select the attribute option you want to associate with this Temperature control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+Attribute Option</span> button in the right-hand pane.</td>
					</tr>
					<tr>
						<td>Control Type </td>
						<td><p>Should be predefined with Temperature control.</p></td>
					</tr>
					<tr>
						<td>View Style</td>
						<td>Select <span class="UIText">Inline</span> or <span class="UIText">Popup</span>. Inline style displays the full slider. Popup style shows the current value in a selectable circle, which expands to a full vertically-oriented slider on tap.  Select <span class="UIText">View Style Preview</span> to see what that control will look like in the mobile app.</p></td>
					</tr>
					<tr>
						<td>Additional Attributes</td>
						<td>You can optionally set two more attributes if you want to show the end-user the current temperature and a heating “status” (as a number or a string):
							<ul class="af-ul">
								<li><span class="UIText">Current Temperature</span> - Select a defined attribute that will report the current temperature, then in the Attribute Options for this attribute, define the <span class="UIText">Current Temp Range</span>: Min, Max, and Number or Size of Steps.</li>
								<li><span class="UIText">Current Status</span> - Select a defined attribute that will report the thermostat “status”, which will appear next to the slider (below for Inline; on the left for Popup).</li>
							</ul>
						</td>
					</tr>
				</tbody>
			</table>
		</div>

		<p>&nbsp;</p>
		<p id="BatteryControl"><span class="UITextLarge">Battery Level</span> - Use when you need to display battery level to the end-user. This control is like the Slider but gets information from two attributes:</p>
		<ul class="af-ul">
			<li>One attribute is a range attribute, intended to display the battery charge level as a percentage.</li>
			<li>The second attribute is a Boolean attribute, used as the Is Charging indicator. A value of “true” is used to indicate active charging. This is a Read-Only control type.</li>
		</ul>
		<p>When you select this control you must define the fields described below:</p>
		<div class="af-table-borders">
			<table>
				<thead>
					<tr>
						<th>Field</th>
						<th>Description</th>
					</tr>
				</thead>
				<tbody>
					<tr>
					<tr>
						<td>Attribute Option</td>
						<td>Select the attribute option you want to associate with this Battery Level control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+Attribute Option</span> button in the right-hand pane.</td>
					</tr>
					<tr>
						<td>Control Type </td>
						<td>Should be predefined with Battery Level control.</td>
					</tr>
						<td>View Style</td>
						<td>Select <span class="UIText">Inline</span> or <span class="UIText">Popup</span>. Inline style displays the battery charge in a rectangular horizontal display. Popup style shows the current value in a selectable circle, which expands to a rectangular vertical display on tap. Select <span class="UIText">View Style Preview</span> to see what that control will look like in the mobile app.</p></td>
					</tr>
					<tr>
						<td>Additional Attributes</td>
						<td>You have the option to select an attribute (Boolean data type) that will indicate whether the device <span class="UIText">Is Charging</span>. If set to “true”, a charging indicator will appear in the UI below the charge value. If set to “false”, no indicator will display in the UI.</td>
					</tr>
				</tbody>
			</table>
		</div>

		<p>&nbsp;</p>
		<p id="SwitchControl"><span class="UITextLarge">Switch</span> - Use when you want to give the user two mutually-exclusive choices. When you select this control you must define the fields described below:</p>
		<div class="af-table-borders">
			<table>
				<thead>
					<tr>
						<th>Field</th>
						<th>Description</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>Attribute Option</td>
						<td>Select the attribute option you want to associate with this Switch control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+Attribute Option</span> button in the right-hand pane.</td>
					</tr>
					<tr>
						<td>Control Type </td>
						<td>Should be predefined with Switch control.</td>
					</tr>
					<tr>
						<td>View Style</td>
						<td><p>Select <span class="UIText">Inline</span> or <span class="UIText">Popup</span>. Inline style displays the full switch; popup style shows the current value in a selectable circle that toggles the value on tap.  Select <span class="UIText">View Style Preview</span> to see what that control will look like in the mobile app.</p></td>
					</tr>
				</tbody>
			</table>
		</div>

		<p>&nbsp;</p>
		<p id="TextBoxControl"><span class="UITextLarge">Text Box</span> - Use the Text Box control for any of the following cases:</p>
			<ul class="af-ul">
				<li>To present menu options that have labels running more than one line. (Menu control options must fit on one line.) The attribute must be Read/Write.</li>
				<li>To hold a multi-line block of static text. Attribute should be Read-Only. This is useful for product or feature descriptions.</li>
				<li>To hold a multi-line block of text that the end-user can edit. This is useful when, for example, you have a sign with a display message that changes. You can use the mobile app to update this message text.</li>
			</ul>
		</p>When you select this control you must define the fields described below:</p>
		<div class="af-table-borders">
			<table>
				<thead>
					<tr>
						<th>Field</th>
						<th>Description</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>Attribute Option</td>
						<td>Select the attribute option you want to associate with this Text Box control. If it doesn’t appear in the drop-down menu, create a new one by selecting the <span class="UIText">+Attribute Option</span> button in the right-hand pane.</td>
					</tr>
					<tr>
						<td>Control Type </td>
						<td>Should be predefined with Text Box control.</td>
					</tr>
					<tr>
						<td>View Style</td>
						<td><span class="UIText">Inline</span> is the only View Style option for the Text Box control. Select <span class="UIText">View Style Preview</span> to see what that control will look like in the mobile app.</p></td>
					</tr>
				</tbody>
			</table>
		</div>

	</li>
	<li>When you are finished defining the UI controls, click <span class="UIText">Save</span> in the upper-right corner of the window.</li>
</ol>


## Define the UI Control Groups

You are now ready to take your UI controls and decide how you want to present them to the end-user in the mobile app. You’ll do that in the UI Control Groups window; it simulates what the end-user will see in the mobile app, and provides some editing tools for you to use.

UI control “groups” are simply organizational devices, like folders, for logically grouping and presenting your controls to the end-user. If your device were a humidifier, you could group Temp and Humidity controls in a Climate group. The end-user could then easily find all their climate controls in a single group, improving ease-of use.

<div class="af-callout">
	<div class="callout-text">
	<p><img src="../img/Note.svg" width="17" style="vertical-align:bottom;padding:0">&nbsp;&nbsp;<strong>NOTE:</strong>  Every UI control must belong to at least one UI control group; orphan controls are not displayed in the mobile app UI. This means that even if you’re planning to use a single UI control, you must create a group and place your loner in it.</p>
	</div>
</div>

Read through and follow the steps below:

<ol class="af-ol">
	<li>
		<p>Click <span class="UIText">UI Control Groups</span> in the left-hand Navigation pane to open the UI Control Groups definition window. We’ll look the example below before starting.</p>
		<p>The left-hand pane, <span class="UIText">Define the UI Control Groups</span>, displays a representation of the device mobile app UI (both shown below). At the top of both the mobile app and the Profile Editor is the “groups ribbon”; below are the controls for the selected group. Note that device Settings and Automation are selections only shown in the mobile app groups ribbon, not in the Profile Editor.</p>
		<p>You’ll see two groups have been defined: <strong>LED</strong> and <strong>Button</strong>. These are shown at the top in the “groups ribbon”. The <strong>LED</strong> group is centered and highlighted, which means it’s “active” and ready for editing. Below the groups ribbon you’ll see that the <strong>LED</strong> control has been added to the <strong>LED</strong> group. In the right-hand pane, <span class="UIText">Available UI Controls</span>, the <strong>Button</strong> control is shown as still “available” to the <strong>LED</strong> group.<img class="img-br" src="../img/APE-ControlGroups.png" alt="Define the UI Control Groups"></p>
	</li>
	<li>You’ll define your groups in the groups ribbon:
		<ol class="af-ol-lower-alpha">
			<li>To <strong>add</strong> a group, click<img class="img-inline" src="../img/AddGroupIcon.png" style="vertical-align:middle">.</li>
			<li>To <strong>edit</strong> a group name, click the name to make the group active and editable, then make your text edits. Note that group names cannot be longer than 39 characters.</li>
			<li>To <strong>reposition</strong> a group in the ribbon, click the group to make it active, then hover your cursor to the left of the group name (outlined in orange) to reveal the textured move handle<img class="img-inline" src="../img/APE-MoveGroup.png" style="vertical-align:middle" width="100">then drag &amp; drop the group &ndash; left or right &ndash; to the new position.</li>
			<li>To <strong>delete</strong> a group, click the group to make it active, then hover your cursor to the right of the group name to reveal the trash icon<img class="img-inline" src="../img/APE-GroupWithTrashCan.png" style="vertical-align:middle" width="100">then click the trash.</li>
		</ol>
	</li>
	<li>Now that you’ve defined your control groups, you can organize them as you’d like them to appear to the end-user on the mobile app screen. Remember you can include a given UI control in multiple groups.</p>
		<ol class="af-ol-lower-alpha">
			<li>In the groups ribbon, click the group you want to work on. If you need to scroll the groups ribbon, hover your cursor over the line separating the groups ribbon from controls display and drag the horizontal scroll bar that appears under your cursor. The group you select will center and be outlined in orange. The group is active and ready to receive controls. 
			</li>
			<li>The controls you defined in the UI Controls window appear in the right-hand column of the window, under the heading <span class="UIText">Available UI Controls</span>. To add a control to the active group, simply drag it from the column of available controls to the controls display below the groups ribbon. Repeat for all the controls you want to add to the active group.</li>
			<li>To reposition a control within the group, simply drag &amp; drop it.</li>
			<li>To remove a control from a group, drag it back to the <span class="UIText">Available UI Controls</span> pane.</li>
		</ol>
	</li>
	<li><p>To quickly switch a control’s View Style or remove it from the group, hover your cursor to the right of a control that’s been added to a group, then click the pencil icon that appears. Editing tools open:</p>
	<img class="img-br" src="../img/APE-EditControl.png" style="vertical-align:baseline">
	</li>
	<li>When you’re finished defining your groups, click <span class="UIText">Save</span>.</li>
</ol>

## Preview the Mobile App UI

It’s possible to preview the UI as you’re creating it, right in the Afero mobile app on your own device. To do this:

<ol class="af-ol">
	<li>Click the <span class="UIText">Preview UI</span> button in the upper-left of the window.</li>
	<li>Check the mobile app UI. You will see that a new device icon has been added to your devices: this is the Preview Device.</li>
		<div class="af-callout">
			<div class="callout-text">
			<p><img src="../img/Note.svg" width="17" style="vertical-align:bottom;padding:0">&nbsp;&nbsp;<strong>NOTE:</strong>  The Preview Device is a virtual device. Changing controls in the Preview UI will not affect the state of any actual device.</p>
			</div>
		</div>

	<li>In the mobile app, tap the Preview Device to open it and see the UI that you defined. You can test the controls, see changes to the Primary Operation, Running State, and so on.</li>
	<li>If you make changes in the Profile Editor, click the <span class="UIText">Preview UI</span> button again to update the mobile app UI; you don’t need to Save to see changes.</li>
	<li>
		<p>You can also exercise your attributes using the Attribute Tester, which opens after you click <span class="UIText">Preview UI</span>.</p>
		<img class="img-br" src="../img/APE-AttributeTester.png" width="400" alt="Using the Attribute Tester"></p>
		<p>To use the Attribute Tester, click any group icon to reveal the associated Attribute(s), type a desired value, and click <span class="UIText">Update</span>. Changes will be visible in the mobile app UI.</p>
	</li>
	<li>When you’re done testing your UI, remove the Preview Device by doing this:  In the mobile app, go to the Preview Device &gt; Settings screen, then tap the Remove Device button.</li>
</ol>
