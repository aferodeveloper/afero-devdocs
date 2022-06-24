# Edge Device Daemon Implementation

Before reading this section, we recommend you familiarize yourself with the Afero Platform concepts by reading [Introducing Afero](../Overview).

One of the core Afero Platform concepts is *attributes*. Attributes represent various device states or functionalities that a user is able to control. The Edge Device daemon on the Linux-based device can function as a hub and edge device to control these attributes.

The Afero Linux Device SDK provides a conceptually simple API called afLib to assist the implementation of edge device functionality. On the Linux-based device, afLib comes in the form of a dynamically-shared library known as libaf-edge. The Edge Device daemon can be implemented by writing a C daemon that uses libevent2 and the af-edge library, as described below. The af-edge library is a shim on top of the Attribute Client API.

From the Afero system point of view, the edge device is implemented by adding attributes to the Afero device Profile. These attributes can have IDs in the range 1 to 1023 and are freely-assignable by you as the developer. The Edge Device daemon bears the responsibility for managing these attributes and connecting them to the hardware.

The Edge Device daemon must run in a libevent2 event loop, and link with the libevent2, af-ipc, af-util, and af-edge libraries.

The af-edge client API is functionally equivalent to the afLib device API. This section translates the concepts from the afLib device API to the af-edge API. For more information about the afLib API please read [afLib API](../API-afLib).

## Building

You will need to link with the dynamically-shared library af-edge.so, its dependencies, and libevent:

```
-laf_edge -laf_attr -laf_ipc -laf_util -levent
```

You might also need to tell your C compiler to use C99 mode:

```
-std=c99
```

The Afero af-edge library provides the `init` script for the Edge Device daemon (to start up the service), provided you use “edged” as the name of the daemon. Install the edged daemon in the /usr/bin/ directory of the image. If you do not, you must include the `init` script as part of the daemon development so the daemon initializes during system startup. Note that the `init` system can be different, depending on the platform (as previously mentioned).

## Using the af-edge API

As an example, a minimal implementation of an edge application is provided for you below:

```
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <af_util.h>
#include <aflib.h>
static void event_handler(const af_lib_event_type_t event_type, const af_lib_error_t error, const uint16_t attr_id, const uint16_t value_len, const uint8_t *value)
{
    AFLOG_INFO("EDGE_EVENT:event_type=%d, error=%d", event_type, error);
    switch(event_type) {
        case AF_LIB_EVENT_MCU_DEFAULT_NOTIFICATION :
            /* you can use this notification to initialize the value of an attribute to its default value */
            break;
        case AF_LIB_EVENT_MCU_SET_REQUEST : {
            /* the service is requesting to set an MCU attribute to the specified value.*/
            /* you must set set_succeeded to true or false */
            bool set_succeeded = true;
            af_lib_send_set_response(s_af_lib, attr_id, set_succeeded, value_len, value);
            break;
        }
        case AF_LIB_EVENT_ASR_NOTIFICATION :
            /* the attribute with the specified attr_id has been set to the specified value with the specified length */
            break;
        case AF_LIB_EVENT_MCU_SET_REQ_SENT :
            /* attribute update notification for the specified attr_id was sent to ASR */
            break;
        case AF_LIB_EVENT_MCU_SET_REQ_REJECTION :
            /* attribute update notification for specified attr_id was rejected by ASR */
            /* error contains the failure code */
        case AF_LIB_EVENT_ASR_GET_REQUEST: :
            /* the ASR has responded to a get request for one of its attributes */
            /* xxx is (bool, 8, 16, 32, 64, str or bytes) depending on your attribute type */            
            /* AF_LIB_SET_REASON_GET_RESPONSE means that this 'set' is a reply to the get request */
            
            af_lib_set_attribute_xxx(s_af_lib, attr_id, your_local_attr, AF_LIB_SET_REASON_GET_RESPONSE);
        
        default :
            /* this should never happen */
            break;
    }
}

int main(int argc, char **argv) {
    struct event_base *ev = event_base_new();

    AFLOG_INFO("EDGE::start");
    /* enable pthreads */
    evthread_use_pthreads();
 

    af_lib_set_event_base(ev);

    if ((s_af_lib = af_lib_create_with_unified_callback(event_handler, NULL)) == NULL) {
        perror("aflib_init");
        exit(1);
    }
   
   /* start the main event loop */ 
   event_base_dispatch(ev);
 
       /* Done: clean up */

   af_lib_shutdown();
   return (retVal);
}
```

If you look at the `main` function you see the steps in setting up afLib on Linux:

1. Create an event_base using the `event_base_new()` function in libevent2.
2. Call `af_lib_set_event_base()` to connect afLib to the event base from step 1.
3. Call `aflib_create_with_unified_callback()` specifying the event handler. The init function will connect to the hub software (“hubby”) and begin monitoring attribute updates. It writes logs to syslog.
4. Call `event_base_dispatch()` or `event_base_loop()` to run your event loop.

Differences from afLib:

- There is no `loop()` call. Instead, you must make sure the libevent2 event loop gets a chance to run, either by calling `event_base_dispatch` or `event_base_loop` periodically.
- There is no ISR or MCU support. The af-edge library assumes that your daemon runs as a normal Linux process on the device.
- Logging is done to syslog, instead of a supplied stream.
- The concept of transport is not supported. The “transport” implements a serial communication protocol for communicating with an MCU attached to the device.

<strong>&#8674;</strong> <em>Next:</em>&nbsp;&nbsp;[OTA Manager Daemon Implementation](../LinuxSDK-OTAManager)