package com.nymi.api.sample;

import java.util.Vector;

import com.nymi.api.wrapper.*;

public class Util {

	private NymiJavaApi napi = NymiJavaApi.getInstance();
    private SampleAppCallbacks callbacks = new SampleAppCallbacks();
    
    public NymiJavaApi getNapi() { return napi; }
    public com.nymi.api.wrapper.NapiCallbacks getCallbacks() { return callbacks; }
    
    public Vector<NymiProvision> getBands() { return callbacks.getBands(); }
	
    public Boolean initNapi(String rootDirectory, int log, int nymulatorPort, String nymulatorHost) {
    	int retcode = napi.init(callbacks, rootDirectory, log, nymulatorPort, nymulatorHost);
    	return retcode == 0;
    }
    
    public Boolean validateBandIndex (int bandIndex) {
        if (bandIndex < 0 || bandIndex >= getBands().size()) {
            System.out.println("Incorrect band index");
            return false;
        } else
        	return true;
    };

    public Boolean validatePattern(String pattern) {
        if (pattern.length() != 5) return false;
        for (char c : pattern.toCharArray()) {
            if (c != '+' && c != '-') return false;
        }
        return true;
    };

	void printHelp(){

	    System.out.println("\nProvisioning commands:");
	    System.out.println("");
	    System.out.println("  `provision-start` enable provisioning bands.");
	    System.out.println("  `provision-stop` disable provisioning bands.");
	    System.out.println("  `accept [+|-]{5}` accept a provisioning pattern.\n     Pattern string has length 5, composed of '+' (led on) or '-' (led off).\n     e.g. `accept +----` to accept a pattern where only the first led is on.");
	    System.out.println("  `provision-getall` get all bands provisioned to this NEA.");
	    System.out.println("  `provision-gethere` get all bands provisioned to this NEA and are present.");
	    System.out.println("  `provision-revoke i` Revoke provisioning of Nymi Band i.");
	    System.out.println("\nCryptographic commands:");
	    System.out.println(" In all cmds below, i is index of the band in std::vector<NymiProvision> bands.");
	    System.out.println("");
	    System.out.println("  `get-random i` get a random number from Nymi Band i.");
	    System.out.println("  `get-signature i` get a signature (of a currently hard coded msg) from Nymi Band i.");
	    System.out.println("  `create-sk i g` create a symmetric key on Nymi Band i.\n     Guarded option: g=1 -> guarded, g=0 -> unguarded.\n     If the key is guarded, key retrieval requires user double tap.");
	    System.out.println("  `get-sk i` get the symmetric key previously created on Nymi Band i.");
	    System.out.println("  `delete-sk i` delete the symmetric key previously created on Nymi Band i.");
	    System.out.println("  `create-totp i g` register totp (with a currently hardcoded totp key) on Nymi Band i.\n     Guarded option: g=1 -> guarded, g=0 -> unguarded.\n     If the key is guarded, key retrieval requires user double tap.");
	    System.out.println("  `get-totp i` get a totp token from Nymi Band i.");
	    System.out.println("  `delete-totp i` delete the totp registered on Nymi Band i.");
	    System.out.println("\nHaptic command:");
	    System.out.println("");
	    System.out.println("  `buzz i s` Send a haptic signal for status s to Nymi Band i.\n     Status option: s=1 -> success signal, s=0 -> error signal");
	    System.out.println("\nInfo command:");
	    System.out.println("");
	    System.out.println("  `info i` recieve a transient snapshot of the state of Nymi Band i.");
	    System.out.println("\nApi notification commands:");
	    System.out.println("");
	    System.out.println("  `get-noti-status` to get status of notification streams");
	    System.out.println("  `onfoundchange-start` receive notification when there is a change in authentication status of a Nymi Band.");
	    System.out.println("  `onfoundchange-stop` don't receive onfoundchange notifications.");
	    System.out.println("  `onpresencechange-start` receive notification when there is a change in presence status of a Nymi Band.");
	    System.out.println("  `onpresencechange-stop` don't receive onpresencechange notifications.");
	}

}
