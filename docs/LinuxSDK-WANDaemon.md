# WAN Daemon Implementation

The WAN system has the following parts that work together. Each component is described in detail on this page:

- [wancontrol script](../LinuxSDK-WANDaemon#wancontrol-script) - Bash script that controls the power states of the modem. You will need to customize this script for your modem.
- [atcmd executable](../LinuxSDK-WANDaemon#atcmd-executable) - Linux executable to send AT commands that is suitable for scripting. You should not need to customize this code.
- [wannetwork script](../LinuxSDK-WANDaemon#wannetwork-script) - Bash script that brings up the WAN network interface and sets up the routing and DNS servers. You will need to customize this code for your OS.
- [Radio Interface Layer (RIL)](../LinuxSDK-WANDaemon#radio-interface-layer-ril) - Static library that manages an AT command interface for network setup, RF environment monitoring, and debugging. You will need to customize this code for your modem.
    - [AT command interface](../LinuxSDK-WANDaemon#at-command-interface) - Part of the RIL that provides general purpose AT command utilities. You may need to modify this code for your modem.
- [WAN daemon components](../LinuxSDK-WANDaemon#wan-daemon-components) - Linux executable that binds all of these pieces together. You may not need to customize this code.

The sections on this page discuss each of these parts in more detail with an emphasis on the motivation for each part’s design. If you must customize anything for a particular modem, the requirements are presented.

## wancontrol Script

### What It Does

The impetus for the WAN control script (wancontrol) is that wireless modems often have specific and complex power-up sequences. For example, the Sierra Wireless modem used on the Afero Secure Hub requires the following sequence of operations to power up:

1. Turn on the master power FET.
2. Wait one second.
3. Pull the PWR_ON line low for two seconds.
4. Release the PWR_ON line.

By encoding these steps into a script, it is easy to prototype the power-on and power-off functionality from the command line. The script is also useful for factory diagnostics because you can turn the modem on and off and execute AT commands (using the atcmd executable described in a later section), without starting the WAN daemon.

The wancontrol script has one more important responsibility: to bring up AT command device nodes and make sure they appear. If they do not appear, the wancontrol script must shut down and indicate a failure using its exit code.

### How It’s Used

The script takes a single argument that specifies the action to be performed:

- on - Powers the modem on or returns immediately if the modem is already on, or
- off - Forces the modem into the powered-off state, first deregistering from the network.

So, to power on the modem, type the following at the command line:

```
$ /usr/lib/af-conn/wancontrol on
```

When finished executing, the script will either set the return value ($?) to 0 if successful, or nonzero if unsuccessful. This return value is required so the WAN daemon can retry automatically if the modem failed to power up.

To test the return value, you can execute the following:

```
$ /usr/lib/af-conn/wancontrol on && /bin/echo "modem powered up successfully"
```

### Requirements

The wancontrol script has the following requirements:

- When executed with the “on” argument, the wancontrol script must put the modem in the powered-on state regardless of the state in which it was originally. Any serial device node required by the RIL (for example /dev/ttyACM0) must be exposed and ready to use. The get_wan_carrier script must be run.

  If the modem fails to power up or the device nodes fail to be exposed, the script must return the modem to the powered-off state with the device nodes removed, and the script must return a nonzero return value.

- When executed with the “off” argument, the wancontrol script must put the modem in the powered-off state regardless of which state it was originally in. The script must disconnect from the network (typically using the AT+COPS=2 AT command) if the AT device is available. The script should power off the modem using AT commands if possible. If not, the script must force the modem into the powered-off state.

  When executed with the off argument, the return value of wancontrol is ignored.

## atcmd Executable

### What It Does

The atcmd executable is a C language command-line application that sends an AT command to a serial device and returns any replies to the command to stdout. This executable is very useful for scripting, and it’s used in the wancontrol and get_wan_carrier scripts. It is provided as sample source code in the af-conn repository.

### How It’s Used

The atcmd executable has one required parameter: the AT command to execute. For example, the following command returns the ICCID of the SIM (the prompt is from the device):

```
root@bento:~# /usr/bin/atcmd at+cimi
310170201421101
root@bento:~#
```

If the AT command returns OK, the atcmd executable doesn’t print anything but sets the return code to 0, indicating successful completion.

If the AT command returns an error, the atcmd executable prints the error returned by the AT command and sets the return code to a nonzero value, indicating a failure. Otherwise, the atcmd executable prints the data returned by the command and sets the return code to 0.

The atcmd executable has two optional switches:

| SWITCH | DESCRIPTION |
| ---- | ------------------------------------------------------------ |
| `‑d` | Allows you to specify the serial device that the AT command is sent to.The default is `/dev/ttyACM0`, although this can be changed by changing the DEFAULT_DEVICE in `atcmd.c`. This is useful because the WAN daemon uses `/dev/ttyACM0` while it’s running on the Afero Secure Hub.To talk to the modem while the WAN daemon is running you have to use `/dev/ttyACM2`; otherwise you’ll cause the WAN daemon to restart the modem.<br><br>root@bento:~# /usr/bin/atcmd -d /dev/ttyACM2 ati<br>HL7548<br>root@bento:~# |
| `‑w` | Allows you to specify the time in seconds for the AT command to execute before timing out.The default timeout is 15 seconds. Some AT commands take longer than 15 seconds to execute; for these commands, you must use the -w command-line option to specify a longer timeout.<br><br>root@bento:~# /usr/bin/atcmd -w 120 at+cops=?<br>+COPS: (2,"AT&T","AT&T","310410",7),(1,"Verizon","Verizon","311480",7),(1,"","","312770",7),(1,"","","310830",7),<br>(1,"Sprint","Sprint","310120",7),(1,"T-Mobile","T-Mobile","310260",7) root@bento:~# |

## wannetwork Script

The OS-level work of setting up networking for the modem network interface is handled by this script. It is intended to separate the OS-specific parts of the modem bringup from the OS-independent parts, which are handled by the WAN daemon.

On the device based on OpenWrt, the WAN network script does the following:

1. Sets the IP address and netmask of the wwan0 network interface.
2. Creates a default route to the wwan0 network interface.
3. Stores the DNS nameservers in /tmp/resolv.conf.wan.
4. Does the same for IPv6.

The wannetwork script has the following API:

```
/usr/lib/af-conn/wannetwork up|down <ip_v4> <subnet_v4> <dns1_v4> <dns2_v4> <ip_v6> <subnet_v6> <dns1_v6> <dns2_v6>
```

Where:

| OPTION        | DESCRIPTION  |
| ----------- | ------------------------------------------------------------ |
| `up`        | Brings the network up. You will need to specify network parameters when you bring the network up. |
| `down`      | Brings the network down cleanly so that you can bring the network up again. |
| `ip_v4`     | IPv4 address of the modem network interface. This is usually provided by the modem. |
| `subnet_v4` | IPv4 subnet specifier. For example, for 255.255.255.0 networks, the subnet is 24. |
| `dns1_v4`   | One IPv4 DNS name server address.                            |
| `dns2_v4`   | Another IPv4 DNS name server address.                        |
| `ip_v6`     | IPv6 address of the modem network interface.                 |
| `subnet_v6` | IPv6 subnet specifier.                                       |
| `dns1_v6`   | One IPv6 DNS name server address.                            |
| `dns2_v6`   | Another IPv6 DNS name server address.                        |

If the WAN daemon doesn’t know one of these arguments, it will set the parameter to an empty string (""). For example, on AT&T, only IPv4 addresses are used, and the wannetwork script is called like this:

```
$ /usr/lib/af-conn/wannetwork up "10.49.101.114" "24" "8.8.8.8" "8.8.4.4" "" "64" "" ""
```

## Radio Interface Layer (RIL)

The Radio Interface Layer is a part of the WAN daemon that is customized for each supported modem. It has the following responsibilities:

- Makes sure the SIM is connected and readable.
- Gets the ICCID and determines the carrier.
- Tells the modem to register onto the network.
- Determines when the modem has registered with the packet switch network.
- Starts the modem PDP context.
- Gets the networking parameters from the modem such as the IP address, netmask, and DNS servers.
- Gets debugging information from the modem such as the camp status, serving cell status, and neighbor cell status.

The RIL implemented on the Afero Secure Hub supports the Sierra Wireless HL75xx series of modems and performs the above functions using AT commands.

### Modem Startup Sequence

To understand the RIL, it is helpful to understand the sequence of operations necessary to bring up a wireless modem.

After the WAN daemon powers up the WAN (using the wancontrol script) it brings up the data connection by interacting with the RIL as follows:

**1**&nbsp;&nbsp;The WAN daemon calls `ril_init()` to initialize the RIL. This function does the following:

***1a***&nbsp;&nbsp;Starts the AT command event handler.

***1b***&nbsp;&nbsp;Turns off AT command echo.

***1c***&nbsp;&nbsp;Gets some modem and SIM parameters for debugging.

***1d***&nbsp;&nbsp;Sets up AT event reporting.

***1e***&nbsp;&nbsp;Initializes an internal cache with the modem’s PDP context profile list.

**2**&nbsp;&nbsp;The WAN daemon gets the SIM ICCID using the `ril_lock_wan_status()` function, retrieving the ICCID from the returned `ril_wan_status_t` structure.

**3**&nbsp;&nbsp;Based on the ICCID and the contents of the `/etc/wan/carriers` file, the WAN daemon determines the APN and PPP authentication parameters.

**4**&nbsp;&nbsp;The WAN daemon calls the `ril_select_network()` function. This function sets up the default PDP context so the modem can register on the Packet Switch network. Then it tells the modem to register (AT+COPS=0).

**5**&nbsp;&nbsp;The WAN daemon calls `ril_get_ps_attach` to get the packet switch attach status until it detects that the modem is attached to the packet switch network.

**6**&nbsp;&nbsp;The WAN daemon calls the `ril_activate_data_call()` function to start the PDP context. The function returns a structure containing the networking parameters that are used by the wannetwork script to set up the network.

**7**&nbsp;&nbsp;The WAN daemon executes the wannetwork script, which brings up the network interface, creates the default route, and sets the DNS servers.

At this point the network is up and can be used by hubby.

If the networking fails for some reason, the WAN daemon brings it down cleanly using the `ril_deactivate_data_call()` function. If the modem itself has stopped operating (for example, if the AT serial device stops responding), the WAN daemon shuts down the RIL cleanly using the `ril_shutdown()` function and then powers off the modem.

### RIL Interface

The RIL interface is defined in `RIL.h`. The functions defined in the interface are described below.

#### ril_init

#####Syntax

```
int ril_init(struct event_base *base, ril_event_callback_t callback, void *context);
```

This function must start up the Radio Interface Layer. The WAN daemon will ensure that the modem is on and the serial device that supports AT commands are up before it executes this function. On the HL75xx RIL, the ril_init function performs the following:

1. Clears an internal data connection state structure.
2. Clears the PDP context cache. This cache is used to help set up the PDP context parameters such as APN and PPP authentication parameters.
3. Starts the AT command input event handler using the event base. In the RIL code there is a separate AT command manager that does this.

The `ril_init` function on the HL75xx RIL calls the `prv_modem_init` function, which performs the following:

1. Turns off AT command echo. Turning off echo simplifies the parsing of the AT command output.
2. Gets the IMEI-SV, ICCID, and IMSI for the WAN status structure. The WAN daemon uses the ICCID stored in the WAN status structure to ascertain the APN.
3. Configures the modem to report errors as numbers, which simplifies the parsing of the AT command output.
4. Configures the modem to report all LTE errors as asynchronous events.
5. Configures the modem to report all packet switch events as asynchronous events.
6. Configures the modem to report camped cell info events.
7. Reads in the internal PDP context database into the PDP context cache.

#####Parameters

| PARAMETER   | DESCRIPTION |
| ---------- | ------------------------------------------------------------ |
| `base`     | The event base of the libevent2 event loop. This is used to add the AT command event that occurs when input is available on the AT command serial device. |
| `callback` | This callback is called when the RIL wants to notify the WAN daemon of an asynchronous event. Right now the only asynchronous event the RIL sends to the WAN daemon is the RIL_EVENT_DATA_CALL_LOST event, which causes the WAN daemon to shut down the network using `ril_deactivate_data_call()` and then bring it up again using `ril_activate_data_call()`. |
| `context`  | Any context the WAN daemon needs in the callback. At the moment the WAN daemon does not use the context and sets it to NULL. |

#####Returns

| RETURN   | DESCRIPTION |
| ---------- | ------------------------------------------------------------ |
| `RIL_ERROR_NONE (0)` | RIL was initialized properly.                                |
| `RIL_ERROR_FATAL`    | Something failed during initialization. The WAN daemon will restart the RIL in this case. |

#### ril_select_network

#####Syntax

```
int ril_select_network(ril_data_call_request_t *dataCallReq);
```

This function must set the APN according to the specified data call request and attempt to automatically camp on a network.

#####Parameters

The WAN daemon fills in the dataCallReq parameter, which is a `ril_data_call_request_t` defined as follows:

```
#define PDN_AUTH_TYPE_LEN_MAX 1
#define PDN_PROTOCOL_LEN_MAX 6
#define PDN_APN_LEN_MAX 100
#define PDN_USER_LEN_MAX 64
#define PDN_PASSWORD_LEN_MAX 64

typedef struct {
    char auth_type[PDN_AUTH_TYPE_LEN_MAX + 1];
    char protocol[PDN_PROTOCOL_LEN_MAX + 1];
    char apn[PDN_APN_LEN_MAX + 1];
    char user[PDN_USER_LEN_MAX + 1];
    char password[PDN_PASSWORD_LEN_MAX + 1];
} ril_data_call_request_t;
```

All of these members are specified as null terminated C strings:

| OPTION | DESCRIPTION |
| ----------- | ------------------------------------------------------------ |
| `auth_type` | Specifies PPP authentication type. Set to “1” for PAP authentication,“2” for CHAP authentication. Only specified if PPP authentication is required for specified APN. |
| `protocol`  | Specifies IP protocol supported. Set to “IP” for IPv4, “IPV4V6” for mixed stack IPv4 and IPv6, and “IPV6” for IPv6. |
| `apn`       | Specifies Access Point Name (APN) of PDP context.            |
| `user`      | Specifies PPP authentication user name and is only specified if PPP authentication is required for specified APN. |
| `password`  | Specifies PPP authentication password and is only specified if PPP authentication is required for specified APN. |

#####Returns

| RETURN  | DESCRIPTION |
| --------------- | ------------------- |
| `RIL_ERR_NONE`  | Function succeeded. |
| `RIL_ERR_FATAL` | Failure occurred.   |

#### ril_get_ps_attach

#####Syntax

```
int ril_get_ps_attach(int *attachP);
```

Determines if the modem is attached to a packet switched network:

- When the modem is attached to a packet switched network, the WAN daemon can start or maintain a PDP context.
- When the modem is not attached to a packet network, a PDP context cannot be started or sustained.

This function is also responsible for getting the WAN status information, which is discussed in more detail in [Concurrency and Debug Data](../LinuxSDK-WANDaemon#concurrency-and-debug-data).

#####Parameters

| PARAMETER | DESCRIPTION |
| --------- | ------------------------------------------------------------ |
| `attachP` | Integer passed by reference in which the function stores the attach status. The attach status is nonzero if the modem is attached and zero otherwise. The WAN daemon uses this value to determine if it should start a PDP context. |

#####Returns

| RETURN     | DESCRIPTION  |
| ------------------ | --------------------------------------------- |
| `RIL_ERR_NONE`     | Function succeeded.                           |
| `RIL_ERR_NONFATAL` | Failure occurred; modem will be power-cycled. |
| `RIL_ERR_FATAL`    | Failure occurred; modem will be power-cycled. |

#### ril_lock_wan_status

#####Syntax

```
ril_wan_status_t *ril_lock_wan_status(void);
```

Locks the mutex protecting the internal WAN status structure and returns a pointer to it. All calls to this must be followed with a call to `ril_unlock_wan_status`.

#### ril_unlock_wan_status

#####Syntax

```
void ril_unlock_wan_status(void);
```

Unlocks the mutex protecting the internal WAN status structure.

#### ril_activate_data_call

#####Syntax

```
int ril_activate_data_call(ril_data_call_response_t *dataCallRsp);
```

Establishes a PDP context on the serving cell and returns the IP parameters. This function performs the following steps on the HL75xx:

1. Enables the channel route (AT+XDATA).
2. Enables DNS (AT+XDNS).
3. Activates the packet data network (AT+CGACT).
4. Gets the IP address (AT+CGPADDR).
5. Gets the DNS servers (AT+XDNS).
6. Enters the data state (AT+CGDATA).
7. Fills in the data call response data structure.

#####Parameters

| PARAMETER | DESCRIPTION |
| ------------- | ------------------------------------------------------------ |
| `dataCallRsp` | Structure that contains the network information of the data call that was just set up. This structure is defined as follows: |

```
typedef struct {
    int subnet_v4;
    int subnet_v6;
    char ip_v4[IPV4_ADDR_SIZE];
    char ip_v6[IPV6_ADDR_SIZE];
    char dns1_v4[IPV4_ADDR_SIZE];
    char dns2_v4[IPV4_ADDR_SIZE];
    char dns1_v6[IPV6_ADDR_SIZE];
    char dns2_v6[IPV6_ADDR_SIZE];
} ril_data_call_response_t;
```

Where:

| PARAMETER | DESCRIPTION |
| ------------- | ------------------------------------------------------------ |
| `subnet_v4`        | Specifies the subnet mask for the IPv4 address. For example, if the subnet mask is 255.255.255.0, the subnet is 24. |
| `subnet_v6`        | Specifies the subnet mask for the IPv6 address.              |
| `ip_v4`            | Null-terminated string containing an IPv4 address in dot notation; for example, “192.168.1.5”. If the modem doesn’t have an IPv4 address, this member must be set to the empty string (""). |
| `ip_v6`            | Null-terminated string containing an IPv6 address in colon notation; for example, “fe80::b283:feff:fea2:3f1”. If the modem doesn’t have an IPv6 address, this member must be set to the empty string (""). |
| `dns1_v4, dns2_v4` | Null-terminated strings containing the first and second IPv4 DNS servers in dot notation; for example, “8.8.8.8”. If the modem can’t retrieve IPv4 DNS addresses, these members must be set to the empty string (""). |
| `dns1_v6, dns2_v6` | Null-terminated strings containing the first and second IPv6 DNS servers in colon notation. If the modem can’t retrieve IPv6 DNS addresses, these members must be set to the empty string (""). |

#####Returns

| RETURN | DESCRIPTION |
| ------------- | ------------------------------------------------------------ |
| `RIL_ERR_NONE`     | Function succeeded.                                          |
| `RIL_ERR_NONFATAL` | Failure occurred, but not bad enough to warrant a modem power cycle. |
| `RIL_ERR_FATAL`    | Failure occurred, requiring the modem be power-cycled; for example, an AT command timed out. |

#### ril_shutdown

#####Syntax

```
void ril_shutdown(void);
```

This function shuts down the RIL cleanly. It is called whenever the modem is shut down; for example, if a fatal RIL error occurs. It must clean up all the resources that it used, including the AT serial device input event and the file descriptor to the AT serial device.

### AT Command Interface

Within the RIL is a set of utility functions for handling AT commands. It is a general-purpose AT command interface that should work with any modem that primarily communicates using AT commands. The AT command API is defined in `at.h`.

The AT command interface has the following high-level functionality:

- Sets up and tears down the AT command interface (`at_init`, `at_shutdown`).
- Handles unsolicited AT events by calling a callback (events and callback defined in `at_init`).
- Sends AT commands and waits for the response (`at_send_cmd`, `at_send_cmd_1_int`, `at_send_cmd_2_int`, `at_send_query`).
- Parses the response to the AT commands line by line (`at_rsp_num_lines`, `at_rsp_next_line`).
- Tokenizes a string to retrieve the output parameters in an AT command response (`at_tokenize_line`).

The HL75xx RIL uses these functions for all AT command management.

#### at_init

#####Syntax

```
int at_init(char *device, struct event_base *base, at_unsol_def_t *defs, int numDefs, void *context);
```

Opens the specified AT command serial device, sets up the serial device input event, and registers a set of callbacks for handling unsolicited AT events. It also creates a mutex and condition variable for thread synchronization. This is discussed in greater detail in [RIL Threading](../LinuxSDK-WANDaemon#ril-threading).

#####Parameters

| PARAMETER | DESCRIPTION |
| ------------- | ------------------------------------------------------------ |
| `device`  | String specifying the serial AT command device; for example, “/dev/ttyACM0”. |
| `base`    | Event base for the main event loop that handles the AT event. |
| `defs`    | Array of at_unsol_def_t structures that specify an unsolicited AT event prefix and a callback to be called when that prefix is received. This is discussed below. |
| `numDefs` | Number of definitions in the defs array.                     |
| `context` | Reference context that can be used by the unsolicited AT event callback. |

#####Returns

| RETURN | DESCRIPTION |
| ------------- | ------------------------ |
| `0`  | Function succeeded.                      |
| `-1` | Function failed; sets errno accordingly. |

The `at_unsol_def_t` structure is defined as follows:

```
typedef void (*at_unsol_callback_t)(char *rest, void *context);

typedef struct {
    char *prefix;
    at_unsol_callback_t callback;
} at_unsol_def_t;
```

An unsolicited event callback returns void and has two parameters:

| RETURN | DESCRIPTION |
| ------------- | ------------------------ |
| `rest`    | The rest of the AT event output when the prefix and the colon (“:”) that follows it are eliminated. |
| `context` | The reference context passed into the `at_init` function.    |

The prefix member of the `at_unsol_def_t` structure defines the prefix of the AT event. For example, on the HL75xx, changes to the packet switch attach status are reported using like this:

```
+CGEV: 1
```
To get a callback when the prefix in this case is “+CGEV”. When the callback is called, the rest parameter points to the “ 1” part of the string that follows the prefix “CGEV” and the colon “:”.

The callback member of the `unsol_def_t` structure defines the callback that is called when the prefix is seen in at the AT command input.

#### at_shutdown

#####Syntax

```
void at_shutdown(void);
```

This function closes the AT command serial device and deletes the input event. It also cleans up the synchronization primitives create in at_init. See [RIL Threading](../LinuxSDK-WANDaemon#ril-threading) for more information.

#### at_start_cmds

#####Syntax

```
void at_start_cmds(void);
```

This function locks the resources used for sending AT commands and parsing the responses. You must call this function before sending a series of AT commands or you will have concurrency issues. See [RIL Threading](../LinuxSDK-WANDaemon#ril-threading) for more information.

#### at_end_cmds

#####Syntax

```
void at_end_cmds(void);
```

This function unlocks the resources used for sending AT commands and parsing the responses. You must call this function after you’ve called the `at_start_cmds` function and finished sending a series of AT commands, or else the next time you call `at_start_cmds`, your RIL will deadlock.

#### at_send_cmd

#####Syntax

```
at_result_t at_send_cmd(at_response_type_t rsp_type, char *prefix, char *opt, int timeout);
```
Sends an AT command with the specified prefix and parameters string to the modem and waits until the specified timeout for a response back that matches the specified prefix and response type. This function blocks until the results of the AT command have been received. As a result, this function must not be called in the event loop.

#####Parameters

| PARAMETER  |  DESCRIPTION	
| ---------- | ------------------------------------------------------------ |
| `rsp_type` | Enum that specifies the expected format of the AT command response. There are three values for the `rsp_type`:<br><br>`AT_RSP_TYPE_OK` - The AT command simply responds with “OK”. For example, the ATE0 command response is “OK”.<br><br>`AT_RSP_TYPE_PREFIX` - The AT command responds with the command itself as a prefix. For example, the “AT+COPS?” command response is something like +COPS: 0,0, "AT&T",7.<br><br>`AT_RSP_TYPE_NO_PREFIX` - The AT command response with the data only. For example, on the Sierra Wireless HL7548, the "ATI" command responds with the identification information only “HL7548”. |
| `prefix`   | Null-terminated string that specifies the AT command prefix. For example, the prefix for “AT+COPS=0” is “+COPS”. |
| `opt`      | Null-terminated string containing the parameters for the AT command or NULL if no parameters are needed. For example, the parameter string for “AT+COPS=0” is “=0”. |
| `timeout`  | Number of seconds the command will execute. If this is set to 0, the default timeout (10 seconds) is used. Some commands take longer; for example, “AT+COPS=0”. |

#####Returns

Returns a code that indicates the success or failure of the AT command invocation. The return code can have the following values:

| RETURN  |  DESCRIPTION |
| ----------------------- | ------------------------------------------------------------ |
| `AT_RESULT_SUCCESS`     | The AT command completed successfully.                       |
| `AT_RESULT_ERROR`       | The AT command returned an error. The error number can be obtained using the `at_rsp_error` function. |
| `AT_RESULT_CME_ERROR`   | The AT command returned a mobile equipment error. The error number can be obtained using the `at_rsp_error` function. |
| `AT_RESULT_CMS_ERROR`   | The AT command returned a message system error. The error number can be obtained using the `at_rsp_error` function. |
| `AT_RESULT_TIMEDOUT`    | The AT command did not return a result before the timeout period. |
| `AT_RESULT_SEND_FAILED` | The AT command failed to send. This usually indicates a failure on the serial device. |

#####Example

```
int res = at_send_cmd(AT_RSP_TYPE_OK, "ATE", "0", 0);
```

#### at_send_cmd_1_int

#####Syntax

```
at_result_t at_send_cmd_1_int(at_response_type_t type, char *prefix, int arg, int timeout);
```

Sends the AT command with the specified prefix and single integer argument. This is a convenience function for the `at_send_cmd` function.

#####Parameters

| PARAMETER  | DESCRIPTION |
| ---------- | ------------------------------------------------------------ |
| `rsp_type` | See the [at_send_cmd](../LinuxSDK-WANDaemon#at_send_cmd) documentation for an explanation of the response type. |
| `prefix`   | Null-terminated string that specifies the AT command prefix. For example, the prefix for “AT+COPS=0” is “+COPS”. |
| `arg`      | Integer argument for the command. For example, the argument for “AT+COPS=” is “0”. |
| `timeout`  | Number of seconds the command will execute. If this is set to 0, the default timeout (10 seconds) is used. Some commands take longer, for example, “AT+COPS=0”. |

#####Example

```
int res = at_send_cmd_1_int(AT_RSP_TYPE_OK, "+COPS", 1, 0);
```

#### at_send_cmd_2_int

#####Syntax

```
at_result_t at_send_cmd_2_int(at_response_type_t type, char *prefix, int arg1, int arg2, int timeout);
```
Sends the AT command with the specified prefix and two integer arguments. This is a convenience function for the `at_send_cmd` function.

#####Parameters

| PARAMETER  |  DESCRIPTION |
| ---------- | ------------------------------------------------------------ |
| `rsp_type` | See the [at_send_cmd](../LinuxSDK-WANDaemon#at_send_cmd) documentation for an explanation of the response type. |
| `prefix`   | Null-terminated string that specifies the AT command prefix. For example, the prefix for “AT+COPS=0” is “+COPS”. |
| `arg1`     | First integer argument for the command.                      |
| `arg2`     | Second integer argument for the command.                     |
| `timeout`  | Number of seconds the command will execute. If this is set to 0, the default timeout (10 seconds) is used. Some commands take longer, for example, “AT+COPS=0”. |

#####Example

```
res = at_send_cmd_2_int(AT_RSP_TYPE_OK, "+CGEREP", 1, 0, 0);
```

This code sends:

```
AT+CGEREP=1,0
```

#### at_rsp_num_lines

#####Syntax

```
int at_rsp_num_lines(void);
```

Returns the number of lines in the response to the AT command sent using at_cmd_send function or one of its variants.

#### at_rsp_next_line

#####Syntax

```
char *at_rsp_next_line(void);
```

Returns a pointer to the null-terminated string containing the next line of the response to the AT command sent using the at_cmd_send function or one of its variants. This function allows you to iterate through the lines of the output until you reach the end. Returns NULL if there are no more response lines.

#####Example

```
char *line;
while  ((line = at_rsp_next_line()) != NULL) {
  /* parse the line */
}
```

#### at_rsp_error

#####Syntax

```
int at_rsperror(void)
```

Returns the error number for the last error if one of the at_send_xxx functions, AT_RESULT_ERROR, AT_RESULT_CME_ERROR, or AT_RESULT_CMS_ERROR.

For example, if you send the following AT command using `at_send_cmd`: “AT+CCID” and the SIM is not present, the modem returns “CME ERROR: 10”. `at_send_cmd` returns AT_RESULT_CME_ERROR, and `at_rsp_error()` returns 10.

#### at_tokenize_line

#####Syntax

```
int at_tokenize_line(char *line, char tok, char **list, int len);
```

Breaks a string into chunks separated by the specified token character. Also removes quotation marks around a substring.

#####Parameters

| PARAMETER | DESCRIPTION |
| ------ | ------------------------------------------------------------ |
| `line` | String to be separated.                                      |
| `tok`  | Token character to be used to split the line, usually “,”.   |
| `list` | Array of pointers to characters that will hold the pieces of the line. |
| `len`  | Number of elements in the list array.                        |

#####Return

The number of actual substrings found.

#####Example

```
char *line = " 1,\"IP\",\"m2m.com.attz\",\"10.51.134.104\",0,0,0,0,0,0"
#define ARRAY_SIZE(_x) (sizeof(_x) / sizeof(_x[0]))
char *tokens[3];
numTokens = at_tokenize_line(line, ',', tokens, ARRAY_SIZE(tokens));
```

At this point `numTokens` = 3, `tokens[0]` = "1", `tokens[1]` = "IP", `tokens[2]` = "m2m.com.attz".

### RIL Threading

The RIL runs in two WAN daemon threads: the main thread, which is running a libevent2 event loop, and the worker thread. The design of the RIL is one reason that the WAN daemon has this worker thread. The RIL could have been written entirely in an event driven environment. In that case the API would look more like the Attribute daemon API: every action would have a callback that would get called when the action was completed. The big downside to this approach is that the RIL functions become much more complex: they need to be written as a state machine.

Consider `ril_get_ps_attach()`. This function performs two AT commands:

- AT+CGATT?
- AT+KCELL=0

Each of these commands writes a command to the AT serial device. The RIL receives the result of the command in the form of an input event in the event loop. In order for the `ril_get_ps_attach()` function to block while getting these values, the function must be running in a different thread from the event loop. Making these functions block simplifies the API because a callback function is not needed. The other thing that is simplified is the `ril_get_ps_attach` code. It looks like a linear series of AT command invocations.

To allow the RIL functions to block, the WAN daemon creates a worker thread from which most of the RIL functions are executed. These functions have one thing in common: they execute multiple AT commands in sequence and block until all of the commands complete.

- `ril_init()`
- `ril_select_network()`
- `ril_get_ps_attach()`
- `ril_activate_data_call()`
- `ril_deactivate_data_call()`

So when the linear AT command sequence is executed, the sequence of operations looks like this:

1. The AT command is written to the serial device.

2. The function sending the command blocks.

3. The event handler gets some data back from the serial device.

4. The data coming back is parsed until the desired response is seen.

5. The event handler notifies the blocked function (using a pthread condition variable) that the response is available.

6. The function that sent the AT command parses the response.

7. And then sends the next AT command.

   And so on.

The following pseudocode shows the proper way to issue a series of AT commands.

```
int ril_function()
{
    int retVal = RIL_ERR_NONE;

    at_start_cmds();

    /* send a command */
    int res = at_send_xxx( ... );
    if (res != AT_RESULT_SUCCESS) {
        /* deal with the error */
        retVal = RIL_ERR_FATAL;
        goto exit;
    }
    char *line;
    while ((line = at_get_next_line()) != NULL) {
        /* parse each line */
    }

    /* send another command */
    int res = at_send_xxx( ... ) ;

    ... and so on ...

exit:
    at_end_cmds();
    return retVal;
}
```

The `at_start_cmds` function locks a mutex that works together with the pthread condition variable mentioned earlier. When you execute the `at_send_xxx` function, the AT command is sent and the thread blocks on a `pthread_cond_timed_wait` function. This function unlocks the mutex so the event thread has a chance to execute.

When the event handler sees input on the AT serial device, it locks the mutex, parses the input and checks if the data matches a proper response to the command. If so, it triggers the condition variable and unlocks the mutex. The other thread starts up again with the mutex locked. With the mutex locked, the data can be parsed without worrying about the event thread clobbering it at the same time.

An upshot of this method is that if you put the thread to sleep using the sleep function, you should call the `at_end_cmds` function before the sleep and the `at_start_cmds` afterward to prevent the event thread from hanging up for too long.

```
at_end_cmds();
sleep(2);
at_start_cmds();
```

#### Concurrency and Debug Data

The WAN daemon provides debugging information in the form of Afero attributes. These attributes are described in the [Attribute Daemon Client Implementation](../LinuxSDK-AttrDaemon). The Radio Interface Layer is responsible for gathering this data, which it stores in a structure of type `ril_wan_status_t`, defined (in `ril.h`) as follows:

```
typedef struct {
    char imeisv[20];
    char iccid[24];
    char imsi[16];
    uint8_t simStatus;
    uint8_t psState;
    uint8_t regState;
    uint8_t rat;
    uint8_t bars;
    uint8_t roamingState;
    uint8_t rac;
    uint8_t pad1;
    uint16_t tac;
    uint16_t pcid;
    int16_t rsrp;
    int16_t rsrqX10;
    int16_t rssnrX10;
    uint16_t pad2;
    char mcc[4];
    char mnc[4];
    uint32_t lac;
    char plmn[32];
    char apn[100];
    char neighborInfo[128];
} ril_wan_status_t;
```

The status and state member definitions are enumerated in `ril.h` as well.

The RIL maintains an internal version of this structure for other parts of the WAN daemon to consume. Because this internal structure gets shared by several threads, the RIL provides a locking mechanism that ensures that if you access any member of the structure properly, it will not be corrupted. This means that the code that consumes the WAN status information must lock the internal structure while accessing the data and then unlock it afterwards. Here’s the code that does this:

```
ril_wan_status_t *wStatus = ril_lock_wan_status();
uint8_t localPsState = wStatus->psState;
ril_unlock_wan_status();
```

In the RIL, if you need to update the WAN status you need to do the same, but you have direct access to the `sWanStatus` structure as shown in the following sample:

```
ril_lock_wan_status();
sWanStatus.psState = 0;
ril_unlock_wan_status();
```

## WAN Daemon Components

The WAN daemon binds all of these pieces together. It consists of four components:

- [Worker thread (wand.c)](../LinuxSDK-WANDaemon#worker-thread)
- [State machine (wand.c)](../LinuxSDK-WANDaemon#state-machine)
- [Network monitor (net.c)](../LinuxSDK-WANDaemon#network-monitor)
- [Attribute handler (server.c)](../LinuxSDK-WANDaemon#attribute-handler)

These components are discussed in more detail in the sections below.

### Worker Thread

The worker thread has already been mentioned above; it is the thread in which all of the RIL functions that talk to the modem run. Conceptually this thread runs in a tight loop executing commands that it receives from a mailbox or queue—except that the queue is only one element deep. The WAN daemon does not queue multiple commands to the worker thread; instead, it tees up a single command. If circumstances change and a different command needs to be teed up, the new command replaces the old one. The reason for this design is that once a new command is issued, the old command is no longer relevant and can be safely ignored.

The worker thread loop is in the `prv_worker_loop` function. It waits for a condition variable to be triggered so that it can execute a command. It then executes the command and goes back to the beginning. If the condition variable wait times out and the WAN is powered up, the signal strength is checked; and if the WAN daemon has been asked to report the RSSI, the signal strength is reported to the Attribute daemon. The signal strength check can fail, for example, if the AT command serial device locks up. We discuss this in more detail in the [State Machine](../LinuxSDK-WANDaemon#statemachine) section.

The worker thread can perform the following functions:

| FUNCTION | DESCRIPTION |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [WAND_COMMAND_MODEM_ON](../LinuxSDK-WANDaemon#wand_command_modem_on) | Powers on the modem.                                         |
| [WAND_COMMAND_MODEM_OFF](../LinuxSDK-WANDaemon#wand_command_modem_off) | Powers off the modem by calling the `prv_modem_off` function. |
| [WAND_COMMAND_ACTIVATE_DATA_CALL](../LinuxSDK-WANDaemon#wand_command_activate_data_call) | Starts a data call while the modem is powered up by calling the `prv_activate_data_call` function. |
| [WAND_COMMAND_REACTIVATE_DATA_CALL](../LinuxSDK-WANDaemon#wand_command_reactivate_data_call) | Tears down and restarts the data call by calling the `prv_reactivate_data_call` function. |
| `WAND_COMMAND_CHECK`                                         | Checks the signal strength by calling `prv_check_signal`. This command is issued when the condition-variable timed-wait times out after five seconds. |

Each of these actions can take quite a long time and is therefore ineligible for handling by the event thread.

#### WAND_COMMAND_MODEM_ON

To execute this command, the worker thread calls the `prv_modem_on` function, which does the following:

1. Powers on the modem by calling the Linux system() function to execute the wancontrol script.
2. Starts up the Radio Interface Layer (RIL) by calling `ril_init`.
3. Gets the SIM ICCID from the RIL.
4. Determines the APN for the connection based on the ICCID.
5. Calls the `ril_select_network` to attach to the mobile network.
6. If any of these fail, the script retries up to three more times for a total of four tries.

Here’s a little more detail about step 4. The `prv_modem_on` function calls the `prv_get_apn_info` function to determine the Access Point Name (APN) that the Radio Interface Layer uses to start the data connection based on the first several digits of the SIM ICCID. The mapping between the ICCID and the APN is specified in the `/etc/wan/carriers` file. On the device, the file looks like this:

```
# This file maps the carrier ID to an APN and protocol on the Sierra module
# The first column is the ICCID prefix that identifies the carrier (required)
# The second column is the APN name (required)
# The third column is the supported IP protocol (IP, IPV4V6, or IPv6, required)
# The fourth column is the authentication type: 0 = PAP, 1 = CHAP
# The fifth column is the username
# The sixth column is the password
891480 VZWINTERNET IPV4V6
8901170 m2m.com.attz IP
894230 soracom.io IP
```

The first column contains the ICCID prefix that identifies the carrier and is required.

The second column contains the APN name and is required.

The third column specifies the IP protocol supported by the APN. It can be one of IP, IPV4V6, or IPV6 and is required.

The fourth column is the authentication type: 1 = PAP, 2 = CHAP. It is only required for APNs that require PPP authentication.

The fifth column is the username. It is only required for APNs that require PPP authentication.

The sixth column is the password. It is only required for APNs that require PPP authentication.

The `prv_get_apn_info` function gets the relevant APN information and stores it in the static `ril_data_call_request_t` structure `sDataCallReq`; for use when activating the data call.

#### WAND_COMMAND_MODEM_OFF

To execute this command, the worker thread calls the `prv_modem_down` function. This function simply shuts down the RIL, shuts down the network by calling the system function to execute the wannetwork script, and shuts down the modem by calling the system function to execute the wancontrol script.

#### WAND_COMMAND_ACTIVATE_DATA_CALL

To execute this command the worker thread calls the `prv_activate_data_call` function. This function does the following:

1. Waits for the modem to become packet switch attached to the network making use of the `ril_get_ps_attach` function.
2. Calls the `ril_activate_data_call` function to start a PDP context on the modem.
3. Calls the `prv_set_up_network` function to set up the networking on the host platform. This is just a wrapper for the wannetwork script.
4. Sets up the network monitor (see [Network Monitor](../LinuxSDK-WANDaemon#networkmonitor) below).

The rest of the function handles the many retry and other failure cases. Here a little more detail about each:

- If the `ril_get_ps_attach` function fails, the failure is considered fatal and the modem is power-cycled.
- If the `ril_activate_data_call` function returns RIL_ERROR_NONFATAL, the function retries activation up to three times.
- If the `ril_activate_data_call` function returns RIL_ERROR_FATAL, the modem is power-cycled.
- If the `prv_set_up_network` function fails, it tries again after which it gives up.

In general, if this function fails, the WAN daemon tries to activate the network again after three seconds.

#### WAND_COMMAND_REACTIVATE_DATA_CALL

To execute this command the worker thread calls `prv_reactivate_data_call`. This function calls `prv_deactivate_data_call` and then calls `prv_activate_data_call` described in the previous section.

The `prv_deactivate_data_call` function shuts down the network watcher (see [Network Monitor](../LinuxSDK-WANDaemon#networkmonitor) below), shuts down the host network using the wannetwork script, and shuts down the data call using the `ril_deactivate_data_call` function.


### State Machine

The heart of the WAN daemon is its state machine. The state machine keeps track of what the WAN daemon is doing and helps determine what the next action should be when an event occurs.

Probably the easiest way to explain the state machine is to enumerate the states:

| STATE   | DESCRIPTION |
| ----------------------------- | ------------------------------------------------------------ |
| `WAND_STATE_OFF`              | WAN is permanently off. This is an exceptional case, and typically occurs only if there is a modem hardware failure or the modem is not populated. |
| `WAND_STATE_WAITING_FOR_DOWN` | State machine is waiting for the system to be put in the powered-off state. |
| `WAND_STATE_WAITING_FOR_UP`   | State machine is waiting for the modem to be powered on and the RIL to be initialized. |
| `WAND_STATE_WAITING_FOR_DATA` | State machine is waiting for a data call to be activated and networking to be started. |
| `WAND_STATE_DATA`             | Data call is active and working.                             |

The WAN system can be in each of these states for a period of time. While the WAN system is in one of these states, many events can occur that can affect which state the state machine goes to. These events are:

| EVENT  | DESCRIPTION |
| -------------------------------- | ------------------------------------------------------------ |
| `WAND_EVENT_MODEM_UP`            | Modem was powered up successfully.                           |
| `WAND_EVENT_MODEM_DOWN`          | Modem was powered down OR the modem failed to be powered up. |
| `WAND_EVENT_MODEM_LOCKED`        | AT serial device is not responding.                          |
| `WAND_EVENT_DATA_CALL_UP`        | Data call was started successfully.                          |
| `WAND_EVENT_DATA_CALL_CALL_DOWN` | Data call failed to be started.                              |
| `WAND_EVENT_DATA_CALL_LOST`      | Established data call was lost.                              |
| `WAND_EVENT_PS_ATTACH_LOST`      | Modem detached from the packet switched network.             |
| `WAND_EVENT_RETRY`               | Retry timer fired and the last command should be retried.    |
| `WAND_EVENT_NO_MODEM`            | wancontrol script indicated that there is no modem and the state machine should go into the WAND_STATE_OFF state. |

The details of how these events affect the state machine, and what commands are issued to the worker thread in response, are not provided here. The best reference is the code itself; specifically, the `prv_handle_wand_event` function, which moves the state machine to the next state in response to an event and sets up the next command to be processed by the worker thread.

The `prv_handle_wand_event` function also must be called to tell the WAN daemon that an event has occurred. For example, in the case of a RIL event, the RIL event callback translates the RIL event into a WAN daemon event and calls the `prv_handle_wand_event` function.

#### State Machine Threading

The `prv_handle_wand_event` is designed to move from one state to another. It can also tee up a command (remember that the worker thread only has one command queue), but it doesn’t block. It uses mutexes to ensure that the relevant data structures are protected; specifically, the WAN state (`sWandStateMutex`) and the worker thread next command (`sCmdMutex`). So you can safely call this function from any thread provided you are not holding either of the aforementioned mutexes. Indeed this function is called in the worker thread, the network monitor thread (described in [Network Monitor](../LinuxSDK-WANDaemon#networkmonitor) below) and the main event loop thread.

### Network Monitor

The network monitor is a simple packet watching system that can detect certain network stall conditions. The code for the network monitor is in `net.c`. The API is in `net.h`.

Basically, when the WAN daemon finishes setting up the network it calls `netwatch_init` specifying a network down callback. This function creates a thread that uses `libpcap` to watch all packets going to the wwan0 interface (hard-coded in the `prv_activate_data_call` function in `wand.c`). This thread matches TCP packets with their corresponding ACKS, and if a packet is not acknowledged after 30 seconds, a DNS lookup is performed to see if the network is still operating. If the DNS check fails, the network down callback is called, which causes the WAN daemon to issue a `WAND_EVENT_DATA_CALL_LOST` event.

The netwatch_shutdown function tears down the thread.

### Attribute Handler

The attribute handler is in `server.c`. It is a straightforward implementation of the Attribute daemon client API and makes a good reference for using that API.

The attributes that this code handles are:

- WAN_RSRP
- WAN_BARS
- WAN_ITF_STATE
- WAN_IMEISV
- WAN_IMSI
- WAN_ICCID
- WAN_RAT
- WAN_REG_STATE
- WAN_PS_STATE
- WAN_MCC
- WAN_MNC
- WAN_LAC
- WAN_CELL_ID
- WAN_ROAMING_STATE
- WAN_PLMN
- WAN_APN
- WAN_SIM_STATUS
- WAN_DL_BIT_RATE
- WAN_UL_BIT_RATE

These attributes are described in more detail in the [Attribute Daemon Client Implementation](../LinuxSDK-AttrDaemon). With few exceptions (WAN_APN, WAN_ITF_STATE, WAN_DL_BIT_RATE, and WAN_UL_BIT_RATE) the attribute values for these attributes are derived from the WAN status structure in the RIL. See the [Concurrency and Debug Data](../LinuxSDK-WANDaemon#concurrency-and-debug-data) section for more details.

  *Next:*&nbsp;&nbsp;[**&#8674;** Connection Manager Daemon Implementation](../LinuxSDK-ConnMgrDaemon)