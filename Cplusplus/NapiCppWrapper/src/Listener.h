//
//  Listener.hpp
//  NapiCpp
//
//  Created by Hanieh Bastani on 2016-05-01.
//  Copyright © 2016 Hanieh Bastani. All rights reserved.
//

#ifndef Listener_hpp
#define Listener_hpp

#include <functional>
#include "NymiApi.h"
#include "NeaCallbackTypes.h"
#include "JsonUtilityFunctions.h"

namespace PrivateListener {
    
    using nljson = nlohmann::json;
    using opHandlerType = std::map<std::string, std::function<void(nljson &jobj)> >;

    //loop variable in waitForMessage
    void setQuit(bool _quit);
    
    //some utility functions
    inline bool printResultIfFalse(std::function<bool(std::vector<std::string>)> call,std::vector<std::string> field, bool print = true){
    
        if (!call(field)){
            if (print) { std::cerr<<"INFO: Json from Napi is missing field "<<field[0]<<std::endl; }
            return false;
        }
        return true;
    }
    
    /*
        Assumptions: a well constructed json object must have the following fields:
            * "successful"
            * "exchange"
            * either "response", or "errors", or "event"
     */
    inline bool wellConstructedJson(nljson &jobj){
        
        nljson::iterator jit;
        auto callHasKey = std::bind(hasKey,std::ref(jobj),std::placeholders::_1,std::ref(jit));
        bool print = false;
        
        return (printResultIfFalse(callHasKey,{"response"},print) || printResultIfFalse(callHasKey,{"errors"},print) || printResultIfFalse(callHasKey,{"event"},print) &&
                printResultIfFalse(callHasKey,{"successful"},print)  &&
                printResultIfFalse(callHasKey,{"exchange"},print)
                );
    }
    
    bool getExchange(nljson &jobj, std::string &exchange, bool errorIfNoExchange);
    bool getPid(nljson &jobj, std::string &pid, napiError &nErr);
    bool getPid(nljson &jobj, std::string &pid);
	bool getOperation(nljson jobj, std::vector<std::string> &op);

    //running on the thread NymiApi::listener
    void waitForMessage();

    //handle operations from napi
    void handleNapiError(nljson &jobj);
    void handleOpProvision(nljson &jobj);
	void handleOpProvisionsChanged(nljson &jobj);
    void handleOpInfo(nljson &jobj);
    void handleOpRandom(nljson &jobj);
    void handleOpSymmetric(nljson &jobj);
    void handleOpSignature(nljson &jobj);
    void handleOpTotp(nljson &jobj);
    void handleOpNotified(nljson &jobj);
    void handleOpApiNotifications(nljson &jobj);
    void handleOpRevokeProvision(nljson &jobj);
    void handleOpKey(nljson &jobj);

    //setters for callbacks to user application, called from NymiApi.
    //needed to define the callbacks in Listener.cpp with setters here, to avoid multiply defined symbols.
	void setNAPI(NymiApi* napi);
	void setOnAgreement(agreementCallback _onAgreement);
    void setOnProvision(newProvisionCallback _onProvision);
	void setOnProvisionChange(provisionChangeCallback _onProvisionChange);
    void setOnError(errorCallback _onError);
    void setProvisionList(getProvisionsCallback _onProvisionList);
    void setOnProvisionModeChange(onStartStopProvisioning _onProvisionModeChange);
    void setOnFoundChange(onNymiBandFoundStatusChange _onFoundChange);
    void setOnPresenceChange(onNymiBandPresenceChange _onPresenceChange);
    void setOnNotificationsGet(onNotificationsGetState _onNotificationGet);

    //json op to handle function mapping
    const opHandlerType opHandler = {
        {"provision",handleOpProvision},
		{"provisions",handleOpProvisionsChanged},
        {"info",handleOpInfo},
        {"random",handleOpRandom},
        {"symmetricKey",handleOpSymmetric},
        {"sign",handleOpSignature},
        {"totp",handleOpTotp},
        {"buzz",handleOpNotified},
        {"notifications",handleOpApiNotifications},
        {"revoke",handleOpRevokeProvision},
        {"key",handleOpKey}
    };
    
} //end namespace Listener

#endif /* Listener_hpp */
