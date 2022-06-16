# ASR-2 Hardware Integration with the Murata Type1LD BLE+Wi-Fi Module

This page describes what you must do for your connected hardware IoT project to work as an edge device in the Afero IoT ecosystem. The ecosystem includes the edge device, the Afero Secure Hub, the Afero mobile app, and the Afero Cloud. This information is also available as a[ Tech Note (.pdf file)](../files/TechNote-Integrate-MurataWiFiBLE-Afero.pdf).

The instructions that follow use the following components:

- Bluetooth® low energy (BLE) + Wi-Fi combo Module (LBEE5PA1LD)
- Afero specific Microchip CryptoAuthentication™ Security Module (ATECC508A-MAHKN)
- SPI flash memory (W25Q32JVSSIQ)

Ordering information is provided below in the [Key Components List](../ASR2HWInt#key-components-list) section.

## Components

The block diagram below illustrates the components required for this integration:

<img src="../img/Murata-Components-BlockDiag.jpg" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

Each component is described in more detail below.

### Murata Type1LD BLE+Wi-Fi Combo Module

Wi-Fi and BLE enable the “internet” in the Internet of Things (IoT). Wi-Fi provides the backhaul to the data from the edge device, and BLE is used to onboard the device and also as a backhaul when a hub is involved.

The Murata LBEE5PA1LD (1LD) combo module provides an embedded wireless LAN and Bluetooth network controller solution, ideal for IoT applications. It is enabled with Cypress Wireless Internet Connectivity for Embedded Devices (WICED) architecture, hosting the Wi-Fi, TCP/IP network stack, security supplicant, and other network application features.

WICED is a Cypress development system that vastly reduces the effort required to add wireless connectivity to embedded devices. For more information, go to the [Murata product page for the Type 1LD module](https://wireless.murata.com/products/rf-modules-1/wi-fi-bluetooth/type1ld.html).

Afero designed the following reference schematic with Murata. (Click to open a [.pdf file of the schematic](../files/1LD_Afero_20181023.pdf).)

<img src="../img/1LD_Afero_20181023.png" width="1000" style="vertical-align:middle;margin:0px 0px;border:none">

### Afero Security Module

Afero uses a security module that stores the keys/certificates required to securely communicate between the Afero Cloud/Afero Secure Hub and the edge device. We integrate this module over I2C.

The Microchip ATECC508A integrates ECDH (Elliptic Curve Diffie Hellman) security protocol, an ultra-secure method providing key agreement for encryption/decryption, along with ECDSA (Elliptic Curve Digital Signature Algorithm) sign-verify authentication for IoT edge devices. The ATECC508A includes an EEPROM array, which can be used for storing security configurations.

Kindly [reach out to Afero](https://www.afero.io/html/home/contact-afero.html) before mass production procurement since Afero must provide authorization to purchase this module. For more information, please read the [Microchip ATECC508A Data Sheet](http://ww1.microchip.com/downloads/en/DeviceDoc/20005928A.pdf).

### Winbond Serial Flash

Afero uses serial flash to execute the code directly from Dual/Quad SPI Flash. This is also used to store data shadowing the contents of the RAM. The flash is a critical component for Over-the-Air (OTA) updates; for an Afero OTA to work, we need a flash, such as the one mentioned below (W25Q32JVSSIQ).

The SPI Flash array is organized into 16,384 programmable pages of 256-bytes each. Up to 256 bytes can be programmed at a time. Pages can be erased in groups of 16 (4KB sector erase), groups of 128 (32KB block erase), groups of 256 (64KB block erase), or the entire chip (chip erase).

For more information, please read the [Winbond W25Q32JV SPI Flash Data Sheet](https://www.winbond.com/resource-files/w25q32jv spi revc 08302016.pdf).

Afero firmware uses certain sets of pins for SPI, I2C, and debug UART. We expect our customers to use the same design as this would immensely minimize firmware customization needs. Please request a schematic and schematic review by submitting a JIRA ticket to the Afero Customer Enablement (ACE) team.



## Antenna Design Recommendations

If customers want to leverage Murata regulatory filings, customers need to follow the recommended antenna layout that Murata calls out in the 1LD datasheet. If that antenna does not work for the customer application, please consult with Murata for assistance.

In general, the following design recommendations are good best practices when not following specific recommendations from a manufacturer:

- Antenna impedance should be as close as possible to 50 ohm and have a return loss of -10dB or better.
- Place a Pi network placeholder on the RF trace during the first build. Once boards are available and matching is done, remove the unwanted matching components to reduce BOM cost.
- Measure antenna gain on the actual design to confirm it fits with Murata filings.

## Programming the Murata Module

Creating a programming port to this module is essential during development and production. We use Segger J-link to flash a firmware image to the Murata module. For details on programming the module, please refer to the Product Specification, “Afero Factory Programmer, Cypress WICED Based Modules, Equipment and Setup”, PN D-SPC-00007-00. Please [contact Afero](https://www.afero.io/html/home/contact-afero.html) for a copy of the specification.

Consider the following for mass production:

- You may decide to expose programming pins on an internal circuit board, or on the outside of your product’s enclosure. Your final design should consider the overall cost of production and the service life of the product.
- The method of programming mentioned above does take some time and is slower. For mass production, programming the SPI flash and MCU in parallel greatly reduces the time but adds significant complexity as it requires more test points and two SEGGER J-Links. Once the product is ready for production, refer to the “Afero Factory Programmer, Cypress WICED Based Modules, Equipment and Setup” specification referenced above.

## FCC Certification

Radio Frequency (RF) devices are required to be properly authorized under 47 CFR part 2 prior to being marketed or imported into the United States. The Office of Engineering and Technology (OET) administers the equipment authorization program. This program is one of the principal ways the Commission ensures that RF devices used in the United States operate effectively without causing harmful interference and otherwise comply with the Commission’s rules. All RF devices subject to equipment authorization must comply with the Commission’s technical requirements prior to importation or marketing.

For information on FCC certification steps, go to the [FCC Equipment Authorization webpage](https://www.fcc.gov/engineering-technology/laboratory-division/general/equipment-authorization).

Other regions have their own regulatory requirements. Please check with your Regulatory and Compliance Engineer for direction on meeting these requirements.

The Murata 1LD Wi-Fi/BLE module has FCC-approval and other regulatory certifications with their reference and antenna design. For more information, refer to the Appendix of the [Murata datasheet](https://wireless.murata.com/datasheet?/RFM/data/type1ld.pdf).

## Key Components List

| DESCRIPTION                                                  | MANUFACTURER | VALUE | QUANTITY | SUPPLIER PART # | SUPPLIER INFO                                                |
| :----------------------------------------------------------- | :----------- | :---- | :------- | :-------------- | :----------------------------------------------------------- |
| Afero specific Microchip CryptoAuthentication Security Module | Microchip    | I2C   | 1        | ATECC508A-MAHKN | [Afero](https://www.afero.io/html/home/contact-afero.html)   |
| Bluetooth low energy (BLE) + Wi-Fi combo Module              | Murata       | IC    | 1        | LBEE5PA1LD      | [Digi-Key](https://www.digikey.com/catalog/en/partgroup/lbee5pa1ld/89234) |
| SPI flash memory                                             | Winbond      | SPI   | 1        | W25Q32JVSSIQ    | [Digi-Key](https://www.digikey.com/product-detail/en/winbond-electronics/W25Q32JVSSIQ-TR/W25Q32JVSSIQCT-ND/7393546) |

 **&#8674;** *Next:*&nbsp;&nbsp;[QR Codes for Afero Products](../QRCodes)