# OAuth API Endpoint

The RESTful API exposes the OAuth function described in this section.

<mark>**&check; Note:**   Before you begin using the API below, be sure you have read the information in [Afero Cloud APIs](../CloudAPIs).</mark>

## OAuth Authentication

The OAuth Authentication endpoint is used to authenticate end-users and generate an access token, which will be used to authenticate requests on behalf of the end-user. For this endpoint, you will need your OAuth Client ID and OAuth Client Secret, which you can view by signing in to the Afero Profile Editor, then going to the operating system menu bar and opening the VIEW > ACCOUNT INFO screen.

### Request

| RESOURCE URL | HTTP METHOD | REQUEST HEADERS                                              | REQUEST PAYLOAD                                              |
| :----------- | :---------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| oauth/token  | POST        | `{  "Content-Type": "application/x-www-form-urlencoded",  "Accept": "application/json",  "Authorization": "Basic ABC9g1Zso4P6wKLebTFcNb==" }` | `{  "username": "username@example.io",  "password": "user_password",  "grant_type": "password" }` |

### Example Response

```
{
  "access_token": "12345678-90AB-CDEF-0123-FED789CBA432",
  "token_type": "bearer",
  "expires_in": 16086,
  "scope": "partner account"
}
```

### Notes

- The Authorization header value should be the string `Basic`, followed by a space, followed by the Base64-encoded representation of your OAuth Client ID, a colon, and then your OAuth Client Secret.

    For example, if your ClientID were “Mork” and your Client Secret were “Mindy”, you would perform a Base64-encoding of `Mork:Mindy` to create the string `TW9yazpNaW5keQ==`.

    The entire Authorization header value would be `Basic TW9yazpNaW5keQ==`.

- In the Request Payload, the `username` and `password` are the username (email address) and password you chose when you activated your Afero account.

- The access token returned in the response must be provided in subsequent API requests (see below), and this value expires.

- The expiration parameter in the response, `expires_in`, is measured in seconds.

 **&#8674;** *Next:* [User API Endpoint](../API-UserEndpoints)