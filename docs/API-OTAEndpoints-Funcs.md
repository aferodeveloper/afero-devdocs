# OTA API Endpoints

Before you begin, make sure you have read the background sections in [Over-the-Air (OTA) Updates API](../API-OTAEndpoints). Below, you’ll find all the endpoint details, organized as follows:

- [Before You Begin](../API-OTAEndpoints-Funcs#beforebegin)
    - [Problem Description](../API-OTAEndpoints-Funcs#probdescr)
    - [Approach](../API-OTAEndpoints-Funcs#approach)
    - [Prerequisites](../API-OTAEndpoints-Funcs#prerequisites)
    - [General Notes on Using the OTA API Endpoints](../API-OTAEndpoints-Funcs#general-notes-on-using-the-ota-api-endpoints)
- [Firmware Type Endpoints](../API-OTAEndpoints-Funcs#firmware-type-endpoints)
    - [POST /v1/ota/partners/{partnerId}/types](../API-OTAEndpoints-Funcs#post-v1otapartnerspartneridtypes)
    - [GET /v1/ota/partners/{partnerId}/types](../API-OTAEndpoints-Funcs#get-v1otapartnerspartneridtypes)
    - [GET /v1/ota/partners/{partnerId}/types/{type}](../API-OTAEndpoints-Funcs#get-v1otapartnerspartneridtypestype)
    - [PUT /v1/ota/partners/{partnerId}/types/{type}](../API-OTAEndpoints-Funcs#put-v1otapartnerspartneridtypestype)
- [Firmware Pool Image Endpoints](../API-OTAEndpoints-Funcs#firmware-pool-image-endpoints)
    - [POST /v1/ota/partners/{partnerId}/pool](../API-OTAEndpoints-Funcs#post-v1otapartnerspartneridpool)
    - [POST /v1/ota/partners/{partnerId}/binaries](../API-OTAEndpoints-Funcs#post-v1otapartnerspartneridbinaries)
    - [POST /v1/ota/partners/{partnerId}/binaries/moveToRepository](../API-OTAEndpoints-Funcs#post-v1otapartnerspartneridbinariesmovetorepository)
    - [GET /v1/ota/partners/{partnerId}/pool](../API-OTAEndpoints-Funcs#get-v1otapartnerspartneridpool)
    - [GET /v1/ota/partners/{partnerId}/pool/types/{type}](../API-OTAEndpoints-Funcs#get-v1otapartnerspartneridpooltypestype)
    - [GET /v1/ota/partners/{partnerId}/pool/types/{type}/versionNumbers/{versionNumber}/associations](../API-OTAEndpoints-Funcs#get-v1otapartnerspartneridpooltypestypeversionnumbersversionnumberassociations)
    - [PUT /v1/ota/partners/{partnerId}/pool/types/{type}/versionNumbers/{versionNumber}](../API-OTAEndpoints-Funcs#put-v1otapartnerspartneridpooltypestypeversionnumbersversionnumber)
- [Firmware Image Endpoints](../API-OTAEndpoints-Funcs#firmware-image-endpoints)
    - [POST /v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages](../API-OTAEndpoints-Funcs#post-v1otapartnerspartneriddevicetypesdevicetypeidfirmwareimages)
    - [GET /v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages](../API-OTAEndpoints-Funcs#get-v1otapartnerspartneriddevicetypesdevicetypeidfirmwareimages)
    - [GET /v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages/types/{type}](../API-OTAEndpoints-Funcs#get-v1otapartnerspartneriddevicetypesdevicetypeidfirmwareimagestypestype)
    - [GET /v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages/types/{type}/versionNumbers/{versionNumber}](../API-OTAEndpoints-Funcs#get-v1otapartnerspartneriddevicetypesdevicetypeidfirmwareimagestypestypeversionnumbersversionnumber)
    - [PUT /v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages/{firmwareImageId}/push](../API-OTAEndpoints-Funcs#put-v1otapartnerspartneriddevicetypesdevicetypeidfirmwareimagesfirmwareimageidpush)
    - [DELETE /v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages/types/{type}/versionNumbers/{versionNumber}](../API-OTAEndpoints-Funcs#delete-v1otapartnerspartneriddevicetypesdevicetypeidfirmwareimagestypestypeversionnumbersversionnumber)
- [Firmware Tag Endpoints](../API-OTAEndpoints-Funcs#firmware-tag-endpoints)
    - [GET /v1/ota/partners/{partnerId}/tags](../API-OTAEndpoints-Funcs#get-v1otapartnerspartneridtags)

## Before You Begin

### Problem Description

Similar to any other software in development, firmware is usually set up in CD/CI fashion to streamline development and deployments; i.e., commits to source control trigger builds, which produce artifacts, which are then stored in an artifact repository for testing by QA. While Afero offers a web application to manage firmware images it requires manual interaction.


### Approach

Depending on the partner’s internal development and build processes, the usage of the APIs for creating and managing firmware records can differ. This document will highlight some common use cases and show how the different APIs can be used to accommodate different development processes. This page will document first the different API endpoints and then describe in detail different use cases and how the APIs are used in each case and why.

### Prerequisites

Afero account authentication flow follows the OAuth 2.0 standard as described in [RFC6749](https://tools.ietf.org/html/rfc6749). The API documentation uses JSON format for data model examples. The full prerequisites are listed below:

- Understand how Afero authorization API (OAuth) works. To read about OAuth, go to [OAuth.net documentation](http://oauth.net/documentation/). Also please read [Cloud API](../CloudAPIs).
- Base Cloud API URL: [https://api.afero.io](https://api.afero.io/).
- Understanding of JSON and common HTTP methods POST, PUT, GET, and DELETE and their usage in the context of RESTful web services.
- Be familiar with public [OAuth/User API Endpoints](../API-UserEndpoints) and [Device API Endpoints](../API-DeviceEndpoints).
- Understanding of how to authenticate with the Afero Cloud and how to use authorization tokens in subsequent requests.
- Client credentials as described in [RFC6749 Section 2](https://tools.ietf.org/html/rfc6749#section-2) for client application authentication.

<div class="af-callout-caution">
<div class="callout-text">
<p><img src="../img/Caution.svg" width="17" style="vertical-align:bottom;padding:0">&nbsp;&nbsp;<strong>CAUTION!</strong>  In production applications, <strong>do not</strong> use developer credentials obtained through the Afero Profile Editor!</p>
</div>
</div>

### General Notes on Using the OTA API Endpoints

- Optional fields can be omitted from request payloads and are omitted from response bodies returned by the service.

- All endpoints returning a list of objects return a paged result set. The default page size is 50 objects.

    - The contents of the result set can be influenced by using paging-related query parameters, such as page number (zero-based index), page size, and sort information (field and sort direction).
    
    - The result set object contains information about the total number of elements, total number pages, number of elements in the result set, and the objects themselves. This information can be used to implement a paging mechanism in clients.

- Path parameters are always required. They are enclosed by two curly braces **`{parameter}`** in the path of the resource for identification purposes. All other REQUEST PARAMETERS are query parameters. Required parameters are highlighted in **bold**.

- Certain IDs and integer values returned by the API are represented as strings. For example, the id and versionNumber fields are such instances. They are represented as strings as they may exceed common ranges for numbers, especially in JavaScript, whose whole integers are only precise up to 53 bits.

    If arithmetic operations need to be performed on these values, they can be converted using a library or type that supports arbitrary-precision integers; i.e., the [BigInteger](https://docs.oracle.com/en/java/javase/11/docs/api/java.base/java/math/BigInteger.html) type in Java, the [big-integer](https://www.npmjs.com/package/big-integer) package, or BigInt type in JavaScript. These fields are annotated as `"<integer_string>"` in the model schemas.

## Firmware Type Endpoints

This section lists the endpoints that manage firmware types. Make sure you have read [About Firmware Types](../API-OTAEndpoints#about-firmware-types).

### POST /v1/ota/partners/{partnerId}/types

Creates a new partner firmware type.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/types`<br>
**HTTP METHOD:**`POST`<br>
**RESPONSE CODE:** `201 (CREATED)`<br>
**REQUEST PARAMETERS:** <br>
&nbsp; - `{partnerID}` - The partner ID.<br>
**REQUEST HEADERS:**
```
{ 
 "Content-Type": "application/json", 
 "Accept": "application/json", 
 "Authorization": "Bearer <access_token>"
}
```

**REQUEST PAYLOAD MODEL SCHEMA:** 
```
{ 
 "name": "<string>", 
 "description": "<string, optional>", 
 "type": <integer>
}
```

**RESPONSE BODY MODEL SCHEMA:**

```
{ 
 "id": "<integer_string>", 
 "name": "<string>", 
 "description": "<string, optional>", 
 "type": <integer>, 
 "partnerId": "<string>", 
 "createdTimestamp": <integer>, 
 "updatedTimestamp": <integer>, 
 "versionAttributeId": <integer>
}                                     
```

### GET /v1/ota/partners/{partnerId}/types

Retrieves partner firmware types.

**RESOURCE URL:**   `/v1/ota/partners/{partnerId}/types`<br>
**HTTP METHOD:** `GET`                               
**RESPONSE CODE:** `200 (OK)`                       
**REQUEST PARAMETERS:**<br>
&nbsp; - `{partnerID}` - The partner ID.                   
**REQUEST HEADERS:**                                                                           
```
{ 
 "Accept": "application/json", 
 "Authorization": "Bearer <access_token>"
}
```
                                  
**RESPONSE BODY MODEL SCHEMA:**                                                                         
```
[ 
  { "id": "<integer_string>", 
    "name": "<string>", 
    "description": "<string, optional>", 
    "type": <integer>, 
    "partnerId": "<string>", 
    "createdTimestamp": <integer>, 
    "updatedTimestamp": <integer>, 
    "versionAttributeId": <integer>
   }
]
```                                       


### GET /v1/ota/partners/{partnerId}/types/{type}

Retrieves a firmware type, by type.

**RESOURCE URL:**  `/v1/ota/partners/{partnerId}/types/{type}`<br>
**HTTP METHOD:** `GET`                                      
**RESPONSE CODE:** `200 (OK)`                                 
**REQUEST PARAMETERS:**                                                                                        
&nbsp; - `{partnerID}` - The partner ID.                             
&nbsp; - `{type}` - The firmware type.                          
**REQUEST HEADERS:**
```                                                                                   
{
 "Accept": "application/json",
 "Authorization": "Bearer <access_token>"
}
```
                                              
**RESPONSE BODY MODEL SCHEMA:**
```
[
 {
  "id": "<integer_string>",
  "name": "<string>",
  "description": "<string, optional>",
  "type": <integer>,
  "partnerId": "<string>",
  "createdTimestamp": <integer>,
  "updatedTimestamp": <integer>,
  "versionAttributeId": <integer> 
 }
]
```

### PUT /v1/ota/partners/{partnerId}/types/{type}

Updates a partner firmware type.
**RESOURCE URL:** `/v1/ota/partners/{partnerId}/types/{type}` 
**HTTP METHOD:** `PUT`                                       
**RESPONSE CODE:** `204 (No Content)`                          
**REQUEST PARAMETERS:**                                                                                        
&nbsp; - `{partnerID}` - The partner ID.                                                              
&nbsp; - `{type}` - The firmware type.                          
**REQUEST HEADERS:**
```                                                                                           
{
 "Content-Type": "application/json",
 "Authorization": "Bearer <access_token>"
}
```
                                             
**REQUEST PAYLOAD MODEL SCHEMA:**                                                                              
```
{
 "name": "<string>",
 "description": "<string, optional>"
}
```                                              
## Firmware Pool Image Endpoints

This section lists the endpoints that manage images in the firmware pool. Make sure you have read [About Firmware Pools and Associations](../API-OTAEndpoints#aboutfwpool).

### POST /v1/ota/partners/{partnerId}/pool

Creates a new firmware record in the firmware image pool.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/pool` <br>
**HTTP METHOD:** `POST`                              
**RESPONSE CODE:** `201 (Created)`                     
**REQUEST PARAMETERS:**                                                                                
&nbsp; - `{partnerID}` - The partner ID.                                                      
**REQUEST HEADERS:**
```                                                                                   
{
  "Content-Type": "application/json",
  "Accept": "application/json",
  "Authorization": "Bearer <access_token>"
}
``` 
                                     
**REQUEST PAYLOAD MODEL SCHEMA:** 
```                                                                   
{ 
 "name": "<string>", 
 "description": 
 "<string, optional>", 
 "type": <integer> "version": "<string>", 
 "url": "<string>", "tags": ["<string, optional>"], 
 "associations": { "key1": ["<string, optional>", "..."]
 }
``` 
                                     
**RESPONSE BODY MODEL SCHEMA:**
```                                                                        
{ 
  "id": "<integer_string>", 
  "name": "<string>", 
  "description": "<string, optional>", 
  "type": <integer>, 
  "partnerId": "<string>", 
  "createdTimestamp": <integer>, 
  "updatedTimestamp": <integer>, 
  "versionAttributeId": <integer>
}
```                                      

**Notes**

- The `associations` field in the request payload is optional. The “key” is the partner ID, the values are an array of device type IDs. A typical payload would look as follows:

```
"associations": {
   "partnerId1": ["deviceTypeId1", "deviceTypeId2"], 
   "partnerId2": ["deviceTypeIdA", "deviceTypeIdB"]
}
```

Associations can be created or deleted any time after a firmware image has been created in the pool.

### POST /v1/ota/partners/{partnerId}/binaries

Uploads a firmware file to a temporary location.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/binaries`<br>
**HTTP METHOD:** `POST`                                  
**RESPONSE CODE:** `200 (OK)`                              
**REQUEST PARAMETERS:**                                                                                    
&nbsp; - `{partnerID}` - The partner ID.                                                          
**REQUEST HEADERS:**
```                                                                                       
{ 
  "Content-Type": ["application/octet-stream", 
  "multipart/form-data"], 
  "Accept": "application/json", 
  "Authorization": "Bearer <access_token>"
} 
```                                      

**RESPONSE BODY MODEL SCHEMA:**                                                                          
```
{ 
 "value": "<string>"
}
```                                                                              

**Notes**

- The API offers two methods to upload a file:
    - Upload via a file stream using the content type `application/octet-stream`, or
    - Upload via a web browser-style file upload using `multipart/form-data`.
- The returned value is a file identifier of the uploaded file in a temporary location. It the SHA-256 hash of the file.

### POST /v1/ota/partners/{partnerId}/binaries/moveToRepository

Moves a file from the temporary location to the permanent firmware image repository.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/binaries/moveToRepository`<br> 
**HTTP METHOD:** `POST`                                                   
**RESPONSE CODE:** `200 (OK)`                                               
**REQUEST PARAMETERS:**                                                                                                     
- `{partnerID}` - The partner ID.                                          
**REQUEST HEADERS:**
```                                                                                                        

{ 
  "Content-Type": "application/json", 
  "Accept": "application/json", 
  "Authorization": "Bearer <access_token>"
}
```                                                           
**REQUEST PAYLOAD MODEL SCHEMA:**                                                                                           
```
{ 
  "value": "<string>",
}
```                                                                                              

**RESPONSE BODY MODEL SCHEMA:**                                                                                            
```
{ 
  "value": "<string>"
}
```                                                                                               

**Notes**

- Use the response body from the file upload endpoint as the payload.
- The response body will contain the URL of the firmware binary in the firmware repository. This URL should be used to update the respective firmware pool record; otherwise, the service will not be able to send OTAs.

### GET /v1/ota/partners/{partnerId}/pool

Retrieves some firmware images from the pool.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/pool`<br>                       
**HTTP METHOD:** `GET`                                                        
**RESPONSE CODE:** `200 (OK)`                                                   
**REQUEST PARAMETERS:**                                                                                                         
&nbsp; - `{partnerID}` - The partner ID.                                                                               
&nbsp; - `tags` - Comma-separated list of tags on which to filter.             
&nbsp; - `page` - Zero-based index of page to retrieve.                        
&nbsp; - `size` - The number of elements per page.                             
&nbsp; - `sort` - The field and sort direction; e.g., `updatedTimestamp`, `description`. 

**REQUEST HEADERS:**
```
{ 
  "Accept": "application/json", 
  "Authorization": "Bearer <access_token>"
}
```                                                               

**RESPONSE BODY MODEL SCHEMA:**
```                                                                                                
{ 
  "number": <integer>, 
  "size": <integer>, 
  "totalPages": <integer>, 
  "numberOfElements": <integer>, 
  "totalElements": <integer>, 
  "content": 
  [ 
    {  
      "id": "<string_integer>",  
      "name": "<string>",  
      "description": "<string, optional>",  
      "type": <integer>,  
      "versionNumber": "<string_integer>",  
      "version": "<string>",  
      "url": "<string>",  
      "tags": ["<string, optional>", "..."],  
      "createdTimestamp": <integer>,  
   	   "updatedTimestamp": <integer>,  
      "partnerId": "<string>" 
   } 
  ], 
  "sort": "<string>"
}
```                                                               

### GET /v1/ota/partners/{partnerId}/pool/types/{type}

Retrieves some firmware images of a specific type from the pool.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/pool/types/{type}`             
**HTTP METHOD:** `GET`                                                        
**RESPONSE CODE:** `200 (OK)`                                                   
**REQUEST PARAMETERS:**                                                                                                         
&nbsp; - `{partnerID}` - The partner ID.                                                                               
&nbsp; - `{type}` - The firmware type.                                           
&nbsp; - `tags` - Comma-separated list of tags on which to filter.             
&nbsp; - `page` - Zero-based index of page to retrieve.                        
&nbsp; - `size` - The number of elements per page.                             
&nbsp; - `sort` - The field and sort direction; e.g., `updatedTimestamp`, `description`. 

**REQUEST HEADERS:**
```                                                                                                            
{ 
  "Accept": "application/json", 
"Authorization": "Bearer <access_token>"
}
```                                                               

**RESPONSE BODY MODEL SCHEMA:**
```                                                                                                 
{ 
  "number": <integer>, 
  "size": <integer>, 
  "totalPages": <integer>, 
  "numberOfElements": <integer>, 
  "totalElements": <integer>, 
  "content": 
    [ 
      {  
        "id": "<string_integer>",  
        "name": "<string>",  
        "description": "<string, optional>",  
        "type": <integer>,  
        "versionNumber": "<string_integer>",  
        "version": "<string>",  
        "url": "<string>",  
        "tags": ["<string, optional>", "..."],  
        "createdTimestamp": <integer>,  
        "updatedTimestamp": <integer>,  
        "partnerId": "<string>" 
     } 
    ], 
    "sort": "<string>"}
```                                                               

### GET /v1/ota/partners/{partnerId}/pool/types/{type}/names/{name}/versions/{version}/exists

Checks if a firmware image with given type, name, and version already exists in the pool.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/pool/types/{type}/names/{name}/versions/{version}/exists` <br>
**HTTP METHOD:** `GET`                                                        
**RESPONSE CODE:** `200 (OK)`                                                   
**REQUEST PARAMETERS:**                                                                                                         
&nbsp; - `{partnerID}` - The partner ID.                                                                               
&nbsp; - `{type}` - The firmware type.                                           
&nbsp; - `{name}` - The name of the firmware image.                              
&nbsp; - `{version}` - The version string of the firmware image.                    
&nbsp; - `excludeFirmwareImageId` - The ID of a record to be excluded. Here, clients can use the ID of a record that is about to be updated to verify that no other record with this information already exists. 

**REQUEST HEADERS:**

```                                                                                                            
{ 
  "Accept": "application/json", 
  "Authorization": "Bearer <access_token>"
}
```                                                               

**RESPONSE BODY MODEL SCHEMA:**
```                                                                                                 
{ 
  "value": <boolean>,
}
```                                                                                                   

**Notes**

- Retrieves all device type associations of a given firmware image.

### GET /v1/ota/partners/{partnerId}/pool/types/{type}/versionNumbers/{versionNumber}/associations

Retrieves all device type associations of a given firmware image.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/pool/types/{type}/versionNumbers/{versionNumber}/associations`<br>
**HTTP METHOD:** `GET`                                                        
**RESPONSE CODE:** `200 (OK)`                                                   
**REQUEST PARAMETERS:**                                                                                                         
&nbsp; - `{partnerID}` - The partner ID.                                                                               
&nbsp; - `{type}` - The firmware type.                                           
&nbsp; - `{versionNumber}` - The version number of the firmware image.                    

**REQUEST HEADERS:**

```                                                                                                            
{ 
  "Accept": "application/json", 
  "Authorization": "Bearer <access_token>"
}
```                                                               

**RESPONSE BODY MODEL SCHEMA:** 
                                                                                               
```
[ 
 { 
   "name": "<string>", 
   "partnerId": "<string>" 
   "email": "<string>", 
   "deviceTypeId": "<string>", 
   "deviceTypeName": "<string>", 
   "firmwareImageId": "<string_integer>" 
 }
]
```                                                              

### PUT /v1/ota/partners/{partnerId}/pool/types/{type}/versionNumbers/{versionNumber}

Updates a firmware image in the pool.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/pool/types/{type}/versionNumbers/{versionNumber}`<br> 
**HTTP METHOD:** `PUT`                                                        
**RESPONSE CODE:** `204 (No Content)`                                           
**REQUEST PARAMETERS:**                                                                                                         
- `{partnerID}` - The partner ID.                                                                               
- `{type}` - The firmware type.                                           
- `{versionNumber}` - The version number of the firmware image.                    

**REQUEST HEADERS:**
```                                                                                                            
{ 
  "Content_Type": "application/json", 
  "Authorization": "Bearer <access_token>"
}
``` 
                                                           
** REQUEST PAYLOAD MODEL SCHEMA:**
```                                                                                               
{ 
  "name": "<string>", 
  "description": "<string, optional>", 
  "version": "<string>", 
  "url": "<string>", 
  "tags": ["<string, optional>", "..."]
}
```                                                               
## Firmware Image Endpoints

This section lists the endpoints that manage firmware images.

### POST /v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages

Creates a new firmware image association with a device type.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages`<br>
**HTTP METHOD:** `POST`                                                       
**RESPONSE CODE:** `201 (Created)`                                              
**REQUEST PARAMETERS:**                                                                                                         
- `{partnerID}` - The partner ID.                                                                               
- `{deviceTypeId}` - The device type ID.

**REQUEST HEADERS:**

```                                                                                                          
{ 
  "Content_Type": "application/json", 
  "Accept": "application/json", 
  "Authorization": "Bearer <access_token>"
}
```                                                               

**REQUEST PAYLOAD MODEL SCHEMA:**
```                                                                                              
{ 
  "name": "<string>", 
  "description": "<string, optional>", 
  "type": "<integer>", "versionNumber": "<string>", 
  "version": "<string>", "url": "<string>", 
  "tags": ["<string, optional>", "..."], 
  "associations": { "key1": ["<string, optional>", "..."]}
}
``` 
                                                             
**RESPONSE BODY MODEL SCHEMA:**
```                                                                                                
{ 
  "id": "<string>", 
  "name": "<string>", 
  "description": "<string, optional>", 
  "type": "<integer>", 
  "versionNumber": "<string>", 
  "version": "<string, optional>", 
  "url": "<string>", 
  "tags": ["<string, optional>", "..."], 
  "createdTimestamp": "<integer>", 
  "updatedTimestamp": "<integer>", 
  "partnerId": "<string>", 
  "deviceTypeId": "<string>"
}
```                                                               

**Notes**

- This endpoint creates an association between a generic firmware image record in the firmware pool with a specific device type. Thus, the `versionNumber` field is a required field in this request payload.
- It is recommended that you use the same object as request payload as is returned from one of the GET endpoints in the Firmware Pool namespace. Modifying the payload from the original pool record will cause errors and prevent OTAs from succeeding!

### GET /v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages

Retrieves some firmware images.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages` <br>
**HTTP METHOD:** `GET`                                                        
**RESPONSE CODE:** `200 (OK)`                                                   
**REQUEST PARAMETERS:**                                                                                                         
&nbsp; - `{partnerID}` - The partner ID.                                                                               
&nbsp; - `{deviceType}` - The device type ID.                                          
&nbsp; -  `tags` - Comma-separated list of tags on which to filter.             
&nbsp; -  `page` - Zero-based index of page to retrieve.                        
&nbsp; -  `size` - The number of elements per page.                             
&nbsp; -  `sort` - The field and sort direction; e.g., `updatedTimestamp`, `description`. 

**REQUEST HEADERS:**
```                                                                                                            
{ 
  "Accept": "application/json", 
  "Authorization": "Bearer <access_token>"
}
```                                                               

**RESPONSE BODY MODEL SCHEMA:**
```                                                                                                 
{ 
  "number": <integer>, 
  "size": <integer>, 
  "totalPages": <integer>, 
  "numberOfElements": <integer>, 
  "totalElements": <integer>, 
  "content": 
  [ 
    {  
      "id": "<string>",  
      "name": "<string>",  
      "description": "<string, optional>",  
      "type": <integer>,  
      "versionNumber": "<string_integer>",  
      "version": "<string, optional>",  
      "url": "<string>",  
      "tags": ["<string, optional>", "..."],  
      "createdTimestamp": <integer>,  
      "updatedTimestamp": <integer>,  
      "partnerId": "<string>",  
      "deviceTypeId": "<string>",  
      "deviceDescriptionId": "<string>", 
      "deviceProfileId": "<string>" 
    } 
  ], 
  "sort": "<string>"
}
```                                                               

**Notes**

- This endpoint retrieves a paged result set of associated firmware image records for a specific device type.
- The firmware image records of type 4 (DEVICE_DESCRIPTION) will have two additional fields that identify the related entities that make up a device Profile in the Afero Platform.

### GET /v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages/types/{type}

Retrieves some firmware images by type.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages/types/{type}`<br>
**HTTP METHOD:** `GET`                                                        
**RESPONSE CODE:** `200 (OK)`                                                   
**REQUEST PARAMETERS:**                                                                                                         
&nbsp; - `{partnerID}` - The partner ID.                                                                               
&nbsp; - `{deviceType}` -The device type ID.                                          
&nbsp; - `{type}` - The firmware type.                                           
&nbsp; - `tags` - Comma-separated list of tags on which to filter.             
&nbsp; - `page` - Zero-based index of page to retrieve.                        
&nbsp; - `size` - The number of elements per page.                             
&nbsp; - `sort` - The field and sort direction; e.g., `updatedTimestamp`, `description`. 

**REQUEST HEADERS:**
```                                                                                                            
{ 
  "Accept": "application/json", 
  "Authorization": "Bearer <access_token>"
}
```                                                               
 
**RESPONSE BODY MODEL SCHEMA:**                                                                                                 
```
{ 
  "number": <integer>, 
  "size": <integer>, 
  "totalPages": <integer>, 
  "numberOfElements": <integer>, 
  "totalElements": <integer>, 
  "content": 
    [ 
      {  
        "id": "<string>",  
        "name": "<string>",  
        "description": "<string, optional>",  
        "type": <integer>,  
        "versionNumber": "<string_integer>",  
        "version": "<string, optional>",  
        "url": "<string>",  "tags": ["<string, optional>"],  
        "createdTimestamp": <integer>,  
        "updatedTimestamp": <integer>,  
        "partnerId": "<string>",  
        "deviceTypeId": "<string>" 
      } 
    ], 
    "sort": "<string>"
}
```                                                               

**Notes**

- This endpoint retrieves the associated firmware image records for a specific device type and firmware type.

### GET /v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages/types/{type}/versionNumbers/{versionNumber}

Retrieves a firmware image by type and version number.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages/types/{type}/versionNumbers/{versionNumber}` <br>
**HTTP METHOD:** `GET`                                                        
**RESPONSE CODE:** `200 (OK)`                                                   
**REQUEST PARAMETERS:**                                                                                                         
&nbsp; - `{partnerID}` - The partner ID.                                                                               
&nbsp; - `{deviceTypeID}` - The device type ID.                                          
&nbsp; - `{type}` - The firmware type.                                           
&nbsp; - `{versionNumber}` - The version number of the firmware image.                    
&nbsp; - `page` - Zero-based index of page to retrieve.                        
&nbsp; - `size` - The number of elements per page.                             
&nbsp; - `sort` - The field and sort direction; e.g., `updatedTimestamp`, `description` 

**REQUEST HEADERS:**
```                                                                                                            
{ 
  "Accept": "application/json", 
  "Authorization": "Bearer <access_token>"
}
```                                                               
 
**RESPONSE BODY MODEL SCHEMA:**
```                                                                                                 
{ 
  "id": "<string_integer>", 
  "name": "<string>", 
  "description": "<string, optional>", 
  "type": <integer>, 
  "versionNumber": "<string_integer>", 
  "version": "<string, optional>", 
  "url": "<string>", 
  "tags": ["<string, optional>"], 
  "createdTimestamp": <integer>, 
  "updatedTimestamp": <integer>, 
  "partnerId": "<string>", 
  "deviceTypeId": "<string>"
}
```                                                               

**Notes**

- This endpoint retrieves one associated firmware image records for a specific device type and firmware type and version number.

### PUT /v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages/{firmwareImageId}/push

Pushes a firmware image to a device.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages/{firmwareImageId}/push`<br>
**HTTP METHOD:** `PUT`                                                        
**RESPONSE CODE:** `202 (Accepted)`                                             
**REQUEST PARAMETERS:**                                                                                                         
&nbsp; - `{partnerID}` - The partner ID.                                                                               
&nbsp; - `{deviceTypeID}` - The device type ID.                                          
&nbsp; - `{firmwareImageId}` - The ID of the firmware image to push to a device.            

**REQUEST HEADERS:**
```                                                                                                            
{ 
  "Content-Type": "application/json", 
  "Authorization": "Bearer <access_token>"
}
```                                                               

**REQUEST PAYLOAD MODEL SCHEMA:**                                                                                              
```
{
   "value": "<string>"
}
```                                                                                                   

**Notes**

- The payload contains the device ID of the target device.

### DELETE /v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages/types/{type}/versionNumbers/{versionNumber}

Pushes a firmware image to a device.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/deviceTypes/{deviceTypeId}/firmwareImages/types/{type}/versionNumbers/{versionNumber}`<br> 
**HTTP METHOD:**`PUT`                                                        
**RESPONSE CODE:** `204 (No Content)`                                           
**REQUEST PARAMETERS:**                                                                                          
&nbsp; - `{partnerID}` - The partner ID.                                              
&nbsp; - `{deviceTypeID}` - The device type ID.                                          
&nbsp; - `{type}` - The firmware type.                                           
&nbsp; - `{versionNumber}` - The version number of the firmware image.                    

**REQUEST HEADERS:**
```                                                                                             
{
  "Authorization": "Bearer <access_token>"
}
```                                                               

**Notes**

- Dissociates a firmware image from a device type.


## Firmware Tag Endpoints

### GET /v1/ota/partners/{partnerId}/tags

Retrieves all firmware tags.

**RESOURCE URL:** `/v1/ota/partners/{partnerId}/tags`<br>
**HTTP METHOD:** `GET`                               
**RESPONSE CODE:** `200 (OK)`                          
**REQUEST PARAMETERS:**                                                                                
&nbsp; - `{partnerID}` - The partner ID.                                                      

**REQUEST HEADERS:**
```                                                                                   
{
  "Accept": "application/json", 
  "Authorization": "Bearer <access_token>"
}
```  
                                    
**RESPONSE BODY MODEL SCHEMA:**                                                                        
```
["<string>", "..."]
```                                                                             