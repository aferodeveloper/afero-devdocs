# Core Development Concepts

In this section, we’ll talk about some concepts behind the Afero development process.

## Attributes and Values

Your device’s attributes and values, and how the Afero Platform uses them, are concepts foundational to development. To understand your device in terms of its attributes and values, think about what your device can do (its attributes), then the ways in which it can do it (its values).

Let’s look at an example, a simple light bulb:
<img src="../img/Lightbulb.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">

**The Light Bulb Example**

Our example light bulb has two attributes:

<ul class="af-ul">
<li>It can be turned on and off, so <strong>Power</strong> is the first attribute.</li>
<li>It can shine varying degrees of brightness, so <strong>Brightness</strong> is the second.</li>
</ul>

For each attribute, we need to decide the possible values:

<ul class="af-ul">
<li>Power can be <strong>either On or Off</strong>.</li>
<li>Brightness can <strong>range from 0% to 100%</strong>.</li>
</ul>

When deciding on attributes, you can create attributes for all the functions your device is capable of, or just selected ones. You are free to decide what kind of control you want to give your user, and how granular you want that control to be. There’s a lot of flexibility built into the system to accommodate your specific design goals. (Read more in [Great Attribute Modeling](../AttrModel).)

Next, we’ll talk about how device attributes and values translate into user controls on a mobile app.

## The Mobile App UI

Now that we have a good idea of the light bulb’s attributes and possible values, think about how a user would control those device functions from a mobile app interface. We’ll stay with our light bulb example:
<img src="../img/Lightbulb.png" width="30" style="vertical-align:middle;margin:0px 0px;border:none">

We want a user to control the light bulb from their Profile Editor, so for each attribute we need to present a suitable UI element that gives the user a reasonable level of control:

<ul class="af-ul">
<li><strong>Power</strong> can have only two values, so a <strong>Switch</strong> that toggles <strong>On</strong> and <strong>Off</strong> is good control choice:
<img src="../img/Switch.png" width="100" style="vertical-align:middle;margin:0px 0px;border:none"></li>
<li><strong>Brightness</strong> can have a range of values, so a <strong>Slider</strong> is an appropriate choice to control this attribute:<br><img src="../img/Slider.png" width="300" style="vertical-align:middle;margin:0px 0px;border:none"></li>
</ul>

The data type and function of each attribute will suggest the logical UI control element to use. Some examples:

<ul class="af-ul">
<li>If you need to display status to the user, use a Value control for an attribute with a data type of UTF8.</li>
<li>If your attribute offers a range of numeric values (for instance an INT16 data type), a Slider control would work nicely.</li>
<li>If your device supports a temperature control, there’s a specially designed Temperature control element for you to use, which is a variation of the Slider.</li>
</ul>

In addition to the controls just mentioned, the Profile Editor offers other standard controls such as buttons, and specialized controls like a battery level display; and more are coming.

All the information that we’ve defined for the light bulb – the attributes and values, the mobile app UI controls – is consumed by the Afero Profile Editor, where it is efficiently packaged into a portable device “Profile”, described next.

## The Afero Profile Editor

We create a device Profile for the light bulb using the Afero Profile Editor app. The Profile Editor systematically gathers the information it needs so it can create three definitions:

<div>
<table class="af-table-nostyle">
<tbody>
<tr>
<td>Some basic information<br>about the type of light bulb</td>
<td><span class="highlight-orange">&#10230;</span></td>
<td>Device Type Definition</td>
</tr>
<tr>
<td>The light bulb’s attributes<br>and possible values</td>
<td><span class="highlight-orange">&#10230;</span></td>
<td>Device Attribute Definition</td>
</tr>
<tr>
<td>The UI controls that<br>represent the light bulb’s<br>attributes in the mobile app</td>
<td><br><span class="highlight-orange">&#10230;</span></td>
<td><br>Mobile App UI Definition</td>
</tr>
</tbody>
</table>
</div>

When the definitions are complete, the Profile Editor “publishes” your device’s information to the Afero Platform, to each place it’s needed. Read about the flow in the next section.

## Device Profile Distribution

The device definitions that the Profile Editor packages into the device Profile are used throughout the Afero Platform. The picture below illustrates the information flow once you click the <span class="UIText">PUBLISH</span> button in the Profile Editor:
<br><img src="../img/APE-ProfileDistr.png" style="vertical-align:middle;margin:0px 0px;border:none">

The device Profile leaves the Profile Editor and enters the Afero Cloud. Profiles are stored in the cloud and retrieved on demand by components within the Afero Platform.

<ul class="af-ul">
<li>For the Afero mobile app, the Attribute and Mobile App UI definitions (in particular, “device-description.json” and “device-presentation.json”, respectively) are used to create the UI for user control of the light bulb’s power and brightness.</li>
<li>For ASR, the Device Attributes are installed into it via the Afero Secure Hub. ASR can then securely report device state to the Afero Cloud and be controlled by the mobile app via the Afero Cloud and hub.</li>
<li>For the device MCU, a header file (“device-description.h”) is generated, which must be included in any MCU code you write that uses  <a id="1536862623.88" href="../API-afLib">afLib</a>.</li>
</ul>
<strong>&#8674;</strong> <em>Next:</em>&nbsp;&nbsp;[Getting Started](../Tutorials)