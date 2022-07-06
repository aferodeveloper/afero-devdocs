# Customize QR Code Labels

The QR code contains an Association ID that is used by the Afero mobile app to onboard the device. It is critical that the correct QR code label be physically applied to the module that generated it.

Every label must include the following two pieces of information:

- QR code, and
- Plain-text version of the Association ID

For more technical QR code information, please refer to the [QR Codes for Afero Products](../QRCodes) page.

The AFP2 Factory Provisioning package includes `.json` files that define the elements of the QR code + text label. (These files are found in the directory `\targets\*ProgrammerName*\labels\`.) The reference code in these files is compatible with Zebra printers (ZPL format) and is commented for ease of use.

**By default, the following configuration is defined in the reference code:**

- **Standard label sizes:** 1.5-inch x 0.5-inch (module label) in 300dpi, and 45x30mm (customer-visible label) in 300dpi
- **Supported 300dpi printers:** Zebra 105SL Plus or Zebra GX430T
- **Module label includes:** P/N (part number) and FCCID, which must be filled out by the customer/manufacturer

If the print job settings require customization, the operator can do that in the correct `.json` file within the `\targets\*ProgrammerName*\labels\` directory:

```
print components
description (select which components are to be printed here) True or False
aId (association ID) TRUE (Mandatory)
dId (device ID) TRUE (Mandatory)
qr (QR code) TRUE (Mandatory)
fcc (FCC ID) (Optional)
pn (Part Number) (Optional)
blobs (Optional Graphical elements such as lines or logos)

printer
description (Printer and Printing Configuration)
printer_dpi (600/300/200)
v_align_dots (Used to fine tune vertical placement. Value from -120 to 120)
h_align_dots (Used to fine tune vertical placement. Value from -120 to 120)
print_darkness (Used to set print darkness. Value from 0-30)
print_rate (Inches per minute. Value from 1-4)
copies (Number of copies to print)

label
description (Information on label loaded in the printer)
inches
    width_inches (Width of label in inches)
    height_inches (Height of label in inches)

qr
description (Code information)
magnification (Size of printed QR code. Value from 1-10)
x_coord (Coordinate of the left edge of the QR code in DOTS)
y_coord (Coordinate of the bottom edge of the QR code in DOTS)

aId
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
