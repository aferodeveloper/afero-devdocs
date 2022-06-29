# Device API Endpoints

The following functions are being exposed via the RESTful API:

- [List Current Device(s) and State Information](../API-DeviceEndpoints#CurrentDevState)
- [Execute an Action on a Device](../API-DeviceEndpoints#DevAction)
- [Update the Friendly Name of a Device](../API-DeviceEndpoints#NameUpdate)

<div class="af-callout">
<div class="callout-text">
<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> Before you begin using the API below, be sure you have read the information in <a href="../CloudAPIs">Afero Cloud APIs</a>.
</div>
</div>

## List Current Device(s) and State Information

List device(s) associated with the specified account.

### Request

| RESOURCE URL                                                 | HTTP METHOD | REQUEST HEADERS                                              | REQUEST PAYLOAD |
| :----------------------------------------------------------- | :---------- | :----------------------------------------------------------- | :-------------- |
| *For all devices:*<br>v1/accounts/{accountId}/devices<br><br>*For single device:*<br> v1/accounts/{accountId}/devices/{deviceId} | GET         | `{  "Content-Type": "application/json",  "Accept": "application/json",  "Authorization": "Bearer <*access_token*>" }` | Not applicable. |

### Example Response, with State Expansion

http://api.afero.io/v1/accounts/acdf8add-915f-495e-8156-b4cfad3fd402/devices?expansions=state


The response below includes details for two devices. Note that this view includes the `deviceState` key and associated data.

```
[
  {
    "deviceId": "f381c366-e102-4d01-b64e-1708ebbd1a7c",
    "profileId": "51415861-0d19-4c54-9c44-42f799fa96a2",
    "updating": false,
    "createdTimestamp": 1448315207125,
    "deviceState": {
      "available": false,
      "visible": false,
      "dirty": false,
      "rebooted": false,
      "connectable": false,
      "connected": false,
      "rssi": 0,
      "linked": false,
      "updatedTimestamp": 1448993366236
    },
    "virtual": false,
    "disconnectNotificationLevel": "LOW",
    "friendlyName": "prototype_1",
    "developerDevice": false,
    "partnerId": "acdf13228-234e-427d-add6-5abdf2jfacdf72",
    "deviceTypeId": "50895f74-5b6f-48a3-9b97-0d26909e0a59"
  },
  {
    "deviceId": "01234462d72ca571",
    "profileId": "855710b5-421c-498c-83e1-1c20a8f25568",
    "updating": false,
    "createdTimestamp": 1447801902003,
    "deviceState": {
      "available": false,
      "visible": false,
      "dirty": false,
      "rebooted": false,
      "connectable": false,
      "connected": false,
      "rssi": 0,
      "location": {
        "latitude": "37.39750353571521",
        "longitude": "-122.1063402868886",
        "lastUpdatedTimestamp": 1448490948016
      },
      "linked": true,
      "updatedTimestamp": 1448650832509
    },
    "virtual": false,
    "disconnectNotificationLevel": "MEDIUM",
    "friendlyName": "autoMate",
    "developerDevice": true,
    "partnerId": "acdf13228-234e-427d-add6-5abdf2jfacdf72",
    "deviceTypeId": "a9fccc1d-b2e1-4822-b4e2-d301b762bd75"
  }
]
```

### Example Response, with Tags Expansion


http://api.afero.io/v1/accounts/acdf8add-915f-495e-8156-b4cfad3fd402/devices?expansions=tags

This is a partial response, showing data for just one device to illustrate the data displayed when the tags expansion is applied. In this case, we see the `deviceTags` key and associated data.

```
...
  {
    "disconnectNotificationLevel": "LOW",
    "deviceTags": [
      {
        "deviceTagType": "SYSTEM",
        "deviceTagId": "45f447db-c312-41a4-9a5c-ab81609cf22a",
        "value": "Entertainment",
        "localizationKey": "entertainment"
      }
    ],
    "virtual": False,
    "createdTimestamp": 1449262135070,
    "deviceTypeId": "a9fccc1d-b2e1-4822-b4e2-d301b762bd75",
    "developerDevice": False,
    "partnerId": "acdf13228-234e-427d-add6-5abdf2jfacdf72",
    "deviceId": "d702ec02-29f4-4aa6-ae0a-30d13616f19c",
    "friendlyName": "",
    "profileId": "855710b5-421c-498c-83e1-1c20a8f25568",
    "updating": False
  }
...
```

### Example Response, with Attributes Expansion

http://api.afero.io/v1/accounts/acdf8add-915f-495e-8156-b4cfad3fd402/devices?expansions=attributes

This is a partial response, showing data for just one device to illustrate the data displayed when the attributes expansion is applied. In this case, we see the `attributes` key and associated data.

```
...
  {
    "deviceId": "f381c366-e102-4d01-b64e-1708ebbd1a7c",
    "profileId": "50895f74-0819-4744-9a44-42f798ed96a2",
    "updating": false,
    "createdTimestamp": 1448315207125,
    "virtual": false,
    "disconnectNotificationLevel": "LOW",
    "friendlyName": "prototype_1",
    "attributes": [
      {
        "id": 1024,
        "data": "0000",
        "updatedTimestamp": 1448650643768
      },
      {
        "id": 1025,
        "data": "0100000000000000",
        "updatedTimestamp": 1448650643908
      },
    ],
    "developerDevice": false,
    "partnerId": "acdf13228-234e-427d-add6-5abdf2jfacdf72",
    "deviceTypeId": "50895f74-5b6f-48a3-9b97-0d26909e0a59"
  }
...
```

### Notes

- This request can be modified with ‘expansions’ to select the type of information that will be returned. Three expansions are available: state, tags, and attributes. They are used by adding them as parameters to the request URL, as shown below.

## Execute an Action on a Device

To execute an action on a given device, submit a request that defines an attribute ID to set, and the value to set it to. In the example below, we are setting attribute ID 1 to value 1 (perhaps to turn on a fan).

### Request

| RESOURCE URL                                       | HTTP METHOD | REQUEST HEADERS                                              | REQUEST PAYLOAD                                              |
| :------------------------------------------------- | :---------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| v1/accounts/{accountId}/devices/{deviceId}/actions | POST        | `{  "Content-Type": "application/json",  "Accept": "application/json",  "Authorization": "Bearer <*access_token*>" }` | `{  "type": "attribute_write",  "attrId": 1,  "data": "01" }` |

### Example Response

```
{
  "type": "attribute_write",
  "requestId": 14,
  "timestampMs": 1449267297894,
  "sender": "ClientApi",
  "source": {
    "type": "user",
    "userId": "123acdfa-asd2-4b26-2cd2-68cfe2acdd8b8",
    "firstName": "Mike",
    "lastName": "Smith"
  }
}
```

### Notes

In the Request Payload:

- The value for `type` can be either `attribute_read` or `attribute_write`. Of course, the specified attribute must support the corresponding operation.
- The value for `data` must be hexadecimal encoded (little endian).
- Because the read/write commands issued by this function are asynchronous, the API call may return before the actual read/write takes place. Keep in mind that if the specified device is offline, the read/write request may not complete.

## Update the Friendly Name of a Device

Change the friendly name of the specified device. This name is visible in the Afero Profile Editor.

### Request

| RESOURCE URL                                            | HTTP METHOD | REQUEST HEADERS                                              | REQUEST PAYLOAD                        |
| :------------------------------------------------------ | :---------- | :----------------------------------------------------------- | :------------------------------------- |
| v1/accounts/{accountId}/devices/{deviceId}/friendlyName | PUT         | `{  "Content-Type": "application/json",  "Accept": "application/json",  "Authorization": "Bearer <*access_token*>" }` | `{  "friendlyName": "cool_new_name" }` |

### Example Response

```
{
  "friendlyName": "cool_new_name"
}
```

<strong>&#8674;</strong> <em>Next:</em>&nbsp;&nbsp;[Over-the-Air (OTA) Update API](../API-OTAEndpoints)