# AFERO CLOUD APIS

The Afero Platform exposes a significant range of functionality via a RESTful API set, accessed via HTTP using TLS. Examples include: listing users and devices, obtaining real-time status information, and acting on devices.

## API Access

Once you have created your account and have signed in to the Afero Profile Editor, your authentication credentials will be available to you.

The base URL for the Afero API is **https://api.afero.io**.

## Security

Afero API is secure. All Afero API requests must be made over SSL, and must be authenticated using an OAuth 2.0 access key. (To read more, go to <a href="http://oauth.net/documentation/" target="_blank">OAuth.net documentation</a>). This means your data is safe, but also means you must perform a couple of security-related steps when you use the API, described in the sections below.

### One-Time Setup

Sign in to the Afero Profile Editor and open the <span class="UIText">VIEW > ACCOUNT INFO</span> window (example shown below). From this Account Information window, you can view your “OAuth Client ID” and “OAuth Client Secret”, which you will need to authenticate requests on behalf of the end-user. These credentials **should not** be shared with untrusted parties!

<img src="../img/AccountInfo.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

### For Each Communication Session

1. Initiate communication with the API server by requesting an OAuth 2.0 access key, providing your OAuth Client ID and OAuth Client Secret. This step is shown in the first API example in the next section.
2. Afero responds with an access key, which will expire after approximately four hours.
3. Include the access key in all HTTP requests to the API endpoints as an Authorization header, as shown in examples in the next section.
