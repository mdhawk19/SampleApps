//
//  NymiApiTypes.h
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-05-01.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#ifndef NymiApiTypes_h
#define NymiApiTypes_h

#include <map>
#include <string>
#include <ostream>

enum class HapticNotification { ERROR = -1, NOTIFY_NEGATIVE = 0, NOTIFY_POSITIVE = 1 };

enum class FoundStatus { ERROR, ANONYMOUS, AUTHENTICATED, IDENTIFIED, PROVISIONING, UNCLASPED, UNDETECTED, UNPROVISIONABLE, DISCOVERED };
enum class PresenceStatus { ERROR, DEVICE_PRESENCE_NO, DEVICE_PRESENCE_UNLIKELY, DEVICE_PRESENCE_LIKELY, DEVICE_PRESENCE_YES };
enum class ProximityState { ERROR, PROXIMITY_STATE_NOT_READY, PROXIMITY_STATE_UNDETECTABLE, PROXIMITY_STATE_DETECTABLE,
    PROXIMITY_STATE_SPHERE1, PROXIMITY_STATE_SPHERE2, PROXIMITY_STATE_SPHERE3, PROXIMITY_STATE_SPHERE4};
enum class KeyType { ERROR, SYMMETRIC, TOTP };

//string to enum mapping
const std::map<std::string,FoundStatus> foundEnum = {
    {"undetected",FoundStatus::UNDETECTED},
    {"unclasped",FoundStatus::UNCLASPED},
    {"unprovisionable",FoundStatus::UNPROVISIONABLE},
    {"anonymous",FoundStatus::ANONYMOUS},
    {"discovered",FoundStatus::DISCOVERED},
    {"provisioning",FoundStatus::PROVISIONING},
    {"identified",FoundStatus::IDENTIFIED},
    {"authenticated",FoundStatus::AUTHENTICATED}
};

const std::map<std::string,PresenceStatus> presentEnum = {
    {"yes",PresenceStatus::DEVICE_PRESENCE_YES},
    {"likely",PresenceStatus::DEVICE_PRESENCE_LIKELY},
    {"unlikely",PresenceStatus::DEVICE_PRESENCE_UNLIKELY},
    {"no",PresenceStatus::DEVICE_PRESENCE_NO}
};

//utility functions
inline FoundStatus stringToFoundStatus(std::string foundStr){
    auto it = foundEnum.find(foundStr);
    if (it != foundEnum.end()){
        return it->second;
    }
    return FoundStatus::ERROR;
}

inline PresenceStatus stringToPresenceStatus(std::string presStr){
    auto it = presentEnum.find(presStr);
    if (it != presentEnum.end()){
        return it->second;
    }
    return PresenceStatus::ERROR;
}

inline std::string foundStatusToString(FoundStatus foundStatus){
    
    switch (foundStatus) {
            
        case FoundStatus::ERROR: return "FoundStatus::Error";
        case FoundStatus::ANONYMOUS: return "FoundStatus::Anonymous";
        case FoundStatus::AUTHENTICATED: return "FoundStatus::Authenticated";
        case FoundStatus::IDENTIFIED: return "FoundStatus::Identified";
        case FoundStatus::PROVISIONING: return "FoundStatus::Provisioning";
        case FoundStatus::UNCLASPED: return "FoundStatus::Unclasped";
        case FoundStatus::UNDETECTED: return "FoundStatus::Undetected";
        case FoundStatus::UNPROVISIONABLE: return "FoundStatus::Unprovisionable";
        case FoundStatus::DISCOVERED: return "FoundStatus::Discovered";
        default: return "FoundStatus::Error";
    }
}

inline std::string presenceStatusToString(PresenceStatus presenceStatus){
    
    switch (presenceStatus) {
            
        case PresenceStatus::ERROR: return "Error";
        case PresenceStatus::DEVICE_PRESENCE_NO: return "Present::No";
        case PresenceStatus::DEVICE_PRESENCE_UNLIKELY: return "Present::Unlikely";
        case PresenceStatus::DEVICE_PRESENCE_LIKELY: return "Present::Likely";
        case PresenceStatus::DEVICE_PRESENCE_YES: return "Present::Yes";
        default: return "Present::Error";
    }
}

inline std::string keyTypeToString(KeyType keyType){

    switch(keyType) {

        case KeyType::SYMMETRIC: return "KeyType::Symmetric";
        case KeyType::TOTP: return "KeyType::Totp";
        default: return "KeyType::Error";
    }
}

// print enums
std::ostream& operator<<( std::ostream& out, const FoundStatus v );
std::ostream& operator<<( std::ostream& out, const PresenceStatus v );

#endif /* NymiApiTypes_h */
