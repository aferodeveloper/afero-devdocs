# afLib Result Codes

The following result codes are defined in `af_lib.h`:

| RESULT                     | INTERPRETATION                                               |
| :------------------------- | :----------------------------------------------------------- |
| AF_SUCCESS                 | The operation completed successfully.                        |
| AF_ERROR_UNKNOWN           | An unknown error occurred.                                   |
| AF_ERROR_BUSY              | ASR is busy; you *could* try your request again in a few moments. |
| AF_ERROR_INVALID_COMMAND   | The command could not be parsed.                             |
| AF_ERROR_QUEUE_OVERFLOW    | The queue is full.                                           |
| AF_ERROR_QUEUE_UNDERFLOW   | The queue is empty.                                          |
| AF_ERROR_INVALID_PARAM     | You used an invalid input parameter.                         |
| AF_ERROR_NOT_SUPPORTED     | What you tried to do is not supported.                       |
| AF_ERROR_NOT_CREATED       | The ASR library has not been created yet.                    |
| AF_ERROR_TIMEOUT           | What you tried to do took too long and timed out.            |
| AF_ERROR_NO_SUCH_ATTRIBUTE | The request made was for an unknown attribute ID.            |
| AF_ERROR_INVALID_DATA      | The data for the given attribute was incorrect.              |
| AF_ERROR_FORBIDDEN         | What you tried to do is not permitted.                       |
| AF_ERROR_ASR_REBOOTING     | We can’t do what you requested at this moment because ASR is rebooting. All GET/SET calls will return this error until the AF_SYSTEM_ASR_STATE_ATTR_ID attribute update is received from ASR. |

 **&#8674;** *Next:*&nbsp;&nbsp;[Attribute Update Reasons and States](../PeripheralUpdates)