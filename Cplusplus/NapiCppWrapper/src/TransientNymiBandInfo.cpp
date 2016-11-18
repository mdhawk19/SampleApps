//
//  NymiDeviceInfo.cpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-04-27.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#include "TransientNymiBandInfo.h"
#include "JsonUtilityFunctions.h"

TransientNymiBandInfo::TransientNymiBandInfo(nljson &jobj):deviceInfo(jobj){}

bool TransientNymiBandInfo::getRssiLast(int &val){
    
    nljson::iterator jit;
    if (hasKey(deviceInfo,{"RSSI_last"},jit)){
        val = jit.value();
        return true;
    }
    return false;
}

bool TransientNymiBandInfo::getRssiSmoothed(int &val){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"RSSI_smoothed"},jit)){
        val = jit.value();
        return true;
    }
    return false;
}

bool TransientNymiBandInfo::getCommandsQueued(std::vector<std::string> &commands){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"commandQueue"},jit)){
        for (auto cmd : jit.value()){
            commands.push_back(cmd);
        }
        return true;
    }
    return false;
}

bool TransientNymiBandInfo::getFirmwareVersion(std::string &val){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"firmwareVersion"},jit)){
        val = jit.value();
        return true;
    }
    val = "";
    return false;
}

bool TransientNymiBandInfo::getFoundState(FoundStatus &val){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"found"},jit)){
        std::string found = jit.value();
        if (foundEnum.find(found) != foundEnum.end()){
            val = foundEnum.find(found)->second;
            return true;
        }
    }
    val = FoundStatus::ERROR;
    return false;
}

bool TransientNymiBandInfo::getPresenceState(PresenceStatus &val){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"present"},jit)){
        std::string present = jit.value();
        if (presentEnum.find(present) != presentEnum.end()){
            val = presentEnum.find(present)->second;
            return true;
        }
    }
    val = PresenceStatus::ERROR;
    return false;
}

bool TransientNymiBandInfo::isProvisioned(bool &val){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"isProvisioned"},jit)){
        val = jit.value();
        return true;
    }
    return false;
}

bool TransientNymiBandInfo::getSinceLastContact(double &val){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"sinceLastContact"},jit)){
        val = jit.value();
        return true;
    }
    return false;
}

bool TransientNymiBandInfo::getAuthenticationWindowRemaining(double &val){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"provisioned","authenticationWindowRemaining"},jit)){
        val = jit.value();
        return true;
    }
    return false;
}

bool TransientNymiBandInfo::getNumCommandsQueued(int &val){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"provisioned","commandsQueued"},jit)){
        val = jit.value();
        return true;
    }
    return false;
}

bool TransientNymiBandInfo::enabledRoamingAuthentication(bool &val){
    
    nljson::iterator jit;
    if (hasKey(deviceInfo,{"provisioned","enabledRoamingAuthSetup"},jit)){
        val =  jit.value();
        return true;
    }
    return false;
}

bool TransientNymiBandInfo::enabledSigning(bool &val){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"provisioned","enabledSigning"},jit)){
        val = jit.value();
        return true;
    }
    return false;
}

bool TransientNymiBandInfo::enabledSymmetricKeys(bool &val){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"provisioned","enabledSymmetricKeys"},jit)){
        val = jit.value();
        return true;
    }
    return false;
}

bool TransientNymiBandInfo::enabledTOTP(bool &val){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"provisioned","enabledTOTP"},jit)){
        val = jit.value();
        return true;
    }
    return false;
}

bool TransientNymiBandInfo::getPid(std::string &val){

    nljson::iterator jit;
    if (hasKey(deviceInfo,{"provisioned","pid"},jit)){
        val = jit.value();
        return true;
    }
    val = "";
    return false;
}