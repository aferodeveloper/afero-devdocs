# QR Codes for Afero Products

This page contains the following information about the QR code labels required for Afero products. You will need this information in the prototyping and manufacturing processes.

- [Labeling Afero Products with QR Codes](../QRCodes#labeling-afero-products-with-qr-codes)
    - [Large QR Code Labels](../QRCodes#large-qr-code-labels)
    - [Small QR Code Labels](../QRCodes#small-qr-code-labels)
    - [Manufacturing Process](../QRCodes#manufacturing-process)
    - [Notes on Label Placement](../QRCodes#notes-on-label-placement)
    - [Customizing the Label for Printing](../QRCodes#customizing-the-label-for-printing)
- [Generating QR Codes for Afero Products](../QRCodes#generating-qr-codes-for-afero-products)
    - [Products Using the Latest QR Code Version](../QRCodes#products-using-the-latest-qr-code-version)
        - [QR Payload Details](../QRCodes#qr-payload-details)
        - [QR Code Details](../QRCodes#qr-code-details)
        - [Plain-Text Association ID](../QRCodes#plain-text-association-id)
        - [Latest Code Examples](../QRCodes#latest-code-examples)
    - [Products Using the Legacy QR Code Version](../QRCodes#products-using-the-legacy-qr-code-version)
        - [About ASR-1 Association ID](../QRCodes#about-the-asr-1-association-id)
        - [Label Printing](../QRCodes#label-printing)
        - [Legacy Code Examples](../QRCodes#legacy-code-examples)

## Labeling Afero Products with QR Codes

Each Afero smart product requires two QR code labels:

- A customer-visible QR code label on the product, described further in [Large QR code Label](../QRCodes#large-qr-code-labels) below.
- A smaller QR code label on the PCB, described further in [Small QR Code Label](../QRCodes#small-qr-code-labels), also below.

### Large QR Code Label

During product onboarding, customers scan the QR code label using the Afero mobile app. This adds the customer’s smart device to the customer’s Afero account, and the product icon and name appear on the Home screen of the mobile app as an entry point for device control.

This use requires the following of the label:

- Must be easily visible and in an area on the product with good lighting (no shadows).
- Dimensions are 45x30mm.
- Contains by default the QR code and Association ID.
- Any additional label text/images can be pre-printed by the label manufacturer.
- The black QR code and Association ID are device-specific and are printed after the factory programming script runs, and after the small QR code is scanned.

An example large QR code label is illustrated below:

<img src="../img/QRCode-Large.png" width="400" style="vertical-align:middle;margin:0px 0px;border:none">

### Small QR Code Label

The smaller label is attached to the product PCB that is running the Afero firmware:

- Dimensions are 1.0 x 0.5 inches.
- Contains by default the QR code, Association ID, Device ID, and FCC ID.
- Placement on the module assists with the manufacturing flow.
- Serves as a backup identifier if the larger label has an issue.

An example small QR code label is illustrated below:

<img src="../img/QRCode-Small.png" width="300" style="vertical-align:middle;margin:0px 0px;border:none">

### Manufacturing Process

The Afero Factory Programmer (AFP2) prints both the large and small QR code labels.

- The small QR code label is printed as part of the PCB factory programming.
- The large QR code label is printed in a later step, when running the label printing script.

The Afero Factory Test verifies that the QR code labels are printed correctly.

### Notes on Label Placement

- Both labels must be designed to adhere very well to the PCB and product body.
- The label sizes identified in the sections above are standard sizes selected for cost savings reasons.
- Any changes from Afero standard requires writing updated Zebra printer language for the new label layout/size. Read the next section, [Customizing the Label for Printing](../QRCodes#CustomLabel), for details.

For more technical details of our QR codes, read the section below, [Generating QR Codes for Afero Products](../QRCodes#QRCodeGen).

### Customizing the Label for Printing

The AFP2 package includes .json files that define the elements of the QR code plus text label. The reference code in these files is compatible with Zebra printers (ZPL format) and is commented for ease of use.

By default, the following configuration is defined in the reference code:

- Standard label sizes: 1.5-inch x 0.5-inch (module label) in 300dpi, and 45x30mm (customer-visible label) in 300dpi
- Supported 300dpi printers: Zebra 105SL Plus or Zebra GX430T
- Module label includes P/N (part number) and FCCID, which must be filled out by the customer/manufacturer

If the print job settings require customization, the operator can do that in the correct .json file, found in the Factory Programmer package, `targets\labels\` directory. An abbreviated file shown below:

```
Print Components
  description (select which components are to be printed here) True or False
  aId (association ID) TRUE (Mandatory)
  dId (device ID) TRUE (Mandatory)
  qr (QR code) TRUE (Mandatory)
  fcc (FCC ID) (Optional)
  pn (Part Number) (Optional)
  blobs (Optional Graphical elements such as lines or logos)

Printer
  description (Printer and Printing Configuration)
  printer_dpi (600/300/200)
  v_align_dots (Used to fine tune vertical placement. Value from -120 to 120)
  h_align_dots (Used to fine tune vertical placement. Value from -120 to 120)
  print_darkness (Used to set print darkness. Value from 0-30)
  print_rate (Inches per minute. Value from 1-4)
  copies (Number of copies to print)

Label
  description (Information on label loaded in the printer)
  inches
  width_inches (Width of label in inches)
  height_inches (Height of label in inches)

QR code
  description (Code information)
  magnification (Size of printed QR code. Value from 1-10)
  x_coord (Coordinate of the left edge of the QR code in DOTS)
  y_coord (Coordinate of the bottom edge of the QR code in DOTS)

Assn ID
  description (Association ID information)
  font_type (Zebra built in fonts: A,B,D E,F,G,H,O,P,Q,R,S,T,U,V)
  font_width (Printed width of the font in DOTS)
  font_height (Printed height of the font in DOTS)
  line_spacing (Spacing between the two lines of association ID)
  char_chunk_size (Number of characters in each printed chunk)
  chunks_per_line (Number of character chunks per line)
  hyphen_font_type (Zebra built in fonts: A,B,D E,F,G,H,O,P,Q,R,S,T,U,V)
  hyphen_font_width (Printed width of each font in DOTS)
  hyphen_font_height (Printed height of each font in DOTS)
  x_coord: (x coordinate of the left edge of the association ID in DOTS)
  y_coord: (y coordinate of the bottom edge of the association ID in DOTS)
```

## Generating QR Codes for Afero Products

This section contains instructions for generating the QR codes that will be printed on the labels, for:

- [Products using the latest QR code version](../QRCodes#products-using-the-latest-qr-code-version), and
- [Products Using the legacy QR code version](../QRCodes#products-using-the-legacy-qr-code-version)

Unless explicitly instructed otherwise, assume you will be generating the latest QR code version.

### Products Using the Latest QR Code Version

The current QR code for an Afero enabled device contains three pieces of information:

- Payload Version Number
- Association ID
- Device ID

There is a colon delimiter character (:) between each piece of information. We will refer to these three pieces of information (including the delimiters) as the “Payload”.

#### QR Payload Details

|                        | DATA SIZE (BYTES) | DATA ENCODING WITHIN PAYLOAD | NUMBER OF ALPHANUMERIC CHARACTERS (AFTER ENCODING) | EXAMPLE DATA             | NOTES                                                        |
| :--------------------- | :---------------- | :--------------------------- | :------------------------------------------------- | :----------------------- | :----------------------------------------------------------- |
| Payload Version Number | 0.5 (4 bits)      | HEXIDECIMAL                  | 1                                                  | 1                        | The only valid Payload Version Number is 1. Other values are reserved for future use and are not valid. |
| Association ID         | 12                | HEXIDECIMAL                  | 24                                                 | ABCDEF1234567890ABCDEF12 |                                                              |
| Device ID              | 8                 | HEXIDECIMAL                  | 16                                                 | 1234567890ABCDEF         |                                                              |

#### QR Code Details

When using the Payload to generate a QR code, the following QR code parameters should be used. (Additional information about QR codes can be found at https://en.wikipedia.org/wiki/QR_code.)

| PARAMETER                                           | REQUIRED VALUE  | NOTES                                                        |
| :-------------------------------------------------- | :-------------- | :----------------------------------------------------------- |
| QR Code Version                                     | 3               | Version 3 QR codes are 29x29 modules in size.                |
| Error Correction                                    | Q               | Quartile-level error correction allows for 25% of the codewords to be restored. |
| Input Mode                                          | Alphanumeric    | Supports all hex-encoded characters (0-9, A-F) as well as the colon (:) delimiter. |
| QR Code Minimum Print Dimensions (excluding border) | 17.19 x 17.19mm | The minimum printed dimensions ensures high-reliability scanning by a variety of smartphone cameras in differing light conditions. The minimum dimensions of 17.19mm2 has a size of 0.593mm2 per module within the QR code. |
| Minimum QR Border Thickness                         | 0.593mm         | Best practice is to leave a border around the QR code that is at least one module thick. In the case of a QR code that is 17.19mm2, each module within the QR code is 0.593mm2; therefore, the minimum border thickness around the QR code should also be 0.593mm. |

#### Plain-Text Association ID

In addition to a QR code (containing the Payload) each label is also required to contain a plain-text version of the Association ID. This can be used for manual-entry by a user in the case that the QR code is unusable (e.g., QR code is damaged, camera on phone is not working). The plain-text ID consists of the 24 characters of the hex-encoded Association ID broken up into six distinct four-character blocks. Each character block is separated by a hyphen.

- **Association ID (formatted for printing as plain-text):** ABCD-EF12-3456-7890-ABCD-EF12
- **Font Type:** Monospaced (such as Consolas or Courier) for all Association ID characters. A proportional font (such as Times New Roman) may be used for the hyphens.

#### Latest Code Examples

| **Example QR Payload**                | 1:ABCDEF1234567890ABCDEF12:1234567890ABCDEF                  |
| ------------------------------------- | ------------------------------------------------------------ |
| **Example QR Code**                   | <img src="../img/BigAssnQRcode.png" width="115" style="vertical-align:middle;margin:0px 0px;border:none"> |
| **Example Plain Text Association ID** | ABCD-EF12-3456-7890-ABCD-EF12                                |
| **Example Label**                     | <img src="../img/BigAssnQRcodeLabel.png" width="375" style="vertical-align:middle;margin:0px 0px;border:none"> |


### Products Using the Legacy QR Code Version

Older ASR-1 modules use a QR code that encodes only one piece of information: the ASR-1 Association ID.

#### About the ASR-1 Association ID

- **Association ID size:** 6 bytes
- **Character encoding for QR code:** HEX
- **Number of characters (after encoding):** 12

Here is an example of one Association ID in its various formats:

- **Binary:** 0001 0010 0011 0100 0101 0110 0001 0010 0011 0100 0101 0110
- **Hex:** 0x123456123456
- **Formatted for printing as plain-text:** 1234-5612-3456

#### Label Printing

Every label must include the following two pieces of information: 1) the QR code, and 2) a plain-text version of the Association ID. The code can optionally include other information, such as an FCC ID.

**1.** QR Code Generation

The following parameters should be used when generating a QR code:

- **Version:** 1 (21x21)
- **Error Connection:** Q (quartile)
- **Input Mode:** Alphanumeric

**2.** Plain-Text Association ID

The plain-text Association ID is used for manual-entry of the Association ID by a user in the case that the QR code is unusable (e.g., QR code is damaged, camera on phone is not working). The plain-text Association ID consists of the 12 characters of the hex-encoded Association ID broken into three distinct four-character blocks. Each character block is separated by a hyphen.

- **Association ID (hex):** 0x123456123456
- **Association ID (formatted for printing as plain-text):** 1234-5612-3456
- **Font Type:** Monospaced (such as Consolas or Courier) for all Association ID characters. A proportional font (such as Times New Roman) may be used for the hyphens.

#### Legacy Code Examples

<img src="../img/ASR1-ExampleQRcodes.png" width="800" style="vertical-align:middle;margin:0px 0px;border:none">



 **&#8674;** *Next:* [Hardware Design Files](../DataSheets)