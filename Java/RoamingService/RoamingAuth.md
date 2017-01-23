The following notes describe the band-facing (NAPI) and relying-party authentication service-facing commands and responses
All the the roaming service data is based on the Nymi roaming authentication sample app
For more information on the roaming authentication process, see the [documentation](http://downloads.nymi.com/sdkDoc/latest/background/#roaming-authenticator)
--------------------------------------------  
# NAPI Commands  
`
[Configure roaming auth on band]  
Sent:
```json
{"path":"roaming-auth-setup/run","operation":["roaming-auth-setup","run"],"request":{"pid":"965f7cf8f1aea84fe5160407f6242bfc","partnerPublicKey":"B6F1B78565E2B8A993370A307151BC7DA2310B307C2948AA5863CF804CD089F2902FB18EC64FF8086F524927FF213F0FB1B2D8948E38DD941DA70BC30EE2F86B"},"exchange":"1234roamingsetup4aedfef32780bc31c4e41b6f00260eeb"}
```
Response:
```json
{"completed":true,"exchange":"1234roamingsetup4aedfef32780bc31c4e41b6f00260eeb","operation":["roaming-auth-setup","run"],"path":"roaming-auth-setup/run","request":{"partnerPublicKey":"B6F1B78565E2B8A993370A307151BC7DA2310B307C2948AA5863CF804CD089F2902FB18EC64FF8086F524927FF213F0FB1B2D8948E38DD941DA70BC30EE2F86B","pid":"4aedfef32780bc31c4e41b6f00260eeb"},"response":{"RAKey":"e4ca03b19f3dee53452d499bc9c7c2546bd454780b4109f5e0c5701be45fcab5e5070be5a90cb0531bd9e5c905ca3ef6b562215b4221488ddf083dbaf889f2ab","RAKeyId":"4cb8aaa9773ccd704674d36ff61a38cb"},"successful":true}
```
[Start roaming auth]    
Sent:
```json
{"path":"roaming-auth/run","operation":["roaming-auth","run"],"request":{"tid":1},"exchange":"*roamingrun*"}
```
Response:
```json
{"completed":true,"exchange":"*roamingrun*","operation":["roaming-auth","run"],"path":"roaming-auth/run","request":{"tid":1},"response":{},"successful":true}
```
Response:
```json
{"completed":false,"event":{"kind":"nonce","nymibandNonce":"059bef35b5000c82928daaeec33c8b26"},"exchange":"*roamingrun*","operation":["roaming-auth","report","nonce"],"path":"roaming-auth/report/nonce","successful":true}
```
  
[Finish roaming auth]  
Sent:
```json
{"path":"roaming-auth-sig/run","operation":["roaming-auth-sig","run"],"request":{"partnerPublicKey":"B6F1B78565E2B8A993370A307151BC7DA2310B307C2948AA5863CF804CD089F2902FB18EC64FF8086F524927FF213F0FB1B2D8948E38DD941DA70BC30EE2F86B","serverNonce":"C32AF73BBEDCFB77DCC9E7407B95007E7A0CEF7C1FAAB2EB3B512354543D6746","serverSignature":"304602210093E4C67BB3047644A7896D69F0E0895971713DB8016990ACE2E23CEE5DB2B383022100EA84B6B848A533EA554A24D625C5FEF136D32207A8C4EC5B129B712D419CFA5E"},"exchange":"*roamingrun*"}
```
Response:
```json
{"path":"roaming-auth-sig/run","request":{"partnerPublicKey":"B6F1B78565E2B8A993370A307151BC7DA2310B307C2948AA5863CF804CD089F2902FB18EC64FF8086F524927FF213F0FB1B2D8948E38DD941DA70BC30EE2F86B","serverNonce":"2BC9F71E10ADBC3564FB0A207DDA115727C040BABA7D12EB52C0E0C92AFF0575","serverSignature":"612336B951DBD977B358672C4E64E17E711655934834CC02BEB445EAC8713D4A3292EBACFE116D50ABA94A504EA7FDDCEB6471D71AF8034E4F6BCAF4F4DC57B8"},"response":{"raKeyId":"514fb47e4dead9dc50a0916053b2cc28","nymibandSig":"ee8802aff5f7361efb2ba45fe061476cd1cc6b7cdb3fb89301790c73412e8828d4eb6c1e632733881ac76728a99e893d3bb47feb67c0e92a61b62ad36c9802ac"},"exchange":null,"completed":true,"operation":["roaming-auth-sig","run"],"successful":true}
```
--------------------------------------------  
# EC Key creation commands
`openssl ecparam -name prime256v1 -genkey -param_enc explicit -out roaming-key.pem`
`openssl req -new -x509 -key private-key.pem -out roaming-cert.pem -days 999`
`cat roaming-key.pem roaming-cert.pem > roaming.pem`
--------------------------------------------  
# Roaming Service Commands
[Get public key of roaming auth server to use in Nymi Band setup]
Curl Command: `curl --silent http://127.0.0.1:9090/roamingservice/provision/getpubkey`  
Response:
```json
{"response":{"partnerPublicKey":"B6F1B78565E2B8A993370A307151BC7DA2310B307C2948AA5863CF804CD089F2902FB18EC64FF8086F524927FF213F0FB1B2D8948E38DD941DA70BC30EE2F86B"},"successful":true}
```

[Provision user on roaming auth server]
Curl Command: `curl --silent --data '{"request":{"userID":"testband2","nbPublicKey":"4a46e4ecb63b8efca07589c20baaf5377a89a2735ee792954217947f8052ee1c28330931ffb7a15c004b820d486c592c86c324c753de943be8a05a5b82b59e5a","nbPublicKeyID":"8782169cd70ab15e4436c3245c470ea7"}}' http://127.0.0.1:9090/roamingservice/provision/newuser`
Response:
```json
{"request":{"nbPublicKey":"4a46e4ecb63b8efca07589c20baaf5377a89a2735ee792954217947f8052ee1c28330931ffb7a15c004b820d486c592c86c324c753de943be8a05a5b82b59e5a","nbPublicKeyID":"8782169cd70ab15e4436c3215c470ea7","userID":"testband2"},"response":"User testband2 successfully added","successful":true}
```
[De-provision (delete) user on roaming auth server]
Curl Command: `curl --silent --data '{"request":{"userID":"testband2"}}' http://127.0.0.1:9090/roamingservice/provision/deleteuser`
Response:
```json
{"request":{"userID":"testband2"},"response":"User testband2 successfully deleted","successful":true}
```
[Start roaming auth session with Nymi Band nonce]
Curl Command: `curl --silent --data '{"request":{"nymibandNonce":"87be576f9f1533bf1d712400e1a45cad","exchange":"A390EFDC53363A06D7B2AC9718A5B37B"}}' http://127.0.0.1:9090/roamingservice/sign`
Response:
```json
{"request":{"nymibandNonce":"87be576f9f1533bf1d712400e1a45cad","exchange":"A390EFDC53363A06D7B2AC9718A5B37B"},"response":{"partnerPublicKey":"B6F1B78565E2B8A993370A307151BC7DA2310B307C2948AA5863CF804CD089F2902FB18EC64FF8086F524927FF213F0FB1B2D8948E38DD941DA70BC30EE2F86B","serverNonce":"EADD5859F80B9B8A7B435757D2E481871D0BB424402B6F664A115105FA0DA992","serverSignature":"304402203572C1B61EBD6371355FF7B9FE87F56C27D9391CF40F11F8A86AC951074E4C7A02200F3DDE5C74140C3D779D6C98FF5E4A2FEA4FEBF8984BA610765B1E87165B4D42"}}
```
[Complete roaming auth session with signed server nonce]
Curl Command: `curl --silent --data '{"request":{"exchange":"A390EFDC53363A06D7B2AC9718A5B37B","signedNonces":"","nbPublicKeyID":""}}' http://127.0.0.1:9090/roamingservice/auth`
Response:
```json
{"request":{"exchange":"F3BB060642BB9FFC8DE0B72B04B2E9CAroamingauth","nbPublicKeyID":"8e51361e7e505a3e4b458b405746c9f0","signedNonces":"9f69746d7373777e5d63e7f59d676537d38c93c6bc9305c8f826fef98cf37c3c695df174bfe577a8c96dd36409eb7dd4394bc8e1616ca358f052619886a06ac8"},"response":"User testuser successfully authenticated","userid":"testuser","successful":true}
```
--------------------------------------------
# Expanded JSON Commands
--------------------------------------------
## Requests
--------------------------------------------
```json
{  
    "path":"roaming-auth-setup/run",
    "operation":[  
        "roaming-auth-setup",
        "run"
    ],
    "request":{  
        "pid":"4aedfef32780bc31c4e41b6f00260eeb",
        "partnerPublicKey":"B6F1B78565E2B8A993370A307151BC7DA2310B307C2948AA5863CF804CD089F2902FB18EC64FF8086F524927FF213F0FB1B2D8948E38DD941DA70BC30EE2F86B"
    },
    "exchange":"1234roamingsetup4aedfef32780bc31c4e41b6f00260eeb"
}
```
```json
{  
    "path":"roaming-auth/run",
    "operation":[  
        "roaming-auth",
        "run"
    ],
    "request":{  
        "tid":1
    },
    "exchange":"*roamingrun*"
}
```
```json
{  
    "path":"roaming-auth-sig/run",
    "operation":[  
        "roaming-auth-sig",
        "run"
    ],
    "request":{  
        "partnerPublicKey":"B6F1B78565E2B8A993370A307151BC7DA2310B307C2948AA5863CF804CD089F2902FB18EC64FF8086F524927FF213F0FB1B2D8948E38DD941DA70BC30EE2F86B",
        "serverNonce":"C32AF73BBEDCFB77DCC9E7407B95007E7A0CEF7C1FAAB2EB3B512354543D6746",
        "serverSignature":"304602210093E4C67BB3047644A7896D69F0E0895971713DB8016990ACE2E23CEE5DB2B383022100EA84B6B848A533EA554A24D625C5FEF136D32207A8C4EC5B129B712D419CFA5E"
    },
    "exchange":"*roamingrun*"
}
```
## Responses
--------------------------------------------
```json
{  
    "completed":true,
    "errors":[  
        [  
            "No authenticated Nymi Band for this provision is present",
            "error"
        ],
        [  
            "refused usually because the tid doesn't exist, the Nymi Band is unclasped, or the work isn't required",
            "error"
        ],
        [  
            "Failed to execute request",
            "error"
        ]
    ],
    "exchange":"1234roamingsetup4aedfef32780bc31c4e41b6f00260eeb",
    "operation":[  
        "roaming-auth-setup",
        "run"
    ],
    "outcome":"refused",
    "path":"roaming-auth-setup/run",
    "request":{  
        "partnerPublicKey":"897B06E658D99A2AB2D722FB91664C12D4AEB4DDEF82043B9B390914AB508785",
        "pid":"4aedfef32780bc31c4e41b6f00260eeb"
    },
    "successful":false
}
```
```json
{  
    "completed":true,
    "exchange":"1234roamingsetup4aedfef32780bc31c4e41b6f00260eeb",
    "operation":[  
        "roaming-auth-setup",
        "run"
    ],
    "path":"roaming-auth-setup/run",
    "request":{  
        "partnerPublicKey":"897B06E658D99A2AB2D722FB91664C12D4AEB4DDEF82043B9B390914AB508785",
        "pid":"4aedfef32780bc31c4e41b6f00260eeb"
    },
    "response":{  
        "RAKey":"e4ca03b19f3dee53452d499bc9c7c2546bd454780b4109f5e0c5701be45fcab5e5070be5a90cb0531bd9e5c905ca3ef6b562215b4221488ddf083dbaf889f2ab",
        "RAKeyId":"4cb8aaa9773ccd704674d36ff61a38cb"
    },
    "successful":true
}
```
```json
{  
    "completed":true,
    "exchange":"*roamingrun*",
    "operation":[  
        "roaming-auth",
        "run"
    ],
    "path":"roaming-auth/run",
    "request":{  
        "tid":1
    },
    "response":{  

    },
    "successful":true
}
```
```json
{  
    "completed":false,
    "event":{  
        "kind":"nonce",
        "nymibandNonce":"059bef35b5000c82928daaeec33c8b26"
    },
    "exchange":"*roamingrun*",
    "operation":[  
        "roaming-auth",
        "report",
        "nonce"
    ],
    "path":"roaming-auth/report/nonce",
    "successful":true
}
```
```json
{  
    "path":"roaming-auth-sig/run",
    "request":{  
        "partnerPublicKey":"B6F1B78565E2B8A993370A307151BC7DA2310B307C2948AA5863CF804CD089F2902FB18EC64FF8086F524927FF213F0FB1B2D8948E38DD941DA70BC30EE2F86B",
        "serverNonce":"2BC9F71E10ADBC3564FB0A207DDA115727C040BABA7D12EB52C0E0C92AFF0575",
        "serverSignature":"612336B951DBD977B358672C4E64E17E711655934834CC02BEB445EAC8713D4A3292EBACFE116D50ABA94A504EA7FDDCEB6471D71AF8034E4F6BCAF4F4DC57B8"
    },
    "response":{  
        "raKeyId":"514fb47e4dead9dc50a0916053b2cc28",
        "nymibandSig":"ee8802aff5f7361efb2ba45fe061476cd1cc6b7cdb3fb89301790c73412e8828d4eb6c1e632733881ac76728a99e893d3bb47feb67c0e92a61b62ad36c9802ac"
    },
    "exchange":null,
    "completed":true,
    "operation":[  
        "roaming-auth-sig",
        "run"
    ],
    "successful":true
}
```
```json
{  
    "request":{  
        "exchange":"F3BB060642BB9FFC8DE0B72B04B2E9CAroamingauth",
        "nbPublicKeyID":"8e51361e7e505a3e4b458b405746c9f0",
        "signedNonces":"9f69746d7373777e5d63e7f59d676537d38c93c6bc9305c8f826fef98cf37c3c695df174bfe577a8c96dd36409eb7dd4394bc8e1616ca358f052619886a06ac8"
    },
    "response":"User testuser successfully authenticated",
    "userid":"testuser",
    "successful":true
}
```
--------------------------------------------
