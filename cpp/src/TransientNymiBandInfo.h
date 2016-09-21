//
//  NymiDeviceInfo.hpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-04-27.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#ifndef NymiDeviceInfo_hpp
#define NymiDeviceInfo_hpp

#include "json/src/json.hpp"
#include "NymiApiEnums.h"

using nljson = nlohmann::json;

class TransientNymiBandInfo {
    
public:
    
    TransientNymiBandInfo(){ /*intentionally empty*/ }
    TransientNymiBandInfo(nljson &jobj);
    
    bool getRssiLast(int &val);
    bool getRssiSmoothed(int &val);
    bool getFirmwareVersion(std::string &val);
    bool getFoundState(FoundStatus &val);
    bool getPresenceState(PresenceStatus &val);
    bool isProvisioned(bool &val);
    bool getSinceLastContact(double &val);
    bool getAuthenticationWindowRemaining(double &val);
    bool getNumCommandsQueued(int &val);
    bool getCommandsQueued(std::vector<std::string> &commands);
    bool enabledRoamingAuthentication(bool &val);
    bool enabledSigning(bool &val);
    bool enabledSymmetricKeys(bool &val);
    bool enabledTOTP(bool &val);
    bool getPid(std::string &val);
  
private:
    nljson deviceInfo;
};


#endif /* NymiDeviceInfo_hpp */
