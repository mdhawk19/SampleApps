#include <iostream>
#include "NymiApi.h"
#include "GenJson.h"
#include "Listener.h"

#ifdef _WIN32
#include <exception>
#endif

NymiApi *NymiApi::nApi = nullptr;

NymiApi * NymiApi::getNymiApi(napi::ConfigOutcome &initResult, errorCallback onError, std::string rootDirectory, napi::LogLevel log, int nymulatorPort, std::string nymulatorHost) {
    
    if (!onError) throw "onError callback is invalid\n";
    
    if (nApi == nullptr) {
        
        nApi = new NymiApi;

#ifdef _WIN32
		// Load NAPI DLL
		nApi->m_hNapi = LoadLibraryA("napi.dll");
		if (!nApi->m_hNapi) {
			std::cerr << "Unable to load NAPI DLL" << std::endl;
			delete nApi;
			nApi = nullptr; //caller can call getNymiApi again to reattempt initialization
			return nApi;
		}

		// Load exported NAPI functions
		nApi->put = (PPUT)GetProcAddress(nApi->m_hNapi, "napiPut");
		if (nApi->put == NULL)
			std::cout << "Got NULL for put" << std::endl;
		nApi->get = (PGET)GetProcAddress(nApi->m_hNapi, "napiGet");
		if (nApi->get == NULL)
			std::cout << "Got NULL for get" << std::endl;
		nApi->try_get = (PTRYGET)GetProcAddress(nApi->m_hNapi, "napiTryGet");
		if (nApi->try_get == NULL)
			std::cout << "Got NULL for try_get" << std::endl;
		nApi->configure = (PCONFIG)GetProcAddress(nApi->m_hNapi, "napiConfigure");
		if (nApi->configure == NULL)
			std::cout << "Got NULL for configure" << std::endl;
		nApi->terminate = (PTERMINATE)GetProcAddress(nApi->m_hNapi, "napiTerminate");
		if (nApi->terminate == NULL)
			std::cout << "Got NULL for terminate" << std::endl;

		std::cout << "Loaded NAPI DLL successfully" << std::endl;
#endif

		nApi->init(initResult, rootDirectory, log, nymulatorPort, nymulatorHost);

        if (initResult != napi::ConfigOutcome::okay) {
            delete nApi;
            nApi = nullptr; //caller can call getNymiApi again to reattempt initialization
            return nApi;
        }

		std::cout << "Initialized NAPI successfully" << std::endl;

		PrivateListener::setNAPI(nApi);
        PrivateListener::setOnError(onError);
    }
    
    return nApi;
}

NymiApi::~NymiApi() {

	PrivateListener::setQuit(true); // Must tell listener thread loop to quit *before* terminating NAPI
#ifdef _WIN32
	nApi->terminate();
#else
	napi::terminate();
#endif
	if (listener.joinable()) {
        listener.join();
	}

	delete storage;

#ifdef _WIN32
	FreeModule(m_hNapi);
#endif

    nApi = nullptr; //in case we call NymiApi::getNymiApi and need to re-initialize Napi again.
};

//private functions
//----------------
napi::PutOutcome NymiApi::portable_put(std::string json) {
	std::cout << "Calling put to Nymi API with: " << json << std::endl;
#ifdef _WIN32
	return nApi->put(json.c_str());
#else
	return napi::put(json.c_str());
#endif

}

//public functions
//----------------
void NymiApi::init(napi::ConfigOutcome &initResult, std::string rootDirectory, napi::LogLevel log, int nymulatorPort, std::string nymulatorHost) {

	if (!storage)
		storage = new ProvisionStorage("provisions.json");

	std::cout << "Initializing NAPI" << std::endl;

	std::string provisions = storage->read();
#ifdef _WIN32
	if (nApi->configure != NULL)
		try {
			initResult = nApi->configure("NapiCppWrapper",
				rootDirectory.c_str(),
				provisions.c_str(),
				log,
				nymulatorPort,
				nymulatorHost.c_str());
		}
		catch (std::exception e) {
			std::cout << "Caught exception in NAPI configure: " << e.what() << std::endl;
		}
#else
	initResult = napi::configure( "NapiCppWrapper",
								   rootDirectory.c_str(),
								   provisions.c_str(),
								   log,
								   nymulatorPort,
								   nymulatorHost.c_str());
#endif

    if (initResult == napi::ConfigOutcome::okay) {
		PrivateListener::setNAPI(nApi);
        listener = std::thread(PrivateListener::waitForMessage);
    }
}

bool NymiApi::startProvisioning(agreementCallback onAgree, newProvisionCallback onProvision) {

    if (!onAgree || !onProvision) return false;
    
    PrivateListener::setOnAgreement(onAgree);
    PrivateListener::setOnProvision(onProvision);
	portable_put(start_prov());
	return true;
}

void NymiApi::acceptPattern(std::string pattern) {

	portable_put(accept_pattern(pattern));
}

void NymiApi::stopProvisioning() {

	portable_put(stop_prov());
}

bool NymiApi::getProvisions(getProvisionsCallback getProvList, ProvisionListType type) {

    if (!getProvList) return false;

    PrivateListener::setProvisionList(getProvList);

    std::string exchange = type == ProvisionListType::ALL ? "provisions" : "provisionsPresent";
	portable_put(get_info(exchange));
	return true;
}

bool NymiApi::setOnProvisionsChange(provisionChangeCallback onProvisionsChange) {
	if (!onProvisionsChange) return false;
	PrivateListener::setOnProvisionChange(onProvisionsChange);
	return true;
}

bool NymiApi::setOnProvisionModeChange(onStartStopProvisioning onProvisionModeChange){
    
    if (!onProvisionModeChange) return false;
    PrivateListener::setOnProvisionModeChange(onProvisionModeChange);
    return true;
}

bool NymiApi::setOnFoundChange(onNymiBandFoundStatusChange onFoundChange) {

    if (!onFoundChange) return false;
    
	portable_put(enable_notification(true, "onFoundChange"));
    PrivateListener::setOnFoundChange(onFoundChange);
    return true;
}

bool NymiApi::setOnPresenceChange(onNymiBandPresenceChange onPresenceChange){
    
   	if (!onPresenceChange) return false;
    
	portable_put(enable_notification(true, "onPresenceChange"));
    PrivateListener::setOnPresenceChange(onPresenceChange);
    return true;
}

void NymiApi::disableOnFoundChange(){
    
	portable_put(enable_notification(false,"onFoundChange"));
}

void NymiApi::disableOnPresenceChange(){
    
	portable_put(enable_notification(false,"onPresenceChange"));
}

bool NymiApi::getApiNotificationState(onNotificationsGetState onNotificationsGet){
    
    if (!onNotificationsGet) return false;
    
    PrivateListener::setOnNotificationsGet(onNotificationsGet);
	portable_put(get_state_notifications());
    return true;
}

bool NymiApi::save(std::vector<NymiProvision> &curbands) {
	// Update provisions.json file with current list of provisions
	std::stringstream ss;
	if (curbands.size() == 0)
		ss << "{}";
	else {
		ss << "{\"devices\":[],\"provisions\":{";
		for (auto prov : curbands) {
			ss << prov.getProvisionString() << ",";
		}
		ss.seekp(-1, ss.cur);
		ss << "}}";
	}
	return storage->write(ss.str());
}
