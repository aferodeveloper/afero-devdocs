# Attribute Daemon Client Implementation

The Attribute daemon is the central nervous system for the device. It communicates changes in device attributes between multiple client daemons on the device though a customized IPC layer. Clients use an event-based API to:

- **Set** the value of an attribute.
- **Get** the value of an attribute.
- **Be notified** when the value of an attribute has changed.

This page contains the following sections:

- [What Are Attributes?](../LinuxSDK-AttrDaemon#what-are-attributes)
- [System Attributes and hubby](../LinuxSDK-AttrDaemon#system-attributes-and-hubby)
- [Writing an Attribute Client](../LinuxSDK-AttrDaemon#writing-an-attribute-client)
- [Handling Endian Issues](../LinuxSDK-AttrDaemon#handling-endian-issues)
- [Attribute Client API Reference](../LinuxSDK-AttrDaemon#attribute-client-api-reference)

## What Are Attributes?

Afero device attributes have the following properties:

- Attributes have an ID, a number between 0 and 65,535 (2^16 - 1). When you get or set an attribute you refer to the attribute by ID. If you get notified, the ID will tell you which attribute changed.
- Attributes have a type. The type helps applications determine how to format the value for input and output. Some examples of types are INT8 (eight-bit signed integer), UTF8S (UTF string).
- Attributes have a value. On the Afero device, all the attribute values are arrays of unsigned bytes. An attribute value can have up to 32767 bytes. The largest attribute used is the Wi-Fi SSID list, and it is around 3kB in size.
- Attributes have an owner. The owner of the attribute can always set the value of the attribute. Nonowners can write to the attribute if it is writable.
- Attributes have a set of flags, indicating what can be done with the attribute. The flags are: WRITABLE and NOTIFY. A writable attribute can be set by a daemon that doesn’t own the attribute. A notify attribute causes the Attribute daemon to tell interested daemons that an attribute has been set.

You can create your own attributes; instructions for doing this are in [Defining Your Own Attributes](../LinuxSDK-DefineAttrs). We use a Potenco as an example.

## System Attributes and hubby

Several Afero attributes that are used to control and debug the device are available. To be visible, they must be defined in the device Profile. However, some are private between hubby and the daemons so developers do not have access to them.

The following is a list of Afero attributes that are used or owned by device daemons. Note that the flags in the table are **Attribute daemon flags**.

| ID    | NAME                    | TYPE       | OWNER    | ATTRD FLAGS  | DESCRIPTION                                                  |
| :---- | :---------------------- | :--------- | :------- | :----------- | :----------------------------------------------------------- |
| 51600 | REPORT_RSSI_CHANGES     | INT8       | ATTRD    | WRITE NOTIFY | Tells the Wi-Fi Station daemon and the WAN daemon to report signal strength changes periodically. |
| 51611 | HUBBY_STATE             | INT8       | HUBBY    | NOTIFY       | State of hubby:<br> 0 - Unknown<br>1 - Connected to the Afero Cloud<br>2 - Unable to connect to the Afero Cloud |
| 51612 | OTA_UPGRADE_PATH        | UTF8S[128] | HUBBY    | NOTIFY       | The attribute ID for transferring the OTA update image path to the OTA Manager. The value is a null-terminated string containing the path of the OTA images. The header is stored in the file with a name using a “.hdr” extension. The OTA update binary is stored in the file with a name using a “.img” extension. |
| 51613 | DEBUG_LEVEL             | INT8       | ATTRD    | WRITE        | The debug level (0-3) of the Attribute daemon. Debug levels are:<br>0 - All debug messages off<br>1 - LOG_DEBUG1; shows all attribute traffic for attrd<br>2 - LOG_DEBUG2; shows all IPC traffic <br>3 - LOG_DEBUG3; verbose |
| 51614 | DEBUG_LEVEL             | INT8       | WIFISTAD | WRITE        | The debug level (0-3) of the Wi-Fi Station daemon. See 51613 above for level definitions. |
| 51615 | DEBUG_LEVEL             | INT8       | WAND     | WRITE        | The debug level (0-3) of the WAN daemon. See 51613 above for level definitions. |
| 51616 | DEBUG_LEVEL             | INT8       | CONNMGR  | WRITE        | The debug level (0-3) of the Connection Manager daemon. See 51613 above for level definitions. |
| 51617 | DEBUG_LEVEL             | INT8       | EDGED    | WRITE        | The debug level (0-3) of the Edge Device daemon. See 51613 above for level definitions. |
| 51618 | OTA_UPGRADE_PATH_PREFIX |            | OTAMGR   |              | The path prefix for the OTA update files (see 51612 OTA_UPGRADE_PATH). If this is not specified, hubby defaults to a prefix of “/tmp/”. |
| 51619 | PROFILE UPDATED         | INT8       | OTAMGR   | WRITE        | The attribute in which hubby will inform the rest of the world that a new profile has just been downloaded successfully and is ready to use. |
| 65001 | UTC_OFFSET_DATA         | BYTES[8]   | HUBBY    | WRITE NOTIFY | Contains the current UTC offset in minutes, the next timestamp to change the UTC offset, and the new UTC offset to apply at the previous timestamp, in that order (2-bytes signed short, 4-bytes int, 2-bytes signed short). |
| 65004 | CONFIGURED_SSID         | UTF8S[33]  | WIFISTAD | NOTIFY       | The SSID the device is currently configured to use.          |
| 65005 | WIFI_RSSI               | INT8       | WIFISTAD | NOTIFY       | The Wi-Fi Receive Signal Strength Indicator.                 |
| 65006 | WIFI_STEADY_STATE       | INT8       | WIFISTAD | NOTIFY       | Used to communicate the Wi-Fi state to the apps outside the Wi-Fi setup. Wi-Fi steady state possibilities:<br>0 = Not Connected<br>1 = Pending<br>2 = Connected<br>3 = Unknown Failure<br>4 = Association Failed<br>5 = Handshake Failed<br>6 = Echo Failed<br>7 = SSID Not Found |
| 65007 | WIFI_SETUP_STATE        | INT8       | WIFISTAD | NOTIFY       | Wi-Fi setup state:<br>0 = Not Connected<br>1 = Pending<br>2 = Connected<br>3 = Unknown Failure<br>4 = Association Failed<br>5 = Handshake Failed<br>6 = Echo Failed<br>7 = SSID Not Found Used during the W-Fi setup process to let the apps know what state the W-Fi connection is in. |
| 65008 | NETWORK_TYPE            | INT8       | CONNMGR  | NOTIFY       | -1 - NONE<br>0 - Ethernet<br>1 - WLAN<br>2 - WAN                   |
| 65019 | REBOOT_REASON           | UTF8S[100] | ATTRD    | NOTIFY       | Reason the device rebooted the last time.                    |
| 65022 | NET_CAPABILITIES        | INT8       | CONNMGR  | NOTIFY       | Bitfield indicating which network interfaces this hardware contains:<br>bit 0 - Wi-Fi 1<br>bit 1 - WAN 1<br>bit 2 - Ethernet 1 |
| 65024 | WIFI_ITF_STATE          | INT8       | CONNMGR  |              | Network interface state:<br>0 - Not Available/Broken<br>1 - Disabled<br>2 - Pending<br>3 - Up |
| 65025 | WIFI_IPADDR             | BYTES[4]   | CONNMGR  |              | Local IPv4 address of the network interface in network byte order (Big Endian). |
| 65026 | WIFI_UPTIME             | INT32      | CONNMGR  |              | Uptime in seconds.                                           |
| 65027 | WIFI_DL_DATA_USAGE      | INT32      | CONNMGR  |              | Downlink data usage in bytes.                                |
| 65028 | WIFI_UL_DATA_USAGE      | INT32      | CONNMGR  |              | Uplink data usage in bytes.                                  |
| 65029 | WIFI_MAC_ADDR           | UINT8[6]   | CONNMGR  |              | MAC address of Wi-Fi interface.                              |
| 65030 | WIFI_KEY_MGMT           | INT8       | WIFISTAD |              | Key management for the interface. Supported at this time:<br>0 - None<br>1 - WPA PSK<br>2 - WPA-EAP<br>3 - IEEE 802.1X |
| 65031 | WIFI_GROUP_CIPHER       | INT8       | WIFISTAD |              | Group (multicast and broadcast) cipher. Supported at this time:<br>0 - None<br>1 - WEP 40 bit<br>2 - WEP 104 bit<br>3 - TKIP<br>4 - AES CCMP |
| 65032 | WIFI_PAIRWISE_CIPHER    | INT8       | WIFISTAD |              | Pairwise (unicast) cipher. Supported at this time:<br>0 - None<br>1 - WEP 40 bit<br>2 - WEP 104 bit<br>3 - TKIP<br>4 - AES CCMP |
| 65034 | WAN_RSRP                | INT16      | WAND     | NOTIFY       | Reference Signal Receive Power. This attribute should be reported when Report RSSI Changes attribute (51600) is nonzero. |
| 65035 | WAN_BARS                | INT8       | WAND     | NOTIFY       | Signal strength in bars. This attribute should be reported when Report RSSI Changes attribute (51600) is nonzero. |
| 65037 | WAN_ITF_STATE           | INT8       | WAND     |              | Network interface state:<br>0 - Not Available/Broken<br>1 - Disabled<br>2 - Pending<br>3 - Up |
| 65038 | WAN_IPADDR              | BYTES[4]   | CONNMGR  |              | Local IPv4 address of the network interface in network byte order (Big Endian). |
| 65039 | WAN_UPTIME              | INT32      | CONNMGR  |              | Uptime in seconds.                                           |
| 65040 | WAN_DL_DATA_USAGE       | INT32      | CONNMGR  |              | Downlink data usage in kilobytes.                            |
| 65041 | WAN_UL_DATA_USAGE       | INT32      | CONNMGR  |              | Uplink data usage in kilobytes.                              |
| 65042 | WAN_IMEISV              | UTF8S[32]  | WAND     |              | Modem International Mobile Equipment Identifier with Software Version. |
| 65043 | WAN_IMSI                | UTF8S[32]  | WAND     |              | SIM International Mobile Subscriber Identity.                |
| 65044 | WAN_ICCID               | UTF8S[32]  | WAND     |              | SIM Integrated Circuit Card Identity.                        |
| 65045 | WAN_RAT                 | INT8       | WAND     |              | Current Radio Access Technology:<br>0 - Unknown<br>1 - GSM<br>2 - EGPRS (EDGE)<br>3 - CDMA 1x<br>4 - UMTS (WCDMA)<br>5 - EvDO<br>6 - LTE |
| 65046 | WAN_REG_STATE           | INT8       | WAND     |              | Registration state. Tentatively:<br>0 - Not registered<br>1 - Registered with a network<br>2 - Searching<br>3 - Registration denied by the visible network<br>4 - Registration state is unknown |
| 65047 | WAN_PS_STATE            | INT8       | WAND     |              | Packet-switched domain attach state of the mobile:<br>0 - Detached<br>1 - Attached<br>2 - Unknown |
| 65048 | WAN_MCC                 | UTF8S[8]   | WAND     |              | Mobile Country Code (MCC) of operator.                       |
| 65049 | WAN_MNC                 | UTF8S[8]   | WAND     |              | Mobile Network Code (MNC) of operator.                       |
| 65050 | WAN_LAC                 | INT32      | WAND     |              | Location Area Code (LAC) or cell ID (28 bits).               |
| 65051 | WAN_CELL_ID             | INT16      | WAND     |              | LTE physical cell ID (0 to 503).                             |
| 65052 | WAN_ROAMING_STATE       | INT8       | WAND     |              | Roaming status: 0 - Home 1 - Roaming 2 - Unknown             |
| 65053 | WAN_PLMN                | UTF8S[32]  | WAND     |              | Short name of Public Land Mobile Network (PLMN) on which the modem is camped. |
| 65054 | WAN_APN                 | UTF8S[100] | WAND     |              | Access Point Name (APN) providing IP connectivity.           |
| 65055 | WAN_SIM_STATUS          | INT8       | WAND     |              | Indicates the state of the card. Valid values:<br>0 - Absent<br>1 - Present<br>2 - Error<br>3 - Unknown |
| 65056 | WAN_DL_BIT_RATE         | INT32      | WAND     |              | Downlink bit rate in bits per second.                        |
| 65057 | WAN_UL_BIT_RATE         | INT32      | WAND     |              | Uplink bit rate in bits per second.                          |
| 65059 | ETH_ITF_STATE           | INT8       | CONNMGR  |              | Network interface state:<br>0 - Not Available/Broken<br>1 - Disabled<br>2 - Pending<br>3 - Up |
| 65060 | ETH_IPADDDR             | BYTES[4]   | CONNMGR  |              | Local IPv4 address of the network interface.                 |
| 65061 | ETH_UPTIME              | INT32      | CONNMGR  |              | Uptime in seconds.                                           |
| 65062 | ETH_DL_DATA_USAGE       | INT32      | CONNMGR  |              | Downlink data usage in kilobytes.                            |
| 65063 | ETH_UL_DATA_USAGE       | INT32      | CONNMGR  |              | Uplink data usage in kilobytes.                              |
| 65064 | ETH_MAC_ADDRESS         | BYTES[6]   | CONNMGR  |              | MAC address of Wi-Fi interface.                              |
| 65065 | SYSTEM_TIME             | INT32      | CONNMGR  |              | UTC system time in seconds since on the Unix Epoch (Jan 1, 1970). |

If the Notify flag is set for an attribute, then the Attribute daemon (attrd) will tell the other daemons when a value changes (hubby being one of them). If the attribute is in the Profile then hubby will send the value to the Afero Cloud; otherwise, it’s dropped. So for hubby to communicate the values to the Afero Cloud, both the Notify flag has to be set **and** the attribute must be in the Profile.

Afero attributes that are integers are always communicated in Little Endian byte order. Therefore daemons that provide values for Afero attributes must convert the values to Little Endian byte format. The Attribute Client library provides functions to perform these conversions. See the [Handling Endian Issues](../LinuxSDK-AttrDaemon#) section below.

## Writing an Attribute Client

An “attribute client” is a daemon that talks to the Attribute daemon through the Attribute Client API. The Wi-Fi Station daemon, the WAN daemon, the Connection Manager, and hubby are all attribute clients. Probably the best way to understand the Attribute Client API is to look at any of the daemons in the af conn repository.

The Attribute Client API is based on libevent, and the callbacks are called in the context of the event loop.

### Simplest Client

Here’s an example of a skeleton you can use if the client does not own any attributes and doesn’t want to receive notifications. Indeed this client does nothing except open the attribute library and wait. It will be the basis for the more complex examples below:

```
#include <stdint.h>
#include <stdlib.h>
#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include "af_attr_client.h"
#include "af_log.h"

static struct event_base *sEventBase = NULL;
uint32_t g_debugLevel = 3;

static void on_open(int status, void *context)
{
    if (status != AF_ATTR_STATUS_OK) {
        /* do something to notify the user of an error */
        return;
    }
}

static void on_close(int status, void *context)
{
    if (status != AF_ATTR_STATUS_OK) {
        /* do something to notify the user of an error */
    }
}

int main(int argc, char *argv[])
{
    openlog("myclient", LOG_PID, LOG_USER);

    sEventBase = event_base_new();
    if (sEventBase == NULL) {
        /* Notify user */
        return AF_ATTR_STATUS_NO_SPACE; /* you can use your own error codes */
    }
    int status = af_attr_open(
        sEventBase,      /* event base */
        "IPC.MYCLIENT",  /* owner name */
        0, NULL,         /* numRanges, ranges */
        NULL,            /* notifyCB */
        NULL,            /* ownerSetCB */
        NULL,            /* ownerGetCB */
        on_close,        /* closeCB */
        on_open,         /* openCB */
        NULL);           /* context */

    if (status != AF_ATTR_STATUS_OK) {
        /* Notify user */
        event_base_free(sEventBase);
        return status;
    }
    event_base_dispatch(sEventBase);
    af_attr_close();
    event_base_free(sEventBase);

    closelog();
    return 0;
}
```

This code defines callbacks for handling open failures and unexpected closes. Then it opens the library. If opening the library is successful, the code starts the event loop. The event loop exits when there are no pending events. In this case, the loop waits forever for attribute events to occur, but because the app owns no attributes, has not registered for notifications, and has not performed a `get` or a `set`, no events occur and the loop just hangs.

Note that the `main` function also opens the system log. This makes sure that the error and debug messages logged by the client library can be attributed to your application.

### Building and Running the Attribute Client

Under development. Will describe what is needed to build and run this code.

### Setting Attributes

We can modify the code to perform a `set` operation after the library is opened; for example, if this is a command-line utility.

First we write a callback function that tells us whether the `set` operation was successful. Note some ways it could fail:

- The Attribute daemon is not running.
- The daemon that owns the attribute is not running.
- The daemon that owns the attribute rejects the `set` operation.

```
static void on_set_finished(int status, uint32_t attributeId, void *context)
{
    if (status != AF_ATTR_STATUS_OK) {
        /* notify user of error condition */
    }
    /* you wouldn't do the following if you're writing a daemon */
    event_base_loopbreak(&sEventBase);
}
```

When the `set` operation finishes, the `on_set_finished` function is called with the status of the operation.

Now we need to initiate the `set` operation at a reasonable time. For our example, we use the `on_open` function. Replace the skeleton’s `on_open function` with this:

```
static void on_open(int status, void *context)
{
    if (status != AF_ATTR_STATUS_OK) {
        /* do something to notify the user of an error */
        return;
    }
    int8_t level = 2;
    status = af_attr_set(AF_ATTR_ATTRD_DEBUG_LEVEL, &level, sizeof(level), on_set_finished, NULL);
    if (status != AF_ATTR_STATUS_OK) {
        event_base_loopbreak(&sEventBase);
    }
}
```

The `af_attr_set call` sets the Attribute daemon debug level.

If you’re writing a daemon, you’d probably call the `af_attr_set` function based on a different event than opening the Attribute Client library. Also you would not exit the event loop when the `set` fails and when it finishes.

If the daemon owns the attribute, the `set` code would be exactly the same; however, in this case you would be writing a daemon and the caveats in the last paragraph would definitely apply.

### Getting Attributes

Let’s modify the original skeleton to perform a `get` operation. First we need a callback that tells us the value of the attribute:

```
static void on_get_response(uint8_t status, uint32_t attributeId, uint8_t *value, int length, void *context)
{
    if (status != AF_ATTR_STATUS_OK) {
        /* notify the user of the get failure)
    } else {
        /* the value parameter points to the value of the attribute. The length parameter contains the length of the value */
        printf ("the value of attribute %d is %d\n", attributeId, *(int8_t *)value);
    }
    event_base_loopbreak(sEventBase);
}
```

When the `get` operation finishes, the `on_get_response` function is called with the status of the `get` operation and the value of the attribute if the operation were successful.

Again, we’ll use the `on_open` function to initiate the `get` operation:

```
static void on_open(int status, void *context)
{
    if (status != AF_ATTR_STATUS_OK) {
        /* do something to notify the user of an error */
        return;
    }

    int8_t level = 2;
    status = af_attr_get(AF_ATTR_ATTRD_DEBUG_LEVEL, on_get_response, NULL);
    if (status != AF_ATTR_STATUS_OK) {
        event_base_loopbreak(&sEventBase);
    }
}
```

The `af_attr_get` call initiates a `get` operation on the Attribute daemon’s debug level.

As with Setting attributes, if you’re writing a daemon, you would probably call the `af_attr_get` function based on a different event than opening the Attribute Client library. Also, you would not exit the event loop when the `get` fails and when it finishes.

### Receiving Notifications

Let's modify the original skeleton to receive notifications. First we need a function to handle the notification:

```
static void on_notify(uint32_t attributeId, uint8_t *value, int length, void *context)
{
/* attribute with specified ID has been set to the specified value, and it has the  specified length */
    printf ("the value of attribute %d is %d\n", attributeId, *(int8_t *)value);
}
```

In the main loop, we add a range of attributes to listen for, and we add the range, number of ranges, and the callback to the `open` function. In our case we’ll listen to the attribute that tells the WAN and Wi-Fi Station daemons to periodically report their RSSI values. The complete `main` function is below:

```
int main(int argc, char *argv[])
{
    openlog("myclient", LOG_PID, LOG_USER);

    sEventBase = event_base_new();
    if (sEventBase == NULL) {
        /* Notify user */
        return AF_ATTR_STATUS_NO_SPACE; /* you can use your own error codes */
    }

    af_attr_range_t r[] = {
        { AF_ATTR_ATTRD_REPORT_RSSI_CHANGES, AF_ATTR_ATTRD_REPORT_RSSI_CHANGES }
    };
    int status = af_attr_open(
        sEventBase,             /* event base */
        "IPC.MYCLIENT",         /* owner name */
        sizeof(r)/sizeof(r[0]), /* numRanges */
        r,                      /* ranges */
        on_notify,              /* notifyCB */
        NULL,                   /* ownerSetCB */
        NULL,                   /* ownerGetCB */
        on_close,               /* closeCB */
        on_open,                /* openCB */
        NULL);                  /* context */

    if (status != AF_ATTR_STATUS_OK) {
        /* Notify user */
        event_base_free(sEventBase);
        return status;
    }

    event_base_dispatch(sEventBase);

    af_attr_close();

    event_base_free(sEventBase);

    closelog();
    return 0;
}
```

This app will loop forever, printing out a message every time the REPORT_RSSI_CHANGES attribute gets set. If you want to exit the first time the attribute gets set, you can put an `event_base_loopbreak` call in the `on_notify` function. To test it you can set the attribute using the Attribute Client:

```
attrc set ATTRD_REPORT_RSSI_CHANGES 01
```

The Attribute Client is described in greater detail in the [Attribute Client API Reference](../LinuxSDK-AttrDaemon#AttrClientAPI) section.

### Owning Attributes

An application that owns attributes needs to specify an `ownerGet` callback. Also, if any of the attributes it owns is writable, it must specify an `ownerSet` callback too. The Attribute daemon defines two attributes for the ATTRTEST client that can be used to demonstrate how to handle `ownerSets` and `ownerGets`. This code is a modified version of the code in the `attr_test.c` function in the Attribute daemon repository.

The first attribute is flagged Writable and Notify and called AF_ATTR_ATTRTEST_SCRATCHWO. We’ll implement an owner `set` function for it:

```
static int on_owner_set(uint32_t attributeId, uint8_t *value, int length, void *context)
{
    /* at this point the new value of the attribute is stored in the value parameter */
    /* and the length is stored in the length parameter. You can do something in     */
    /* in response to the set operation */
    printf ("attribute %d was set. The first byte of the value is 0x%02x and the length is %d\n", attributeId, value[0], length);
    return AF_ATTR_STATUS_OK;
}
```

We will also implement a read-only attribute called SCRATCH_RO. First we write an owner `get` function for it:

```
static void on_get_request(uint32_t attributeId, uint16_t getId, void *context)
{
    if (attributeId == AF_ATTR_ATTRTEST_SCRATCHRO) {
        char reply[] = "This is the value.";
        af_attr_send_get_response(AF_ATTR_STATUS_OK, getId, (uint8_t *)reply,
        sizeof(reply));
    } 
    else {
        af_attr_send_get_response(AF_ATTR_STATUS_ATTR_ID_NOT_FOUND, getId, NULL, 0);
    }   
}
```

Sometimes additional operations are needed to get the attribute value. If this operation takes a long time or needs the event loop to provide the value, you can delay the call to `af_attr_send_get_response` to later. It does not have to be called in the `on_get_request` callback.

There are two additional things we need to do:

1. Tell the Attribute daemon that we own these attributes by setting the client name in the `open` call to ATTRTEST.
2. Set the `ownerSet` and `ownerGet` callbacks in the `open` call.

The full `main` function is shown below:

```
int main(int argc, char *argv[])
{
    openlog("myclient", LOG_PID, LOG_USER);

    sEventBase = event_base_new();
    if (sEventBase == NULL) {
        /* Notify user */
        return AF_ATTR_STATUS_NO_SPACE; /* you can use your own error codes */
    }

    int status = af_attr_open(
        sEventBase,             /* event base */
        "IPC.MYCLIENT",         /* owner name */
        0, NULL,                /* numRanges, ranges */
        on_notify,              /* notifyCB */
        on_set_request,         /* ownerSetCB */
        on_get_request,         /* ownerGetCB */
        on_close,               /* closeCB */
        on_open,                /* openCB */
        NULL);                  /* context */

    if (status != AF_ATTR_STATUS_OK) {
        /* Notify user */
        event_base_free(sEventBase);
        return status;
    }

    event_base_dispatch(sEventBase);

    af_attr_close();

    event_base_free(sEventBase);

    closelog();
    return 0;
}
```

You can use the Attribute Client to `set` and `get` these values:

```
attrc set ATTRTEST_SCRATCHWO "A"
attrc get ATTRTEST_SCRATCHRO s
```

The Attribute Client is described in greater detail in the [Attribute Client API Reference](../LinuxSDK-AttrDaemon#AttrClientAPI) section.

### Adding Your Own Attributes and Daemons

 If you want to add attributes that are supported by the attribute system you’ll need to modify the `af_attr_def.h` file. The attributes are defined in the \_ATTRIBUTES macro. The format for a single attribute is:

`_ATTRDEF(<attributeID>, <short_name>, 0, <get_timeout>, <short_owner_name>, <flags>),`

For example, the ATTRD_DEBUG_LEVEL is defined like this:

`_ATTRDEF(51613, DEBUG_LEVEL, 0, 5, ATTRD, AF_ATTR_FLAG_WRITABLE), \`

The attribute name is composed of the short owner name, an underscore delimiter, followed by the attribute short name. If you use this attribute name in your code you need to prefix the name with “AF_ATTR_”, for example, AF_ATTR_ATTRD_DEBUG_LEVEL.

To add an attribute to an existing daemon you just create a new entry in the _ATTRIBUTES macro.

If you want to create a new daemon, you add the name to the _OWNERS macro:

```
#define _OWNERS              \ 
        _OWNERDEF(UNKNOWN),  \ 
        _OWNERDEF(ATTRD),    \ 
        _OWNERDEF(ATTRTEST), \ 
        _OWNERDEF(FSD),      \ 
        _OWNERDEF(WAN),      \ 
        _OWNERDEF(CONNMGR),  \ 
        _OWNERDEF(WIFISTAD), \ 
        _OWNERDEF(HUBBY),    \ 
        _OWNERDEF(EDGED),    \ 
        _OWNERDEF(OTAMGR),   \ 
        _OWNERDEF(MYCLIENT), \
```

Then you can add an attribute to the _ATTRIBUTES macro. In this example we provide a debug level attribute.

`_ATTRDEF(1102, DEBUG_LEVEL, 0, 5, MYCLIENT, AF_ATTR_FLAG_WRITABLE), \`

Whenever you modify the attribute definitions you need to recompile the Attribute daemon and all code that depends on it to ensure everything works properly.

## Handling Endian Issues

Afero attributes are all in Little Endian byte order. You should write your code to support both Little Endian and Big Endian architectures in case you decide to migrate your code to a different architecture. The Attribute Client library provides the following functions to help convert integers between host byte order and Little Endian.

| FUNCTION        | DESCRIPTION                                                  |
| :--------------------- | :----------------------------------------------------------- |
| `af_attr_store_uint16` | Stores a 16-bit unsigned integer in host byte order to an attribute value buffer in Little Endian byte order. |
| `af_attr_store_int16`  | Stores a 16-bit signed integer in host byte order to an attribute value buffer in Little Endian byte order. |
| `af_attr_store_uint32` | Stores a 32-bit unsigned integer in host byte order to an attribute value buffer in Little Endian byte order. |
| `af_attr_store_int32`  | Stores a 32-bit signed integer in host byte order to an attribute value buffer in Little Endian byte order. |
| `af_attr_get_uint16`   | Retrieves a 16-bit unsigned integer from an attribute value buffer in Little Endian byte order and return its value in host byte order. |
| `af_attr_get_int16`    | Retrieves a 16-bit signed integer from an attribute value buffer in Little Endian byte order and return its value in host byte order. |
| `af_attr_get_uint32`   | Retrieves a 32-bit unsigned integer from an attribute value buffer in Little Endian byte order and return its value in host byte order. |
| `af_attr_get_int32`    | Retrieves a 32-bit signed integer from an attribute value buffer in Little Endian byte order and return its value in host byte order. |

The Attribute Client API Reference, directly below, has more information about these functions.

## Attribute Client API Reference

### Data Types

#### af_attr_range_t

```
typedef struct {
    uint32_t first;
    uint32_t last;
} af_attr_range_t;
```

Specifies a range of attributes. The range includes the specified endpoints.

### Callback Definitions

#### af_attr_notify_callback_t

```
typedef void (*af_attr_notify_callback_t) (uint32_t attributeId, uint8_t *value, int length, void *context);
```

Informs the client that the attribute with the specified ID has been set to the specified value with the specified length.

#### af_attr_set_request_callback_t

```
typedef int (*af_attr_set_request_callback_t) (uint32_t attributeId, uint8_t *value, int length, void *context);
```

Informs the client that another client has attempted to set the specified attribute that it owns. The owner returns the status of the `set` operation. For example, the `set` could fail because the client is busy modifying the same attribute.

#### af_attr_get_request_callback_t

```
typedef void (*af_attr_get_request_callback_t) (uint32_t attributeId, uint16_t getId, void *context);
```

Informs the client that someone has requested an attribute that it owns. The owner is responsible for calling the `af_attr_send_get_response` function. This call can be made in this callback. Alternatively the `af_attr_send_get_response` function can be called later if the operations required to `get` the attribute value would block the event loop.

#### af_attr_status_callback_t

```
typedef void (*af_attr_status_callback_t) (int status, void *context);
```

Informs the client of the success or failure of an open or close operation. If an open operation failed, the status parameter indicates why the open failed. If the library closed unexpectedly, the status parameter indicates why the library closed. This callback is not called if you close the library using the `af_attr_close` function.

#### af_attr_set_response_callback_t

```
typedef void (*af_attr_set_response_callback_t)(int status, uint32_t attributeId, void *setContext);
```

Used in conjunction with the `af_attr_set` function. This function is called when a client Sets an attribute to indicate whether the `set` operation was successful or not. If not, the status parameter contains the failure reason.

#### af_attr_get_response_callback_t

```
typedef void (*af_attr_get_response_callback_t)(uint8_t status, uint32_t attributeId, uint8_t *value, int length, void *context);
```

Used in conjunction with the `af_attr_get` function. This function is called when the value the client is Getting is available. If the `get` failed, the status parameter contains the reason.

### af_attr_open

#####Prototype

```
int af_attr_open (struct event_base *base, char *clientName, uint16_t numListenRanges, 
    af_attr_range_t *listenRanges, af_attr_notify_callback_t notifyCb, 
    af_attr_set_request_callback_t ownerSetCb, 
    af_attr_get_request_callback_t getReqCb, 
    af_attr_status_callback_t closeCb, af_attr_status_callback_t openCb, void *context);
```

#####Parameters

| `base`            | Event base for the daemon’s event loop. You need to set up the event loop using `event_base_new()` before calling this function. |
| ----------------- | ------------------------------------------------------------ |
| `clientName`      | Name of the client. All Attribute Client names must start with the “IPC.” prefix. The name must match the name in the `af_attr_def.h` include file. |
| `numListenRanges` | Number of ranges in `listenRanges` list.                     |
| `listenRanges`    | Array of ranges of attribute IDs for which the client wants attribute change notifications. |
| `notifyCb`        | Callback that is called when an attribute in one of the `listenRanges` has changed. Set to NULL if you don’t need to receive notifications. |
| `ownerSetCb`      | Callback that is called when another client changes an attribute this client owns. Set to NULL if the daemon doesn’t own any attributes that can be set. |
| `getReqCb`        | Callback that is called when another client has requested the value of an attribute this client owns. |
| `closeCb`         | Attribute Client library has closed unexpectedly.            |
| `openCb`          | Attribute Client library either opened successfully or failed to open. |
| `context`         | Reference context for all of the callbacks.                  |

#####Description

Opens a connection to the Attribute daemon.

#####Return Value

Returns an error code on failure. If the return value is AF_ATTR_STATUS_OK, you need to wait for the `open` callback to complete before you can know the final status of the `open` operation.

### af_attr_set

#####Prototype

```
int af_attr_set (uint32_t attributeId, uint8_t *value, int length, af_attr_set_response_callback_t setCB, void *setContext);
```

#####Parameters

| `attributeId` | ID of the attribute to set.                                  |
| ------------- | ------------------------------------------------------------ |
| `value`       | Pointer to a byte array containing the value of the attribute. |
| `length`      | Length of the value byte array.                              |
| `setCB`       | Callback that is called to indicate if the `set` was successful or not. |
| `setContext`  | Reference context for the `set` callback.                    |

#####Description

Sets the attribute with the specified ID to the specified value. All attributes are byte arrays with a length. This means that the attribute value is not necessarily a null-terminated string.

If the client is the owner of the attribute and the notify bit is set in the attribute flags, the change is propagated to the Attribute daemon and any interested listeners.

If the client is not the owner of the attribute, the setting is propagated to the Attribute daemon and then to the owner. If the owner allows the change and the Notify bit is set in the attribute’s flags, the change is propagated to the interested listeners too.

#####Return Value

Returns an error code on failure. If the return value is AF_ATTR_STATUS_OK, you need to wait for the callback to complete before you can know the final status of the `set` operation.

### af_attr_get

#####Prototype

```
int af_attr_get (uint32_t attributeId, af_attr_get_response_callback_t cb, void *context);
```

#####Parameters

| `attributeId` | ID of the attribute to `get`.                                |
| ------------- | ------------------------------------------------------------ |
| `cb`          | Callback that is called to return the value of the attribute. |
| `context`     | Reference context for the `get` callback.                    |

#####Description

Gets the attribute with the specified ID. This is an asynchronous call, and the result is returned using the specified callback function. This function requests the attribute value from the Attribute daemon, who forwards the request to the client that owns the attribute. The owner returns the value of the attribute to the Attribute daemon, who returns it to the requesting client via the callback.

#####Return Value

Returns an error code on failure. If the return value is AF_ATTR_STATUS_OK, you need to wait for the callback to complete before you can know the final status of the `get` operation.

### af_attr_send_get_response

#####Prototype

```
int af_attr_send_get_response (int status, uint16_t getId, uint8_t *value, int length);
```

#####Parameters

| `status` | Status value you can set if you can’t return the specified attribute. |
| -------- | ------------------------------------------------------------ |
| `getId`  | The getId that was passed in from the `af_get_request_callback_t` used by the Attribute Client library to match this response to the original request. |
| `value`  | Byte buffer containing the attribute value to be returned.   |
| `length` | Length of the request to be returned.                        |

#####Description

Sends a response to a `get` request received from the `af_attr_get_request_callback_t` in the af_attr_open function. This function sends the response to the Attribute daemon, who forwards it to the requesting client.

#####Return Value

Returns an error code on failure or AF_STATUS_OK if the response was successfully sent to the Attribute daemon.

### af_attr_close

#####Prototype

```
void af_attr_close (void);
```

#####Description

Closes the Attribute Client library. Calling this function will not call the `close` callback function specified in the `af_attr_open function`.

### af_attr_store_uint16

#####Prototype

```
void af_attr_store_uint16(uint8_t *dst, uint16_t value);
```

#####Description

Stores the specified 16-bit unsigned integer value into the specified destination attribute value buffer in Little Endian byte order. This is a convenience function that works on both Big Endian and Little Endian architectures. If the `dst` parameter is NULL, this function has no effect.

### af_attr_get_uint16

#####Prototype

```
uint16_t af_attr_get_uint16(uint8_t *src);
```

#####Description

Returns the 16-bit unsigned integer value from the specified source attribute value buffer in Little Endian byte order. This is a convenience function that works on both Big Endian and Little Endian architectures. If the `src` parameter is NULL, this function returns 0.

### af_attr_store_int16

#####Prototype

```
void af_attr_store_int16(uint8_t *dst, int16_t value);
```

#####Description

Stores the specified 16-bit signed integer value into the specified destination attribute value buffer in Little Endian byte order. This is a convenience function that works on both Big Endian and Little Endian architectures. If the `dst` parameter is NULL, this function has no effect.

### af_attr_get_int16

#####Prototype

```
int16_t af_attr_get_int16(uint8_t *src);
```

#####Description

Returns the 16-bit signed integer value from the specified source attribute value buffer in Little Endian byte order. This is a convenience function that works on both Big Endian and Little Endian architectures. If the `src` parameter is NULL, this function returns 0.

### af_attr_store_uint32

#####Prototype

```
void af_attr_store_uint32(uint8_t *dst, uint32_t value);
```

#####Description

Stores the specified 32-bit unsigned integer value into the specified destination attribute value buffer in Little Endian byte order. This is a convenience function that works on both Big Endian and Little Endian architectures. If the `dst` parameter is NULL, this function has no effect.

### af_attr_get_uint32

#####Prototype

```
uint32_t af_attr_get_uint32(uint8_t *src);
```

#####Description

Returns the 32-bit unsigned integer value from the specified source attribute value buffer in Little Endian byte order. This is a convenience function that works on both Big Endian and Little Endian architectures. If the `src` parameter is NULL, this function returns 0.

### af_attr_store_int32

#####Prototype

```
void af_attr_store_int32(uint8_t *dst, int32_t value);
```

#####Description

Stores the specified 32-bit signed integer value into the specified destination attribute value buffer in Little Endian byte order. This is a convenience function that works on both Big Endian and Little Endian architectures. If the `dst` parameter is NULL, this function has no effect.

### af_attr_get_int32

#####Prototype

```
int32_t af_attr_get_int32(uint8_t *src);
```

#####Description

Returns the 32-bit signed integer value from the specified source attribute value buffer in Little Endian byte order. This is a convenience function that works on both Big Endian and Little Endian architectures. If the `src` parameter is NULL, this function returns 0.