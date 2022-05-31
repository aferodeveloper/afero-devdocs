# Device Attribute RAM Size

Your Profile’s attributes are allocated a limited amount of RAM, the limit being a function of which module you are using for your project. To quickly see how much RAM your Profile’s attributes are using, check the PROFILE RAM USAGE indicator at the top of the Profile Editor Attributes window (read more in [Attribute Sizing](../AttrDef#attribute-sizing)). But if you want *details*, read the sections below.

## Calculating Buffer Length

The RAM required by any attribute is affected by the size and number of buffers to hold attribute data.

The buffer length, in bytes, is calculated as follows:

```
unit_buffer_length = max(max_attribute_value_byte_length, 4)
```

This equation reflects a requirement to align our buffers on four-byte boundaries.

### Basic Attribute Value-Buffer Length

For every attribute we maintain an **actual** and **desired** value, each value requiring a buffer. Total RAM required for attribute value-buffers is thus twice the unit_buffer_length:

```
buffer_length = (max(max_attribute_value_byte_length, 4) * 2)
```

Below are a couple examples of calculating total RAM requirement for attributes.

- For an MCU attribute of type SINT8, the value-buffer length is:<br>
  `(max(1, 4) * 2) = 8 bytes`
- For an MCU attribute of type SINT64, the value-buffer length is:<br>
  `(max(8, 4) * 2) = 16 bytes`

### Latched Attribute Value-Buffer Length

Latched attributes require an additional two buffers, so the formula is:

```
latched_value_buffer_length = (max(max_value_byte_length, 4) * 4)
```

As an example, for a latched SINT8 attribute, the value-buffer length is:
`(max(1, 4) * 4) = 16 bytes`

### Queued Attributes

Queued attributes use more RAM than basic attributes. The additional RAM required for a queued attribute is comprised of a header plus a queue-ful of 'elements':

- HEADER: 32 bytes for the structure that holds queued state/settings.
- ELEM_SIZE: Queue elements come in two types ('normal' or timestamped), and the element size depends on the type:
    - For a normal queued attribute: `ELEM_SIZE = max_attribute_value_byte_length + 6 bytes`
    - For a timestamped queued attribute: `ELEM_SIZE = max_attribute_value_byte_length + 16 bytes`

Thus, the total *additional* RAM taken by any attribute that is queued is:

```
queued_attribute_supplementary_length = HEADER + (ELEM_SIZE * maximum elements in queue)
```

## Calculating Profile Size

The general formula for calculating the size of a Profile in RAM is:

```
Profile Size = Profile Overhead + sum((attribute overhead + value_buffer_length) for each attribute)
```

This general formula can be used for any MCU attribute.

GPIO attributes (both unlatched and latched) have an internal structure that affects memory usage. These cases are all detailed in the table below:

| OBJECT                           | SIZE                                       | NOTES                                                        |
| :------------------------------- | :----------------------------------------- | :----------------------------------------------------------- |
| Profile Overhead                 | 16 bytes                                   | Version, attribute count, pointers to other structs.         |
| Attribute Overhead               | 28 bytes                                   | This is the general attribute overhead for all attribute types. It includes flags, values, and other pointers. |
| Attribute Overhead (latched)     | Basic Attribute Overhead + 16 = 44 bytes   | Latched attributes require additional fields.                |
| Attribute Overhead (queued)      | 28 + queued_attribute_supplementary_length | Queued attributes require additional overhead.               |
| Attribute Overhead (timestamped) | 8                                          | Timestamped attributes require additional overhead.          |
| MCU Attribute                    | Attribute Overhead + value_buffer_length   | Examples:<p>Size for a SINT8 or BOOLEAN attribute = 28 bytes overhead + 8 bytes value attribute length = 36 bytes.<p>Size for a 255 byte UTF8S or BYTES attribute = 28 bytes overhead + 510 bytes value attribute length = 538 bytes. |
| GPIO Attribute (non-latched)     | 80 bytes                                   | GPIO attributes are actually two attributes that have a fixed length. The value attribute is SINT16 and the configuration attribute is SINT64.<br><br>Thus, the total size for a non-latched GPIO attribute is:<br>(8 bytes value attribute length + 28 bytes overhead) + <br>(16 bytes config attribute length + 28 bytes overhead) = 80 bytes |
| GPIO Attribute (latched)         | 104 bytes                                  | Latched GPIOs have a larger per-attribute overhead plus two more value buffers:<p>(16 bytes latched value attribute length + 44 bytes overhead, latched) +<br>(16 bytes config attribute length + 28 bytes overhead) = 104 bytes |

## Calculating Maximums

If you are building a Profile and need to calculate the amount of RAM required for your attributes, the formula is:

```
(RAM Available for Profile - Profile Overhead) / Attribute Total Bytes
```

The total Profile size is based on attribute type(s) as well as the size of the attribute value(s).

### Examples

Given that the RAM available for Profiles is 4kB, we can calculate the maximum number of attributes a Profile can contain:

**GPIOs**

- If your Profile consists of four non-latched GPIO attributes, the total space required is:
  `16 + (4 * 80) = 336 bytes`
- If all the attributes are latched, the total space increases to:
  `16 + (4 * 104) = 432 bytes`

**Small Values**

- If your Profile consists of attributes of size: BOOLEAN, SINT8, SINT16, SINT32, or FIXED_15_16, the maximum number of attributes is:
  `(4096 - 16) / 36 = 113 attributes`

**Large Values**

- If your Profile consists of all strings or byte arrays of the maximum size (255 bytes), then the maximum number of attributes is:
  `(4096 - 16) / 538 = 7 attributes`