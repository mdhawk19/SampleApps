//
//  JsonUtilityFunctions.h
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-08-17.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#ifndef JsonUtilityFunctions_h
#define JsonUtilityFunctions_h

#include <vector>
#include "json.hpp"

using nljson = nlohmann::json;

inline bool hasKey(nljson &jobj, std::vector<std::string> keyPath, nljson::iterator &jit) {

    auto keyIt = keyPath.begin();
    jit = jobj.find(*keyIt++);
    if (jit == jobj.end()) return false;

    if (keyIt != keyPath.end()){

        std::vector<std::string> subPath(keyIt++,keyPath.end());
        return hasKey(jit.value(),subPath,jit);
    }
    else return true;
}

inline bool isKeyValue(nljson &jobj, std::vector<std::string> keyPath, nljson::iterator &jit, bool val) {

    if (hasKey(jobj,keyPath,jit)){
        bool valFromJson = jit.value();
        if (valFromJson == val){
            return true;
        }
    }
    return false;
}

#endif /* JsonUtilityFunctions_h */
