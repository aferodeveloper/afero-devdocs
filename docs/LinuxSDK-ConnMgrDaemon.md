# Connection Manager Daemon Implementation

The Connection Manager (ConnMgr) is an event-based daemon. It manages the network/wireless connectivity between IoT devices and the Afero Cloud. ConnMgr manages the following network connections (if and when they are supported) on the device to ensure Afero Cloud connectivity:

- Ethernet - The Ethernet interface (eth0); typically connected to the corporate or home Ethernet port.
- Wi-Fi - Wi-Fi interface (wlan0); connected to an Access Point (AP).
- Wireless modem - WAN interface (wwan0); connected via an LTE cellular carrier.

The Connection Manager daemon consists of the following components, each described in detail on this page:

- [Firewall scripts](../LinuxSDK-ConnMgrDaemon#firewall-scripts) - Bash scripts that manage the firewall (via iptables) on the device.
- [Afero service whitelist](../LinuxSDK-ConnMgrDaemon#afero-service-whitelist) - ASCII file that contains the list of servers with which the firewall allows traffic. It’s also responsible for the management of the whitelist.
- [Traffic monitoring and route selection engine](../LinuxSDK-ConnMgrDaemon#traffic-monitoring-and-traffic-route-selection) - Monitors traffic from the Ethernet and Wi-Fi connections (if they are enabled). In the event a selected connection becomes unavailable, it performs a route switch to a connection that is available (based on the selection criteria).
- [Whitelist IP extraction (from DNS entry)](../LinuxSDK-ConnMgrDaemon#whitelist-ip-extraction-via-dns) - Because Afero services are hosted in the Amazon Cloud and the load balancing service is provided by the Amazon Cloud, the IP address in the services whitelist are not static. This component helps us adapt to the dynamic changes of the IP addresses in the Amazon Cloud by modifying the firewall with updated firewall rules, thereby permitting traffic with Afero services.
- [Attribute management](../LinuxSDK-ConnMgrDaemon#attribute-management) - Responsible for managing the connection to the Attribute daemon and for the `get` and `set` operations of the attributes owned by ConnMgr.

The sections below discuss each component in more detail with an emphasis on the motivation behind the design. If you must customize any sections for a particular component, the requirements are presented.

Before we discuss each component in detail, let’s discuss how the network connections are set up. Network setup is platform-dependent; it is not the responsibility of ConnMgr. However, ConnMgr has an inherent dependency on network configuration and setup. This discussion facilitates the understanding of design requirements and goals.

## Connection Manager Running Modes

The Connection Manager has two running modes, Firewall-Enabled and Firewall-Disabled.

### Firewall-Enabled Mode

By default, the ConnMgr daemon runs with Afero firewall functionality enabled. When the ConnMgr daemon is running in the Firewall-Enabled mode, ConnMgr is responsible for invoking the Firewall script (`/usr/lib/af-conn/init_firewall.sh`), which sets up the Afero firewall rule chains, which in turn manage the access to Afero Cloud services. Typically, the invocation is done in one of the system init scripts, but that is dependent on the platform. The advantage of having ConnMgr perform this functionality is that ConnMgr is independent of the platform initialization scheme.

In the Firewall-Enabled mode, ConnMgr reads the Services Whitelist to grant access to the Afero Cloud services by creating a permission rule in the Firewall table. Furthermore, it uses the DNS extraction module to extract the dynamic rolling Cloud IP address to the Firewall table.

### Firewall-Disabled Mode

There are times when a user may not want to enable the Afero firewall. In this case, the user can use the option `-d` at invocation time to disable the Afero firewall functionality:

```
$ connmgr -d &
```

When the ConnMgr daemon runs with the firewall disabled, ConnMgr skips the services whitelist creation step and the DNS Extraction module is, in essence, disabled. In summary, the following functionalities are not applicable:

- Firewall scripts
- Services whitelist
- Whitelist IP extraction via DNS

## Network and Wi-Fi Configuration

In this section, we discuss the various network connection setups. ConnMgr is not responsible for setting up these networks, but only monitors the supported network connections. The network configuration setups are achieved by configuration files/scripts/applications supported by the particular platform, which is dependent on its network configuration infrastructure.

For example, these network configuration files can be found in the directories `/etc` and `/etc/config` on the OpenWrt device:

- `network` – Configuration file used to set up the various network interfaces such as Ethernet, WLAN, and wireless modem.
- `wireless` - Configuration file used to set up the wireless radio and connection credentials, as well as the Master/AP, client configuration.
- `firewall` - Configuration file used to set up the default firewall chains based on the network configuration file.
- `firewall.user` - Configuration file that contains the user-defined rules specific to the product.
- `dhcp` - Dynamic Host Configuration Protocol configuration file.

The network interface may vary depending on what is being supported by the device’s hardware configuration. For the network configuration implementation, the following naming convention is used:

| NETWORK              | NETWORK NAME | INTERFACE NAME* | COMMENTS                                                     | EXAMPLE CONFIGURATIONS (OPENWRT-BASED)                       |
| :------------------- | :----------- | :-------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| Ethernet             | ethnet       | eth0            | Typically used to connect to a corporate LAN or a user’s home network via an Ethernet jack. | config interface 'ethnet'<br>option ifname 'eth0'<br>option macaddr 90:6F:18:00:04:50<br>option proto 'dhcp'<br>option metric '10' |
| Wireless Station     | stanet       | wlan0           | When the wireless is configured as station mode.             | config interface 'stanet'<br>option ifname 'wlan0'<br>option proto 'dhcp'<br>option metric '20' |
| LTE Cellular network | modemnet     | wwan0           | The LTE cellular setup is controlled by the Afero WAN daemon (wand).The configuration specifics found in the configuration files are provided for completeness only. | Created by WAN daemon (wand).                                |

<div class="af-callout-caution">
	<div class="callout-text">
	<p><img src="../img/Caution.svg" width="17" style="vertical-align:bottom;padding:0">&nbsp;&nbsp;<strong>CAUTION!</strong>&nbsp;  The interface naming convention must be followed because ConnMgr assumes these are the interface names. If you change the “interface name”, you must make sure all the changes are propagated to all the various scripts and files supported by ConnMgr by using the <code>netif_names</code> configuration file. See directly below for details.</p>
	</div>
</div>

### netif_names Configuration File

This script provides an easy mechanism to allow the implementer to tell connmgr (and wifistad) the network interface names if non-conventional names are used. The script, `get_netif_names` is used by the deamons to retrieve the network interface names.

The format of this configuration uses `<key>=<value>`. For example:

```
ETH_INTERFACE_0='eth0'
WAN_INTERFACE_0='wwan0'
WIFISTA_INTERFACE_0='wlan0'
WIFIAP_INTERFACE_0='wlan0-1'
BRIDGE_INTERFACE_0='br-apnet'
```

connmgr expects the configuration file to be at the following location:

```
/etc/af-conn/netif_names
```

## Firewall Scripts

Firewall scripts are used to set up the firewall rule chains and manage the rule entries in each rule chain. Note that the Linux user space application program, iptables, is used to manage the firewall. The Linux firewall is an extremely flexible firewall utility built for Linux operating systems. Its functionality/detail is out of this document’s scope so please consult the appropriate documents to understand it better. In this document, we refer to the Linux-based firewall as “the firewall”.

The purpose of the firewall is to control the traffic that comes in and out of the device. Specifically, we want to secure the device and not allow just any traffic to go through it. The permitted traffic is specified by the Afero service listed in the Services Whitelist.

Typically, iptables use three different chains: input, forward, and output. These chains control the incoming, forwarding, and outgoing traffic, respectively. ConnMgr augments these three chains with the following rule chains:

INPUT

- AFERO_ALLOW_SERVICE_INPUT
- AFERO_ALLOW_WHITELIST_INPUT

OUTPUT

- AFERO_ALLOW_SERVICE_OUTPUT
- AFERO_ALLOW_WHITELIST_OUTPUT

FORWARD

- AFERO_FORWARD_ACCEPT_SERVICE
- AFERO_FORWARD_WHITELIST

The AFERO_ALLOW_SERVICE_[XX] contains the firewall rules that allow the traffic of the following well-known internet services: DNS, ICMP request, NTP, DHCP.

The AFERO_ALLOW_WHITELIST_[XX] contains the firewall rules that allow the traffic of the Afero services to the Afero Cloud: OTA service, Echo service, Conclave service, etc.

The firewall scripts consist of the following Bash scripts:

- `init_firewall.sh` – Initialization script for the firewall that flushes the existing firewall rules, created the rule chains and adds rules allowing IP addresses corresponding to white-listed domain names to the firewall.
- `fwcfg.sh` – Script to add/delete a firewall rule.
- `switch_route.sh` – Script that performs route management.

The `init_firewall.sh` script creates the AFERO_* firewall rule chain names, and adds the initial rules to permit the mentioned internet services and the Afero whitelist services. These two scripts are part of the system init scripts; invocation may be platform-dependent. On the other hand, `fwcfg.sh`and `switch_route.sh` scripts are invoked by ConnMgr directly.

ConnMgr expects these files in the following directories:

- `/etc/af-conn/whitelist`
- `/usr/lib/af-conn/fwcfg.sh`
- `/usr/lib/af-conn/switch_route.sh`

### Using the Script Commands

#### /usr/lib/af-conn/fwcfg.sh

This command adds or deletes an IP address to the AFERO_ALLOW_WHITELIST_[XXXX] or restarts the firewall.

```
root@raspberrypi3:~# /usr/lib/af-conn/fwcfg.sh –help

usage – fwcfg.sh add|del|add_forwarding|del_forwarding <ip_addr> <comment> [check_fw]
      -- fwcfg.sh restart
<ip_addr> : IP address
<comment> : comment or dns name
<check_fw>: 0 or 1, 1 indicates to check existing rule for <ip_addr> when add
```

For example:

```
$ /usr/lib/af-conn/fwcfg.sh add "52.25.105.3"  "afero service"
$ /usr/lib/af-conn/fwcfg.sh del "52.25.105.3"  "afero service"
```

#### /usr/lib/af-conn/switch_route.sh

This command switches the default route to the specified interface.

For example:

```
$ /usr/lib/af-conn/switch_route.sh wlan0
```

## Afero Service Whitelist

The ASCII file `/etc/af-conn/whitelist` contains a list of the URLs or addresses of the servers used for connectivity services, referred to as the whitelist. The whitelist defines which servers are allowed network access on the device. The format of the whitelist requires that each URL or address be a single entry. The format supports white spaces, empty lines, and comments preceded by the pound (#) sign.

The firewall scripts use this whitelist to define the firewall rules allowing traffic from these services. The Whitelist is read in the `/usr/lib/af-conn/init_firewall.sh` script. It creates an incoming and output permit rule for each server in the whitelist.

The whitelist is also used by the DNS IP extraction module for extracting the IP addresses in the event the server IP changes or if there are multiple balancers available for the same service.

### Platform Notes

- There are two different versions of the file: one for the development or DEV deployment, another one for PROD deployment system. Typically, the DEV deployment is for internal development purpose. Based on the build information, the appropriate file is copied to the image.
- The firewall script entry point is platform-dependent. The implementors may have to do extra work to provide the support for triggering the scripts.

## Traffic Monitoring and Traffic Route Selection

The goal of network connection management is to provide Cloud connectivity to peripheral IoT devices and direct data traffic using the selected network interface. One of the main responsibilities of ConnMgr is to monitor the supported and enabled network interfaces to ensure this connectivity. ConnMgr keeps track of the network connection state, monitors the connection state, and then manages network selection if state changes. For example, in the event of a network connection outage, connection will switch to the next “best” network interface available.

Three network interfaces are supported: Ethernet, Wi-Fi, and wireless modem (WAN); however, not all devices support all three network interfaces. Some devices support only one of these network interfaces (such as Wi-Fi), or a combination of two network interfaces.

### Network Connection States

ConnMgr maintains a network connection state for each network connection. The states are defined as follows:

```
/* INIT */
 NETCONN_STATUS_ITFDOWN_SU = 0,
/* INIT transition state: Dev name is created, no service yet (i.e no ip) */
 NETCONN_STATUS_ITFDOWN_SX = 1,
/* network connection (ie. ethernet) not supported */
 NETCONN_STATUS_ITFNOTSUPP_SX = 2,
/* Interface is UP, Service UNKNOWN */
 NETCONN_STATUS_ITFUP_SU = 3,
/* Interface is UP, confirm connection is good (i.e ping to conclave ok) */
 NETCONN_STATUS_ITFUP_SS = 4,
/* Interface is UP, Service failed */
 NETCONN_STATUS_ITFUP_SF = 5
```

The network connection state acts as a trigger to activate a network switch. Each time the state changes, the module checks to see if there is a need to update and switch the traffic to a "better" network connection. If the criteria to switch is met, then the network is switched and the route table is updated accordingly. The network connection state information is also propagated to other daemons. Specifically, the Wi-Fi network connection state is passed to the Wi-Fi Station daemon, which uses it to reconfigure the Bento device as an AP or Extender.

### Network Connection Monitoring

ConnMgr monitors the network connection using a pcap (packet capture) session that captures the incoming traffic from the external network. When there is incoming traffic from the interface, ConnMgr resets the flags to indicate that the network is reachable and alive. Each monitored interface also has a timer, currently set to two seconds. Every two seconds, the timer handler function checks the flags. One of the flags is an idle timer counter; when the idle timer counter reaches a preset limit, then ConnMgr sends an “echo” message to the service in the Cloud. If the service in the Cloud replies back, we know the network connection is reachable by the service and so we can assume the service is up.

ConnMgr also listens to network device hotplug events and netlink_route events for administrative operational events, such as interface down or interface up by operators. Upon such an event, the traffic route selection engine is invoked to see if we need to switch to a different interface.

### Network Connection Selection Scheme

When the selected network connection becomes unreachable by the Cloud and doesn’t respond to the “echo” response (for whatever reason), ConnMgr assumes that this network is dead and unreachable by the service in the Cloud. It then tries to switch to an available network connection based on the following selection philosophy:

- **Predefined Preference** (most economical) - Preference is in the order of Ethernet, Wi-Fi, then wireless modem. The usage of the wireless modem has a cost associated with it, therefore it’s defined as the last preferred network connection. Wireless modem is only used when all other network connections fail.

    For example, if all three network connections are functional, then ConnMgr selects Ethernet as the network to pass traffic. If the Ethernet connection is down or becomes unreachable, then by preference ConnMgr selects the Wi-Fi connection to pass traffic, assuming it’s up. As a last resort, when both the Ethernet and Wi-Fi connections are out, ConnMgr will use the wireless modem connection.

- **Device Link Status** - The connection status embeds the connection quality (device link) information; e.g., if the Afero Cloud service is “reachable”. The selection scheme always favors the network interface connection with a link status that is connectable and reachable by the Afero Cloud service (i.e., NETCONN_STATUS_ITFUP_SS). For example, if both the Ethernet and Wi-Fi network interfaces are connected, by the predefined preference, we would select Ethernet. However, if we cannot confirm that the Afero Cloud service is reachable on the Ethernet interface, the selection scheme would then choose the Wi-Fi network connection once it is confirmed to be reachable by the Afero Cloud.

Once an interface is selected as the traffic-routing interface, ConnMgr uses the routing table to manage the traffic route selection and directs the data traffic to that particular interface. The route management is done by the script `/usr/lib/af-conn/switch_route.sh`.

## Whitelist IP Extraction via DNS

One of the design principles for the Afero Linux SDK is security. As mentioned, we use iptables and firewall mechanisms to control access to the device. By design, we “lock down" the device INPUT, OUTPUT traffic, except for the allowed Afero services:

- Conclave server (Afero Cloud service)
- Log server
- OTA service
- Echo server

Also allowed are a few well-known functions used by the Afero Linux SDK daemons: ping (request and reply only), DNS, NTP, DHCP.

One of the issues encountered is how to manage the Afero services and punch holes in the firewall to allow these services. The Afero services are hosted in the Cloud and typically behind load balancers, and its service has a DNS-compliant URL name. The DNS-compliant URL name maps to a set of dynamically changing IP addresses during runtime. The challenges are: how to capture these IP addresses at runtime and allow the service traffic to pass through the firewall, hence enabling these services.

ConnMgr implements the functionality of extracting the service IP addresses from the DNS queries on these services. Each time a connection needs to be made to the Afero service on the device, a DNS query is sent out as a result of making a `getaddrinfo` call to get its IP address or addresses. The DNS IP Mapping extraction functionality uses the pcap session to capture the DNS replies to the queries, and parse the answer to extract the Afero service IP addresses. The script `/usr/lib/af-conn/fwcfg.sh` is used to add the IP address to the firewall.

## Attribute Management

On the device, each daemon is responsible for the attributes relevant to its own functionality. These attributes can hold statistical-related, operational state/status-related, or network management-related information. The supported/defined attributes are documented in the [Device Attribute Registry](../AttrRegistry). ConnMgr must register with the Attribute daemon (attrd) at initialization time, and must implement `get` and `set` functionality for each supported attribute.

There are other attributes defined, but ConnMgr supports the following attribute only:

| ATTRIBUTE    | ID    | SIZE | TYPE  | DEFAULT | RANGE | VALUES                                     |
| :----------- | :---- | :--- | :---- | :------ | :---- | :----------------------------------------- |
| Network Type | 65008 | 1    | SINT8 | -       | -1–2  | -1 - None<br>0 - Ethernet<br>1 - WLAN<br>2 - WAN |

Developers can add and support their own attributes by first adding these attributes into the attrd database, then implementing the `get` and `set` functionality of these attributes.

<div class="af-callout-caution">
	<div class="callout-text">
	<p><img src="../img/Caution.svg" width="17" style="vertical-align:bottom;padding:0">&nbsp;&nbsp;<strong>CAUTION!</strong>&nbsp; These attributes must be part of the device Profile to work properly.</p>
	</div>
</div>

