# User API Endpoint

The RESTful API exposes the functionality related to Users described in this section.

<mark>**&check; Note:**Before you begin using the API below, be sure you have read the information in [Afero Cloud APIs](../CloudAPIs).</mark>

## Get Information About a User

The `users/me` endpoint provides a variety of user-specific information useful for personalization and other tasks.

### Request

| RESOURCE URL | HTTP METHOD | REQUEST HEADERS                                              | REQUEST PAYLOAD |
| :----------- | :---------- | :----------------------------------------------------------- | :-------------- |
| v1/users/me  | GET         | `{  "Content-Type": "application/json",  "Accept": "application/json",  "Authorization": "Bearer <*oauth access_token*>" }` | Not applicable. |

### Example Response

```
{
  "accountAccess": [
    {
      "account": {
        "accountId": "acdf8add-h15f-495e-8156-b4cfad3fd402",
        "createdTimestamp": 1449162900727,
        "description": "Primary Account",
        "type": "CUSTOMER"
      },
      "privileges": {
        "canWrite": true,
        "owner": true
      }
    }
  ],
  "credential": {
    "credentialId": "mike@sample.com",
    "failedAttempts": 0,
    "lastUsedTimestamp": 1449166732313,
    "type": "EMAIL",
    "verified": true
  },
  "firstName": "Mike",
  "lastName": "Smith",
  "partnerAccess": [
    {
      "partner": {
        "clientId": "asdf13228-234e-427d-add6-5asdf2jfacdf72",
        "clientSecret": "asdf23ewf14wf34rgdcdf34ewdage",
        "createdTimestamp": 1443458477948,
        "name": "Mike's Devices",
        "partnerId": "acdf13228-234e-427d-add6-5abdf2jfacdf72"
      },
      "privileges": {
        "inviteUsers": true,
        "manageDeviceProfiles": true,
        "owner": true,
        "viewDeviceInfo": true
      }
    }
  ],
  "tos": [
    {
      "currentVersion": 1,
      "needsAcceptance": false,
      "tosType": "user",
      "url": "https://cdn.afero.io/tos/user/v1/user.html",
      "userVersion": 1
    },
    {
      "currentVersion": 1,
      "needsAcceptance": true,
      "tosType": "developer",
      "url": "https://cdn.afero.io/tos/developer/v1/developer.html",
      "userVersion": 1
    },
    {
      "currentVersion": 1,
      "needsAcceptance": true,
      "tosType": "general",
      "url": "https://cdn.afero.io/tos/general/v1/general.html",
      "userVersion": 1
    }
  ],
  "userId": "123acdfa-asd2-4b26-2cd2-68cfe2acdd8b8"
}
```

 *Next:*&nbsp;&nbsp;[**&#8674;** Device API Endpoints](../API-DeviceEndpoints)