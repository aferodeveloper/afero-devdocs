# FACTORY PROGRAMMING

This page is for Afero partners and describes the factory programming process, which is the final stage in product deployment. The first section below describes what must be completed before entering this stage. In this document we refer to Afero, Afero Partners, Partner’s Manufacturers, and the product End-Users.

The factory programming process is described in the following sections:

#####Before You Begin

- [Prerequisite Tasks](../FactoryProg#prerequisite-tasks)
- [Factory Programmer Block Diagram](../FactoryProg#factory-programmer-block-diagram)

#####The Steps

[1. Gather Factory Programmer Requirements](../FactoryProg#1-gather-factory-programmer-requirements)<br>
[2. Request Factory Programmer from Afero](../FactoryProg#2-request-factory-programmer-from-afero)<br>
[3. Set Up Hardware/Software for Factory](../FactoryProg#3-set-up-hardwaresoftware-for-factory)<br>
[4. Program Devices at Factory](../FactoryProg#4-program-devices-at-factory)<br>
[5. Print Product Labels](../FactoryProg#5-print-product-labels)<br>
[6. Run Validation Tests](../FactoryProg#Val6-run-validation-testsidate)<br>

## Prerequisite Tasks

Before you can move into factory programming, make sure you have completed the tasks described below. Contact the Afero Customer Enablement (ACE) team if you need details on fulfilling these tasks:

- Signed a product addendum to the Master Service Agreement (MSA) with Afero.
- Transitioned point of contact from ACE team to Program Manager (PM).
- Established an [ExaVault account](https://afero.exavault.com/) to facilitate secure file sharing between you, your manuifacturer, and Afero. Make sure you have received the ExaVault directory name and account credentials from Afero.
- Defined your Device Type and developed a final device Profile, which has been signed by Afero. A signed device Profile is ready for production and cannot be altered or corrupted; the process uses a cryptographic hash to validate authenticity and integrity.

## Factory Programmer Block Diagram

The diagram below illustrates the connections for the host MCU, ASR module, and the associated factory programmers:

<img src="../img/FactProgBlockDiag.png" width="900" style="vertical-align:middle;margin:0px 0px;border:none">

## 1. Gather Factory Programmer Requirements

Afero will create your Factory Programmer, but requires some inputs from the partner.

Be sure you have published your final device Profile to at least one prototype development device, using the email address you used that first triggered the creation of your partner account.

| ✓    | INFORMATION NEEDED                        | DESCRIPTION                                                  | EXAMPLE          |
| :--- | :---------------------------------------- | :----------------------------------------------------------- | :--------------- |
| ☐    | Partner Email Address                     | This must be the same email you used to sign in to the Afero Profile Editor:<ul><li>when creating the initial Profile for development, and</li><li>when creating the final device Profile to be used on production devices.</li></ul><img src="../img/FactProg-01.png" width="200" style="vertical-align:middle;margin:0px 0px;border:none"> | afero@ABCinc.com |
| ☐    | Final Production Device Profile Version&nbsp;# | This is the final, tested, and Afero certified version of the device Profile that will be programmed onto each production device in the factory. You will find your final device Profile Version # by doing the following:<br><br>**1**&nbsp;&nbsp;Sign in to the Afero Profile Editor using the email account used to create the final device Profile.<br><br>**2**&nbsp;&nbsp;Open the project holding the final device Profile.<br><br>**3**&nbsp;&nbsp;Go to the Publish window and look in the first table. Identify the correct Device Type row, and note the PROFILE VERSION number (see illustration below).<br><br><img src="../img/FactProg-02.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none"> | v23371           |
| ☐    | Device Type Name                          | This is a unique name across all the devices in your account that identifies which devices the Profile can be installed on. When created, you paired it with a hardware Module Type, which remains part of its essential device type definition.To find your Device Type Name, do the following:<br> **1**&nbsp;&nbsp;Sign in to the Afero Profile Editor using the email account used to create the final device Profile.<br><br>**2**&nbsp;&nbsp;Open the project holding the final device Profile.<br><br>**3**&nbsp;&nbsp;Go to the Device Type window and note the name in the DEVICE TYPE NAME field, as shown below.<br><br><img src="../img/FactProg-03.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none"><br><br>This name should also match the Device Type name displayed on the Publish window (shown in the illustration above). | Kitty Tracker    |

## 2. Request Factory Programmer from Afero

The Afero Factory Programmer (AFP2) is created for you by Afero and is packaged in a `.zip` file. It contains all the software you will need to program your PCB in the factory.

AFP2 will handle the following tasks:

1. (*Optional*) Running pre-programming tests to verify the hardware assembly. (These tests would be provided by the partner’s manufacturer; entry hooks into AFP2 are available to run some additional Python scripts should they be needed.)
2. Programming/flashing of binary images to the main Afero Platform (typically bootloader, device Profile, app) and supplementary chips (e.g., WINC3400 or external SPI flash).
3. (*Optional*) Programming/flashing of binary images to the host MCU platform (if partner has modified AFP2 to also program their MCU).
4. (*Optional*) Running post-programming tests to verify device functionality. (Again these would be defined by the partner’s manufacturer.)
5. Extracting and recording of device-specific information (such as device IDs, MAC addresses, image versions, unique identifiers, etc.) to an Afero factory (`.afero`) file.
6. Generation and printing of the module/PCB’s and product’s QR code label.

| ✓    | TASK                                                         | DESCRIPTION                                                  | RESPONSIBILITY                    |
| :--- | :----------------------------------------------------------- | :----------------------------------------------------------- | :-------------------------------- |
| ☐    | Submit partner email, final device Profile version #, and Device Type Name to Program Manager. | This is the information you’ve gathered above in [Step 1](../FactoryProg#AFP-Requirements). | PartnerAfero Program Manager (PM) |
| ☐    | Factory programmer is created and tested.                    | The Afero Hardware team will take the partner inputs and create the `AFP2.zip file`. | Afero Hardware Team               |
| ☐    | Factory programmer is handed to partner/factory.             | Using the partner’s [ExaVault account](https://afero.exavault.com/), Afero transfers the factory programmer to the partner. | Afero PM                          |

## 3. Set Up Hardware/Software for Factory

| ✓    | TASK                                         | DESCRIPTION                                                  | RESPONSIBILITY         |
| :--- | :------------------------------------------- | :----------------------------------------------------------- | :--------------------- |
| ☐    | Set up the PC and required software.         | **1**&nbsp;&nbsp;You will need a PC running Windows 7 or later.<br><div class="af-callout"><div class="callout-text"><p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> Check the name of your PC to make sure it does not contain any non-Unicode characters. Only UTF-8 characters are supported by the AFP so if your machine name does not comply with this requirement, please change the name of your Windows PC. You can check and edit the name by clicking the lower-left Windows menu, then selecting Settings &gt; System &gt; About &gt; Rename this PC.</div></div>**2**&nbsp;&nbsp;Install the latest version of Python 2.7 from [www.python.org/downloads](https://www.python.org/downloads/). (On the date of this writing the latest is Python 2.7.15 with this direct link: [www.python.org/downloads/release/python-2715](https://www.python.org/downloads/release/python-2715/)).<br><br>**3**&nbsp;&nbsp;Add `c:\python27` and `c:\python27\scripts` to the environment path variables. (Read [Add Environmental Variables Path on a PC](../AddPath) for details on how to do this.)<br><br>**4**&nbsp;&nbsp;Open Windows Command Prompt with administrator privilege and run these two commands:<br>&nbsp;&nbsp;`pip install pip -U`<br>&nbsp;&nbsp;`pip install pyserial`<br><br>**5**&nbsp;&nbsp;Install 7-Zip from [7zip.org](https://www.7-zip.org/download.html).<br><br>**6**&nbsp;&nbsp;Install Notepad++ from [notepad-plus-plus.org](https://notepad-plus-plus.org/). | Partner’s Manufacturer |
| ☐    | Design and test factory programming fixture. | You must set up the factory programming fixture and debugger/programmer (such as a SEGGER J-Link, which is used as an example in the instructions that follow):<br><br>**1**&nbsp;&nbsp;The factory programming fixture is a jig that is connected to the PC. This fixture a) supplies power, and b) creates a physical connection between ASR and a PC running a Python script. The programming fixture must supply 2.4 – 3.5V DC to VDD, and Ground to GND.The SEGGER J-Link debugger/programmer communicates with the programming fixture to flash the firmware images. The J-Link is connected to the SWDIO and SWDCLK pins on the Afero module.<br><br>**2**&nbsp;&nbsp;The SEGGER J-Link also monitors VDD and GND. (See [www.segger.com/products/debug-probes/j-link/models/j-link-base/](https://www.segger.com/products/debug-probes/j-link/models/j-link-base/).)<br><br>To install the J-Link driver go to:  [www.segger.com/downloads/jlink/#jlinksoftwareanddocumentationpack](https://www.segger.com/downloads/jlink/#jlinksoftwareanddocumentationpack) and download “J-Link Software and Documentation Pack for Windows”.<br><br>After installation, be sure to add J-Link to your environment path variable. (Read [Add Environmental Variables Path on a PC](../AddPath) for details on how to do this.) | Partner’s Manufacturer |
| ☐    | Design product’s final board.                | The circuit design must expose the programming pins used to load the production Afero device Profile and latest firmware. Please consider the following:<ul><li>Make sure that the connections (e.g., pads) on your circuit board are physically accessible so your programming fixture can connect to them easily in the factory.</li><li>You may decide to expose programming pins on an internal circuit board, or on the outside of your product’s enclosure. Your final design should consider the overall cost of production and the service life of the product.</li><li>Deciding to expose test pins outside the enclosure adds expense, but it allows you to program, label, and test the fully-assembled product in the factory. This strategy also supports testing in a service centre without disassembling a product that has been returned.</li> | Partner’s Manufacturer |
| ☐    | Purchase and test QR code/label printer.     | **1**&nbsp;&nbsp;Procure a printer that connects to the PC for generating the device labels, which includes the QR code.<br><br> Standard label sizes:<ul><li>Labels that are placed on the module are 0.5-in tall by 1-inch wide.</li><li>Labels placed on the appliance or device are 30mm tall by 45mm wide.</li></ul>Recommended (supported) label printers are both 300dpi:<ul><li>Zebra Gx430t, or</li><li>Zebra 105SL Plus</li></ul><div class="af-callout"><div class="callout-text"><p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> Be sure to install the correct Zebra print driver on Windows and verify that the printer appears in Settings > Printers and Scanners.</div></div><br>**2**&nbsp;&nbsp;Print a test page.<br><br>**3**&nbsp;&nbsp;Customize the appropriate .json printer label file, included in the unzipped factory programmer (AFP2) subdirectory: `\targets\*ProgrammerName*\labels\`. The files are named by printer model. Read details about default settings and the values you must customize in [Customize QR Code Labels](../CustomizeLabel). | Partner’s Manufacturer |
| ☐    | Procure QR code scanner.                     | Recommended scanner is the Datalogic QUICKSCAN QD2430.If the QR code scanner doesn’t work correctly, check the [QuickScan Quick Reference Guide](https://www.bsr.at/mediafiles/Handbuch/Datalogic/Quick_Reference_Guide_QuickScan_QD2430_EN.pdf). In the Guide are QR codes you can use for programming the QR code scanner to work with the PC you’ve connected to. | Partner’s Manufacturer |

## 4. Program Devices at Factory

This step is broken into four sub-steps:

- [4.1 Set Up Factory Programming Station(s)](../FactoryProg#41-set-up-factory-programming-stations)
- [4.2 Program ASR Modules Using Afero Factory Programmer](../FactoryProg#42-program-asr-modules-using-afero-factory-programmer)
- [4.3 Program the Host MCU](../FactoryProg##43-program-the-host-mcu)
- [4.4 Register Device(s) with the Afero Cloud](../FactoryProg#44-register-devices-with-the-afero-cloud)

### 4.1 Set Up Factory Programming Station(s) 

| ✓    | TASK                              | DESCRIPTION                                                  | RESPONSIBILITY         |
| :--- | :-------------------------------- | :----------------------------------------------------------- | :--------------------- |
| ☐    | Set up factory station(s).        | Each factory station must be set up with a PC with AFP2 software installed, and connected to the factory programming jig (via J-Link), 300dpi label printer, and QR code scanner. | Partner’s Manufacturer |
| ☐    | Set up label printer.             | Label printer must be loaded with correct label size for PCB labels. | Partner’s Manufacturer |
| ☐    | Connect the programming fixture.  | Connect the programming fixture to a switchable power supply and your PC. The programming fixture will interface with the assembled board, supplying power to and communicating with ASR. | Partner’s Manufacturer |
| ☐    | Unzip AFP2 file.                  | Use 7-Zip to unzip the encrypted file `afp2-*ProgrammerName-BuildNumber*.zip` to a convenient location on your C: drive.<br><br>To unzip, highlight the zip file, then right-click to select 7-Zip> Extract to `afp2-*ProgrammerName-BuildNumber*`. You will need a password to unzip the file, which Afero will give you. | Partner’s Manufacturer |
| ☐    | Customize the `config.json` file. | The `config.json` file is located in the `targets\*ProgrammerName*\` directory in the unzipped factory programmer (AFP2).<br><br>Use Notepad++ to edit the file. Fields that must be customized are marked with the value CHANGE_ME. These customized values are required and will appear in the factory database record. | Partner’s Manufacturer |
| ☐    | Complete printer setup.           | **1**&nbsp;&nbsp;Open a command line window and navigate to the folder in which the factory programmer (AFP2) was unzipped.<br><br>**2**&nbsp;&nbsp;Within that directory, go to the `core\util` directory by typing:`cd core\util`<br><br>**3**&nbsp;&nbsp;You will find printer configuration scripts used to map the Zebra printer to LPT1:<ul><li>setup-zebra-105SL.bat</li><li>setup-zebra-gx430t.bat</li></ul><div class="af-callout"><div class="callout-text"><p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> If you are running the programmer on a non-English PC, you must edit the appropriate batch file to reference the correct path to the `prncntg.vrb` file, given your locale.<br><br>For example, this line in the batch file contains the en-US locale in the path:<br><br>`cscript C:\Windows\System32\Printing_Admin_Scripts\en-US\prncnfg.vbs -t -p "ZDesignerGX430t" +shared -h ZEBRA300`<br><br>Make sure your printer’s batch file references the locale being used on your PC.</div></div><br>**4**&nbsp;&nbsp;Run the appropriate script for the printer that the factory will be using.<div class="af-callout"><div class="callout-text"><p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> If the printer you are using is not one of the two listed, the `.bat` scripts can be copied and the name changed to match the printer being installed; however, the label configuration files referenced above in [3. Set Up Hardware/Software for Factory](../FactoryProg#3-set-up-hardwaresoftware-for-factory) may not be compatible with your printer.</div></div> | Partner’s Manufacturer |

### 4.2 Program ASR Modules Using Afero Factory Programmer

On every PCB manufactured, the following steps are performed by a factory line worker at a factory station.

Afero recommends that you program the ASR module **first** to ensure that the host MCU is not running code that will hold ASR in RESET or otherwise interfere with the ASR programming step.

Most MCUs are designed with the I/Os floating; as a result, the ASR RESET line between the MCU and ASR will float if the MCU program has been erased or is held in reset. An external pullup on the ASR RESET line is a good practice.



| ✓    | TASK                              | DESCRIPTION                                                  | RESPONSIBILITY         |
| :--- | :-------------------------------- | :----------------------------------------------------------- | :--------------------- |
| ☐    | Connect board and fixture.        | Connect the assembled board to the programming fixture.      | Partner’s Manufacturer |
| ☐    | Run the programming script.       | **1**&nbsp;&nbsp;Apply power to the fixture and run the Factory Programming batch file script from the command line in the unzipped factory programmer (AFP2) directory:<br><br>`afp-*ProgrammerName*.bat`<br><br>**2**&nbsp;&nbsp;You will receive a “PASS” prompt when the programming has completed. You can then power down the programming fixture and remove the board.<br><br>**3**&nbsp;&nbsp;A corresponding QR code for the PCB will emerge from the QR code printer. | Partner’s Manufacturer |
| ☐    | Place QR code label on PCB.       | Make sure that the label is permanently affixed to the PCB.  | Partner’s Manufacturer |
| ☐    | Repeat for all PCBs manufactured. | Do this at each of the factory stations.                     | Partner’s Manufacturer |

### 4.3 Program the Host MCU

<div class="af-callout-caution">
	<div class="callout-text">
	<p><img src="../img/Caution.svg" width="17" style="vertical-align:bottom;padding:0">&nbsp;&nbsp;<strong>CAUTION!</strong>&nbsp;&nbsp;Program the host MCU <strong>after</strong> programming ASR. If you must program the host MCU before ASR (strong recommendation is to program ASR first), the code on the host MCU must be designed so the ASR RESET line is not held low during ASR programming.</p>
	</div>
</div>


| ✓    | TASK                       | DESCRIPTION                                  | RESPONSIBILITY         |
| :--- | :------------------------- | :------------------------------------------- | :--------------------- |
| ☐    | Program the host MCU.      | Use the process defined for the partner MCU. | Partner’s Manufacturer |
| ☐    | Factory test the host MCU. | Use the process defined for the partner MCU. | Partner’s Manufacturer |

### 4.4 Register Device(s) with the Afero Cloud

| ✓    | TASK                                      | DESCRIPTION                                                  | RESPONSIBILITY         |
| :--- | :---------------------------------------- | :----------------------------------------------------------- | :--------------------- |
| ☐    | Locate the `.afero` file.                 | The AFP2 script generates a factory output file in the `_output` directory. This file is named:`01_*ComputerName_ProgrammerName_YYYYMMDDTimeZone*.afero`<br><br>The first field designates the Factory File Type and will always be `01` for files generated from the factory programming script.<br><br>This file is comma-separated and contains the fields listed below by default:<ul><li>profile version id</li><li>profile fw typeboot</li><li>loader version idboot</li><li>loader fw type</li><li>app version id</li><li>app fw type</li><li>wifi version id</li><li>wifi fw type</li><li>device id</li><li>association id</li><li>company code</li><li>ble mac</li></ul>Assuming you have customized the `config.json` configuration file, as instructed above in [step 4.1](../FactoryProg#41-set-up-factory-programming-stations), these modified fields will also be included in the file:<ul><li>partNo</li><li>manufacturer</li><li>factory</li><li>station id</li><li>hw version</li></ul> | Partner’s Manufacturer |
| ☐    | Post `.afero` file to Afero.              | At the end of each manufacturing day, post this `.afero` file to the [ExaVault site](https://afero.exavault.com/) in a directory specified by Afero. Archive back-up copies of each `.afero` file in a secure location at the factory.Be careful not to overwrite existing files on the ExaVault server. | Partner’s Manufacturer |
| ☐    | Afero sends email listing files uploaded. | The SFTP server sends a daily email listing all files uploaded. Double-check that the daily email includes all files that were uploaded. | Partner’s Manufacturer |
| ☐    | Afero processes `.afero` file.            | Once the file has been validated by Afero, PCBs are ingested by the Afero service. The file is then moved to a “Completed” directory and the factory is left with a record of devices in a separate SFTP folder. The ingested PCBs are now enabled for use in the Afero Platform. | Afero                  |

## 5. Print Product Labels

| ✓    | TASK                                    | DESCRIPTION                                                  | RESPONSIBILITY                      |
| :--- | :-------------------------------------- | :----------------------------------------------------------- | :---------------------------------- |
| ☐    | Set up factory programming stations(s). | Each factory station must have a PC with AFP2 software installed, must be connected to the QR code scanner and a 300dpi label printer with the correct label size for the product. | Partner’s Manufacturer              |
| ☐    | Start running label printing script.    | Run the AFP2 script for printing the large QR code label for the outside of the product. You will find this script at the top level within the unzipped factory programmer (AFP2) directory, named `label-large-*ProgrammerName*.bat`. This is the QR code that the end-user will scan when onboarding the product. | Partner’s Manufacturer<br><br>AFP2 software |
| ☐    | Scan the QR code on the PCB.            | You will be prompted by the script to scan the QR code on the PCB label. | Partner’s Manufacturer<br><br>AFP2 software |
| ☐    | AFP2 prints the QR code label.          | Install the PCB in the product, placing the matching QR code label on the product body in an easily-scannable location. Ensure the label is permanently affixed to the product. | Partner’s Manufacturer              |
| ☐    | Repeat for each product.                | Run through the label printing for each product manufactured. | Partner’s Manufacturer              |

## 6. Run Validation Tests

These tests ensure that the QR label on the product is ready for the end-user to scan, and that the RSSI (Received Signal Strength Indicator) is acceptable.

Depending on what module you are using, BLE or BLE+Wi-Fi, you will run a different set of tests:

- [6.1 Validate QR Codes and BLE RSSI Levels](../FactoryProg#61-validate-qr-codes-and-ble-rssi-levels)
- [6.2 Validate QR Codes and BLE+Wi-Fi RSSI Levels](../FactoryProg#BLE+62-validate-qr-codes-and-blewi-fi-rssi-levels)

Follow the instructions that apply to your product’s radio module.

### 6.1 Validate QR Codes and BLE RSSI Levels

| ✓    | TASK                                               | DESCRIPTION                                                  | RESPONSIBILITY                  |
| :--- | :------------------------------------------------- | :----------------------------------------------------------- | :------------------------------ |
| ☐    | Set up factory test station(s).                    | Each factory station must have an Apple Mac mini with Afero RSSI Measurement software installed (included in AFP2), and connected to a QR code scanner. | Partner’s Manufacturer          |
| ☐    | Connect the product to power.                      | The BLE module will power on and start advertising.          | Partner’s Manufacturer          |
| ☐    | Run RSSI Measurement software.                     | While the software is running, go ahead and scan the QR code (the next step). | Partner’s Manufacturer          |
| ☐    | Scan the QR code on the product.                   | The QR code provides the Device ID and Association ID.       | Partner’s Manufacturer          |
| ☐    | Evaluate the results of RSSI Measurement software. | Afero RSSI Measurement software:<ul><li>Scans for BLE advertisement message w/Device ID from the QR code in the step directly preceding.</li><li>Validates that the QR code is readable and matches the firmware programmed on the PCB.</li><li>Measures RSSI and provides PASS/FAIL result confirming the RF performance.</li><li>If the result is PASS, it saves the RSSI measurement, Device ID, and Association ID to a `.afero` file. This file is comma-separated and is named:<p>`03_*ComputerName_ProgrammerName_YYYYMMDDTimeZone*.afero`<p>where `03` (the Factory File Type) indicates it was generated by the BLE RSSI test.</li><li>If the Device ID is not found or if the RSSI measurement is low, it will display a FAIL message.</li></ul> | Afero RSSI Measurement software |
| ☐    | Upload `.afero` files.                             | At the ***end\*** of each day of manufacturing, save the `.afero` files from each factory programming station and upload to the [Afero ExaVault server](https://afero.exavault.com/).<br><br>Multiple stations can be run in parallel and upload using the same ExaVault account credentials. | Partner’s Manufacturer          |
| ☐    | Afero Cloud validation.                            | Afero validates the contents of the `.afero` files and confirms partner PCBs have already been ingested into the Afero Cloud by double-checking against the `.afero` files uploaded after factory programming. | Afero                           |

### 6.2 Validate QR Codes and BLE+Wi-Fi RSSI Levels

| ✓    | TASK                                               | DESCRIPTION                                                  | RESPONSIBILITY                  |
| :--- | :------------------------------------------------- | :----------------------------------------------------------- | :------------------------------ |
| ☐    | Set up factory test station(s).                    | Each factory station must have a PC:with a serial portAfero RSSI Measurement software installed (included in AFP2)connected to a QR code scanner | Partner’s Manufacturer          |
| ☐    | Make the connections.                              | Power the module and connect the PC to the UART interface on the module:<br><br>**1**&nbsp;&nbsp;Connect 12 volts DC to the GND and VCC pins. Connect the Tx pin from a USB to Serial cable to the Rx pin on the connector:<p><img src="../img/FactProgRSSI.png" width="200" style="vertical-align:middle;margin:0px 0px;border:none"><br><br>**2**&nbsp;&nbsp;The PC will send a command over the Rx pin to the module to put it into factory test mode. In this mode, the module will become a Wi-Fi access point that the PC can scan for. | Partner’s Manufacturer          |
| ☐    | Run RSSI Measurement software.                     | While the software is running, go ahead and scan the QR code (the next step). | Partner’s Manufacturer          |
| ☐    | Scan the QR code on the product.                   | The QR code provides the Device ID and Association ID.       | Partner’s Manufacturer          |
| ☐    | Evaluate the results of RSSI Measurement software. | Afero RSSI Measurement software:<ul><li>Scans for BLE advertisement message w/Device ID from the QR code in the step directly preceding.</li><li>Validates that the QR code is readable and matches the firmware programmed on the PCB.</li><li>Measures RSSI and provides PASS/FAIL result confirming the RF performance.</li><li>If the result is PASS, it saves the RSSI measurement, Device ID, and Association ID to a `.afero` file. This file is comma-separated and is named:<p>`02_*ComputerName_ProgrammerName_YYYYMMDDTimeZone*.afero`<p>where `02` (the Factory File Type) indicates it was generated by the Wi-Fi RSSI test.</li><li>If the Device ID is not found or if the RSSI measurement is low, it will display a FAIL message.</li></ul> | Afero RSSI Measurement software |
| ☐    | Upload `.afero` files.                             | At the ***end\*** of each day of manufacturing, save the `.afero` files from each factory programming station and upload to the [Afero ExaVault server](https://afero.exavault.com/).<p><p>Multiple stations can be run in parallel and upload using the same ExaVault account credentials. | Partner’s Manufacturer          |
| ☐    | Afero Cloud validation.                            | Afero validates the contents of the `.afero` files and confirms partner PCBs have already been ingested into the Afero Cloud by double-checking against the `.afero` files uploaded after factory programming. | Afero                           |
