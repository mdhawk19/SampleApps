#pragma once

#include <thread>
#include "NeaCallbackTypes.h"
#include "json-napi.h"

class NymiApi {

public:

	~NymiApi();

	enum class ProvisionListType { ALL, PRESENT };

    static NymiApi *getNymiApi(nymi::ConfigOutcome &initResult, errorCallback onError, std::string rootDirectory, nymi::LogLevel log = nymi::LogLevel::normal, int nymulatorPort = -1, std::string nymulatorHost = "");
	
	bool startProvisioning(agreementCallback onPattern, newProvisionCallback onProvision);
	void acceptPattern(std::string pattern);
	void stopProvisioning();
	bool getProvisions(getProvisionsCallback getProvList, ProvisionListType type);
    bool setOnProvisionModeChange(onStartStopProvisioning onProvisionModeChange);

	bool setOnFoundChange(onNymiBandFoundStatusChange onFoundChange);
    bool setOnPresenceChange(onNymiBandPresenceChange onPresenceChange);
    void disableOnFoundChange();
    void disableOnPresenceChange();
    bool getApiNotificationState(onNotificationsGetState onNotificationsGet);

private:

	//initialization and singleton pattern
	static NymiApi *nApi;
	NymiApi() { /*intentionally empty*/ };
	NymiApi(NymiApi &dontAllowCopy) { /*intentionally empty*/ }
    NymiApi(NymiApi &&dontAllowMove) { /*intentionally empty*/ }

	void init(nymi::ConfigOutcome &initResult, std::string rootDirectory, nymi::LogLevel log, int nymulatorPort = -1, std::string nymulatorHost = "");

	//receive json communication from napi
	std::thread listener;
};