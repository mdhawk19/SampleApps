//
//  NymiProvision.cpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-04-22.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#include "NymiProvision.h"
#include "json-napi.h"
#include "GenJson.h"
#include <random>
#include <utility>

static std::mt19937 rng;
static std::uniform_int_distribution<std::mt19937::result_type> dist(0, 9999);
void seed_random(){ rng.seed(std::random_device()()); }

std::map<std::string, NymiProvision::NeaCallback> NymiProvision::nymiProvisions = std::map<std::string, NymiProvision::NeaCallback>();

NymiProvision::NymiProvision() { seed_random(); }
NymiProvision::NymiProvision(const NymiProvision &other) :m_pid(other.getPid()) { seed_random(); }
NymiProvision::NymiProvision(std::string pid):m_pid(pid) { seed_random(); }

bool NymiProvision::getRandom(randomCallback onRandom){
    
    if (!onRandom) return false;
    
	std::string exchange = std::to_string(dist(rng));
	exchange += "random" + getPid();
    nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onRandom)));
    nymi::jsonNapiPut(get_random(getPid(),exchange));
    return true;
}

bool NymiProvision::createSymmetricKey(bool guarded, createdKeyCallback onCreatedKey){
    
    if (!onCreatedKey) return false;
    
    std::string exchange = std::to_string(dist(rng));
    exchange += "createsymkey" + getPid();
    nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onCreatedKey)));
    std::string createsk = create_symkey(getPid(),guarded,exchange);
    std::cout<<"sending msg: "<<createsk<<std::endl;
    nymi::jsonNapiPut(createsk);
    return true;
}

bool NymiProvision::getSymmetricKey(symmetricKeyCallback onSymmetric) {

    if (!onSymmetric) return false;
    
	std::string exchange = std::to_string(dist(rng));
	exchange += "getsymkey" + getPid();
	nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onSymmetric)));
	nymi::jsonNapiPut(get_symkey(getPid(),exchange));
    return true;
}

bool NymiProvision::signMessage(std::string msghash, ecdsaSignCallback onMessageSigned) {

    if (!onMessageSigned) return false;
    
	std::string exchange = std::to_string(dist(rng));
	exchange += "sign" + getPid();
	nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onMessageSigned)));
	nymi::jsonNapiPut(sign_msg(getPid(), msghash, exchange));
    return true;
}

bool NymiProvision::createTotpKey(std::string totpKey, bool guarded, createdKeyCallback onCreatedKey) {

    if (!onCreatedKey) return false;
    
	std::string exchange = std::to_string(dist(rng));
	exchange += "createTotp" + getPid();
	nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onCreatedKey)));
	nymi::jsonNapiPut(set_totp(getPid(),totpKey,guarded,exchange));
    return true;
}

bool NymiProvision::getTotpKey(totpGetCallback onTotpGet) {

    if (!onTotpGet) return false;
    
	std::string exchange = std::to_string(dist(rng));
	exchange += "getTotp" + getPid();
	nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onTotpGet)));
	nymi::jsonNapiPut(get_totp(getPid(), exchange));
    return true;
}

bool NymiProvision::sendNotification(HapticNotification notifyType, onNotificationCallback onNotified) {

    if (!onNotified) return false;
    
	std::string exchange = std::to_string(dist(rng));
	exchange += "notify" + getPid();
	nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onNotified)));
	nymi::jsonNapiPut(notify(getPid(), notifyType == HapticNotification::NOTIFY_POSITIVE, exchange));
    return true;
}

bool NymiProvision::getDeviceInfo(deviceInfoCallback onDeviceInfo){
    
    if (!onDeviceInfo) return false;
    
    std::string exchange = std::to_string(dist(rng));
    exchange += "deviceinfo" + getPid();
    nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onDeviceInfo)));
    nymi::jsonNapiPut(get_info(exchange));
    return true;
}

bool NymiProvision::revokeKey(KeyType keyType, revokedKeyCallback onRevokeKey){

    if (!onRevokeKey) return false;

    std::string exchange = std::to_string(dist(rng));
    exchange += "deviceinfo" + getPid();
    nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onRevokeKey)));

    std::string keyStr;
    switch(keyType) {
        case KeyType::SYMMETRIC: keyStr = "symmetric"; break;
        case KeyType::TOTP: keyStr = "totp"; break;
        default: return false;
    }

    nymi::jsonNapiPut(delete_key(getPid(),keyStr,exchange));
    return true;
}

bool NymiProvision::revokeProvision(bool onlyIfAuthenticated, onProvisionRevokedCallback onProvRevoked){

    if (!onProvRevoked) return false;

    std::string exchange = std::to_string(dist(rng));
    exchange += "revokeprovision" + getPid();
    nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onProvRevoked)));
    nymi::jsonNapiPut(revoke_provision(getPid(),onlyIfAuthenticated,exchange));
    return true;
}