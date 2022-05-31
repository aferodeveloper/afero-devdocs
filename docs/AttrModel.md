# Great Attribute Modeling

When defining attributes in a Profile for Afero devices, it’s important to model the data in a way that supports easy client design and implementation, as well as good reporting. This page will lay out some best practices and an example to help you define a comprehensive and efficient attribute set for your device.

## Modeling Guidelines

Below are four guidelines to help you take maximum advantage of the Afero attribute model.

###  &#10004; Start with the Client Application

The best way to implement an efficient data model is to start with the client application team. Build the list of things they want to represent in the user interface and work backwards to determine the attributes required to support those features.

For most applications, you can simply take the interesting local variables you have in your firmware and make them Afero attributes; such as a Boolean that holds whether the device is powered on or off.

###  &#10004; Store One Value Per Attribute

In general, it’s a good idea to store only one value per attribute. This makes for a cleaner history and reporting data, as well as making things better for the client applications. The one time you may want to consider storing multiple values in an attribute is when they are all part of a single transaction. For example, a command attribute that takes a command type followed by command parameters is better sent as one attribute so that all the data can be parsed at the same time.

Storing things like comma-separated strings or json objects as attribute values will definitely limit what can be done when reporting on those values. It will also increase the size of the attribute value and reduce response time.

###  &#10004; Use Smallest Size Needed

Whenever possible, express data in the smallest form possible. Usually this just means using the correct data type for the actual value. Expressing data types like numbers is not only inefficient from a size point of view, it complicates comparisons when doing reporting over a large data set.

###  &#10004; Minimize Update Frequency

For frequency, no more than one write/update per second is preferable. Much slower than that, if possible, is recommended. Remember, a high update rate can affect your overall solution cost. If a device reports attribute values rapidly during operation even if those values haven’t changed, those updates will cause excessive traffic between the MCU and ASR. Also, there’s no need to write a group of related attributes if only one has changed. Only send updates that are meaningful to you.

Writing to ASR does not mean the attribute has been synchronized with the Cloud; synchronization may take some time depending on the radio network or connectivity in general. So if you overwrite an attribute before it’s synchronized, the Cloud will not know about it (with an exception for attributes defined as [latch](../AttrDef#Latching)). Read more about allowing adequate attribute write timing in [Don’t Forget to Call af_lib_loop()](../MCUCodingTips#dont-forget-to-call-af_lib_loop).

The MCU can sample as often as desired; there’s no limit on attribute reads since they come directly from ASR with no Cloud overhead. But again, **only push useful data**.

## Modeling Method

One way to build a attribute model for your device is to organize all the attribute information in a table, one attribute per row. Assign each attribute a number using column *1*. MCU attributes can be numbered 1-1023.

| *1*<br>NO. | *2*<br>FEATURE | *3*<br>DESCRIPTION | *4*<br>STATES | *5*<br>DATA TYPE | *6*<br>WRITABILITY | *7*<br>DEFAULT VALUE | *8*<br>UI CONTROL | *9*<br>SEND FREQUENCY | *10*<br>DEPENDENCIES |
| :------: | :----------: | :--------------: | :---------: | :------------: | :--------------: | :----------------: | :-------------: | :-----------------: | :----------------: |
| 1       |             |                 |            |               |                 |                   |                |                    |                   |
| 2       |             |                 |            |               |                 |                   |                |                    |                   |
| ⋮       |             |                 |            |               |                 |                   |                |                    |                   |
| 1023    |             |                 |            |               |                 |                   |                |                    |                   |

The information entered in the table will answer these questions:

- Columns *2-4* - What are all the device features and possible states?
- Column *5* - What data type is best suited for holding the state values?
- Column *6* - What feature states will the end user either view (read-only) or control (read/write) using the mobile app?
- Column *7* - Are there default values for any of the feature settings?
- Column *8* - How will the end user view or control each feature setting in the mobile app UI? (*column 8*)
- Column *9* - How frequently will an attribute value be sent to the Cloud; what event will trigger the send?
- Column *10* - Does the availability of a feature depend on the selection of any other features?

## Smart Toaster Oven Example

In this example we’ll define the attributes for a smart midline toaster oven. We’ll step through the toaster oven features (a.k.a. attributes), numbering and naming them so we can use that information when defining these attributes in the Afero Profile Editor. The full table is shown at the end of the example in [The Toaster Oven Data Model](../AttrModel#the-toaster-oven-data-model).

First, let’s ask, “What controls does the toaster oven provide?” The Start Button is pretty obvious, let’s begin with that.

### Feature 1: Start Button

Let’s use the table to define the Start button characteristics. We’ll later use this information when defining the attribute in the Profile Editor.

| NO.  | FEATURE      | DESCRIPTION                                        | STATES    | DATA TYPE | WRITABILITY | DEFAULT VALUE | UI CONTROL | SEND FREQUENCY | DEPENDENCIES                  |
| :--- | :----------- | :------------------------------------------------- | :-------- | :-------- | :---------- | :------------ | :--------- | :------------- | :---------------------------- |
| 1    | Start Button | Button that starts/stops the currently-set action. | &bull; Stop<br>&bull; Start | Boolean   | Read/Write  | Stop          | Menu       | On Change      | Any Cooking Mode is selected. |

- The Start button has only two states, Start and Stop, so we can model it with a Boolean attribute. Of course, we could use an Integer, or even a String, but remember that we should try to use the smallest attribute that is suited to the range of values required.
- Being a physical control, the Start button allows the end-user to change the value of its associated attribute, so it must be defined as a Read/Write attribute.
- We’ll give it a default value of Stop.
- We’ve got some flexibility when deciding what sort of UI Control we want to use to represent the Start button in the mobile app UI. A Menu control is one option: it’s called “Menu” because it can be used to allow selection of one from many choices, but it is rendered as a group of buttons, and it works just fine for a two-state control. Let’s choose Menu.
- Now, we need to know how often we should send the state of the attribute up to the Afero Cloud. In the case of the Start Button, there is no need to send that data at any time other than when it has changed, so we’ll pick a send frequency On Change.
- Finally, this feature will only activate if a Cooking Mode has been selected (see Feature 2 below).

### Feature 2: Cooking Mode

The next feature we’ll look at is the cooking mode. On the physical oven unit, the end-user selects a cooking mode using a dial.

| NO.  | FEATURE           | DESCRIPTION                        | STATES                                                       | DATA TYPE | WRITABILITY | DEFAULT VALUE | UI CONTROL | SEND FREQUENCY | DEPENDENCIES                                   |
| :--- | :---------------- | :--------------------------------- | :----------------------------------------------------------- | :-------- | :---------- | :------------ | :--------- | :------------- | :--------------------------------------------- |
| 2    | Cooking Mode Dial | Selector for setting cooking mode. | 1 = Bake<br>2 = Convection<br> 3 = Broil<br> 4 = Toast<br> 5 = Warm<br> 6 = Reheat | SINT8     | Read/Write  |               | Menu       | On Change      | Feature is available when unit is not cooking. |

- This toaster oven offers six cooking options: Bake, Convection, Broil, Toast, Warm, and Reheat. To accommodate these options, we need an 8-byte signed integer (SINT8). Each baking option will map to a numeric value, as shown.
- Because the end-user will be setting the cooking method, this attribute must be Read/Write.
- We don’t want to set a default value for this setting, because the availability of some features are dependent on the cooking method selected. For example, the features *Toast Shade*, *Toast Slices*, and *Bagel* (below) are dependent on the end-user’s selecting the Toast cooking method.
- Because there are six options, the obvious UI control choice is Menu.
- Like with the Start Button, we only need to notify the Afero Cloud when the setting has changed, so the send frequency will be On Change.
- This feature has no dependencies on any other attribute states, except that no other cooking mode be running.

### Feature 3: Toast Slices

This toaster oven requires you set how many slices you want to toast in the oven at once. The toaster can hold up to six slices, but the MCU code accommodates two options.

| NO.  | FEATURE                | DESCRIPTION                                       | STATES  | DATA TYPE | WRITABILITY | DEFAULT VALUE | UI CONTROL | SEND FREQUENCY | DEPENDENCIES                      |
| :--- | :--------------------- | :------------------------------------------------ | :------ | :-------- | :---------- | :------------ | :--------- | :------------- | :-------------------------------- |
| 3    | Number of Toast Slices | Toggle to set number of slices you plan to toast. | 1–3<br> 4–6 | Boolean   | Read/Write  | 1–3           | Menu       | On Change      | Cooking Mode *Toast* is selected. |

- The two options for *number of slices* are: 1-3 and 4-6. A Boolean data type can accommodate these two possible values.
- As usual, because the user must be able to select number of slices, the attribute must be Read/Write.
- By default, we’ll say 1–3 slices.
- With two options that must be clearly spelled out, let’s use a Menu UI Control.
- We’ll notify the Cloud when the setting changes (On Change).
- For this feature to be selectable, the Cooking Mode *Toast* must be selected.

### Feature 4: Toast Shade



| NO.  | FEATURE     | DESCRIPTION                            | STATES                                                       | DATA TYPE | WRITABILITY | DEFAULT VALUE | UI CONTROL | SEND FREQUENCY | DEPENDENCIES                      |
| :--- | :---------- | :------------------------------------- | :----------------------------------------------------------- | :-------- | :---------- | :------------ | :--------- | :------------- | :-------------------------------- |
| 4    | Toast Shade | Selector for setting toast “doneness”. | Range from 1-10, increments of 1, where labels show:<br>1 = Defrost<br>3 = Light<br>5 = Medium<br>7 = Dark <br>10=Very&nbsp;Dark | SINT8     | Read/Write  | Medium (5)    | Slider     | On Change      | Cooking Mode *Toast* is selected. |

- Toast shade has range of settings, from defrost to very dark. We can constrain user selections to discrete values, 1-10, so a data type of SINT8 will do the trick.
- Because this is a setting the user makes, it must be Read/Write.
- Let’s select middle-of-the-road Medium (5 in the range) as the default setting.
- With a range of options to display, we’ll use a Slider in the mobile app.
- There’s no need to notify the Cloud unless the setting has changed.

### Feature 5: Bagel

The density of a bagel vs. toast means toasting a bagel to a given doneness takes longer. This toaster oven allows the end-user to specify a bread vs. bagel option via a button.

| NO.  | FEATURE | DESCRIPTION                                              | STATES     | DATA TYPE | WRITABILITY | DEFAULT VALUE | UI CONTROL | SEND FREQUENCY | DEPENDENCIES                      |
| :--- | :------ | :------------------------------------------------------- | :--------- | :-------- | :---------- | :------------ | :--------- | :------------- | :-------------------------------- |
| 5    | Bagel   | Toggle to set bagel function (adds time to toast cycle). | BagelBread | Boolean   | Read/Write  | Off           | Switch     | On Change      | Cooking Mode *Toast* is selected. |

- The Bagel setting has two possible states: Bagel or Bread. A Boolean data type can hold the two values.
- This attribute is settable by the end-user, so must be Read/Write.
- The assumption being that most people will be toasting bread, the default state will be Bread.
- Let’s use the Switch UI control for this one. Switch controls are nice for simple states, like On/Off or True/False.
- The Cloud only needs to be notified On Change.

### Feature 6: Degree Units

This oven accommodates the entry and reporting of both Celsius and Fahrenheit temperature scales. Using the physical oven controls, the end-user sets their preference using a toggle switch.

| NO.  | FEATURE      | DESCRIPTION               | STATES | DATA TYPE | WRITABILITY | DEFAULT VALUE | UI CONTROL | SEND FREQUENCY | DEPENDENCIES                 |
| :--- | :----------- | :------------------------ | :----- | :-------- | :---------- | :------------ | :--------- | :------------- | :--------------------------- |
| 6    | Degree Units | Toggle between °C and °F. | °C<br>°F  | Boolean   | Read/Write  | °F            | Menu       | On Change      | Feature is always available. |

- The Degree Units setting has two possible values (°C and °F), so a Boolean data type will be sufficient.
- The attribute must be Read/Write so the end-user can toggle it.
- Default depends on where the toaster oven is being used. Since this model is being sold in Canada, we will make the default °C.
- We will use a Menu UI control for this setting, so we can clearly label °C vs °F.
- The Cloud only needs to be updated On Change.
- This feature is not tied to any other functions, so is always available.

### Feature 7: Current Temp

The oven’s current temperature should always be available to the end-user once a Cooking Mode selection is made for either Bake, Convection, or Broil.

| NO.  | FEATURE      | DESCRIPTION                          | STATES                 | DATA TYPE | WRITABILITY | DEFAULT VALUE | UI CONTROL | SEND FREQUENCY | DEPENDENCIES                                               |
| :--- | :----------- | :----------------------------------- | :--------------------- | :-------- | :---------- | :------------ | :--------- | :------------- | :--------------------------------------------------------- |
| 7    | Current Temp | Display of current oven temperature. | 130–500 °F (54-250 °C) | SINT8     | Read-Only   |               | Value      | On Change      | Cooking Mode *Bake*, *Convection*, or *Broil* is selected. |

- The oven temperature can range from 130-500 °F (or 54-250 °C), so a SINT8 data type will work.
- Because the Current Temp display reflects the actual oven temperature and the end-user won’t be controlling it, it can be Read-Only.
- There is no default current temperature, only actual current temperature, so no default value for this attribute.
- We will use a Value UI control to display the current temperature since it will be a read-only display of a string.
- The Current Temp isn’t needed by the Cloud until the end-user has selected a Cooking Mode and then the Start Button.
- The current temperature is meaningless until Cooking Mode *Bake*, *Convection*, or *Broil* is selected.

### Feature 8: Set Temp

For Cooking Modes *Bake* and *Convection*, the end-user must select a baking temperature. For these modes, the oven is preset to 150 °F or 65 °C. The end-user can adjust the target temperature up to 500 °F or 250 °C. Once the target temperature is reached, a bell rings once. (If the user selects *Broil* cooking mode, the target temperature is not adjustable, but is set to 500 °F or 250 °C., automatically.)

| NO.  | FEATURE  | DESCRIPTION                              | STATES     | DATA TYPE | WRITABILITY | DEFAULT VALUE   | UI CONTROL | SEND FREQUENCY | DEPENDENCIES                         |
| :--- | :------- | :--------------------------------------- | :--------- | :-------- | :---------- | :-------------- | :--------- | :------------- | :----------------------------------- |
| 8    | Set Temp | Selector to set target oven temperature. | 100–500 °F | SINT8     | Read/Write  | 150 °F or 65 °C | Slider     | On Start       | Cooking Mode *Bake* or *Convection*. |

- The temperature will range from 130-500 °F (54-250 °C), so a SINT8 data type will work.
- The end-user will be setting the temp, so must be Read/Write.
- The oven will default to a setting of 150 °F or 65 °C.
- The user must be able to select a temperature from the range of possible oven temp values, so a Slider will work. We can use increments of 25 °F, which means there will be approximately 15 steps in the Slider.
- The Cloud needs to get an update once the end-user has selected the Start Button, after setting a temperature or accepting the default.
- The end-user has selected a Cooking Mode of *Bake* or *Convection*.

### Feature 9: Timer

The user will be able to set a timer while using Cooking Modes *Bake*, *Convection*, or *Broil*. The timer can be set for up to four hours, in minute/hour increments.

| NO.  | FEATURE | DESCRIPTION                       | STATES    | DATA TYPE | WRITABILITY | DEFAULT VALUE | UI CONTROL | SEND FREQUENCY | DEPENDENCIES                                               |
| :--- | :------ | :-------------------------------- | :-------- | :-------- | :---------- | :------------ | :--------- | :------------- | :--------------------------------------------------------- |
| 9    | Timer   | Selector to set count-down timer. | 0–60 mins | SINT8     | Read/Write  |               | Slider     | On Change      | Cooking Mode *Bake*, *Convection*, or *Broil* is selected. |

- The timer can set for up to four hours, so we will use a data type of SINT8.
- The timer must be set by the end-user so it will be Read/Write.
- The default setting is zero (00:00).
- A slider will work the best for setting a timer. Set points can be placed every 10 minutes.
- The Cloud should get notified each time the end-user changes the default setting.
- The timer setting is available after the user has selected a Cooking Mode option of *Bake*, *Convection*, or *Broil*.

### Feature 10: Timer Ring

Once a set timer has run down, it will ring until the end-user shuts it off.

| NO.  | FEATURE    | DESCRIPTION                                                  | STATES     | DATA TYPE | WRITABILITY | DEFAULT VALUE | UI CONTROL | SEND FREQUENCY | DEPENDENCIES                              |
| :--- | :--------- | :----------------------------------------------------------- | :--------- | :-------- | :---------- | :------------ | :--------- | :------------- | :---------------------------------------- |
| 10   | Timer Ring | Ringer that goes off when timer reaches zero. Rings until shut off. | Silent<br>Ring | Boolean   | Read/Write  | Silent        | Menu       | On Change      | Timer must have run down to zero to ring. |

- The timer is either On (Ringing) or Off (Stopped), so a Boolean data type will work.
- The user must be able to turn the ringer of so it must be Read/Write.
- Default state is Stop (silent).
- For clarity, we’ll use a Menu control with Stop as the only selectable option.
- The Cloud must be notified when the end-user taps Stop.
- Timer Ring is triggered once the set timer has run down to zero.

### The Toaster Oven Data Model

The table below lays out the data model. You can even use the *No.* and *Feature* column contents to number and name your attributes as you enter them in the Profile Editor.

| NO.  | FEATURE                | DESCRIPTION                                                  | STATES                                                       | DATA TYPE | WRITABILITY | DEFAULT VALUE   | UI CONTROL | SEND FREQUENCY | DEPENDENCIES                                               |
| :--- | :--------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- | :-------- | :---------- | :-------------- | :--------- | :------------- | :--------------------------------------------------------- |
| 1    | Start Button           | Button that starts/stops the currently-set action.           | Start<br>Stop                                                    | Boolean   | Read/Write  | Stop            | Menu       | On Change      | Any Cooking Mode is selected.                              |
| 2    | Cooking Mode           | Selector for setting cooking mode.                           | 1 = Bake<br>2&nbsp;=&nbsp;Convection<br>3 = Broil<br>4 = Toast<br>5 = Warm<br>6 = Reheat | SINT8     | Read/Write  | Bake            | Menu       | On Change      | Feature is available when unit is not cooking.             |
| 3    | Number of Toast Slices | Toggle to set number of slices you plan to toast.            | 1–3<br>4–6                                                      | Boolean   | Read/Write  | 1–3             | Menu       | On Change      | Cooking Mode *Toast* is selected.                          |
| 4    | Toast Shade            | Selector for setting toast “doneness”.                       | Range from 1–10, increments of 1, where labels reflect:<br>1 = Defrost<br>3 = Light<br>5 = Medium<br>7 = Dark<br>10 = Very Dark | SINT8     | Read/Write  | Medium (5)      | Slider     | On Change      | Cooking Mode *Toast* is selected.                          |
| 5    | Bagel                  | Toggle to set bagel function (adds time to toast cycle).     | BagelBread                                                   | Boolean   | Read/Write  | Off             | Switch     | On Change      | Cooking Mode *Toast* is selected.                          |
| 6    | Degree Units           | Toggle between °C and °F.                                    | °C °F                                                        | Boolean   | Read/Write  | °F              | Menu       | On Change      | Feature is always available.                               |
| 7    | Current Temp           | Display of current oven temperature.                         | 100–500 °F                                                   | SINT8     | Read-Only   | None            | Value      | On Change      | Cooking Mode *Bake*, *Convection*, or *Broil* is selected. |
| 8    | Set Temp               | Selector to set target oven temperature.                     | 100–500 °F                                                   | SINT8     | Read/Write  | 150 °F or 65 °C | Slider     | On Start       | Cooking Mode *Bake* or *Convection* is selected.           |
| 9    | Timer                  | Selector to set count-down timer.                            | 0–60 mins                                                    | SINT8     | Read/Write  | None            | Slider     | On Change      | Cooking Mode *Bake*, *Convection*, or *Broil* is selected. |
| 10   | Timer Ring             | Ringer that goes off when timer reaches zero. Rings until shut off. | SilentRing                                                   | Boolean   | Read/Write  | Silent          | Switch     | On Change      | Timer must have run down to zero to ring.                  |

Note that in addition to the information gathered above, before you can complete your device’s Profile and UI presentation using the Afero Profile Editor, you will need to specify a few more details, such as attribute value max size, temperature setting increments, and so on, depending on how your MCU code functions.