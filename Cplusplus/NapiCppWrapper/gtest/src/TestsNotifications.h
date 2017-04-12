//
//  TestsNotifications.hpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-08-12.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#ifndef TestsNotifications_hpp
#define TestsNotifications_hpp

class NymiApi;

struct resultPack2{
    
    bool napiResponse = false;
    std::condition_variable cv;
    std::mutex mtx;
};

bool notiOnFoundChange(NymiApi * napi);
bool notiOnPresenceChange(NymiApi * napi);
bool notificationGet(NymiApi * napi);

#endif /* TestsNotifications_hpp */
