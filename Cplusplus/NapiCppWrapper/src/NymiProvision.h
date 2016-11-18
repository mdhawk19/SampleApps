//
//  NymiProvision.hpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-04-22.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#ifndef NymiProvision_hpp
#define NymiProvision_hpp

#include <functional>
#include <string>
#include <map>
#include "NeaCallbackTypes.h"

class NymiProvision {
    
    friend class NymiApi;

public:
    
    NymiProvision();
    NymiProvision(const NymiProvision &other);
    NymiProvision(std::string pid);
    
    inline std::string getPid() const { return m_pid; }

    bool getRandom(randomCallback onRandom);
    bool createSymmetricKey(bool guarded, createdKeyCallback onCreatedKey);
	bool getSymmetricKey(symmetricKeyCallback onSymmetric);
	bool signMessage(std::string message, ecdsaSignCallback onMessageSigned);
	bool createTotpKey(std::string totpKey, bool guarded, createdKeyCallback onCreatedKey);
	bool getTotpKey(totpGetCallback onTotpGet);
	bool sendNotification(HapticNotification notifyType, onNotificationCallback onNotified);
    bool getDeviceInfo(deviceInfoCallback onDeviceInfo);
    bool revokeKey(KeyType keyType, revokedKeyCallback onRevokedKey);
    bool revokeProvision(bool only_if_authenticated, onProvisionRevokedCallback onProvRevoked);

private:
    
    std::string m_pid;

	class NeaCallback {

        //maybe a structure such as a union would be better suited here
        std::function<void(bool, std::string, napiError)> fn1;
		std::function<void(bool, std::string,std::string, napiError)> fn2;
		std::function<void(bool, std::string,std::string, std::string, napiError)> fn3;
		std::function<void(bool, std::string,HapticNotification, napiError)> fn4;
        std::function<void(bool, std::string,TransientNymiBandInfo&, napiError)> fn5;
        std::function<void(bool, std::string, KeyType, napiError)> fn6;

	public:
		NeaCallback(std::function<void(bool, std::string, napiError)> _fn) :fn1(_fn) {}
		NeaCallback(std::function<void(bool, std::string, std::string, napiError)> _fn) :fn2(_fn) {}
		NeaCallback(std::function<void(bool, std::string, std::string, std::string, napiError)> _fn) :fn3(_fn) {}
		NeaCallback(std::function<void(bool, std::string, HapticNotification, napiError)> _fn) :fn4(_fn) {}
        NeaCallback(std::function<void(bool, std::string, TransientNymiBandInfo&, napiError)> _fn) :fn5(_fn) {}
        NeaCallback(std::function<void(bool, std::string, KeyType, napiError)> _fn) :fn6(_fn) {}

		void operator()(bool arg1, std::string arg2, napiError arg3) {
			if (fn1) fn1(arg1,arg2,arg3);
		}
		void operator()(bool arg1, std::string arg2,std::string arg3, napiError arg4) {
			if (fn2) fn2(arg1, arg2,arg3,arg4);
		}
		void operator()(bool arg1, std::string arg2,std::string arg3, std::string arg4, napiError arg5) {
			if (fn3) fn3(arg1,arg2,arg3,arg4,arg5);
		}
		void operator()(bool arg1, std::string arg2, HapticNotification arg3, napiError arg4) {
			if (fn4) fn4(arg1,arg2,arg3,arg4);
		}
        void operator()(bool arg1, std::string arg2,TransientNymiBandInfo &arg3, napiError arg4) {
            if (fn5) fn5(arg1,arg2,arg3,arg4);
        }
        void operator()(bool arg1, std::string arg2, KeyType &arg3, napiError arg4) {
            if (fn6) fn6(arg1,arg2,arg3,arg4);
        }
	};

public:
    static std::map<std::string, NeaCallback> nymiProvisions;
};

#endif /* NymiProvision_h */
