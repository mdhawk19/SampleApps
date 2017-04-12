//
//  TestsNotifications.cpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-08-12.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#include <iostream>
#include <mutex>
#include <condition_variable>
#include "TestsNotifications.h"
#include "NymulatorHooks.h"
#include "NymiApi.h"

/*------------------
 test notifications
 -------------------*/

const int napiTimeout = 3000;    //timeout, in miliseconds, we wait for a response from Napi

bool notiOnFoundChange(NymiApi * napi){
    
    resultPack2 res;
    FoundStatus beforeStatus, afterStatus;
    
    onNymiBandFoundStatusChange onFoundChange = [&res,&beforeStatus,&afterStatus](std::string pid, FoundStatus before, FoundStatus after) {
        std::cout << "onFoundChange, pid: "<<pid<<", before: " << foundStatusToString(before) << ", after: " << foundStatusToString(after) << std::endl;
        
        beforeStatus = before;
        afterStatus = after;
        
        std::lock_guard<std::mutex> lock(res.mtx);
        res.napiResponse = true;
        res.cv.notify_one();
    };
    
    //enable onFoundChange notification
    napi->setOnFoundChange(onFoundChange);
    std::this_thread::sleep_for(std::chrono::milliseconds(napiTimeout));
    
    //test unclasp
    nymuUnclaspBand();
    
    {
        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
    }
    
    //return early if unclasp failed to generate notification event
    bool opSuccess = (beforeStatus == FoundStatus::AUTHENTICATED && afterStatus == FoundStatus::UNCLASPED);
    if (!opSuccess) {
        napi->disableOnFoundChange();
        std::this_thread::sleep_for(std::chrono::milliseconds(napiTimeout));
        return opSuccess;
    }
    
    //reset
    res.napiResponse = false;
    
    //test clasp
    nymuClaspBand();
    
    {
        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
    }
    
    napi->disableOnFoundChange();
    std::this_thread::sleep_for(std::chrono::milliseconds(napiTimeout));
    opSuccess = (beforeStatus == FoundStatus::UNCLASPED && afterStatus == FoundStatus::AUTHENTICATED);
    return opSuccess;
}

bool notiOnPresenceChange(NymiApi * napi){
    
    resultPack2 res;
    PresenceStatus beforeStatus, afterStatus;
    
    onNymiBandPresenceChange onPresenceChange = [&res,&beforeStatus,&afterStatus](std::string pid, PresenceStatus before, PresenceStatus after, bool authenticated) {
        std::cout <<std::boolalpha<< "onPresenceChange, pid: "<<pid<<", before: " << presenceStatusToString(before) << ", after: " << presenceStatusToString(after) <<", authenticated: "<<authenticated<< std::endl;
        
        beforeStatus = before;
        afterStatus = after;
        
        std::lock_guard<std::mutex> lock(res.mtx);
        res.napiResponse = true;
        res.cv.notify_one();
    };
    
    //enable onFoundChange notification
    napi->setOnPresenceChange(onPresenceChange);
    std::this_thread::sleep_for(std::chrono::milliseconds(napiTimeout));
    
    //test unclasp
    nymuUnclaspBand();
    
    {
        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
    }
    
    //return early if unclasp failed to generate notification event
    bool opSuccess = (beforeStatus == PresenceStatus::DEVICE_PRESENCE_YES && afterStatus == PresenceStatus::DEVICE_PRESENCE_NO);
    if (!opSuccess) {
        napi->disableOnPresenceChange();
        std::this_thread::sleep_for(std::chrono::milliseconds(napiTimeout));
        return opSuccess;
    }
    
    //reset
    res.napiResponse = false;
    
    //test clasp
    nymuClaspBand();
    
    {
        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
    }
    
    napi->disableOnPresenceChange();
    std::this_thread::sleep_for(std::chrono::milliseconds(napiTimeout));
    opSuccess = (beforeStatus == PresenceStatus::DEVICE_PRESENCE_NO && afterStatus == PresenceStatus::DEVICE_PRESENCE_YES);
    return opSuccess;
}

bool notificationGet(NymiApi * napi){
    
    //allow Napi to process enable/disable notification request, before getting the notification state.
    //This test *will* fail if delay=0;
    const int delay = 500;
    
    resultPack2 res;
    std::map<std::string,bool> notificationsMap;
    
    onNymiBandPresenceChange onPresenceChange = [](std::string pid, PresenceStatus before, PresenceStatus after, bool authenticated) {};
    onNymiBandFoundStatusChange onFoundChange = [](std::string pid, FoundStatus before, FoundStatus after) {};
    
    
    onNotificationsGetState onNotificationsGet = [&res,&notificationsMap](std::map<std::string,bool> notificationsState){
        
        std::cout<<"Notifications state: \n";
        for (auto entry : notificationsState){
            std::cout<<entry.first<<" : "<<entry.second<<std::endl;
        }
        
        notificationsMap = std::move(notificationsState);
        
        std::lock_guard<std::mutex> lock(res.mtx);
        res.napiResponse = true;
        res.cv.notify_one();
    };
    
    std::cout<<"Notification status Test 1: onFoundChange false, onPresenceChange false\n";
    napi->disableOnPresenceChange();
    napi->disableOnFoundChange();
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    napi->getApiNotificationState(onNotificationsGet);
    {
        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
        
        bool opSuccess = (!notificationsMap["onFoundChange"] && !notificationsMap["onPresenceChange"]);
        if (!opSuccess){
            std::cout<<"Notification status Test 1 failed\n";
            return opSuccess;
        }
        res.napiResponse = false;
    }
    
    std::cout<<"Notification status Test 2: onFoundChange false, onPresenceChange true\n";
    napi->setOnPresenceChange(onPresenceChange);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    napi->getApiNotificationState(onNotificationsGet);
    {
        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
        
        bool opSuccess = (!notificationsMap["onFoundChange"] && notificationsMap["onPresenceChange"]);
        if (!opSuccess){
            std::cout<<"Notification status Test 2 failed\n";
            return opSuccess;
        }
        res.napiResponse = false;
    }
    
    std::cout<<"Notification status Test 3: onFoundChange true, onPresenceChange true\n";
    napi->setOnFoundChange(onFoundChange);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    napi->getApiNotificationState(onNotificationsGet);
    {
        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
        
        bool opSuccess = (notificationsMap["onFoundChange"] && notificationsMap["onPresenceChange"]);
        if (!opSuccess){
            std::cout<<"Notification status Test 3 failed\n";
            return opSuccess;
        }
        res.napiResponse = false;
    }
    
    std::cout<<"Notification status Test 4: onFoundChange true, onPresenceChange false\n";
    napi->disableOnPresenceChange();
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    napi->getApiNotificationState(onNotificationsGet);
    {
        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
        
        bool opSuccess = (notificationsMap["onFoundChange"] && !notificationsMap["onPresenceChange"]);
        if (!opSuccess){
            std::cout<<"Notification status Test 4 failed\n";
            return opSuccess;
        }
        res.napiResponse = false;
    }
    
    std::cout<<"Notification status Test 5: onFoundChange false, onPresenceChange false\n";
    napi->disableOnPresenceChange();
    napi->disableOnFoundChange();
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    napi->getApiNotificationState(onNotificationsGet);
    {
        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
        
        bool opSuccess = (!notificationsMap["onFoundChange"] && !notificationsMap["onPresenceChange"]);
        if (!opSuccess){
            std::cout<<"Notification status Test 5 failed\n";
            return opSuccess;
        }
        res.napiResponse = false;
    }
    
    return true;
}
