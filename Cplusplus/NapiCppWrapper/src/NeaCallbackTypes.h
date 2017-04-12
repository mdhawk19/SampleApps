//
//  NeaCallbacks.h
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-05-04.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#ifndef NeaCallbacks_h
#define NeaCallbacks_h

#include <vector>
#include <string>
#include <map>
#include <utility>
#include <functional>
#include "NymiApiEnums.h"

class NymiProvision;
class TransientNymiBandInfo;

struct napiError {
    
    std::string errorString;
    std::vector<std::pair<std::string,std::string> > errorList;
};

//init and error
using errorCallback = std::function<void(napiError nErr)>;

//callbacks for provisioning a new band
using agreementCallback = std::function<void(std::vector<std::string> patterns)>;
using newProvisionCallback = std::function<void(NymiProvision newprov)>;
using provisionChangeCallback = std::function<void(std::vector<NymiProvision> curBands)>;

//callbacks for getting api state
using getProvisionsCallback = std::function<void(std::vector<NymiProvision> provisions)>;
using onNotificationsGetState = std::function<void(std::map<std::string,bool> notificationsState)>;
using onStartStopProvisioning = std::function<void(std::string newState)>;

//callbacks for user-initiated operations on a provisioned Nymi Band
using randomCallback =              std::function<void(bool opResult, std::string pid, std::string rand,                napiError)>;
using symmetricKeyCallback =        std::function<void(bool opResult, std::string pid, std::string sk,                  napiError)>;
using ecdsaSignCallback =           std::function<void(bool opResult, std::string pid, std::string sig, std::string vk, napiError)>;
using ecdsaSignSetupCallback =		std::function<void(bool opResult, std::string pid,									napiError)>;
using totpGetCallback =             std::function<void(bool opResult, std::string pid, std::string totp,                napiError)>;
using onNotificationCallback =      std::function<void(bool opResult, std::string pid, HapticNotification,              napiError)>;
using deviceInfoCallback =          std::function<void(bool opResult, std::string pid, TransientNymiBandInfo&,          napiError)>;
using createdKeyCallback =          std::function<void(bool opResult, std::string pid, KeyType,                         napiError)>;
using revokedKeyCallback =          std::function<void(bool opResult, std::string pid, KeyType,                         napiError)>;
using onProvisionRevokedCallback =  std::function<void(bool opResult, std::string pid,                                  napiError)>;

//api notification - one way flow of information from api to nea.
//notifications reported through these callbacks are not initiated by a specific user action or request, other than enabling the notification itself.
using onNymiBandFoundStatusChange = std::function<void(std::string pid, FoundStatus before, FoundStatus after)>;
using onNymiBandPresenceChange = std::function<void(std::string pid, PresenceStatus before, PresenceStatus after, bool authenticated)>;

#endif /* NeaCallbacks_h */
