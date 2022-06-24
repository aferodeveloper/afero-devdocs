# Useful Debugging Methods

This section includes a couple useful methods that you can copy/paste into your code. You can see these methods used in the afBlink example.

<mark>**Note:**&nbsp; Due to limitations in available memory, if you’re working with an Arduino Uno, it’s not likely you’ll be able to use these methods fully. If it’s possible for you to use a board with more memory, such as a Teensy, you’ll have a great deal more freedom, including the use of these debug methods. If you are restricted to an Uno or system with similar available memory, you may be able to include a subset of these calls, and get more reliable performance by reducing the ATTR_PRINT_MAX_VALUE_LEN.</mark>

```
#define ATTR_PRINT_HEADER_LEN     60
#define ATTR_PRINT_MAX_VALUE_LEN  512   // Each byte is 2 ASCII characters in HEX.
#define ATTR_PRINT_BUFFER_LEN     (ATTR_PRINT_HEADER_LEN + ATTR_PRINT_MAX_VALUE_LEN)

char attr_print_buffer[ATTR_PRINT_BUFFER_LEN];

void getPrintAttrHeader(const char *sourceLabel, const char *attrLabel, const uint16_t attributeId, const uint16_t valueLen) {
    memset(attr_print_buffer, 0, ATTR_PRINT_BUFFER_LEN);
    snprintf(attr_print_buffer, ATTR_PRINT_BUFFER_LEN, "%s id: %s len: %05d value: ", sourceLabel, attrLabel, valueLen);
}

void printAttrBool(const char *sourceLabel, const char *attrLabel, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    getPrintAttrHeader(sourceLabel, attrLabel, attributeId, valueLen);
    if (valueLen > 0) {
        strcat(attr_print_buffer, *value == 1 ? "true" : "false");
    }
    Serial.println(attr_print_buffer);
}

void printAttr8(const char *sourceLabel, const char *attrLabel, const uint8_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    getPrintAttrHeader(sourceLabel, attrLabel, attributeId, valueLen);
    if (valueLen > 0) {
        char intStr[6];
        strcat(attr_print_buffer, itoa(*((int8_t *)value), intStr, 10));
    }
    Serial.println(attr_print_buffer);
}

void printAttr16(const char *sourceLabel, const char *attrLabel, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    getPrintAttrHeader(sourceLabel, attrLabel, attributeId, valueLen);
    if (valueLen > 0) {
        char intStr[6];
        strcat(attr_print_buffer, itoa(*((int16_t *)value), intStr, 10));
    }
    Serial.println(attr_print_buffer);
}

void printAttr32(const char *sourceLabel, const char *attrLabel, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    getPrintAttrHeader(sourceLabel, attrLabel, attributeId, valueLen);
    if (valueLen > 0) {
        char intStr[11];
        strcat(attr_print_buffer, itoa(*((int32_t *)value), intStr, 10));
    }
    Serial.println(attr_print_buffer);
}

void printAttrHex(const char *sourceLabel, const char *attrLabel, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    getPrintAttrHeader(sourceLabel, attrLabel, attributeId, valueLen);
    for (int i = 0; i < valueLen; i++) {
        strcat(attr_print_buffer, String(value[i], HEX).c_str());
    }
    Serial.println(attr_print_buffer);
}

void printAttrStr(const char *sourceLabel, const char *attrLabel, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    getPrintAttrHeader(sourceLabel, attrLabel, attributeId, valueLen);
    int len = strlen(attr_print_buffer);
    for (int i = 0; i < valueLen; i++) {
        attr_print_buffer[len + i] = (char)value[i];
    }
    Serial.println(attr_print_buffer);
}


void printAttribute(const char *label, const uint16_t attributeId, const uint16_t valueLen, const uint8_t *value) {
    switch (attributeId) {
        case AF_BLINK:
            printAttrBool(label, "AF_BLINK", attributeId, valueLen, value);
            break;

        case AF_MODULO_LED:
            printAttr16(label, "AF_MODULO_LED", attributeId, valueLen, value);
            break;

        case AF_GPIO_0_CONFIGURATION:
            printAttrHex(label, "AF_GPIO_0_CONFIGURATION", attributeId, valueLen, value);
            break;

        case AF_MODULO_BUTTON:
            printAttr16(label, "AF_MODULO_BUTTON", attributeId, valueLen, value);
            break;

        case AF_GPIO_3_CONFIGURATION:
            printAttrHex(label, "AF_GPIO_3_CONFIGURATION", attributeId, valueLen, value);
            break;

        case AF_BOOTLOADER_VERSION:
            printAttrHex(label, "AF_BOOTLOADER_VERSION", attributeId, valueLen, value);
            break;

        case AF_SOFTDEVICE_VERSION:
            printAttrHex(label, "AF_SOFTDEVICE_VERSION", attributeId, valueLen, value);
            break;

        case AF_APPLICATION_VERSION:
            printAttrHex(label, "AF_APPLICATION_VERSION", attributeId, valueLen, value);
            break;

        case AF_PROFILE_VERSION:
            printAttrHex(label, "AF_PROFILE_VERSION", attributeId, valueLen, value);
            break;

        case AF_SYSTEM_ASR_STATE:
            printAttr8(label, "AF_SYSTEM_ASR_STATE", attributeId, valueLen, value);
            break;

        case AF_SYSTEM_LOW_POWER_WARN:
            printAttr8(label, "AF_ATTRIBUTE_LOW_POWER_WARN", attributeId, valueLen, value);
            break;

        case AF_SYSTEM_REBOOT_REASON:
            printAttrStr(label, "AF_REBOOT_REASON", attributeId, valueLen, value);
            break;

        case AF_SYSTEM_LINKED_TIMESTAMP:
            printAttr32(label, "AF_SYSTEM_LINKED_TIMESTAMP", attributeId, valueLen, value);
            break;
    }
}
```

Here’s a brief example usage snippet:

```
void attrEventCallback(const af_lib_event_type_t eventType,
                       const af_lib_error_t error,
                       const uint16_t attributeId,
                       const uint16_t valueLen,
                       const uint8_t* value) {
    printAttribute("attrEventCallback", attributeId, valueLen, value);
    // snip //
}
```

<strong>&#8674;</strong> <em>Next:</em>&nbsp;&nbsp;[Handling MCU OTA Updates](../MCU-OTA)