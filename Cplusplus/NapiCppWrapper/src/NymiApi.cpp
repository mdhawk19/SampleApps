#include <iostream>
#include "NymiApi.h"
#include "GenJson.h"
#include "Listener.h"

NymiApi *NymiApi::nApi = nullptr;

NymiApi * NymiApi::getNymiApi(nymi::ConfigOutcome &initResult, errorCallback onError, std::string rootDirectory, nymi::LogLevel log, int nymulatorPort, std::string nymulatorHost) {
    
    if (!onError) throw "onError callback is invalid\n";
    
    if (nApi == nullptr) {
        
        nApi = new NymiApi;
        nApi->init(initResult, rootDirectory, log, nymulatorPort, nymulatorHost);

        if (initResult != nymi::ConfigOutcome::okay) {
            delete nApi;
            nApi = nullptr; //caller can call getNymiApi again to reattempt initialization
            return nApi;
        }

        PrivateListener::setOnError(onError);
    }
    
    return nApi;
}

NymiApi::~NymiApi() {

    if (listener.joinable()) {
        PrivateListener::setQuit(true);
        listener.join();                    //must be joined before calling napiTerminate
        nymi::jsonNapiTerminate();

        std::cout << "NymiApi terminated\n";
    }

    nApi = nullptr; //in case we call NymiApi::getNymiApi and need to re-initialize Napi again.
};

//public functions
//----------------
void NymiApi::init(nymi::ConfigOutcome &initResult, std::string rootDirectory, nymi::LogLevel log, int nymulatorPort, std::string nymulatorHost) {
	
	initResult = nymi::jsonNapiConfigure(rootDirectory, log, nymulatorPort, nymulatorHost);

    if (initResult == nymi::ConfigOutcome::okay) {
        listener = std::thread(PrivateListener::waitForMessage);
    }
}

bool NymiApi::startProvisioning(agreementCallback onAgree, newProvisionCallback onProvision) {

    if (!onAgree || !onProvision) return false;
    
    PrivateListener::setOnAgreement(onAgree);
    PrivateListener::setOnProvision(onProvision);
    nymi::jsonNapiPut(start_prov());
    return true;
}

void NymiApi::acceptPattern(std::string pattern) {

	nymi::jsonNapiPut(accept_pattern(pattern));
}

void NymiApi::stopProvisioning() {

	nymi::jsonNapiPut(stop_prov());
}

bool NymiApi::getProvisions(getProvisionsCallback getProvList, ProvisionListType type) {

    if (!getProvList) return false;

    PrivateListener::setProvisionList(getProvList);

    std::string exchange = type == ProvisionListType::ALL ? "provisions" : "provisionsPresent";
    nymi::jsonNapiPut(get_info(exchange));
    return true;
}

bool NymiApi::setOnProvisionModeChange(onStartStopProvisioning onProvisionModeChange){
    
    if (!onProvisionModeChange) return false;
    PrivateListener::setOnProvisionModeChange(onProvisionModeChange);
    return true;
}

bool NymiApi::setOnFoundChange(onNymiBandFoundStatusChange onFoundChange) {

    if (!onFoundChange) return false;
    
    nymi::jsonNapiPut(enable_notification(true, "onFoundChange"));
    PrivateListener::setOnFoundChange(onFoundChange);
    return true;
}

bool NymiApi::setOnPresenceChange(onNymiBandPresenceChange onPresenceChange){
    
   	if (!onPresenceChange) return false;
    
    nymi::jsonNapiPut(enable_notification(true, "onPresenceChange"));
    PrivateListener::setOnPresenceChange(onPresenceChange);
    return true;
}

void NymiApi::disableOnFoundChange(){
    
    nymi::jsonNapiPut(enable_notification(false,"onFoundChange"));
}

void NymiApi::disableOnPresenceChange(){
    
    nymi::jsonNapiPut(enable_notification(false,"onPresenceChange"));
}

bool NymiApi::getApiNotificationState(onNotificationsGetState onNotificationsGet){
    
    if (!onNotificationsGet) return false;
    
    PrivateListener::setOnNotificationsGet(onNotificationsGet);
    nymi::jsonNapiPut(get_state_notifications());
    return true;
}