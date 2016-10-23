package com.nymi.api.sample;

import java.util.Arrays;
import java.util.Scanner;
import java.util.Vector;

import com.nymi.api.LibNapi;
import com.nymi.api.wrapper.NymiApi;

public class SampleMain {
	
	public static void main(String[] args) {

		Util util = new Util();
		
	    try {
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
	        if (util.getNapi().init(util.getCallbacks(),".", LibNapi.LogLevel.normal, nPort, "127.0.0.1") != LibNapi.ConfigOutcome.okay) {
	            System.out.println("NymiApi initialization failed");
	            return;
	        }

	        System.out.println("-*-*-> NymiApi initialization succeeded. Enter `help` for list of supported commands. <-*-*-\n");
	        util.getNapi().startProvisioning();

	    }
	    catch (Exception e) {
	        System.out.println("NymiApi initialization failed: " + e.getMessage());
	        return;
	    }
	    

		//console interface, wait for user command
	    String user_input;
	    Scanner reader = new Scanner(System.in);
	    
	    while (true) {

	    	user_input = reader.next();
	    	String[] rawcmd = user_input.split("[ ]+");
	        Vector<String> fullcommand = new Vector<String>(Arrays.asList(rawcmd));
	        String command = fullcommand.elementAt(0);

	        int bandIndex = -1;
            Boolean guarded = false;
            String pattern = "";

            switch (fullcommand.size()) {
            case 1:
            	break;
            case 2:
            	try {
            		bandIndex = new Integer(fullcommand.elementAt(1));
            		break;
            	} catch (Exception e) { // If it isn't an int assume it's a pattern string
            		pattern = fullcommand.elementAt(1);
            		break;
            	}
            case 3:
            	bandIndex = new Integer(fullcommand.elementAt(1));
            	guarded = (fullcommand.elementAt(1) == "1");
            default:
            	System.out.println("Improper number of arguments.  Type 'help' for a list of coammnds and their syntax.");
            	break;
            }
            
	        switch (command) {
	        case "accept":
	            if (!util.validatePattern(pattern)) {
	                System.out.println("Pattern must be 5 characters long, and composed only of '+' (led on) and '-' (led off) characters.");
	                continue;
	            }
				System.out.println("Accepting pattern "  +  pattern);
				util.getNapi().acceptPattern(pattern);
				break;
			case "provision-start":
				util.getNapi().startProvisioning();
				break;
			case "provision-stop":
				util.getNapi().stopProvisioning();
				break;
			case "provision-getall":
				util.getNapi().getProvisions(NymiApi.ProvisionListType.ALL);
				break;
			case "provision-gethere":
				util.getNapi().getProvisions(NymiApi.ProvisionListType.PRESENT);
				break;
			case "exit":
				reader.close();
				return;
			case "get-random":
	            if (util.validateBandIndex(bandIndex)) {
	                util.getBands().get(bandIndex).getRandom();
				}
				break;
			case "create-sk":
	            if (util.validateBandIndex(bandIndex))
	                util.getBands().get(bandIndex).createSymmetricKey(guarded);
                break;
	        case "get-sk":
	            if (util.validateBandIndex(bandIndex)) {
	                util.getBands().elementAt(bandIndex).getSymmetricKey();
	            }
				break;
			case "get-signature":
	            if (util.validateBandIndex(bandIndex)) {
	                util.getBands().get(bandIndex).signMessage("7a1a5ee210a9dd4fc0a12319c394862f7caa7fe513bbbc8d22032f8e87e6c307");
	            }
				break;
			case "create-totp":
	            if (util.validateBandIndex(bandIndex)) {
                    String totpkeystr = "48656c6c6f21deadbeef";    //hard coded in sample app, should be user provided.
                    util.getBands().get(bandIndex).createTotpKey(totpkeystr, guarded);
	            }
				break;
			case "get-totp":
       	            if (util.validateBandIndex(bandIndex)) {
	                util.getBands().get(bandIndex).getTotpKey();
	            }
				break;
			case "buzz":
				// We use the "guarded" value even though it doesn't mean guarded in this context
	            if (util.validateBandIndex(bandIndex)) {
	            	NymiApi.HapticNotification notval = guarded ? NymiApi.HapticNotification.NOTIFY_POSITIVE : NymiApi.HapticNotification.NOTIFY_NEGATIVE;
                    util.getBands().get(bandIndex).sendNotification(notval);
	            }
				break;
	        case "info":
	            if (util.validateBandIndex(bandIndex)) {
	                util.getBands().get(bandIndex).getDeviceInfo();
	            }
				break;
	        case "get-noti-status":
	            util.getNapi().getApiNotificationState();
				break;
	        case "onfoundchange-stop":
	            util.getNapi().disableOnFoundChange();
				break;
	        case "onpresencechange-stop":
	            util.getNapi().disableOnPresenceChange();
				break;
	        case "onfoundchange-start":
	            util.getNapi().enableOnFoundChange();
				break;
	        case "onpresencechange-start":
	            util.getNapi().enableOnPresenceChange();
				break;
	        case "provision-revoke":
	            if (util.validateBandIndex(bandIndex)) {
	                util.getBands().get(bandIndex).revokeProvision(false);
	            }
				break;
	        case "delete-sk":
	            if (util.validateBandIndex(bandIndex)) {
	                util.getBands().get(bandIndex).revokeKey(NymiApi.KeyType.SYMMETRIC);
	            }
				break;
	        case "delete-totp":
	            if (util.validateBandIndex(bandIndex)) {
	                util.getBands().get(bandIndex).revokeKey(NymiApi.KeyType.TOTP);
	            }
				break;
	        case "help":
	            util.printHelp();
				break;
	        default:
	            System.out.println("Command not found. Enter `help` to see list of supported commands.");
				break;
		}	
	}

	}
}