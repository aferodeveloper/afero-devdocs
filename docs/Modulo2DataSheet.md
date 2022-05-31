# Modulo-2 Data Sheet

Modulo-2 is a Wi-Fi and Bluetooth® low energy development board that makes it easy to prototype and build connected products used in conjunction with the Afero IoT Platform. It enables ultra-low power connectivity for data communication. The product integrates a Wi-Fi/Bluetooth® radio, RF front-end, a microcontroller running the Afero custom firmware, security IC, and crystal into a small package. This is an ideal solution for Internet of Things (IoT) applications.

This page contains the following sections:

- [Introduction](../Modulo2DataSheet#introduction)
- [Mechanical Specification](../Modulo2DataSheet#mechanical-specification)
- [DC Electrical Specification](../Modulo2DataSheet#dc-electrical-specification)
- [RF Electrical Specification](../Modulo2DataSheet#rf-electrical-specification)
- [Environmental Specification](../Modulo2DataSheet#environmental-specification)
- [Ordering Information](../Modulo2DataSheet#ordering-information)
- [Compliance Notices](../Modulo2DataSheet#compliance-notices)

## Introduction

### Modulo-2 Features

- Model name: Afero Modulo-2 Secure WLAN Radio Development Board
- Afero model number: Modulo-2
- WLAN type: 802.11 b/g/n 1x1
- WLAN radio frequencies: 2.4GHz
- Number of WLAN antennas: One (1)
- WLAN antenna configuration: Microstrip Monopole PCB antenna
- WLAN encryption: 64-bit WEP, 128-bit WEP, WPA-PSK, WPA2-PSK
- Bluetooth radio: Bluetooth® low energy wireless technology (4.0)
- Number of Bluetooth antennas: One (1) shared with WLAN
- Bluetooth antenna configuration: Shared with WLAN
- Bluetooth power level: 4dBm
- Maximum transmit power: 20 dBm
- Receive sensitivity: -85dBm
- Power consumption: <500 mW (average)
- Host interface: UART, SPI, 3.3V CMOS
- Security features: Cryptographic co-processor with secure, hardware-based key storage
- Interface pins: Four (4) multifunction IOs
- Power (VCC): 5VDC +10/-5%
- Operating temperature: 0 - +85C
- Storage temperature: -20 - +85C
- Operating humidity: 10-85% RH, non-condensing
- Storage humidity: 0-90% RH, non-condensing
- LED indicators: One (1)
- Buttons: One (1)
- Dimensions: 61.5mm x 17.8mm x 11.5mm (L x W x H)
- Weight (Earth ABL): 7g
- Package: 600 mil DIP form factor
- Compliance: FCC/IC, CE, TELEC
- Certifications: Bluetooth SIG

### Block Diagram

<img src="../img/Mod2Block1.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

## Mechanical Specification

### Mechanical Drawing

<img src="../img/Modulo-2MechDrawingRevA.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

### Antenna Keepout

<img src="../img/Modulo-2MechDrawingRevA.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

### Pin Definitions

| PIN #  | NAME         | DESCRIPTION                 | PIN #  | NAME         | DESCRIPTION                 |
| :----- | :----------- | :-------------------------- | :----- | :----------- | :-------------------------- |
| **1**  | GND          | Ground (Square Pin)         | **19** | IO0          | General Purpose I/O         |
| **2**  | NC           |                             | **20** | IO1          | General Purpose I/O         |
| **3**  | NC           |                             | **21** | IO2          | General Purpose I/O         |
| **4**  | NC           |                             | **22** | IO3          | General Purpose I/O         |
| **5**  | NC           |                             | **23** | HOST_SPI_CLK | SPI Clock                   |
| **6**  | NC           |                             | **24** | HOST_INT     | Host Interrupt Active LowNC |
| **7**  | NC           |                             | **25** | NC           |                             |
| **8**  | NC           |                             | **26** | IO4          | RESERVED                    |
| **9**  | HOST_TX_MOSI | Host UART Transmit          | **27** | IO5          | RESERVED                    |
| **10** | HOST_RX_MISO | Host UART Receive           | **28** | IO6          | RESERVED                    |
| **11** | NC           |                             | **29** | IO7          | RESERVED                    |
| **12** | HOST_CTS_SCS | Chip Select                 | **30** | NC           |                             |
| **13** | HOST_TX_MOSI | Host SPI Transmit           | **31** | RESET_B      | Reset Active Low            |
| **14** | HOST_RX_MISO | Host SPI Receive            | **32** | NC           |                             |
| **15** | VCC_3P3      | Internal 3V3 LDO Power Rail | **33** | NC           |                             |
| **16** | DBG_UART_TX  | UART Transmit               | **34** | NC           |                             |
| **17** | DBG_UART_RX  | UART Receive                | **35** | NC           |                             |
| **18** | GND          | Ground                      | **36** | VCC_USB      | External 5V USB Power Rail  |
|        |              |                             |        |              |                             |

<img src="../img/Mod2_MarkUp.jpg" width="350" style="vertical-align:middle;margin:0px 0px;border:none">

### Mechanical Characteristics

| ITEM       | TEST CONDITIONS                                |
| :--------- | :--------------------------------------------- |
| Assembly   | No defects of wiring, soldering and assembling |
| Appearance | No dirt, rust, corrosion, or foreign material  |
| Mass       | 7g +/- 2g                                      |
|            |                                                |

## DC Electrical Specification

### Absolute Maximum Rating

| DESCRIPTION     | MIN  | MAX  | UNIT |
| :-------------- | :--- | :--- | :--- |
| VIN             | -0.3 | +5.8 | V    |
| VCC             | -0.3 | +4.0 | V    |
| IVCC (3.3V Out) | 550  |      | mA   |

### Recommended Operating Conditions

| PARAMETER                            | MIN  | MAX  | UNIT |
| :----------------------------------- | :--- | :--- | :--- |
| Operating Temperature Range          | 0    | +85  | °C   |
| VIN                                  | +4.5 | +5.5 | V    |
| VCC (Supplied by internal regulator) | +3.0 | +3.6 | V    |
| IVCC (3.3V Out)                      |      | 100  | mA   |
|                                      |      |      |      |

### Standard Test Conditions

Ambient Conditions: 25C +/- 5C, 65%RH +/- 10%

Power Supply: VIN 5V +/- 5%

### Typical Power Consumption

Measurements taken from 12 VDC power supply.

| POWER CONSUMPTION                   | MIN  | TYP  | MAX  | UNIT |
| :---------------------------------- | :--- | :--- | :--- | :--- |
| TX Off                              |      | 495  |      | mW   |
| TX Mode (1Mbps 11b)                 |      | 1300 |      | mW   |
| TX Mode (11Mbps 11b)                |      | 1160 |      | mW   |
| TX Mode (6Mbps 11g)                 |      | 1200 |      | mW   |
| TX Mode (54Mbps 11g)                |      | 935  |      | mW   |
| TX Mode (MCS0 11n)                  |      | 1250 |      | mW   |
| TX Mode (MCS7 11n)                  |      | 765  |      | mW   |
| RX Mode                             |      | 475  |      | mW   |
| Typical Usage (appliance operation) |      | 350  |      | mW   |

### GPIO DC Characteristics

| SYMBOL       | PARAMETER                 | CONDITIONS          | MIN          | TYP  | MAX          | UNIT |
| :----------- | :------------------------ | :------------------ | :----------- | :--- | :----------- | :--- |
| VIL          | Low-Level Input Voltage   | All Pins            | -0.3         | -    | 0.3 x VVDDIO | V    |
| VIH          | High-Level Input Voltage  | All Pins            | 0.7 x VVDDIO | -    | VVDDIO + 0.3 | V    |
| VOL          | Low-Level Output Voltage  | All Pins            | -            | -    | .04          | V    |
| VOH          | High-Level Output Voltage | All Pins            | VVDDIO - 0.4 | -    | -            | V    |
| VHYS         | Hysteresis Voltage        | All Pins            | 0.15         | -    | -            | mV   |
| IOL          | Sink Current              | VOL = 0.4V          |              |      |              |      |
| SPI_CLK_UC   | -                         | -                   | 6            | mA   |              |      |
| All Others   | -                         | -                   | 2            |      |              |      |
| NRST         | -                         | -                   | 4            |      |              |      |
| IOH          | Source Current            | VOH = VVDDIO - 0.4V |              |      |              |      |
| SPI_CLK_UC   | -                         | -                   | -6           | mA   |              |      |
| All Others   | -                         | -                   | -2           |      |              |      |
| NRST         | -                         | -                   | -4           |      |              |      |
| IIL          | Low-Level Input Current   | Pull-up OFF         | -1           | -    | 1            | μA   |
| Pull-up ON   | 10                        | -                   | 50           |      |              |      |
| IIH          | High-Level Input Current  | Pull-down OFF       | -1           | -    | 1            | μA   |
| Pull-down ON | 10                        | -                   | 50           |      |              |      |
| RPULLUP      | Pull-up Resistor          | All Pins            | 70           | 100  | 140          | kΩ   |
| RPULLDOWN    | Pull-down Resistor        | All Pins            | 70           | 100  | 140          | kΩ   |

### Pin Description

| PIN # | PIN NAME | I/O  | PIN DESCRIPTION                       |
| :---- | :------- | :--- | :------------------------------------ |
| 1     | VCC      |      | Power In 5.0V +10/-5% or 12.0V +/-10% |
| 2     | RX       | I    | UART RX                               |
| 3     | TX       | O    | UART TX                               |
| 4     | GND      |      | Ground                                |

## RF Specification

Conditions: 25°C, VIN=5V, Parameters measured at RF connector.

### Wireless LAN Module Specifications

| WIRELESS LAN MODULE SPECIFICATIONS |                                                             |
| :--------------------------------- | ----------------------------------------------------------- |
| Model                              | Modulo-2                                                    |
| Frequency Range                    | 2400MHz to 2483MHz                                          |
| Output Power (Max)                 | IEEE 802.11b: 20dBm IEEE 802.11g: 17dBm IEEE 802.11n: 17dBm |

### RF Characteristics for IEEE802.11b

11Mbps mode unless otherwise specified.

| ITEMS                                      | CONTENTS          |      |      |      |
| :----------------------------------------- | :---------------- | :--- | :--- | :--- |
| Specification                              | IEEE802.11b       |      |      |      |
| Mode                                       | DSSS/CCK          |      |      |      |
| Channel frequency                          | 2400 – 2483 MHz   |      |      |      |
| Data rate                                  | 1, 2, 5.5, 11Mbps |      |      |      |
|                                            |                   |      |      |      |
| TX CHARACTERISTICS                         | MIN               | TYP  | MAX  | UNIT |
| Total radiated power level                 | 8                 | 10   | 18   | dBm  |
| Spectrum mask (HT20)                       |                   |      |      |      |
| 1st side lobes (to fc ±11 MHz)             |                   | -36  | -30  | dBr  |
| 2nd side lobes (to fc ±22MHz)              |                   | -55  | -50  | dBr  |
| Modulation accuracy (EVM)                  |                   | -18  |      | dB   |
| Frequency tolerance                        |                   | 5    |      | ppm  |
| RX CHARACTERISTICS                         | MIN               | TYP  | MAX  | UNIT |
| Minimum input level sens. (TIS, PER ≤ 10%) |                   | -85  |      | dBm  |

### RF Characteristics for IEEE802.11g

54Mbps mode unless otherwise specified.

| ITEMS                                      | CONTENTS                         |      |      |      |
| :----------------------------------------- | :------------------------------- | :--- | :--- | :--- |
| Specification                              | IEEE802.11g                      |      |      |      |
| Mode                                       | OFDM                             |      |      |      |
| Channel frequency                          | 2400 – 2483MHz                   |      |      |      |
| Data rate                                  | 6, 9, 12, 18, 24, 36, 48, 54Mbps |      |      |      |
|                                            |                                  |      |      |      |
| TX CHARACTERISTICS                         | MIN                              | TYP  | MAX  | UNIT |
| Total radiated power level                 | 8                                | 10   | 15   | dBm  |
| Spectrum mask                              |                                  |      |      |      |
| at fc ±11 MHz                              |                                  | -50  | -20  | dBr  |
| to fc ±20MHz                               |                                  | -55  | -28  | dBr  |
| to fc > = ±30MHz                           |                                  | -60  | -45  | dBr  |
| Constellation error (EVM)                  |                                  | -27  | -25  | dB   |
| Frequency tolerance                        |                                  | 5    |      | ppm  |
| Chip clock frequency tolerance             |                                  | 5    |      | ppm  |
| RX CHARACTERISTICS                         | MIN                              | TYP  | MAX  | UNIT |
| Minimum input level sens. (TIS, PER ≤ 10%) |                                  | -70  |      | dBm  |

### RF Characteristics for IEEE802.11n

MCS7 mode unless otherwise specified.

| ITEMS                                      | CONTENTS                                |      |      |      |
| :----------------------------------------- | :-------------------------------------- | :--- | :--- | :--- |
| Specification                              | IEEE802.11n – 2.4GHz                    |      |      |      |
| Mode                                       | OFDM                                    |      |      |      |
| Channel Frequency                          | 2400 – 2483MHz                          |      |      |      |
| Data rate                                  | 6.5, 13, 19.5, 26, 39, 52, 58.5, 65Mbps |      |      |      |
| TX CHARACTERISTICS                         | MIN                                     | TYP  | MAX  | UNIT |
| Total radiated power level                 | 3                                       | 5    | 10   | dBm  |
| Spectrum mask (HT20)                       |                                         |      |      |      |
| at fc ±11 MHz                              |                                         | -50  | -20  | dBr  |
| to fc ±20MHz                               |                                         | -55  | -28  | dBr  |
| to fc > = ±30MHz                           |                                         | -60  | -45  | dBr  |
| Constellation error (EVM)                  |                                         | -29  | -27  | dB   |
| Frequency tolerance                        |                                         | 5    |      | ppm  |
| Chip clock frequency tolerance             |                                         | 5    |      | ppm  |
| RX CHARACTERISTICS                         | MIN                                     | TYP  | MAX  | UNIT |
| Minimum input level sens. (TIS, PER ≤ 10%) |                                         | -68  |      | dBm  |

### Bluetooth® Low Energy Characteristics

| ITEMS               | MIN                   | TYP    | MAX   |
| :------------------ | :-------------------- | :----- | :---- |
| Specification       | Bluetooth® 4.0        |        |       |
| Mode                | Bluetooth® low energy |        |       |
| Frequency range     | 2400 - 2483MHz        |        |       |
| TX CHARACTERISTICS  |                       |        |       |
| Power               | -                     | 0dBm   | +4dBm |
| RX CHARACTERISTICS  |                       |        |       |
| Receive sensitivity | -                     | -93dBm | -     |

## Environmental Specification

### Absolute Maximum Rating

| DESCRIPTION           | MIN  | MAX  | UNIT |
| :-------------------- | :--- | :--- | :--- |
| Storage temperature   | -20  | 85   | °C   |
| Storage humidity      | 0    | 90   | %RH  |
| Operating temperature | 0    | 85   | °C   |
| Operating humidity    | 10   | 85   | %RH  |

### Recommended Operating Conditions

| PARAMETER                   | MIN  | MAX  | UNIT |
| :-------------------------- | :--- | :--- | :--- |
| Operating temperature range | 0    | 85   | °C   |

## Ordering Information

| MODULE   | PART NUMBER    | NOTES                 |
| :------- | :------------- | :-------------------- |
| Modulo-2 | A-FGS-00005-00 | Header pins installed |

## Compliance Notices

### FCC Class B Notice

This device complies with Part 15 of the FCC Rules. Operation is subject to the following two conditions:

1. This device may not cause harmful interference.
2. This device must accept any interference received, including interference that may cause undesired operation.

**&check; Note:** This equipment has been tested and found to comply with the limits for a Class B digital device, pursuant to Part 15 of the FCC Rules. These limits are designed to provide reasonable protection against harmful interference in a residential installation. This equipment generates, uses and can radiate radio frequency energy and, if not installed and used in accordance with the instructions, may cause harmful interference to radio communications. However, there is no guarantee that interference will not occur in a particular installation. If this equipment does cause harmful interference to radio or television reception, which can be determined by turning the equipment off and on, the user is encouraged to try to correct the interference by one or more of the following measures:

- Reorient or relocate the receiving antenna.
- Increase the separation between the equipment and receiver.
- Connect the equipment into an outlet on a circuit different from that to which the receiver is connected.
- Consult the dealer or an experienced radio/television technician for help.

Changes or modifications not expressly approved by the party responsible for compliance could void the user’s authority to operate the equipment.

In order to comply with FCC radio frequency (RF) exposure limits, antennas should be located at a minimum of 7.9 inches (20 cm) or more from the body of all persons.

### ICES Class B Notice – Avis NMB-003, Classe B

This Class B digital apparatus complies with Canadian ICES-003.

Cet appareil numérique de la classe B est conforme à la norme NMB-003 du Canada.

This device complies with Industry Canada’s licence-exempt RSSs. Operation is subject to the following two conditions: (1) This device may not cause interference; and (2) This device must accept any interference, including interference that may cause undesired operation of the device.

Le présent appareil est conforme aux CNR d’Industrie Canada applicables aux appareils radio exempts de licence. L’exploitation est autorisée aux deux conditions suivantes : (1) l’appareil ne doit pas produire de brouillage, et (2) l’utilisateur de l’appareil doit accepter tout brouillage radioélectrique subi, même si le brouillage est susceptible d’en compromettre le fonctionnement.

### EU Declaration of Conformity

Afero, Inc. declares that Modulo-2, PN A-PCB-00028-00, complies with the essential requirements and other relevant provisions of Directive 2014/53/EU. A copy of the Declaration of conformity is available on request.

Afero, Inc.
4970 El Camino Real
Suite 210
Los Altos, CA 94022

View letter stating [EU Declaration of Conformity (pdf)](../files/Modulo2-EU-Decl-Conform.pdf).

### RoHS

This device conforms to RoHS (Restriction of Hazardous Substances) European Union regulations that set maximum concentration limits on hazardous materials used in electrical and electronic equipment.

### WEEE

This product has been supplied from an environmentally aware manufacturer that complies with the Waste Electrical and Electronic Equipment (WEEE) Directive 2002/96/EC. The “crossed-out wheelie bin” symbol is placed on this product to encourage you to recycle wherever possible.

 **&#8674;** *Next:* [ASR-2 Hardware Integration Guide](../ASR2HWInt)