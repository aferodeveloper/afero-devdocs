# Tutorial 2: Creating a Device Profile

Before you start, we assume that you’ve already run through [Tutorial 1: Linking Modulo](../Lesson1). If not, please start there.

<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0 ">  <strong>NOTE:</strong> This tutorial is designed for the Modulo-1 and Modulo-2 dev boards, but not for the Modulo-1B.
</div>
</div>

<ol class="af-ol">
	<li>If you haven’t already, download the Afero Profile Editor, for <a id="1501883700.09" href="http://cdn.afero.io/latest-ape/win" target="_blank">Windows</a> or <a id="1501883699.99" href="http://cdn.afero.io/latest-ape/mac" target="_blank">macOS</a>. Open and sign in to the Profile Editor. The Startup window appears:<br><img src="../img/APE-StartScreen-Tut.png" width="600" alt="Startup Window"></li>
	<li>For the purposes of this tutorial, we’re going to save you some typing by providing a pre-configured Modulo project.
		<ol class="af-ol-lower-alpha">
			<li>First, you will need the appropriate Sample Project. The easiest way to get this is from the Profile Editor system <span class="UIText">Tools</span> menu. From this menu, select <span class="UIText">Sample Projects &gt; <a id="1507320504.19" href="http://github.com/aferodeveloper/APE-Project-Profiles" target="_blank">GitHub Repository</a></span>. (All Modulo projects, and more, are available from this GitHub repository.)</li>
			<li>On the GitHub web page that opens, click the green <span class="UIText">Clone or download</span> button then select <span class="UIText">Download ZIP</span>. Navigate to your Profile Editor project directory to save the zip file.</li>
			<li>Once saved, go to the Profile Editor project directory on your filesystem and double-click the file to decompress it. You can keep all the projects in the unzipped project profiles directory, or delete the projects you’re not interested in.</li>
			<li>Back in the Afero Profile Editor Startup window, select the <span class="UIText">Open</span> button, navigate to the directory where you unzipped the project, and select the directory that holds the Modulo project you want to use in this lesson.</li>
		</ol>
	</li>
	<li>Once the project is open, click <span class="UIText">Device Type</span> in the Navigation pane in the left of the window. To be sure we’re all starting in the same view, your window should look something like this: <img src="../img/Mod2_DeviceType.png"  width="600" alt="Device Type Pane">
<div class="af-callout">
	<div class="callout-text">
	<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> A control can appear in multiple groups, but <em>must</em> appear in at least one group oNotice the <span class="UIText">Module Type</span> of the Profile is specified right at the top of the pane (in the example, we’re working with a Modulo-2.) This module type was specified in the pre-built Profile you downloaded; when creating a Profile from scratch, you must specify the module type right from the beginning. Doing so is the first step in the <span class="UIText">New Profile</span> window. It’s critical that the module type in the Profile match the hardware you’re using in your project!
	</div>
</div>
<p>In the <span class="UIText">Device Type</span> view, you can set the device name, type a detailed device description, and set the device icon. Of these items, only the icon will be visible to the end-user. Notice the <span class="UIText">Preview</span> panel on the right of the view &ndash; it shows the selected device icon as it will appear in the mobile app.</p>
<p>Since we’re working with a pre-configured Profile, you don’t need to make any changes, but if you like, feel free to do so. When you’re finished, click <span class="UIText">Save</span> to save any changes you might have made.</p>
</li>
<li><p>Click <span class="UIText">Attributes</span> in the left-hand Navigation pane. You’ll see that the four Modulo GPIO pins are listed. Only two GPIO pins are needed for this project, pins 0 and 3. Click the On/Off switch to <span class="UIText">ON</span> for these two pins; you’ll see them highlighted in the right-hand <span class="UIText">Preview</span> pane:</p>
<img src="../img/AttributeDefinition-Tut.png" width="500" alt="Preview Pane">
</li>
<li>For this project, we’ve defined two attributes: one for GPIO 0, which is connected to the LED on the Modulo; and one for GPIO 3, which is tied to the pushbutton. We’ll take a detailed look at the attributes now. Note that selected values are shown in white letters on an orange background.
<ol class="af-ol-lower-alpha">
<li><p>Click <span class="UIText">LED Attribute</span>, the attribute name for GPIO 0, to open its detailed view:</p><img src="../img/Tut2_LED_Attr.png" width="375" alt="Attribute Definitions">
<ul class="af-ul">
<li><span class="UIText">Attribute Name</span> is a required field for all attributes. We’ve named this one <span class="UIText">LED Attribute</span> but you can change it in this field.</li>
<li>To control the LED from the mobile app, <span class="UIText">Operation Mode</span> must be <span class="UIText">Output</span>. This is because Outputs are Read/Write, and sending a command from the mobile app requires writing to an attribute.</li>
<li>GPIO 0 is given a <span class="UIText">Default Logic Level</span> of 0, which sets the default state of the LED to OFF.</li>
<li>Leave both <span class="UIText">Options</span>, <span class="UIText">PWM</span> (pulse-width modulation) and <span class="UIText">Pulse</span>,  deselected.</li>
<li>Change the <span class="UIText">Active</span> selector set to <span class="UIText">Low</span>. Because the cathode of the LED is connected to GPIO 0, the LED turns on when the pin goes <span class="UIText">Low</span>.</li>
<li>Don’t select any of the <span class="UIText">Bind to Attributes</span> buttons.</li>
</ul>
</li>
<li>Click <span class="UIText">Button Attribute</span>, the attribute name for GPIO 3, to open its detailed view:<img src="../img/Tut2_Button_Attr.png" width="400" alt="Attribute Definitions">
<ul class="af-ul">
<li><span class="UIText">Attribute Name</span> is a required field for all attributes. We’ve named this one <span class="UIText">Button Attribute</span> but you can change it here.</li>
<li>Leave GPIO3 <span class="UIText">Operation Mode</span> set to <span class="UIText">Input</span>. This will display the state of the button when pressed in the mobile app.</li>
<li>Change the <span class="UIText">Active</span> setting to <span class="UIText">Low</span>. When pressed, the button connects GPIO 3 to ground so the pin is considered active when it goes <span class="UIText">Low</span>.</li>
<li>The <span class="UIText">Pull Up</span> setting is required for to keep the GPIO pin inactive when the button is not pressed.</li>
<li>With <span class="UIText">Is Toggle</span> set, the status displayed by the mobile app will switch with every momentary button push. If <span class="UIText">Is Toggle</span> is not set, the state displayed will always reflect the current button state.</li>
<li>Leave the <span class="UIText">Debounce Time</span> set to 0, the <span class="UIText">Active</span> selector set to <span class="UIText">High</span>, and don’t select any of the <span class="UIText">Bind to Attributes</span> buttons.</li>
</ul>
</li>
<li>Click <span class="UIText">Save</span> when you are finished defining project attributes.</li>
</ol>
</li>
<li><p>To move on to defining the mobile app UI for this project, click <span class="UIText">UI Controls</span> in the left-hand Navigation pane.</p>
<p>You will probably not be surprised to find that two UI controls have been defined since our project uses two attributes. It’s common, though not a rule, to have one UI control for every attribute. As examples: some attribute values will be used “behind-the-scenes” to calculate the values of other attributes, and you can assign the same control to multiple attributes and multiple controls to the same attribute.</p>
<p>There are two steps to defining the UI controls: a) we first define the “attribute options” for each attribute; and b) we then assign UI controls to each attribute.</p>
<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> You might be wondering why defining attribute options is separate from assigning controls. The reason is so you can easily assign multiple controls to the same attribute without having to retype a lot of details (although that doesn’t apply to this Tutorial). To learn more about attribute options, read <a id="1560458198.45" href="/AppUIDef#Options">Define the Mobile App UI &gt; Define the Attribute Options</a>.</p>
</div>
</div>

<ol class="af-ol-lower-alpha">
<li>In the far-right pane under the heading <span class="UIText">Attribute Options</span>, click the pencil icon in the <span class="UIText">LED:LED Attribute</span> box to open its Attribute Option Details dialog:</p>
<img src="../img/Les2-LEDAttrOpt.png" width="450" alt="LED Attribute Options">
<p>You will see that we have defined the following:</p>
<ul class="af-ul">
<li>The label for this control will display as “LED”.</li>
<li>The LED control is selected as <span class="UIText">Primary Operation</span>, which means you can control the LED from the Home screen of the mobile app by double-tapping the device icon.</li>
<li>Note that two <span class="UIText">Value Options</span> have been defined. These present the two different states the on-board LED can have: 1 is On, 0 is Off. You can edit the labels if you want, but keep the values as defined.</li>
<li>Check the <span class="UIText">Running State</span> box next to the On state for the LED. This will change the color of the device in the mobile app when the LED is on.</li>
<li>We don’t need to complete the <span class="UIText">Range Options</span> for this attribute since we’ll be using a Menu control, which does not require range definitions.</li>
<li>Click <span class="UIText">OK</span> to close the Attribute Option Details dialog.</li>				
</ul>
<p>Now, again in the far-right pane, click the pencil icon in the <span class="UIText">Button:Button Attribute</span> box to open its Attribute Option Details dialog:</p>
<img src="../img/Les2-ButtonAttrOpt.png" width="400" alt="LED Attribute Options">
<ul class="af-ul">
<li>The label for this control will display as “Button”.</li>
<li>This attribute happens to have the same <span class="UIText">Value Options</span> as the LED attribute, but is not set as the Primary Operation.</li>
<li>Again, we don't need any <span class="UIText">Range Options</span> defined.</li>
<li>Click <span class="UIText">OK</span> to close the Attribute Option Details dialog.</li>				
</ul>
<li>
<p>Now we will assign a UI Control to each attribute. There are many types of UI Controls available, but in this project are using the <span class="UIText">Menu</span> control for both.</p>
<img src="../img/Dfn_UI_Ctls.png" width="375" alt="Menu Controls">
<p>Click the first attribute, <span class="UIText">LED Attribute (I/O 0): Menu</span>, to open it for editing.</p>
<img src="../img/AttributeDef-LED.png" width="500" alt="LED Control">
<ul class="af-ul">
<li>For <span class="UIText">attribute option</span> we’ve selected LED, and for <span class="UIText">control type</span> we’ve selected Menu.</li>
<li>For <span class="UIText">View Style</span>, we’ve selected <span class="UIText">INLINE</span>, which means all menu options will be shown. (<span class="UIText">Popup</span> View Style means the end-user must tap the control to reveal all menu options.) You can see examples by expanding <span class="UIText">View Style Preview</span> (shown in the screenshot above).</li>
</ul>
</li>
<p>Now click <span class="UIText">Button Attribute (I/O 3): Menu</span> to open it.</p>
<img class="img-basic" src="../img/AttributeDef-Button.png" width="500" alt="Button Attribute">
<p>Notice that we’ve also selected the <span class="UIText">INLINE</span> View Style.</p>
</li>
<li>Click <span class="UIText">Save</span> when you are finished defining UI controls.</li>
</ol>
</li>
<li><p>Click <span class="UIText">UI Control Groups</span> in the left-hand Navigation pane to group your controls.</p>
<p>The left-hand pane, <span class="UIText">Define the UI Control Groups</span>, displays a representation of the device mobile app UI. At the top of both the mobile app and the Profile Editor is the “groups ribbon”; below are the controls for the selected group. Note that device Settings is a selection only shown in the mobile app groups ribbon, not in the Profile Editor.</p>
<ol class="af-ol-lower-alpha">
<li><p>We’ve already set up two groups, shown in the groups ribbon and labeled <strong>LED</strong> and <strong>Button</strong>. (We’ve already created groups for this project, but if we hadn’t, you would click <img src="../img/AddGroupIcon.png" vertical-align="baseline"> to create a new group.)</p></li>
<li>On the right, you’ll see all the controls you defined on the UI Controls window, identified by their Default Label. These are the controls available for grouping.</li>
<li><p>Click the group labeled <strong>LED</strong> to make it active and ready for editing. The group will be highlighted in orange:</p>
<img src="../img/Control_Groups_LED.png" width="500" alt="UI Control Group">
<p>Below the selected group, you’ll see the <strong>LED</strong> menu control. We’ve already moved the <strong>LED</strong> menu into the <strong>LED</strong> group, but if we hadn’t, you would add a control by dragging it from the <span class="UIText">Available UI Controls</span> pane on the right into the controls display on the left, below the groups ribbon.</p></li>
<li><p>To remove a control from a group, simply drag it back to the <span class="UIText">Available UI Controls</span> pane.</p>
<div class="af-callout">
	<div class="callout-text">
	<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> A control can appear in multiple groups, but <em>must</em> appear in at least one group or it won’t appear at all in the mobile app UI. That means that even if you have just one UI control, you’ll need to create a UI control group to contain it.</p>
	</div>
</div>
</li>
<li>Click <span class="UIText">Save</span> when you are finished defining UI control groups.</li>
</ol>
</li>
<li>
<p>Now that your device Profile is complete, click <span class="UIText">Publish </span> in the left-hand Navigation pane to install it on your Modulo.</p>
<p>If you intend to perform the next steps yourself (and you should!), you’ll need to set up some hardware:</p>
<ul class="af-ul">
<li>You should have your Modulo connected to USB (for power only at this time).</li>
<li>You should have the Afero app running and open on your smartphone.</li>
</ul>
<p>Before we get started, take a look at the <span class="UIText">Publish</span> window:</p>
<ul class="af-ul">
<li>This window shows you all the developer devices on your account. For each available device, the Status displays the Bluetooth signal strength, e.g. -40dBm, in an orange badge.</li>
<li>The Device Activity log will show all attribute-related communication between device and service in real-time. It is initially populated by the latest cached values from the Cloud.</li>
</ul>
<p><strong>OK, time to Publish:</strong></p>
<ol class="af-ol-lower-alpha">
<li>Select the device(s) you want to update by selecting the corresponding orange checkbox(es).</li>
<li>Click <span class="UIText">Publish</span>.</li>
<li>The update can take up to one minute. You’ll see an update message in the Device Activity and the SW Version will change once the update has completed.</li>
</ol>
</li>
<li>Try out the new mobile app UI for your device! Notice that the LED is on, which is indicated on the Home screen. Because it’s the Primary Operation defined with a Boolean attribute, tapping the large device icon on the Home screen will toggle the LED on and off.
<img src="../img/ios_app.png" width="900" alt="Mobile App">
<p>You should now be able to control the Modulo LED using the LED buttons on your smartphone, and the pushbutton on the Modulo board should toggle the On/Off Button control in the mobile app UI.</p>
</li>
</ol>


<strong>&#8674;</strong> <em>Next:</em>&nbsp;&nbsp;[Tutorial 3: Afero + Arduino](../Lesson3)