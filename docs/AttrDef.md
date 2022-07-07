# Define the Device Attributes

Now is the time to describe your device in terms of attributes and values. The Attribute Definition window is where you will do this. Instructions on using this window are presented in the following sections:

- [What Is an Attribute?](../AttrDef#what-is-an-attribute)
- [Does Your Device Include a Microcontroller (MCU)?](../AttrDef#does-your-device-include-a-microcontroller-mcu)
- [Define the GPIO Attributes](../AttrDef#define-the-gpio-attributes)
- [Define the MCU Attributes](../AttrDef#define-the-mcu-attributes)

Before we start editing, take a minute to think about how your device’s functions map to attributes and values.

## What Is an Attribute?

Think of an attribute as a variable shared between your IoT device and the end-user’s Afero mobile app. Your device sets that variable/attribute in ASR to a value reflecting device state. That value is communicated to the Afero Cloud (for ASR-1 via either a standalone hub or “softhub” built into the Afero mobile app), and from there to the end-user’s mobile app. This causes the app UI to update, reflecting the current value of the attribute. Conversely, the end-user can manipulate the mobile app UI to cause a change in the value of an attribute: the new value is communicated to ASR in your device via the Afero Cloud then hub, and a device action is triggered by that attribute value.

### Examples

A simple control used by many devices is **Power**. The values, or possible settings, are **On** and **Off**. When the device is turned Off, that state is reflected in a dedicated Power attribute. The attribute Off value is sent to the Afero Cloud via a hub, and from there to the Afero mobile app on the end-user’s smartphone. The device representation on the mobile app would most likely include a Power switch control, which would then be switched to Off. If the end-user then set the switch to On, the updated attribute value would be sent back the other way: app → Afero Cloud → hub (for ASR-1) → ASR, and the device would be turned on.

Not all attributes are Boolean; a more sophisticated device might use an attribute to represent Cleaning Mode; the settings (values) could be Spot Clean, Scheduled Clean, and Full Clean. For more about attribute modeling, read [Great Attribute Modeling](../AttrModel).

### Attribute Sizing

Your module has a defined amount of memory reserved for both system and developer-defined attributes – your Profile cannot exceed this limit. Each module has a different amount of RAM reserved, so as you add attributes to your Profile, select the <span class="UIText">PROFILE RAM USAGE</span> indicator at the top of the Attributes window to see remaining space:

<img src="../img/Attribute-RAM-Gauge.png" width="300" style="vertical-align:middle;margin:0px 0px;border:none">

The <span class="UIText">GREY BAR</span> indicates space used by system attributes. The <span class="UIText">BLUE BAR</span> indicates space used by developer-defined attributes. Hover over the indicator bar to view actual remaining space, in bytes and percentage. For more details, read [Device Attribute RAM Size](../AttributeRAM).

### More Attribute Facts
<ul class="af-ul">
<li>Attribute history is stored in the Afero Cloud (30 days by default).</li>
<li>Attribute changes can be tracked and graphed across one-to-n devices.</li>
<li>Afero Inspector and Afero Console are developer tools available for viewing attribute changes and running analytics.</li>
<li>Attributes are accessible via a REST API.</li>
<li>Afero can pull data from your cloud to push to device attributes.</li>
<li>Afero can push attribute data back to your cloud.</li>
</ul>

<!-- <h2>Is Your Device a Potenco?</h2>
<p>If you are working on a Profile for a Potenco, you can define attributes; but because there is no MCU and no GPIOs, the Attribute window will be different from other modules. Please read <a id="1508538521.67" href="#HubAttributes">Define the Potenco Attributes</a> for more information.</p> -->

## Does Your Device Include a Microcontroller (MCU)?
<p>There are two types of devices that could use ASR:</p>
<ul class="af-ul">
<li>
<p><strong>Devices without an MCU</strong> - A relatively simple device may not have a microcontroller, but still have sensors or controls. For this type of application, ASR exposes four GPIO pins that can be connected to a device that does not need to perform any processing beyond communicating attribute state with Afero. These four attributes are referred to as <span class="UIText">GPIO attributes</span>.</p>
<p>GPIO attributes are, of course, tied directly to GPIO pin state, and therefore are defined by characteristics you would expect from pin interfaces: they can be Input or Output, you can activate internal pull-up or pull-down resistors, etc.</p>
</li>
<li>
<p><strong>Devices with an MCU</strong> - A more complicated device might include an MCU, and so might perform logic involving attribute values communicated with Afero. For this purpose, ASR allows you to define 1024 attributes that are not directly tied to the state of a pin on ASR. These are referred to as <span class="UIText">MCU Attributes</span>.</p>
<p>MCU attributes can be Read-Only or Read/Write, and you can specify the data type for each. (Read <a id="004-Otq90j3" href="/AttrDef#define-the-mcu-attributes">Define the MCU Attributes</a> for details on data types.)</p>
<p>Two MCU serial protocols are supported:</p>
<ul class="af-ul-2">
<li>Serial Peripheral Interface bus (SPI)</li>
<li>Universal Asynchronous Receiver/Transmitter (UART)</li>
</ul>
</li>
</ul>
<p>To learn how to set up communication between ASR and your MCU, please start by reading <a id="1521219335.91" href="/MCUtoHachi">MCU to ASR Communication.</a></p>
<p>You can update the firmware on your MCU by using the Afero OTA Manager in conjunction with the Profile Editor. Read more about this below in <a id="1527203501.08" href="/AttrDef#define-the-mcu-attributes">Define the MCU Attributes (Step 4)</a> and in the <a id="1527203500.98" href="/OTAMgr">Afero OTA Manager User Guide</a>.</p>

## Define the GPIO Attributes
<p>We’ll start by defining the GPIO attributes. Note that not all modules have GPIO pins.</p>
<ol class="af-ol">
<li><p>Click <span class="UIText">ATTRIBUTES</span> in the Navigation pane to open the Attribute Definition window. (We’ll talk about the <span class="UIText">MCU PROTOCOL</span> drop-down menu when we <a href="../AttrDef#define-the-mcu-attributes">define MCU attributes</a>, later.) By default the GPIO attributes appear, ready to be defined:</p>
<img src="../img/AttributeDefinition.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">
</li>
<li><p>Click an attribute’s <span class="UIText">ON/OFF</span> toggle switch to turn it ON and open the attribute’s definition editor. Depending on the Operation Mode you select for your GPIO pin (<span class="UIText">INPUT</span> vs. <span class="UIText">OUTPUT</span>), different options will appear:</p>
<ul class="af-ul">
<li><strong>GPIO Input Options</strong><br><img src="../img/GPIO-Attributes-Input.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none"></li>
<li><strong>GPIO Output Options</strong><br><img src="../img/GPIO-Attributes-Output.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none"></li>
</ul>
<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> If you turn an attribute <span class="UIText">OFF</span>, you will lose its current definition. To retain your attribute definition but close that attribute’s editor, click the name of the attribute shown in orange at the top-left of the editor. Save your work at any time by clicking the SAVE button in the upper-right of the window.
</div>
</div>
</li>
<li>Complete the fields using the information in the table below:
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
<td>Attribute Name</td>
<td>Type a name that will help you identify this attribute in other contexts. This name will never be visible to the end-user.</td>
</tr>
<tr>
<td rowspan="2">Operation Mode</td>
<td><p><span class="UIText">Input</span> - Inputs are read-only. <!--<span class="UIText">Is Toggle</span> and <span class="UIText">Comparator</span> are mutually exclusive. The <span class="UIText">Comparator</span> option is <strong>not</strong> available for the Modulo-2.--></p>
<ul class="af-ul">
<li><p><span class="UIText">Pull Up</span> or <span class="UIText">Pull Down</span> - Select one of these options (they are mutually exclusive) to specify the inclusion of a pull-up or pull-down resistor on the pin, or select neither for <span class="UIText">No Pull</span>. Any digital input <!-- (i.e., not ADC or Comparator) -->can be configured as Pull Up, Pull Down, or No Pull.</p>
<p><strong>Note:</strong> On Modulo boards, GPIO 0 is connected to an LED in series with a resistor. When the pin is pulled low the LED turns on; when floating or high the LED turns off. If you enable <span class="UIText">Pull Down</span> on this pin the LED partly turns on and the pin stays between logic levels and does not properly go <span class="UIText">LOW</span>. This does not cause functionality problems but will show up with a voltmeter. So on this pin only, <span class="UIText">Pull Down</span> will not work properly unless the LED hardware is manually disconnected, as a voltage divider circuit will be formed.</p></li>

<li><span class="UIText">Is Toggle</span> - This is a special case for momentary-contact buttons. When <span class="UIText">Is Toggle</span> is selected, a pulse on the input line (i.e., transition from one state to another and then back again) will cause the attribute value to toggle between 0 and 1. This lets you implement a push button.</li>

<li><span class="UIText">Debounce Time</span> - This field allows you to apply a debounce interval (measured in milliseconds) to the associated input. A typical use case would be to ensure that a single press on a noisy pushbutton is interpreted as a single event. Specify 0 to disable the option; the allowed range of debounce intervals is 30-10000 ms.</li>

<!-- <li><span class="UIText">Comparator</span> - (Does not apply to the Modulo-2.) You can configure only one GPIO as a comparator; select from GPIOs 0-2. Once you’ve specified the GPIO to be used as a comparator, select a <span class="UIText">Reference Voltage</span> value from the drop-down menu. Reference Voltage can be a fraction of V<sub>CC</sub>, or the voltage on GPIO3. An attribute set as a comparator will go high if the voltage applied to the associated GPIO pin exceeds your specified Reference Voltage.</p></li> -->

<li><p><span class="UIText">Count</span> - The Count option is only available for Input attributes and does not work with the Is Toggle option.</p>
<p>When you specify an input as <span class="UIText">Count</span>, the attribute value becomes a counter and increments every time the input pin is pulsed. Unlike other GPIO input attribute values, it is <strong>writeable</strong>, which allows you to reset the value as well as start from a value other than 0. Count attributes should accurately count pulses up to a rate of about 5Hz. </p></li>

<li id="Latching"><span class="UIText">Latch</span> - Latching is currently only available for Input attributes. Latching causes a change in an attribute value to be held until it can be successfully transmitted to the Afero Cloud. This is useful for applications like door or leak sensors where you want to make sure the user is notified of a change even if the state goes back to normal. In these cases the value starts at 0. If an event occurs, the value goes to 1 and that value will be held until a hub can connect. Once the connection is made, the peripheral will send the latched value followed by the current value. If you have a rule to notify the user when an event occurs, that rule will fire when the latched value is received even if the current value is 0.</p></li>
</ul>
</td>
</tr>
<tr>
<td><p><span class="UIText">Output</span> - Outputs are both read and write.</p>
<ul class="af-ul">
<li><span class="UIText">Default Logic Level</span> - Appears if neither PWM nor Pulse is  selected. This option represents the binary pin-level state, and should be either 0 or 1. Remember that every time a device loses power and restarts, default values are restored, so think carefully about use cases and how this could affect the end-user.</li>
<li><span class="UIText">PWM</span> - You can have a maximum of two pins configured as PWMs. Select from any of the GPIOs (0 - 3). For this option, you must specify a few parameters:
<ul class="af-ul-2">
<li> <span class="UIText">PWM Frequency</span> - Integer value for frequency in Hz, based on the module you’re using:</li>
<ul class="af-ul-3">
<li>Modulo-1: 1 to 1,000,000</li>
<li>All others: 12 to 100,000</li>
</ul>
<li> <span class="UIText">Default Duty Cycle</span> - Expressed as a percent, which should be an integer from 0 to 100. Again, every time the device is reset, this default value is restored.</li>
</ul>
</li>
<li><p><span class="UIText">Pulse</span> - You can have a maximum of two GPIO pins configured to pulse, and they cannot be bound to other attributes. Note also that if you have an MCU enabled, you will see a variable 30-millisecond delay (jitter) around your pulses. This is because the ASR CPU will be busy with the MCU request while trying to pulse. For details, read <a id="004-n6VMnHR" href="/AttrDef#more-about-pulsing">More About Pulsing</a>.</p>
<p>To define the pulse behavior, you must specify a few parameters:</p>
<ul class="af-ul-2">
<li><span class="UIText">Pulse Active Time</span> - The pulse width, in milliseconds. Minimum pulse width: 50 ms; maximum: 1000 ms.</li>
<li><span class="UIText">Pulse Inactive Time</span> - The time, in milliseconds, between the end of one pulse and the start of the next. Minimum time: 50 ms; maximum: 1000 ms.</li>
<li><span class="UIText">Default Value</span> - For a pulse attribute, the default value field plays a special role: it represents the number of pulses emitted by the pin <strong>at boot</strong>. It is important to recognize that setting a non-zero default value for a pulse is a special case, indeed one that may only rarely be used. If you set the default value to 10, for example, the pin will pulse 10 times when ASR boots, and then halt, and the default value will have no effect on pin behavior until the next boot. For most cases, the <strong>attribute value</strong> of a pulse attribute is more important than the <strong>default value</strong>. Read more directly below.</li>
<li><span class="UIText">Attribute Value</span> - The value assigned to this attribute indicates the count of pulses that should be emitted by the pin. When you set the attribute value &ndash; either directly, using one of the <code>af_lib_set_attribute*()</code> calls, or via a UI interaction &ndash; the pin will begin pulsing immediately. The attribute value will count down from the initial setting to zero as the pulsing occurs, and pulsing stops when the attribute value reaches zero. To initiate pulsing again, you may set the attribute to another non-zero value.</li>
</ul>
</li>
</ul>
</td>
</tr>
<tr>
<td>Active</td>
<td>By default, an attribute value of 1 sets the I/O voltage to <span class="UIText">High</span>. To invert that setting so a value of 1 sets the voltage to <span class="UIText">Low</span>, select that option. Active Low can be set for all GPIOs that are configured as Input or Output, but are not configured as Toggle (for Input) or PWM (for Output).</td>
</tr>
<tr>
<td>Bind to Attributes</td>
<td>
<p>Use binding to have changes from one attribute propagate to another attribute locally, without having to go through the Afero Cloud. For details, read <a id="004-xCOc0bf" href="/AttrDef#more-about-binding">More About Binding</a>.</p>
<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="17" style="vertical-align:bottom;padding:0">&nbsp;&nbsp;<strong>NOTE:</strong> When you bind attributes together, if one of the bound attributes has the <span class="UIText">Store in Flash</span> option selected, then all the bound attributes must have the <span class="UIText">Store in Flash</span> option selected.</p>
</div>
</div>
</td>
</tr>
<tr>
<td>Store in Flash</td>
<td>
<p>This option is available to:</p>
<ul class="af-ul">
<li><strong>Output</strong> attributes <strong>not</strong> set to Pulse (see note below), and</li>
<li><strong>Input</strong> attributes that <strong>are</strong> set to <span class="UIText">IsToggle</span>.</li>
</ul>
<p>Selecting this checkbox ensures that an attribute value persists in flash memory if power is interrupted. A good example of this is when a powered-on plug loses, then regains, power. When power is restored, you want the plug to return to the “on” state, the same state it was in prior to losing power.</p>
<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="17" style="vertical-align:bottom;padding:0">&nbsp;&nbsp;<strong>NOTE:</strong>  There is a reason <span class="UIText">Store in Flash</span> is not available to Output attributes set to <span class="UIText">Pulse</span>. If a pulsing attribute values were stored in flash, the value would be written with every pulse, and that could result in exhausting flash lifetime earlier than expected.</p>
</div>
</div>
</td>
</tr>
<tr id="UseTimestamp">
<td>Use ASR Timestamps</td>
<td>
<p>This option is available to all attributes, <strong>but only on Modulo-1B, Modulo-2C, and Modulo-2D</strong>.</p>
<p>Select this option if you want to report the actual time an attribute was updated, whether the device was offline or online at the time of update.</p>
<p>Note that if you want to track the history of updates and not just the last time something happened, you can store the timestamps in a queue using a specified logic that’s used for reporting. You configure the queue by selecting a “queuing policy” in the drop-down menu described below.</p>
</td>
</tr>
<tr id="QueueConfig">
<td>Queue Configuration</td>
<td>
<p>This option is available to all attributes, <strong>but only on Modulo-1B, Modulo-2C, and Modulo-2D</strong>.</p>
<p>If you want to keep track of updates to an attribute, you can configure a queue that stores these updates. If you have selected the <span class="UIText">Use ASR Timestamps</span> option, the time of update is also stored.</p>
<p>To set up a queue, you must select a Queuing Policy that specifies the <strong>queue order</strong> and <strong>what happens when the queue is full</strong>. After selecting a policy, you will define the Queue Size, which must be an integer between 0 and 65,535. <strong>However</strong>, the size of the queue must fit in your device Profile RAM, so the upper limit will most likely be much lower.</p>
<p>Read the details of each queuing policy in <a id="1554851017.08" href="/AttrDef#all-about-queuing">All About Queuing</a>.</p>
</td>
</tr>
</tbody>
</table>
</div>
</li>
</ol>

### More About Pulsing
<p>This option gives you a straightforward way to have a GPIO pin send pulses of a defined duration in response to an attribute change. An easy way to visualize this operation is to set up a pulsing GPIO attribute with a slider control. From the mobile app, set the attribute value using the slider… and then watch as the slider moves back down to zero as the pin pulses.</p>
<p>Note that the <strong>number</strong> of pulses is not among the fixed parameters you defined for the attribute. You could define an attribute and always use it as a one-shot pulse (by always setting the attribute value to one), or you could define an attribute that pulses a variable number of times (perhaps blinking an LED for a number of times requested by a UI setting) &ndash; the difference would be in the run-time attribute value, not in the attribute definition.</p>
<p>Finally, let’s clarify the behavior of the default value. The default value sets the <strong>initial</strong> attribute value (the pulse count), and pulses are emitted as that value counts down to 0. This happens only once, and won’t be repeated until ASR resets to initial conditions. Given this, it is anticipated that most users will specify a default value of 0, so that no pulsing occurs until the attribute value is explicitly set; but the “pulse at startup” capability is there if you need it.</p>

### More About Binding
<p>We’ll use an example of binding to explain its use further. A good example is an Afero controlled power outlet, in which you would have an attribute that enables or disables power to the socket. On the power outlet itself, you’d also have a switch so the user can manually enable or disable power. What you want to do is “bind” the value of the switch attribute to the value of the power attribute so that if the user operates the switch, the outlet turns on and off. If the user changes the power value from the Afero Cloud, the attribute value for the switch will also update. For example:</p>
<div class="af-table-borders">
<table>
<thead>
<tr>
<th>Operation</th>
<th>Power Attribute</th>
<th>Switch Attribute</th>
<th>Notes</th>
</tr>
</thead>
<tbody>
<tr>
<td>All off</td>
<td>0</td>
<td>0</td>
<td>Start with a steady state.</td>
</tr>
<tr>
<td rowspan="2">User presses switch on outlet.</td>
<td>0</td>
<td>1</td>
<td>Switch attribute toggles to 1.</td>
</tr>
<tr>
<td>1</td>
<td>1</td>
<td>Binding causes power attribute to match switch attribute.</td>
</tr>
<tr>
<td rowspan="2">User turns off power via the Afero mobile app.</td>
<td>0</td>
<td>1</td>
<td>Power attribute goes to 0.</td>
</tr>
<tr>
<td>0</td>
<td>0</td>
<td>Binding causes switch attribute to match power attribute.</td>
</tr>
</tbody>
</table>
</div>

<p>In the case of binding, there is always a <strong>source attribute</strong> that is the changeable one, and a <strong>destination attribute</strong> that is being updated to match the value of the source attribute. In some cases, the destination attribute will not be writable. In these cases, the update operation will silently fail. Picture a case where you want an I/O input attribute to bind to an I/O output attribute. If the input attribute changes, the output attribute updates to match the new value. But if the output attribute is changed (via the Afero mobile app for example), the input attribute is not updated. This allows binding to work in one direction and not fail in the other.</p>

#### Binding Constraints

<p>Binding causes a change in one IO’s attribute value to update the value of another. As some IO attribute values are controlled by external hardware, binding can only work with certain IO configurations.</p>
<div class="af-table-borders">
<table>
<thead>
<tr>
<th>Configuration</th>
<th>Will Update?</th>
<th>Notes</th>
</tr>
</thead>
<tbody>
<tr>
<td>Input</td>
<td>No</td>
<td>Attribute value is controlled by external signal.</td>
</tr>
<tr>
<td>Input Toggle</td>
<td>Yes</td>
<td>Attribute value is independent of external signal. Pull configuration does not matter.</td>
</tr>
<!--						<tr>
<td>Input Comparator</td>
<td>No</td>
<td>Attribute value is controlled by external signal.</td>
</tr> -->
<tr>
<td>Output</td>
<td>Yes</td>
<td></td>
</tr>
<tr>
<td>Output PWM	</td>
<td>Yes</td>
<td>Since value is 0 to 100 (instead of 0 or 1), a PWM can only be used to bind to another PWM output.</td>
</tr>
</tbody>
</table>
</div>

### All About Queuing
<p>Queued attributes maintain a history of previous updates, as opposed to non-queued attributes, which are overwritten when the device is offline. The policy for what to do when the queue is full and the queue order is configurable.</p>
<p>“Queuing policy” is a combination of queue order and queue-full action, described below.</p>

#### Queue Order
<ul class="af-ul">
<li>FIFO (first in, first out)</li>
<li>LIFO (last in, first out)</li>
</ul>

#### Queue-Full Action
<p>Queue-full actions determine what to do when the queue is full. They are applied independently, as a function of whether the device is offline or online.</p>
<ul class="af-ul">
<li>Replace oldest - Remove the oldest item and add the new item based on queue order.</li>
<li>Fail - Do not add the new item to the queue. Instead, return an error to the MCU or ASR.</li>
<li>Flow control - Do not add the new item to the queue. Instead, flow control the MCU or ASR. After space becomes available in the queue, notify the MCU or ASR.</li>
</ul>

#### Queuing Policy Values
<p>Use the table below to understand the values in the Queuing Policy drop-down menu:</p>
<div class="af-table-borders">
<table>
<thead>
<tr>
<th>Value</th>
<th>Queue Order</th>
<th>Queue-Full When Offline</th>
<th>Queue-Full When Online</th>
</tr>
</thead>
<tbody>
<tr>
<td>FIFO/R/R</td>
<td>FIFO</td>
<td>Replace oldest</td>
<td>Replace oldest</td>
</tr>
<tr>
<td>FIFO/R/FC</td>
<td>FIFO</td>
<td>Replace oldest</td>
<td>Flow control</td>
</tr>
<tr>
<td>FIFO/F/FC</td>
<td>FIFO</td>
<td>Fail</td>
<td>Flow control</td>
<tr>
<td>LIFO/R/R</td>
<td>LIFO</td>
<td>Replace oldest</td>
<td>Replace oldest</td>
</tr>
<tr>
<td>LIFO/R/FC</td>
<td>LIFO</td>
<td>Replace oldest</td>
<td>Flow control</td>
</tr>
<tr>
<td>LIFO/F/FC</td>
<td>LIFO</td>
<td>Fail</td>
<td>Flow control</td>
</tr>

</tbody>
</table>
</div>

## Define the MCU Attributes
<p>Depending on whether your MCU device is a Potenco or another MCU device, the interface for defining the attributes will differ. Go to the section relevant for your MCU device:</p>
<ul class="af-ul">
<li><a id="1557960008.68" href="/AttrDef/#define-potenco-attributes">Define Potenco Attributes</a></li>
<li><a id="1557960008.79" href="/AttrDef/#define-non-potenco-mcu-attributes">Define (Non-Potenco) MCU Attributes</a></li>
</ul>

### Define Potenco Attributes
<p>First you will configure your Potenco, then add attributes:</p>
<ol class="af-ol">
<li>
<p>Once you’ve clicked <span class="UIText">Attributes</span> in the right-hand navigation bar, you’ll see the <span class="UIText">Define the MCU Attributes</span> window:</p>
<img class="img-br" src="../img/APE-PotencoConfig.png" width="300" alt="Potenco Configuration">
<li>
<p>In the Supported Network Interfaces pane, select all the network interfaces that your device supports. Select from <span class="UIText">Wi-Fi</span>, <span class="UIText">Ethernet</span>, and <span class="UIText">WAN</span>.</p>
</li>
<li>
<p>In the Device Configuration pane, if you want your MCU to receive the UTC time updates from ASR, select the <span class="UIText">Receive UTC Time</span> checkbox. Read more in  <a id="1557956392.18" href="/SetMCUTime">Using the Afero Cloud to Keep Time on the MCU</a>.</p>
</li>
<li>
<p>Now you’re ready to add an attribute. Click the <span class="UIText">+Device Attribute</span> button to open the device attribute editor:
<img class="img-br" src="../img/APE-PotencoAttrEditor.png" width="500" alt="Potenco Attribute Editor"></p>
</li>
<li><p>Complete the fields using the information in the table below:</p>
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
<td>Attribute&nbsp;ID</td>
<td>Accept the default attribute ID that pre-populates the field, or type another number between 1 and 1023, inclusive. The number must be unique to the Profile.</td>
</tr>
<tr>
<td>Attribute Name</td>
<td>Type a name that will help you identify this attribute in other contexts. This name is not visible to the end-user.</td>
</tr>
<tr>
<td>Default Value</td>
<td>Type a value that will be the initial setting for this function. Keep in mind that the Default Value must be compatible with the Data Type; if it’s not, you’ll be warned when you try to save. Also remember that every time a device loses power and restarts, default values are restored. Think about how this could affect the end-user.</td>
</tr>
<tr>
<td>Data Type</td>
<td><p>ASR uses the data type to allocate the storage size for the attribute. Select from the following:</p>
<ul class="af-ul">
<li>SINT8 - Signed integer (8 bit)</li>
<li>SINT16 - Signed integer (16 bit)</li>
<li>SINT32 - Signed integer (32 bit)</li>
<li>SINT64 - Signed integer (64 bit)</li>
<li>Q_15_16 - Signed fixed-point integer (32 bit)</li>
<li>BOOLEAN - Logical true/false</li>
<li>UTF8S - String encoded as UTF-8</li>
<li>BYTES - Byte array</li>
</ul>
</td>
</tr>
<tr>
<td>Max Size</td>
<td>Maximum size, in bytes, of the attribute data. The max size is pre-defined for all data types, with UTF8S and BYTES the highest at 1536 bytes. Keep in mind that attribute memory is preallocated in ASR, so choose a max that will definitely handle your application needs; if you exceed allocated space at runtime, you will almost certainly overwrite something else.</td>
</tr>
<tr>
<td>Read/Write</td>
<td>MCU attributes are Read-Only by default. If you need to write attributes to your device, then select the <span class="UIText">Write</span>  checkbox as well.</td>
</tr>
</tbody>
</table>
</div>
</li>
<li>Once you’ve defined all your attributes, click <span class="UIText">Save</span> in the upper-right corner of the window.</li>
</ol>

### Define (Non-Potenco) MCU Attributes
<p>First you will configure your MCU, then add attributes:</p>
<ol class="af-ol">
<li>
<p>Click the <span class="UIText">MCU Configuration</span> toggle switch to turn it <span class="UIText">ON</span> and select the protocol your MCU is using, either <span class="UIText">SPI</span> or <span class="UIText">UART</span>.</p>
<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="17" style="vertical-align:bottom;padding:0">&nbsp;&nbsp;<strong>NOTE:</strong>  For the “Modulo-2D” Module Type, only UART protocol is supported.</p>
</div>
</div>
<p><img class="img-br" src="../img/MCU-Config.png" alt="MCU Firmware Updates"></p>

</li>
<li><p>If you have selected <span class="UIText">UART</span>, you must select the following transmission options from the drop-down menus:</p>
<div class="af-table-borders">
<table>
<thead>
<tr>
<th>Option</th>
<th>Description</th>
</tr>
</thead>
<tbody>
<tr>
<td>Baud Rate</td>
<td>The rate at which incoming bits are read, expressed in bits per second (bps). Select from <span class="UIText">4800</span>, <span class="UIText">9600</span>, <span class="UIText">38400</span>, or <span class="UIText">115200</span>.</td>
</tr>
<tr>
<td>Parity Bit</td>
<td>For error-checking, a bit added to a string of binary code to make sure the total number of 1-bits in the string is even or odd. Select from <span class="UIText">None</span>, <span class="UIText">Odd</span>, or <span class="UIText">Even</span>.</td>
</tr>
<tr>
<td>Data Width</td>
<td>Number of bits per data frame. Select from <span class="UIText">5</span>, <span class="UIText">6</span>, <span class="UIText">7</span>, <span class="UIText">8</span>, or <span class="UIText">9</span>.</td>
</tr>
<tr>
<td>Stop Bits</td>
<td>Number of bits used to signal the end of the data transmission. Select from <span class="UIText">1</span>, <span class="UIText">1.5</span>, or <span class="UIText">2</span>.</td>
</tr>
</tbody>
</table>
</div>
</li>
<li>
<p><strong>The <span class="UIText">Receive UTC Time</span> setting applies only to ASR-2 and Modulo-2</strong>: If you want your MCU to receive the UTC time updates from ASR, select the <span class="UIText">Receive UTC Time</span> checkbox. Read more in  <a id="1532627214.14" href="/SetMCUTime">Using the Afero Cloud to Keep Time on the MCU</a>.</p></li>
<li>
<p>Use the <span class="UIText">Firmware OTA Updates</span> section if you are planning to send firmware image updates to your MCU over-the-air using the OTA Manager. You must select at least one firmware Image Type checkbox to enable OTA firmware updates for your MCU.</p>
<p>If you don’t see any Image Types, or need an Image Type that isn’t shown, go to the <a id="1527203501.19" href="https://otamanager.afero.io/" target="_blank">Afero OTA Manager</a> and create a new Image Type. Once you’ve saved this new Image Type, return to the Profile Editor to select the Image Type and complete your Profile.</p>
<p>For more information on preparing your MCU application code to receive OTA updates, please read <a id="1532627214.24" href="/MCU-OTA">Handling MCU OTA Updates</a>.</p>
</li>				
<li>
<p>Now you’re ready to add an MCU attribute. Click the <span class="UIText">+MCU Attribute</span> button to open the MCU attribute definition editor:
<img class="img-br" src="../img/Define_MCU_Attr.png" width="500" alt="MCU Attribute"></p></li>
<li><p>Complete the fields using the information in the table below:</p>
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
<td>Attribute&nbsp;ID</td>
<td>Accept the default attribute ID that pre-populates the field, or type another number between 1 and 1023, inclusive. The number must be unique to the Profile.</td>
</tr>
<tr>
<td>Attribute Name</td>
<td>Type a name that will help you identify this attribute in other contexts. This name is not visible to the end-user.</td>
</tr>
<tr>
<td>Default Value</td>
<td>Type a value that will be the initial setting for this function. Keep in mind that the Default Value must be compatible with the Data Type; if it’s not, you’ll be warned when you try to save. Also remember that every time a device loses power and restarts, default values are restored. Think about how this could affect the end-user.</td>
</tr>
<tr>
<td>Data Type</td>
<td><p>ASR uses the data type to allocate the storage size for the attribute. Select from the following:</p>
<ul class="af-ul">
<li>SINT8 - Signed integer (8 bit)</li>
<li>SINT16 - Signed integer (16 bit)</li>
<li>SINT32 - Signed integer (32 bit)</li>
<li>SINT64 - Signed integer (64 bit)</li>
<li>Q_15_16 - Signed fixed-point integer (32 bit)</li>
<li>BOOLEAN - Logical true/false</li>
<li>UTF8S - String encoded as UTF-8</li>
<li>BYTES - Byte array</li>
</ul>
</td>
</tr>
<tr>
<td>Max Size</td>
<td>Maximum size, in bytes, of the attribute data. The max size is pre-defined for all data types, with UTF8S and BYTES the highest at 255 bytes. Keep in mind that attribute memory is preallocated in ASR, so choose a max that will definitely handle your application needs; if you exceed allocated space at runtime, you will almost certainly overwrite something else.</td>
</tr>
<tr>
<td>Read/Write</td>
<td>MCU attributes are Read-Only by default. If you need to write attributes to your device, then select the <span class="UIText">Write</span>  checkbox as well.</td>
</tr>
<tr>
<td>Use ASR Timestamps</td>
<td>Refer to the <a id="1554851017.19" href="#UseTimestamp">Use ASR Timestamps</a> section above.</td>
</tr>
<tr>
<td>Queue Configuration</td>
<td>Refer to the <a id="1554851017.39" href="#QueueConfig">Queue Configuration</a> section above. To understand the queuing policy options, read <a id="1554851017.29" href="#all-about-queuing">All About Queuing</a>.
</td>
</tr>
</tbody>
</table>
</div>
</li>
<li>Once you’ve defined all your attributes, click <span class="UIText">Save</span> in the upper-right corner of the window.</li>
</ol>

#### MCU Attribute Tips
<p>Here are some helpful tips when editing MCU attribute definitions:</p>
<ul class="af-ul">
<li>To define more MCU attributes, click the <span class="UIText">+MCU Attribute</span> button to open the attribute definition editor.</li>
<li>To delete an attribute, open it for editing then click the trash icon in the upper-right of the window.</li>
<li>If you’ve added several attributes, scrolling can become annoying; clicking an attribute name at the top-left of the attribute editor will collapse/expand the editor.</li>
<li> <strong>Important!</strong> Once you’ve enabled MCU attributes and updated the Profile on your ASR, it will expect to have an MCU available on the serial bus, and will attempt to communicate with that device. If there is no device present, communication will fail and recovery may require power-cycling. So it’s important to have your MCU connected to ASR <strong>before</strong> you publish a Profile that includes MCU attribute definitions.</li>
</ul>

