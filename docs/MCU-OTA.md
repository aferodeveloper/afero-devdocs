# Handling MCU OTA Updates

In a [previous topic](../RebootRequests), we looked at how ASR can receive an over-the-air (OTA) firmware update, and should be ready to handle the update by rebooting once the update is complete.

With Release 2.0 of the Afero firmware, we added a new feature: the ability to use the Afero OTA service to deliver not only updates of ASR firmware, but also updates to your MCU application code (or indeed any arbitrary file) running on developer devices. This page will describe what you’ll need to include in your application code to accept an MCU OTA update.

## Overview of Handling an MCU OTA Update

At a high level, the steps involved in an MCU OTA update are as described below.

### Preparation

- Use the Afero OTA Manager to [prepare a firmware update](../OTAMgr) for your device. Note that the MCU OTA mechanism can be used to deliver any type of data to your device, not just MCU code.

- Use the Afero Profile Editor to [define MCU OTA handling](../AttrDef#define-the-mcu-attributes) for the device Profile involved.<br><br>
In order to support receipt of MCU OTA updates, your device Profile must include a few special attributes, including `AF_MCU_OTA_INFO` and `AF_MCU_OTA_TRANSFER`, which convey control information and the transferred data itself, respectively. All special attributes are added for you by the Afero Profile Editor when you select a particular firmware type to be received. Read more about this in the in the Profile Editor User Guide, [Define the MCU Attributes](../AttrDef#define-the-mcu-attributes) section.

- You’ll include code in your MCU application to handle the messaging and data transfers involved in an MCU OTA update. The purpose of this page is to help you with that.

### OTA Delivery

- You’ll use the Afero OTA Manager to [deploy a firmware image](../OTAMgr#deploy-a-firmware-image) to your device.

- The incoming update will be handled as described below. Your MCU code will:
    - Receive an AF_LIB_EVENT_ASR_NOTIFICATION event for attribute AF_MCU_OTA_INFO.
    - Extract from that notification the incoming file type and amount of data to be received.
    - Respond that it is ready to receive the data.
    - Receive the data in the form of AF_LIB_EVENT_ASR_NOTIFICATION events for attribute AF_MCU_OTA_TRANSFER.
    - For each data chunk received, respond with the amount of data received so far. When all expected data has been received, MCU will respond with AF_MCU_OTA_STOP_TRANSFER_OFFSET.
    - Calculate a SHA-256 checksum of the data received, and send that checksum with the message AF_OTA_VERIFY_SIGNATURE, which requests validation of the data.
    - Receive an AF_LIB_EVENT_ASR_NOTIFICATION event for attribute AF_MCU_OTA_INFO, with either:
        - AF_OTA_APPLY, indicating that the data transfer occurred correctly and the download can be used; or,
        - AF_OTA_FAIL, indicating that the checksum did not match, and the downloaded data should be discarded.

### Example MCU OTA: Console Output

Below you’ll see some annotated console output showing an MCU OTA of an 8K test file. The output is from an instrumented form of the example code available at the bottom of this page; we suggest you look at this output and the code together.

```
➀
attrEventCallback received AF_LIB_EVENT_ASR_NOTIFICATION with attrId AF_MCU_OTA_INFO
calling handle_ota_info()

   handle_ota_info: ota_info->state = AF_OTA_TRANSFER_BEGIN
   handle_ota_info: about to get an MCU OTA for type 101 and size 8088
   handle_ota_info: would open file 'ota_file_type.101' to store MCU OTA
   handle_ota_info: Responding with an UPDATE to attribute id AF_MCU_OTA_INFO, which tells ASR to start sending the OTA

➁
attrEventCallback received AF_LIB_EVENT_ASR_NOTIFICATION with attrID AF_MCU_OTA_TRANSFER
calling handle_ota_transfer()

   handle_ota_transfer: received this chunk: 249; total received so far: 249 of 8088 bytes, at 296.08 bytes/sec.
   handle_ota_transfer: Called setAttribute to report that we've received 249 bytes so far.

➂
attrEventCallback received AF_LIB_EVENT_ASR_NOTIFICATION with attrId AF_MCU_OTA_TRANSFER
calling handle_ota_transfer()

   handle_ota_transfer: received this chunk: 6; total received so far: 255 of 8088 bytes, at 21.82 bytes/sec.
   handle_ota_transfer: Called setAttribute to report that we've received 255 bytes so far.

➃
attrEventCallback received AF_LIB_EVENT_ASR_NOTIFICATION with attrId AF_MCU_OTA_TRANSFER
calling handle_ota_transfer()

   handle_ota_transfer: received this chunk: 249; total received so far: 504 of 8088 bytes, at 309.32 bytes/sec.
   handle_ota_transfer: Called setAttribute to report that we've received 504 bytes so far.

// SNIP - several chunk transfers omitted in the interest of brevity //

➄
attrEventCallback received AF_LIB_EVENT_ASR_NOTIFICATION with attrId AF_MCU_OTA_TRANSFER
calling handle_ota_transfer()

   handle_ota_transfer: received this chunk: 249; total received so far: 7921 of 8088 bytes, at 323.38 bytes/sec.
   handle_ota_transfer: Called setAttribute to report that we've received 7921 bytes so far.

➅
attrEventCallback received AF_LIB_EVENT_ASR_NOTIFICATION with attrId AF_MCU_OTA_TRANSFER
calling handle_ota_transfer()

   handle_ota_transfer: received this chunk: 167; total received so far: 8088 of 8088 bytes, at 276.49 bytes/sec.
   handle_ota_transfer: Looks like we're finished. Received: 8088 of 8088 bytes, at 287.10 bytes/sec.
   handle_ota_transfer: Called setAttribute with AF_MCU_OTA_STOP_TRANSFER_OFFSET to say we're done.
   handle_ota_transfer: Finalize the SHA and package it for sending
   handle_ota_transfer: sending up SHA for file

➆
attrEventCallback received AF_LIB_EVENT_ASR_NOTIFICATION with attrId AF_MCU_OTA_INFO
calling handle_ota_info()

   handle_ota_info: ota_info->state = AF_OTA_APPLY
```

**➀** The console output begins when an MCU OTA update is triggered (externally) for this device; for example, by the Afero OTA Manager. At that point the MCU receives notification with the attribute ID = AF_MCU_OTA_INFO, which is treated as a signal to call `handle_ota_info()`.

The attribute *value* received by the callback is cast to a pointer to an `af_ota_info_t`, so we can access the fields by name. Within the `ota_info`, the `state` field is AF_OTA_TRANSFER_BEGIN, which indicates that there’s an incoming OTA.

The `ota_info` also provides us with incoming data size, file name, and file type. In the example code, we track these details (and more) in the global `ota_state`, but that’s an implementation detail (and don’t confuse the global `ota_state` with the `ota_info->state`!). Track progress however you like; the only requirement is that you *must* keep a running count of the bytes downloaded.

At this point, real-world code would probably open a file to save into; in the example we’ll just create a fake file pointer.

Once you’re ready to receive the data, you must call `af_lib_set_attribute_bytes()` for attribute AF_MCU_OTA_INFO, including the same value and valueLen parameters you received in the first place. This signals ASR to start sending OTA update data.If you do *not* want the OTA to proceed after you’ve received the AF_OTA_TRANSFER_BEGIN (e.g. if you attempted to open a file for the data, but that operation failed), then you should set the "state" field in the received "value" to AF_OTA_IDLE, and then call `af_lib_set_attribute_bytes()`. 

**➁**  In response to our `set_attribute()` call signalling that the OTA should begin, we receive notification with the attribute ID = AF_MCU_OTA_TRANSFER. This begins the phase during which the actual data transfer takes place; we handle it in `handle_ota_transfer()`.

The attribute value in AF_MCU_OTA_TRANSFER notifications consists of the data being transferred (one variable-sized chunk at a time), prepended by 4 bytes indicating the offset into the full data payload represented by the current chunk.

After each chunk of data is received, the MCU must respond with how many bytes it has received. To do this, MCU calls `af_lib_set_attribute_bytes()` for attribute AF_MCU_OTA_TRANSFER, with a value equal to the number of bytes of data received so far. When the number of bytes received equals the total bytes expected (i.e. when the transfer is complete), the `received_so_far` value should be set to AF_MCU_OTA_STOP_TRANSFER_OFFSET, which signals that OTA update is complete.

As noted previously, the example code calculates the SHA-256 of the data as it is received. Calculation of the SHA is required, and can be done during the transfer, or once it is complete.

You will notice that the example code tracks the rate of data transfer–this is not required, but is included for demonstration purposes. 

**➂** Another chunk of data is received. Note that the amount of data per chunk can vary widely: in this case only 6 bytes came across. 
**Currently, the maximum chunk size is 253 bytes.** 

**➃** And another chunk. After this one, we snipped out several chunk-transfers, just for readability. 

**➄** Another chunk…we’re getting close to completion!             |

**➅** When your code detects that all the expected data has been received, the MCU must request verification of the data it has received. To do this this, your code must include the SHA checksum and the state `AF_OTA_VERIFY_SIGNATURE` in an `af_ota_info_t` struct, and send that in a call to `af_lib_set_attribute_bytes()` for attribute `AF_MCU_OTA_INFO`. 

**➆** Finally: in response to the verification request, the MCU callback gets one more AF_LIB_EVENT_ASR_NOTIFICATION, in which the `ota_info->state` either confirms or denies the data was received correctly. If the state = AF_OTA_APPLY, the data SHA verified successfully. If the state is AF_OTA_FAIL, the SHA did not have the expected value, and the data should not be trusted, and your code will typically discard it. 

At this point, the MCU OTA update is complete! Assuming the data was downloaded and verified, your MCU code should take whatever context-dependent steps are required to install and use it.

### Example Code to Handle an MCU OTA Update

The simplified sketch code below provides examples of handlers for AF_MCU_OTA_INFO and AF_MCU_OTA_TRANSFER events. This code was used to generate the console output shown on this page.

In order to accept and receive an MCU OTA update, your application code must:

- Handle AF_LIB_EVENT_ASR_NOTIFICATION events for attribute AF_MCU_OTA_INFO, with an attribute value that represents an ```af_ota_info_t``` struct (which is defined in ```af_mcu_ota.h```).

    These AF_MCU_OTA_INFO events inform your MCU code when an MCU OTA update is available, when it’s complete, and so on. The ```af_ota_info_t``` struct contains a lot of information, but one of the highest-level pieces is the ```ota_state_t```. Your code should watch for, and react to the following OTA states:
    
     - AF_OTA_IDLE
     - AF_OTA_TRANSFER_BEGIN
     - AF_OTA_TRANSFER_END
     - AF_OTA_APPLY
     - AF_OTA_FAIL

    In the example code below, this state-handling functionality is contained in the function `handle_ota_info()`.

- Handle AF_LIB_EVENT_ASR_NOTIFICATION events for attribute AF_MCU_OTA_TRANSFER.

    AF_MCU_OTA_TRANSFER events represent the actual data transfer; the value in each of these messages is comprised of a chunk of data being sent, plus 4-byte prefix indicating the current offset into the total data payload.

    In the example code, we handle AF_MCU_OTA_TRANSFER events in the function `handle_ota_transfer()`.

- Calculate a SHA-256 checksum for the downloaded data. Once the download is complete this checksum must be sent to ASR, where it will be verified. Your code will receive confirmation/denial based upon this verification, and should not use the downloaded data unless the checksum is confirmed.

    In the example code, we calculate the SHA as data arrives because the example does not store the download; in real-world applications you are free to calculate the SHA as data arrives, or after the download is complete. The example code handles this task in `handle_ota_transfer()`.

<mark>**Note:**&nbsp; The example code provides the skeleton for handling receipt of an incoming MCU OTA update, but does **not** include any procedures required to install, such as an OTA update once downloaded. Because such procedures will be platform-specific, writing that code is your responsibility.</mark>

```
typedef struct {
  af_ota_begin_info_t begin_info;
  af_ota_apply_info_t apply_info;
  char filename[64];
  int file;
  uint32_t received_so_far;
  long transfer_start_time;
  long chunk_start_time;
} ota_state_t;

static ota_state_t ota_state;
#define OTA_FILE_NAME_PREFIX    "ota_file_type"

static isc_sha256_t sha256;                 // Used for our running SHA of incoming data
af_ota_info_t sha_ota_info;                 // Context for reporting SHA

static void handle_ota_info(const uint16_t valueLen, const uint8_t* value) {

  af_ota_info_t* ota_info = (af_ota_info_t*) value;
  int res = AF_SUCCESS;
  switch (ota_info->state) {
    case AF_OTA_IDLE:
      break;

    case AF_OTA_TRANSFER_BEGIN: {
      Serial.println("handle_ota_info: ota_info->state = AF_OTA_TRANSFER_BEGIN");
      memset(&ota_state, 0, sizeof(ota_state));
      ota_state.begin_info.ota_type = af_utils_read_little_endian_16((const uint8_t*) &ota_info->info.begin_info.ota_type);
      ota_state.begin_info.size = af_utils_read_little_endian_32((const uint8_t*) &ota_info->info.begin_info.size);

      Serial.print("handle_ota_info: about to get an MCU OTA for type ");
      Serial.print(ota_state.begin_info.ota_type);
      Serial.print(" and size ");
      Serial.println(ota_state.begin_info.size);

      // Open a file to save the data we're about to get
      snprintf(ota_state.filename, sizeof(ota_state.filename), "%s.%d", OTA_FILE_NAME_PREFIX, ota_state.begin_info.ota_type);
      Serial.print("handle_ota_info: would open file '"); Serial.print(ota_state.filename); Serial.println("' to store MCU OTA");

      // Example uses a fake file pointer--merely non-null value
      ota_state.file = 1;
      isc_sha256_init(&sha256);

      // Responding with an UPDATE to attribute id AF_MCU_OTA_INFO so ASR will start sending the OTA data
      res = af_lib_set_attribute_bytes(af_lib, AF_MCU_OTA_INFO, valueLen, value, AF_LIB_SET_REASON_LOCAL_CHANGE);
    }
      break;

    case AF_OTA_TRANSFER_END:
      Serial.println("handle_ota_info: ota_info->state = AF_OTA_TRANSFER_END");
      break;

    case AF_OTA_APPLY:
      Serial.println("handle_ota_info: ota_info->state = AF_OTA_APPLY, which confirms SHA verified");
      // At this point app should take whatever action desired with downloaded data
      break;

    case AF_OTA_FAIL:
      Serial.println("handle_ota_info: ota_info->state = AF_OTA_FAIL (SHA didn't verify)");
      break;

  }
}

static void handle_ota_transfer(const uint16_t valueLen, const uint8_t* value) {

  // The format of this attribute is the first 4 bytes are the offset of the data and the rest are the actual data.
  // Updates to this attribute should just contain the next offset you want the data from (or AF_MCU_OTA_STOP_TRANSFER_OFFSET to halt transfer)
  uint32_t offset = af_utils_read_little_endian_32(value);
  bool done = false;

  if (offset != ota_state.received_so_far) {
    // Odd case: we're getting data for an offset that we don't expect. Respond with the correct value
    Serial.print("handle_ota_transfer: got offset ");
    Serial.print(offset);
    Serial.print(" but was expecting ");
    Serial.println(ota_state.received_so_far);
  } else {
    uint32_t amount_to_write = valueLen - sizeof(offset);

    ota_state.received_so_far += amount_to_write;
    long now = af_utils_millis();
    float chunk_speed = ((float) amount_to_write / (now - ota_state.chunk_start_time)) * 1000;

    // Update the SHA. In this example code, we don't save data, we just calculate SHA as data arrives.
    isc_sha256_update(&sha256, (uint8_t*) value + sizeof(offset)-1, amount_to_write);

    Serial.print("handle_ota_transfer: received this chunk: ");
    Serial.print(amount_to_write);
    Serial.print("; total received so far: ");
    Serial.print(ota_state.received_so_far);
    Serial.print(" of ");
    Serial.print(ota_state.begin_info.size);
    Serial.print(" bytes, at ");
    Serial.print(chunk_speed);
    Serial.println(" bytes/sec.");

    // Stop if we've received all data
    if (ota_state.received_so_far >= ota_state.begin_info.size) {
      float total_speed = ((float) ota_state.begin_info.size / (now - ota_state.transfer_start_time)) * 1000;
      Serial.print("handle_ota_transfer: Looks like we're finished. Received: ");
      Serial.print(ota_state.received_so_far);
      Serial.print(" of ");
      Serial.print(ota_state.begin_info.size);
      Serial.print(" bytes, at ");
      Serial.print(total_speed);
      Serial.println(" bytes/sec.");
      ota_state.file = 0;
      ota_state.received_so_far = AF_MCU_OTA_STOP_TRANSFER_OFFSET;
      done = true;
    }
  }

  ota_state.chunk_start_time = af_utils_millis();
  uint8_t result[sizeof(uint32_t)];
  af_utils_write_little_endian_32(ota_state.received_so_far, result);
  int res = af_lib_set_attribute_bytes(af_lib, AF_MCU_OTA_TRANSFER, sizeof(result), result, AF_LIB_SET_REASON_LOCAL_CHANGE);
  if (res != AF_SUCCESS) {
    Serial.print("handle_ota_transfer: error setting attribute ");
    Serial.print(AF_MCU_OTA_TRANSFER);
    Serial.print("; got result ");
    Serial.println(res);
  } else {
    if (done) {
      Serial.print("handle_ota_transfer: Called setAttribute with AF_MCU_OTA_STOP_TRANSFER_OFFSET to say we're done.");
    }
    else {
      Serial.print("handle_ota_transfer: Called setAttribute to report that we've received ");
      Serial.print(ota_state.received_so_far);
      Serial.println(" bytes so far.");
    }
  }

// If we're done because we got all the data expected then send SHA to ASR to verify
  if (done) {
    Serial.println("Finalize the SHA and package it for sending");
    isc_sha256_final(sha_ota_info.info.verify_info.sha, &sha256);

    Serial.println("handle_ota_transfer: sending up SHA for file");
    sha_ota_info.state = AF_OTA_VERIFY_SIGNATURE;
    res = af_lib_set_attribute_bytes(af_lib, AF_MCU_OTA_INFO, sizeof(sha_ota_info), (uint8_t * ) & sha_ota_info, AF_LIB_SET_REASON_LOCAL_CHANGE);
    if (res != AF_SUCCESS) {
      Serial.print("handle_ota_transfer: error setting attribute: ");
      Serial.print(AF_MCU_OTA_INFO);
      Serial.print("; err: ");
      Serial.println(res);
    }
  }
}

void attrEventCallback(const af_lib_event_type_t eventType,
                       const af_lib_error_t error,
                       const uint16_t attributeId,
                       const uint16_t valueLen,
                       const uint8_t* value) {

  switch (eventType) {

    case AF_LIB_EVENT_ASR_NOTIFICATION:

      switch (attributeId) {
        case AF_MCU_OTA_INFO:
          Serial.println("attrEventCallback received attribute AF_MCU_OTA_INFO");
          handle_ota_info(valueLen, value);
          break;
        case AF_MCU_OTA_TRANSFER:
          Serial.println("attrEventCallback received attribute AF_MCU_OTA_TRANSFER");
          handle_ota_transfer(valueLen, value);
          break;
        case AF_SYSTEM_ASR_STATE:
// SNIP //
          break;
      }
      break;
  }
}

void setup() {
// SNIP - the usual setup() //
}

void loop() {
//SNIP - the usual loop() //
}
```