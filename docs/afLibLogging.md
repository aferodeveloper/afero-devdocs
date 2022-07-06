# Console Logging

afLib provides a set of functions to support logging to the serial console. For Arduino, use of `af_logger` is optional; existing Arduino Serial debugging methods will still work as they always have. When porting afLib to other MCU platforms, implementing `af_logger` will allow the library to use your MCU’s logging platform to provide debugging and error output. `af_logger` is implemented in a platform-specific and a platform-independent header file; for example, to use `af_logger` on Arduino would would require including both “`af_logger.h`” and “`arduino_logger.h`”.

## af_logger()

### Description

A generic implementation to print debugging output to the serial console.

### Syntax

There are several forms of `af_logger()`; the call you choose depends upon what type of data you intend to print. Here are the currently--available calls:

```
af_logger_format_t:
    AF_LOGGER_BIN = 2,
    AF_LOGGER_OCT = 8,
    AF_LOGGER_DEC = 10,
    AF_LOGGER_HEX = 16

void arduino_logger_start(int baud_rate);
void arduino_logger_stop();
void af_logger_print_value(int32_t val);
void af_logger_print_buffer(const char* val);
void af_logger_print_formatted_value(int32_t val, af_logger_format_t format);
void af_logger_println_value(int32_t val);
void af_logger_println_buffer(const char* val);
void af_logger_println_formatted_value(int32_t val, af_logger_format_t format);
```

For other platforms, check the `<platform>_logger.h` header file for proper parameters to `<platform>_logger_start()` function.

### Parameters

| PARAMETER    | DESCRIPTION                                      |
| -------- | --------------------------------------------------------- |
| `val`    | Value to be printed.                                      |
| `format` | Optional `af_logger_format_t` to determine output format. |

Note that, because `val` is declared as a signed 32-bit type, `af_logger()` will not properly print values of unsigned 32-bit numbers larger than 2147483647. If you need support for such values, you should use the standard Arduino `Serial.print()`, or implement your own logger.

### Returns

None.

### Examples

1. This line:<br>   ```af_logger_println_buffer("Rebooted - Always set MCU attributes after reboot");```<br>will print "**Rebooted - Always set MCU attributes after reboot**", followed by a newline.

2. On Arduino, the following two lines of code are functionally identical:<br>```Serial.println( int8variable, HEX );```<br>```  af_logger_println_formatted_value( int8variable, AF_LOGGER_HEX );```

