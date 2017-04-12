//
//  TestFunctions.cpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-08-11.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#include <condition_variable>
#include <mutex>
#include <iostream>
#include "TestsProvision.h"
#include "NymulatorHooks.h"
#include "NymiApi.h"
#include "NymiProvision.h"

/* -----------------------------------------------------------------
 functions that test functionality that requires a provisioned band
 -------------------------------------------------------------------*/

static NymiProvision nBand;
const int napiTimeout = 3000;    //timeout, in miliseconds, we wait for a response from Napi

void resetResultPack(resultPack &res){

    res.opSuccess = false;
    res.napiResponse = false;
}


bool provisionBand(NymiApi * napi){
    
    resultPack res;
    
    //set up provisioning callbacks
    agreementCallback onPattern = [&napi](std::vector<std::string> pattern) {
        std::cout << "Pattern(s) discovered: ";
        for (auto p : pattern) {
            std::cout << p << " ";
        }
        std::cout<<std::endl;
        napi->acceptPattern(pattern.at(0));
    };
    
    newProvisionCallback onProvision = [&](NymiProvision newprov) { //pass by reference?
        std::cout << "Successfully provisioned with pid: " << newprov.getPid() << std::endl;
        napi->stopProvisioning();
        nBand = std::move(newprov);
        
        std::lock_guard<std::mutex> lock(res.mtx);
        res.opSuccess = true;
        res.cv.notify_one();
    };
    
    napi->startProvisioning(onPattern, onProvision);
    
    //add and put band in provisioning mode
    nymuAddBand();
    nymuProvisionBand();
    
    //wait for provisioning to complete
    std::unique_lock<std::mutex> lock(res.mtx);
    res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.opSuccess; });
    
    return res.opSuccess;
}

bool unprovisionBand(){

    resultPack res;

    onProvisionRevokedCallback onProvisionRevoked = [&res](bool opResult, std::string pid, napiError err){

        std::cout<<"Provision revoked on Nymi Band with pid "<<pid<<std::endl;

        std::lock_guard<std::mutex> lock(res.mtx);
        res.napiResponse = true;
        res.opSuccess = true;
        res.cv.notify_one();
    };

    nBand.revokeProvision(true,onProvisionRevoked);

    std::unique_lock<std::mutex> lock(res.mtx);
    res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });

    return res.opSuccess;
}

bool getRandom(){
    
    resultPack res;
    
    randomCallback onRandom = [&res](bool opResult, std::string pid,std::string prand, napiError err) {
        if (opResult) {
            std::cout << "Received pseudo random number: " << prand <<" for band with pid: "<<pid<< std::endl;
            res.opSuccess = true;
        }
        else {
            std::cout<<"Prg error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
        }
        
        std::lock_guard<std::mutex> lock(res.mtx);
        res.napiResponse = true;
        res.cv.notify_one();
    };
    
    nBand.getRandom(onRandom);
    
    std::unique_lock<std::mutex> lock(res.mtx);
    res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
    
    return res.opSuccess;
}

bool getSymmetric(){
    
    resultPack res;
    
    symmetricKeyCallback onSk = [&res](bool opResult, std::string pid,std::string sk, napiError err) {
        if (opResult) {
            std::cout << "Received symmetric key: " << sk <<" for band with pid: "<<pid<< std::endl;
            res.opSuccess = true;
        }
        else{
            std::cout<<"Symmetric key get error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
        }

        std::lock_guard<std::mutex> lock(res.mtx);
        res.napiResponse = true;
        res.cv.notify_one();
    };

    createdKeyCallback onSkCreated = [&onSk](bool opResult, std::string pid, KeyType keyType, napiError err) {
        if (!opResult) {
            std::cout<<"Symmetric key create error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
            return;
        }

        nBand.getSymmetricKey(onSk);
    };

    {
        bool guarded = false;
        nBand.createSymmetricKey(guarded,onSkCreated);

        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
    }

    if (!res.opSuccess) return false;   //already failed

    resetResultPack(res);

    //now test revoking the key
    revokedKeyCallback onRevokedKey = [&res](bool opResult, std::string pid, KeyType keyType, napiError err){

        std::cout<<"KeyType "<<keyTypeToString(keyType)<<" revoked on Nymi Band with pid "<<pid<<std::endl;

        std::lock_guard<std::mutex> lock(res.mtx);
        res.napiResponse = true;
        res.opSuccess = (keyType == KeyType::SYMMETRIC);
        res.cv.notify_one();
    };

    {
        nBand.revokeKey(KeyType::SYMMETRIC, onRevokedKey);

        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
    }

    return res.opSuccess;
}

bool getTotp(){

    resultPack res;

    totpGetCallback onTotp = [&res](bool opResult, std::string pid,std::string totp, napiError err) {
        if (opResult) {
            std::cout << "Received totp key: " << totp <<" for band with pid: "<<pid<< std::endl;
            res.opSuccess = true;
        }
        else {
            std::cout<<"Totp get error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
        }

        std::lock_guard<std::mutex> lock(res.mtx);
        res.napiResponse = true;
        res.cv.notify_one();
    };

    createdKeyCallback onTotpCreate = [&onTotp](bool opResult, std::string pid, KeyType keyType, napiError err) {
        if (!opResult) {
            std::cout<<"Totp set error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
            return;
        }
        std::cout << "Totp key set with result " << opResult <<" for band with pid: "<<pid<< std::endl;

        nBand.getTotpKey(onTotp);
    };

    {
        bool guarded = false;
        std::string totpkeystr = "48656c6c6f21deadbeef";
        nBand.createTotpKey(totpkeystr, guarded, onTotpCreate);

        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
    }

    if (!res.opSuccess) return false;   //already failed

    resetResultPack(res);

    //now test revoking the key
    revokedKeyCallback onRevokedKey = [&res](bool opResult, std::string pid, KeyType keyType, napiError err){

        std::cout<<"KeyType "<<keyTypeToString(keyType)<<" revoked on Nymi Band with pid "<<pid<<std::endl;

        std::lock_guard<std::mutex> lock(res.mtx);
        res.napiResponse = true;
        res.opSuccess = (keyType == KeyType::TOTP);
        res.cv.notify_one();
    };

    {
        nBand.revokeKey(KeyType::TOTP, onRevokedKey);

        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
    }

    return res.opSuccess;
}

bool sign(){
    
    resultPack res;
    
    ecdsaSignCallback onSign = [&res](bool opResult, std::string pid,std::string sig, std::string vk, napiError err) {
        if (opResult) {
            std::cout << "Received signature: " << sig <<", with verification key: "<<vk <<" for band with pid: "<<pid<<std::endl;
            res.opSuccess = true;
        }
        else {
            std::cout<<"Received error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
        }
        
        std::lock_guard<std::mutex> lock(res.mtx);
        res.napiResponse = true;
        res.cv.notify_one();
    };
    
    nBand.signMessage("7a1a5ee210a9dd4fc0a12319c394862f7caa7fe513bbbc8d22032f8e87e6c307",onSign);
    
    std::unique_lock<std::mutex> lock(res.mtx);
    res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
    
    return res.opSuccess;
}

bool notify(){
    
    resultPack res;
    HapticNotification notifyType;
    
    onNotificationCallback onNotified = [&res,&notifyType](bool opResult, std::string pid, HapticNotification type, napiError err) {
        if (opResult) {
            std::cout<< "Notification result: " << opResult << ", Notification type: " << (int)type <<" for band with pid: "<<pid<< std::endl;
            notifyType = type;
            res.opSuccess = true;
        }
        else {
            std::cout<<"Received error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
        }
        
        std::lock_guard<std::mutex> lock(res.mtx);
        res.napiResponse = true;
        res.cv.notify_one();
    };
    
    //test positive notification
    {
        nBand.sendNotification(HapticNotification::NOTIFY_POSITIVE, onNotified);
        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
    }
    
    //return if already have an error
    if (!res.opSuccess || notifyType != HapticNotification::NOTIFY_POSITIVE) { return res.opSuccess; }
    
    resetResultPack(res);
    
    //test negative notification
    {
        nBand.sendNotification(HapticNotification::NOTIFY_NEGATIVE, onNotified);
        std::unique_lock<std::mutex> lock(res.mtx);
        res.cv.wait_for(lock,std::chrono::milliseconds(napiTimeout),[&]{ return res.napiResponse; });
    }
    
    return (res.opSuccess && notifyType == HapticNotification::NOTIFY_NEGATIVE);
}
