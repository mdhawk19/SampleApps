//
//  NymiProvision.cpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-04-22.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#include "NymiProvision.h"
#include "napi.h"
#include "GenJson.h"
#include <random>
#include <utility>

static std::mt19937 rng;
static std::uniform_int_distribution<std::mt19937::result_type> dist(0, 9999);
void seed_random(){ rng.seed(std::random_device()()); }

std::map<std::string, NymiProvision::NeaCallback> NymiProvision::nymiProvisions = std::map<std::string, NymiProvision::NeaCallback>();

NymiProvision::NymiProvision(NymiApi* napi):m_napi(napi) { seed_random(); }
NymiProvision::NymiProvision(const NymiProvision &other) :m_pid(other.getPid()), m_privkey(other.getPriv()) { seed_random(); }
NymiProvision::NymiProvision(std::string pid, std::string secret):m_pid(pid),m_privkey(secret) { seed_random(); }

std::string NymiProvision::getProvisionString() {
	std::stringstream tmpProv;

	tmpProv << "\"" << m_pid << "\":{\"pid\":\"" << m_pid << "\",\"provision\":\"" << m_privkey << "\",\"provisioned\":true}";
	return tmpProv.str();
}

bool NymiProvision::getRandom(randomCallback onRandom){
    
    if (!onRandom) return false;
    
	std::string exchange = std::to_string(dist(rng));
	exchange += "random" + getPid();
    nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onRandom)));
    m_napi->portable_put(get_random(getPid(),exchange));
    return true;
}

bool NymiProvision::createSymmetricKey(bool guarded, createdKeyCallback onCreatedKey){
    
    if (!onCreatedKey) return false;
    
    std::string exchange = std::to_string(dist(rng));
    exchange += "createsymkey" + getPid();
    nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onCreatedKey)));
    std::string createsk = create_symkey(getPid(),guarded,exchange);
    std::cout<<"sending msg: "<<createsk<<std::endl;
    m_napi->portable_put(createsk);
    return true;
}

bool NymiProvision::getSymmetricKey(symmetricKeyCallback onSymmetric) {

    if (!onSymmetric) return false;
    
	std::string exchange = std::to_string(dist(rng));
	exchange += "getsymkey" + getPid();
	nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onSymmetric)));
	m_napi->portable_put(get_symkey(getPid(),exchange));
    return true;
}

bool NymiProvision::signSetup(ecdsaSignSetupCallback onSignSetup) {
	if (!onSignSetup) return false;

	std::string exchange = std::to_string(dist(rng));
	exchange += "sign" + getPid();
	nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onSignSetup)));
	m_napi->portable_put(sign_setup(getPid(), "NIST256P", exchange));
	return true;
}

bool NymiProvision::signMessage(std::string msghash, ecdsaSignCallback onMessageSigned) {

    if (!onMessageSigned) return false;
    
	std::string exchange = std::to_string(dist(rng));
	exchange += "sign" + getPid();
	nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onMessageSigned)));
	m_napi->portable_put(sign_msg(getPid(), msghash, exchange));
    return true;
}

bool NymiProvision::createTotpKey(std::string totpKey, bool guarded, createdKeyCallback onCreatedKey) {

    if (!onCreatedKey) return false;
    
	std::string exchange = std::to_string(dist(rng));
	exchange += "createTotp" + getPid();
	nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onCreatedKey)));
	m_napi->portable_put(set_totp(getPid(),totpKey,guarded,exchange));
    return true;
}

bool NymiProvision::getTotpKey(totpGetCallback onTotpGet) {

    if (!onTotpGet) return false;
    
	std::string exchange = std::to_string(dist(rng));
	exchange += "getTotp" + getPid();
	nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onTotpGet)));
	m_napi->portable_put(get_totp(getPid(), exchange));
    return true;
}

bool NymiProvision::sendNotification(HapticNotification notifyType, onNotificationCallback onNotified) {

    if (!onNotified) return false;
    
	std::string exchange = std::to_string(dist(rng));
	exchange += "notify" + getPid();
	nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onNotified)));
	m_napi->portable_put(notify(getPid(), notifyType == HapticNotification::NOTIFY_POSITIVE, exchange));
    return true;
}

bool NymiProvision::getDeviceInfo(deviceInfoCallback onDeviceInfo){
    
    if (!onDeviceInfo) return false;
    
    std::string exchange = std::to_string(dist(rng));
    exchange += "deviceinfo" + getPid();
    nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onDeviceInfo)));
    m_napi->portable_put(get_info(exchange));
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

    m_napi->portable_put(delete_key(getPid(),keyStr,exchange));
    return true;
}

bool NymiProvision::revokeProvision(bool onlyIfAuthenticated, onProvisionRevokedCallback onProvRevoked){

    if (!onProvRevoked) return false;

    std::string exchange = std::to_string(dist(rng));
    exchange += "revokeprovision" + getPid();
    nymiProvisions.insert(std::make_pair(exchange, NymiProvision::NeaCallback(onProvRevoked)));
    m_napi->portable_put(revoke_provision(getPid(),onlyIfAuthenticated,exchange));
    return true;
}