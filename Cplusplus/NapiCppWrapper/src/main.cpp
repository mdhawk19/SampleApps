#include "NymiApi.h"
#include "NymiProvision.h"
#include "TransientNymiBandInfo.h"
#include <iostream>
#include <sstream>

#define NCL_VERBOSITY "*"

void printHelp(){

    std::cout<<"\nProvisioning commands:\n";
    std::cout<<"\n";
    std::cout<<"  `provision-start` enable provisioning bands.\n";
    std::cout<<"  `provision-stop` disable provisioning bands.\n";
    std::cout<<"  `accept [+|-]{5}` accept a provisioning pattern.\n     Pattern string has length 5, composed of '+' (led on) or '-' (led off).\n     e.g. `accept +----` to accept a pattern where only the first led is on.\n";
    std::cout<<"  `provision-getall` get all bands provisioned to this NEA.\n";
    std::cout<<"  `provision-gethere` get all bands provisioned to this NEA and are present.\n";
    std::cout<<"  `provision-revoke i` Revoke provisioning of Nymi Band i.\n";


    std::cout<<"\nCryptographic commands:\n";
    std::cout<<" In all cmds below, i is index of the band in std::vector<NymiProvision> bands.\n";
    std::cout<<"\n";
    std::cout<<"  `get-random i` get a random number from Nymi Band i.\n";
    std::cout<<"  `get-signature i` get a signature (of a currently hard coded msg) from Nymi Band i.\n";
    std::cout<<"  `create-sk i g` create a symmetric key on Nymi Band i.\n     Guarded option: g=1 -> guarded, g=0 -> unguarded.\n     If the key is guarded, key retrieval requires user double tap.\n";
    std::cout<<"  `get-sk i` get the symmetric key previously created on Nymi Band i.\n";
    std::cout<<"  `delete-sk i` delete the symmetric key previously created on Nymi Band i.\n";
    std::cout<<"  `create-totp i g` register totp (with a currently hardcoded totp key) on Nymi Band i.\n     Guarded option: g=1 -> guarded, g=0 -> unguarded.\n     If the key is guarded, key retrieval requires user double tap.\n";
    std::cout<<"  `get-totp i` get a totp token from Nymi Band i.\n";
    std::cout<<"  `delete-totp i` delete the totp registered on Nymi Band i.\n";

    std::cout<<"\nHaptic command:\n";
    std::cout<<"\n";
    std::cout<<"  `buzz i s` Send a haptic signal for status s to Nymi Band i.\n     Status option: s=1 -> success signal, s=0 -> error signal\n";

    std::cout<<"\nInfo command:\n";
    std::cout<<"\n";
    std::cout<<"  `info i` recieve a transient snapshot of the state of Nymi Band i.\n";

    std::cout<<"\nApi notification commands:\n";
    std::cout<<"\n";
    std::cout<<"  `get-noti-status` to get status of notification streams\n";
    std::cout<<"  `onfoundchange-start` receive notification when there is a change in authentication status of a Nymi Band.\n";
    std::cout<<"  `onfoundchange-stop` don't receive onfoundchange notifications.\n";
    std::cout<<"  `onpresencechange-start` receive notification when there is a change in presence status of a Nymi Band.\n";
    std::cout<<"  `onpresencechange-stop` don't receive onpresencechange notifications.\n";
}


int main() {

    NymiApi *napi;
	std::vector<NymiProvision> bands;
    
    onStartStopProvisioning onProvModeChage = [](std::string newState) {
        
        std::cout<<"Provisioning mode is now "<<newState<<std::endl;
    };
    
    agreementCallback onPattern = [](std::vector<std::string> patterns) {
        std::cout << "Pattern(s) discovered: ";
        for (auto p : patterns) {
            std::cout << p << " ";
        }
        if (!patterns.empty()){
            std::cout<<"\nConfirm the pattern on your Nymi Band by the accept command. example: enter `accept "<<patterns.at(0)<<"`";
        }
        std::cout<<std::endl;
    };
    
    newProvisionCallback onProvision = [&](NymiProvision newprov) {
        std::cout << "Successfully provisioned with pid: " << newprov.getPid() << std::endl;
        napi->stopProvisioning();
		bands.push_back(newprov);
    };
    getProvisionsCallback displayProv = [&](std::vector<NymiProvision> provisions) {
        
        std::cout << "Provisions:\n";
        for (auto &p : provisions) {
            std::cout << p.getPid() <<std::endl;
			bands.push_back(p);
        }
    };
    randomCallback onRandom = [](bool opResult, std::string pid,std::string prand, napiError err) {

        if (!opResult) {
            std::cout<<"Received error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
            return;
        }
		std::cout << "Received pseudo random number: " << prand <<" for band with pid: "<<pid<< std::endl;
	};
	symmetricKeyCallback onSk = [](bool opResult, std::string pid,std::string sk, napiError err) {
        if (!opResult) {
            std::cout<<"Received error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
            return;
        }
		std::cout << "Received symmetric key: " << sk <<" for band with pid: "<<pid<< std::endl;
	};

    createdKeyCallback onKeyCreated = [](bool opResult, std::string pid, KeyType keyType, napiError err) {
        if (!opResult) {
            std::cout<<"Received error in key creation "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
            return;
        }
        std::cout << "Created key type "<<keyTypeToString(keyType)<<" for band with pid: "<<pid<< std::endl;
    };

	ecdsaSignCallback onSign = [](bool opResult, std::string pid,std::string sig, std::string vk, napiError err) {
        if (!opResult) {
            std::cout<<"Received error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
            return;
        }
        std::cout << "Received signature: " << sig <<", with verification key: "<<vk <<" for band with pid: "<<pid<<std::endl;
	};

	totpGetCallback onTotp = [](bool opResult, std::string pid,std::string totp, napiError err) {
        if (!opResult) {
            std::cout<<"Received error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
            return;
        }
		std::cout << "Received totp key: " << totp <<" for band with pid: "<<pid<< std::endl;
	};
	onNotificationCallback onNotified = [](bool opResult, std::string pid, HapticNotification type, napiError err) {
        if (!opResult) {
            std::cout<<"Received error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
            return;
        }
        std::cout<< "Notification result: " << opResult << ", Notification type: " << (int)type <<" for band with pid: "<<pid<< std::endl;
	};
    deviceInfoCallback onDeviceInfo = [](bool opResult, std::string pid,TransientNymiBandInfo &tnbi, napiError err) {
        if (!opResult) {
            std::cout<<"Received error "<<err.errorString<<" for band with pid: "<<pid<<std::endl;
            return;
        }

        std::string nbiPid;
        if (tnbi.getPid(nbiPid)) { std::cout<<"Info for Nymi band with provision id: "<<nbiPid<<std::endl; }

        FoundStatus found;
        if (tnbi.getFoundState(found)) { std::cout<<"  Authentication status: "<<found<<std::endl; }

        PresenceStatus presence;
        if (tnbi.getPresenceState(presence)) { std::cout<<"  Presence status: "<<presence<<std::endl; }

        int rssi;
        if (tnbi.getRssiLast(rssi)) { std::cout<<"  Last RSSI: "<<rssi<<std::endl; }

        int rssiSmoothed;
        if (tnbi.getRssiSmoothed(rssiSmoothed)) { std::cout<<"  Smoothed RSSI: "<<rssiSmoothed<<std::endl; }

        std::string firmwareVersion;
        if (tnbi.getFirmwareVersion(firmwareVersion)) { std::cout<<"  Firmware version: "<<firmwareVersion<<std::endl; }

        bool isProv;
        if (tnbi.isProvisioned(isProv)) { std::cout<<"  Provisioned: "<<std::boolalpha<<isProv<<std::endl; }

        double sinceLastContact;
        if (tnbi.getSinceLastContact(sinceLastContact)) { std::cout<<"  Time since last contact: "<<sinceLastContact<<std::endl; }

        double authWindowRemaining;
        if (tnbi.getAuthenticationWindowRemaining(authWindowRemaining)) { std::cout<<"  Authentication window remaining: "<<authWindowRemaining<<std::endl; }

        int numCmdQueued;
        if (tnbi.getNumCommandsQueued(numCmdQueued)) { std::cout<<"  Number of commands queued: "<<numCmdQueued<<std::endl; }

        std::vector<std::string> commands;
        if (tnbi.getCommandsQueued(commands) && !commands.empty()) {
            std::cout<<"  Commands queued: \n";
            for (auto cmd : commands){ std::cout<<"    "<<cmd<<std::endl; }
        }
    };

    onNymiBandPresenceChange onPresenceChange = [](std::string pid, PresenceStatus before, PresenceStatus after, bool authenticated) {
        std::cout <<std::boolalpha<< "onPresenceChange, pid: "<<pid<<", before: " << before << ", after: " << after <<", authenticated: "<<authenticated<< std::endl;
    };
    onNymiBandFoundStatusChange onFoundChange = [](std::string pid, FoundStatus before, FoundStatus after) {
        std::cout << "onFoundChange, pid: "<<pid<<", before: " <<before << ", after: " <<after << std::endl;
	};
    onNotificationsGetState onNotificationsGet = [](std::map<std::string,bool> notificationsState){
        
        std::cout<<"Notifications state: \n";
        for (auto entry : notificationsState){
            std::cout<<entry.first<<" : "<<entry.second<<std::endl;
        }
    };

    onProvisionRevokedCallback onProvisionRevoked = [](bool opResult, std::string pid, napiError err){
		if (err.errorList.size() != 0)
			std::cout << "ERROR revoking provision on Nymi Band with pid " << pid << ": " << err.errorString << std::endl;
		else
	        std::cout<<"Provision revoked on Nymi Band with pid " << pid << std::endl;
    };

    revokedKeyCallback onRevokedKey = [](bool opResult, std::string pid, KeyType keyType, napiError err){

        std::cout<<keyTypeToString(keyType)<<" revoked on Nymi Band with pid "<<pid<<std::endl;
    };

    errorCallback onError = [](napiError nErr) {
        std::cout << nErr.errorString << std::endl;
    };
    
    try {
        nymi::ConfigOutcome initResult;
        /*
           If using the Nymulator 
           ----------------------
           On both OSX and Windows, when connecting to the Nymulator, nPort needs to be the 
           port which the Nymulator is running on. When you run the Nymulator, this port number 
           is at the top center of the app, next to "Running on Port"

           OSX: Link against libnapi-net.a
           Windows: Link against napi-net.lib

           If using a physical Nymi Band
           -----------------------------
           OSX: nPort is ignored.
                Link against libnapi.a
           Windows: nPort needs to be the port which the NymiBluetoothService is running on. By default, this is *9089*.
                Link against napi-net.lib

           Note that on Windows, there is only one Napi library, however nPort value changes depending on whether
           you talk to the physical band or a Nymulator band.

           On OSX, there are two different Napi libraries, one for connecting to the Nymulator (be sure to set the port),
           and one for connecting to the Nymi Band.
        */
        int nPort = 9089;
        napi = NymiApi::getNymiApi(initResult,onError, ".", nymi::LogLevel::normal, nPort, "127.0.0.1");

        if (initResult != nymi::ConfigOutcome::okay) {
            std::cout << "NymiApi initialization failed\n";
            return 1;
        }

        std::cout << "-*-*-> NymiApi initialization succeeded. Enter `help` for list of supported commands. <-*-*-\n\n";
        napi->setOnProvisionModeChange(onProvModeChage);
        napi->startProvisioning(onPattern, onProvision);

    }
    catch (...){
        std::cout<<"NymiApi initialization failed\n";
        return 1;
    }
    
    auto validateBandIndex = [&bands](int &bandIndex, std::istringstream &iss)->bool{
        if (!(iss >>bandIndex)){
            std::cout<<"Command format: `command i` where i is an integer representing index of Nymi Band `bands` vector.\n";
            return false;
        }
        if (bandIndex < 0 || bandIndex >= bands.size()) {
            std::cout << "Incorrect band index\n";
            return false;
        }
        return true;
    };

	//console interface, wait for user command
    std::string user_input;
    while (getline(std::cin, user_input)){
    
        std::istringstream iss(user_input);
        std::string command;
        iss >> command;

		if (command == "accept") {
            std::string pattern;
            iss >> pattern;
            auto validatePattern = [](std::string pattern)->bool{

                if (pattern.size() != 5) return false;
                for (char c : pattern) {
                    if (c != '+' && c != '-') { return false; }
                }
                return true;
            };
            if (!validatePattern(pattern)) {
                std::cout<<"Pattern must be length 5, and only composed of '+' (led on) and '-' (led off) characters.\n";
                continue;
            }
			std::cout << "accepting pattern " << pattern << std::endl;
			napi->acceptPattern(pattern);
		}
		else if (command == "provision-start"){
			napi->startProvisioning(onPattern, onProvision);
		}
		else if (command == "provision-stop"){
			napi->stopProvisioning();
		}
		else if (command == "provision-getall"){
			napi->getProvisions(displayProv, NymiApi::ProvisionListType::ALL);
		}
		else if (command == "provision-gethere"){
			napi->getProvisions(displayProv, NymiApi::ProvisionListType::PRESENT);
		}
		else if (command == "exit"){
			if (napi)
				delete napi;
			exit(0);
		}
		else if (command == "get-random"){
            int bandIndex = -1;
            if (validateBandIndex(bandIndex,iss)){
                bands.at(bandIndex).getRandom(onRandom);
			}
		}
		else if (command == "create-sk"){
            int bandIndex = -1;
            if (validateBandIndex(bandIndex,iss)){
                bool guarded;
                if (iss >> guarded){
                    bands.at(bandIndex).createSymmetricKey(guarded,onKeyCreated);
                }
                else {
                    std::cout<<"Command format: `create-sk i g`, g is 0 or 1, for guarded. i is Nymi Band index.\n guarded means that user confirmation in the form of a double tap on the Nymi Band would be required when an NEA attempts to retrieve this key later on.\n";
                }
            }
		}
        else if (command == "get-sk"){
            int bandIndex = -1;
            if (validateBandIndex(bandIndex,iss)){
                bands.at(bandIndex).getSymmetricKey(onSk);
            }
        }
		else if (command == "get-signature"){
            int bandIndex = -1;
            if (validateBandIndex(bandIndex,iss)){
                bands.at(bandIndex).signMessage("7a1a5ee210a9dd4fc0a12319c394862f7caa7fe513bbbc8d22032f8e87e6c307",onSign);
            }
		}
		else if (command == "create-totp"){
            int bandIndex = -1;
            if (validateBandIndex(bandIndex,iss)){
                bool guarded;
                if (iss >> guarded){
                    std::string totpkeystr = "48656c6c6f21deadbeef";    //hard coded in sample app, should be user provided.
                    bands.at(bandIndex).createTotpKey(totpkeystr, guarded, onKeyCreated);
                }
                else {
                    std::cout<<"Command format: `create-totp i g`, g is 0 or 1, for guarded. i is Nymi Band index.\nGuarded means that user confirmation in the form of a double tap on the Nymi Band would be required when an NEA attempts to retrieve this key later on.\n";
                }
            }
		}
		else if (command == "get-totp"){
            int bandIndex = -1;
            if (validateBandIndex(bandIndex,iss)){
                bands.at(bandIndex).getTotpKey(onTotp);
            }
		}
		else if (command == "buzz"){
            int bandIndex = -1;
            if (validateBandIndex(bandIndex,iss)){
                bool buzzStatus;
                if (iss >> buzzStatus){
                    HapticNotification notval = buzzStatus ? HapticNotification::NOTIFY_POSITIVE : HapticNotification::NOTIFY_NEGATIVE;
                    bands.at(bandIndex).sendNotification(notval, onNotified);
                }
                else {
                    std::cout<<"Command format: `buzz i s`, s is 0 or 1, for status. i is Nymi Band index.\nTo communicate a positive status with Nymi Band i, use `buzz i 1`.\nTo communicate a negative status with Nymi Band i, use `buzz i 0`.\n";
                }
            }
		}
        else if (command == "info"){
            int bandIndex = -1;
            if (validateBandIndex(bandIndex,iss)){
                bands.at(bandIndex).getDeviceInfo(onDeviceInfo);
            }
        }
        else if (command == "get-noti-status"){
            napi->getApiNotificationState(onNotificationsGet);
        }
        else if (command == "onfoundchange-stop"){
            napi->disableOnFoundChange();
        }
        else if (command == "onpresencechange-stop"){
            napi->disableOnPresenceChange();
        }
        else if (command == "onfoundchange-start"){
            napi->setOnFoundChange(onFoundChange);
        }
        else if (command == "onpresencechange-start"){
            napi->setOnPresenceChange(onPresenceChange);
        }
        else if (command == "provision-revoke"){
            int bandIndex = -1;
            if (validateBandIndex(bandIndex,iss)){
                bands.at(bandIndex).revokeProvision(true,onProvisionRevoked);
            }
        }
        else if (command == "delete-sk"){
            int bandIndex = -1;
            if (validateBandIndex(bandIndex,iss)){
                bands.at(bandIndex).revokeKey(KeyType::SYMMETRIC, onRevokedKey);
            }
        }
        else if (command == "delete-totp"){
            int bandIndex = -1;
            if (validateBandIndex(bandIndex,iss)){
                bands.at(bandIndex).revokeKey(KeyType::TOTP, onRevokedKey);
            }
        }
        else if (command == "help"){
            printHelp();
        }
        else {
            std::cout<<"Command not found. Enter `help` to see list of supported commands.\n";
        }
	}
    return 0;
}
