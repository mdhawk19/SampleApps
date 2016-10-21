package com.nymi.api.sample;

import java.util.Scanner;

import com.nymi.api.LibNapi;
import com.nymi.api.wrapper.NymiApi;

public class SampleMain {

	

	
	public static void main(String[] args) {

		Util util = new Util();
		
	    try {
	        LibNapi.ConfigOutcome initResult;
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
	        if (util.napi.init(util.callbacks,".", LibNapi.LogLevel.normal, nPort, "127.0.0.1") != LibNapi.ConfigOutcome.okay) {
	            System.out.println("NymiApi initialization failed");
	            return;
	        }

	        System.out.println("-*-*-> NymiApi initialization succeeded. Enter `help` for list of supported commands. <-*-*-\n");
	        util.napi.startProvisioning();

	    }
	    catch (Exception e){
	        System.out.println("NymiApi initialization failed: " + e.getMessage());
	        return;
	    }
	    

		//console interface, wait for user command
	    String user_input;
	    Scanner reader = new Scanner(System.in);
	    while (user_input = reader.next()) {

	        String[] fullcommand = user_input.split("[ ]+");
	        String command = fullcommand[0];

			if (command == "accept") {
	            String pattern;
	            iss >> pattern;
	            auto validatePattern = [](String pattern)->Boolean{

	                if (pattern.size() != 5) return false;
	                for (char c : pattern) {
	                    if (c != '+' && c != '-') { return false; }
	                }
	                return true;
	            };
	            if (!validatePattern(pattern)) {
	                System.out.println("Pattern must be length 5, and only composed of '+' (led on) and '-' (led off) characters.\n";
	                continue;
	            }
				System.out.println(  +  "accepting pattern "  +  pattern  +  std::endl;
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
	                Boolean guarded;
	                if (iss >> guarded){
	                    bands.at(bandIndex).createSymmetricKey(guarded,onKeyCreated);
	                }
	                else {
	                    System.out.println("Command format: `create-sk i g`, g is 0 or 1, for guarded. i is Nymi Band index.\n guarded means that user confirmation in the form of a double tap on the Nymi Band would be required when an NEA attempts to retrieve this key later on.\n";
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
	                Boolean guarded;
	                if (iss >> guarded){
	                    String totpkeystr = "48656c6c6f21deadbeef";    //hard coded in sample app, should be user provided.
	                    bands.at(bandIndex).createTotpKey(totpkeystr, guarded, onKeyCreated);
	                }
	                else {
	                    System.out.println("Command format: `create-totp i g`, g is 0 or 1, for guarded. i is Nymi Band index.\nGuarded means that user confirmation in the form of a double tap on the Nymi Band would be required when an NEA attempts to retrieve this key later on.\n";
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
	                Boolean buzzStatus;
	                if (iss >> buzzStatus){
	                    HapticNotification notval = buzzStatus ? HapticNotification::NOTIFY_POSITIVE : HapticNotification::NOTIFY_NEGATIVE;
	                    bands.at(bandIndex).sendNotification(notval, onNotified);
	                }
	                else {
	                    System.out.println("Command format: `buzz i s`, s is 0 or 1, for status. i is Nymi Band index.\nTo communicate a positive status with Nymi Band i, use `buzz i 1`.\nTo communicate a negative status with Nymi Band i, use `buzz i 0`.\n";
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
	            System.out.println("Command not found. Enter `help` to see list of supported commands.\n";
	        }
		}
	    return 0;	
	
	}

}
