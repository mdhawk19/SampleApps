//
//  NymiApiEnums.cpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-08-17.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#include "NymiApiEnums.h"

std::ostream& operator<<(std::ostream& out, const FoundStatus val){
    
    return out << foundStatusToString(val);
}

std::ostream& operator<<(std::ostream& out, const PresenceStatus val){
    
    return out << presenceStatusToString(val);
}
