package com.nymi.api.sample;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map.Entry;

import com.nymi.api.wrapper.NapiCallbacks;
import com.nymi.api.wrapper.NapiError;
import com.nymi.api.wrapper.NymiJavaApi;
import com.nymi.api.wrapper.NymiJavaApi.FoundStatus;
import com.nymi.api.wrapper.NymiJavaApi.HapticNotification;
import com.nymi.api.wrapper.NymiJavaApi.KeyType;
import com.nymi.api.wrapper.NymiJavaApi.PresenceStatus;
import com.nymi.api.wrapper.NymiProvision;
import com.nymi.api.wrapper.TransientNymiBandInfo;

public class SampleAppCallbacks implements NapiCallbacks {

	private NymiJavaApi napi = NymiJavaApi.getInstance();
	private List<NymiProvision> bands = new ArrayList<>();
	
	public List<NymiProvision> getBands() { return bands; }

	@Override
	public void onDeviceInfo(boolean opResult, String pid, TransientNymiBandInfo transientInfo, NapiError napiError) {
        if (!opResult) {
            System.out.println("Received error " + napiError.errorString + " for band with pid: " + pid);
            return;
        }

        System.out.println("Info for Nymi band with provision id: " + transientInfo.getPid());
        System.out.println("  Authentication status: " + transientInfo.getFoundState());
        System.out.println("  Presence status: " + transientInfo.getPresenceState());
        System.out.println("  Last RSSI: " + transientInfo.getRssiLast());
        System.out.println("  Smoothed RSSI: " + transientInfo.getRssiSmoothed());
        System.out.println("  Firmware version: " + transientInfo.getFirmwareVersion());
        System.out.println("  Provisioned: " + transientInfo.isProvisioned());
        System.out.println("  Time since last contact: " + transientInfo.getSinceLastContact());
        System.out.println("  Authentication window remaining: " + transientInfo.getAuthenticationWindowRemaining());
        System.out.println("  Number of commands queued: " + transientInfo.getNumCommandsQueued());
        List<String> commands = transientInfo.getCommandsQueued();
        if (!commands.isEmpty()) {
            System.out.println("  Commands queued: ");
            for (String cmd : commands){ System.out.println("    " + cmd); }
        }
	}

	@Override
	public void onEcdsaSign(boolean opResult, String pid, String signature, String verificationKey, NapiError napiError) {
        if (!opResult) {
            System.out.println("Received error " + napiError.errorString + " for band with pid: " + pid);
            return;
        }
        System.out.println("Received signature: "  +  signature  + ", with verification key: " + verificationKey  + " for band with pid: " + pid);
	}

	@Override
	public void onAgreement(List<String> patterns) {
        System.out.println("Pattern(s) discovered: ");
        for (String p : patterns) {
            System.out.print(p  +  " ");
        }
        System.out.println("");
        if (!patterns.isEmpty()){
            System.out.println("\nConfirm the pattern on your Nymi Band by the accept command. example: enter `accept " + patterns.get(0));
        }
        System.out.println(" ");
	}

	@Override
	public void onProvision (NymiProvision newProvision) {
        System.out.println("Successfully provisioned with pid: " + newProvision.getPid());
        napi.stopProvisioning();
		bands.add(newProvision);
	}
	
	@Override
	public void onError(NapiError napiError) {
		System.out.println("Got unknown or unhandled ERROR: " + napiError.errorString);
	}

	@Override
	public void onGetProvisions(List<NymiProvision> provisions) {
        System.out.println("Provisions:");
        for (NymiProvision p : provisions) {
            System.out.println(p.getPid() );
			bands.add(p);
        }
	}

	@Override
	public void onKeyCreation(boolean opResult, String pid, KeyType keyType, NapiError napiError) {
        if (!opResult) {
            System.out.println("Received error in key creation " + napiError.errorString + " for band with pid: " + pid);
            return;
        }
        System.out.println("Created key type " + keyType.toString() + " for band with pid: " + pid);
	}

	@Override
	public void onKeyRevocation(boolean opResult, String pid, KeyType keyType, NapiError napiError) {
        if (!opResult) {
            System.out.println("Received error in key revocation " + napiError.errorString + " for band with pid: " + pid);
            return;
        }
        System.out.println(keyType.toString() + " revoked on Nymi Band with pid " + pid);
	}

	@Override
	public void onNewProvision(NymiProvision newProvision) {
        System.out.println("Successfully provisioned with pid: "  +  newProvision.getPid());
        napi.stopProvisioning();
		bands.add(newProvision);
	}

	@Override
	public void onNotification(boolean opResult, String pid, HapticNotification type, NapiError napiError) {
		if (!opResult) {
	        System.out.println("Received error " + napiError.errorString + " for band with pid: " + pid);
	        return;
	    }
	    System.out.println( "Notification result: "  +  opResult  +  ", Notification type: "  +  type.toString() + " for band with pid: " + pid);
	}

	@Override
	public void onNotificationsGetState(HashMap<String, Boolean> notificationsState) {
        System.out.println("Notifications state: ");
        for (Entry<String,Boolean> entry : notificationsState.entrySet()){
            System.out.println(entry.getKey() + " : " + entry.getValue());
        }
	}

	@Override
	public void onNymiBandFoundStatusChange(String pid, FoundStatus before, FoundStatus after) {
        System.out.println("onFoundChange, pid: " + pid + ", before: "  + before  +  ", after: "  + after );
	}

	@Override
	public void onNymiBandPresenceChange(String pid, PresenceStatus before, PresenceStatus after, boolean authenticated) {
        System.out.println("onPresenceChange, pid: " + pid + ", before: "  +  before.toString()  +  ", after: "  +  after.toString()  + ", authenticated: " + authenticated);
	}

	@Override
	public void onProvisionRevoked(boolean opResult, String pid, NapiError napiError) {
		if (napiError.errorList.size() != 0)
			System.out.println("ERROR revoking provision on Nymi Band with pid "  +  pid  +  ": "  +  napiError.errorString );
		else
	        System.out.println("Provision revoked on Nymi Band with pid "  +  pid );
	}

	@Override
	public void onRandom(boolean opResult, String pid,String random, NapiError napiError) {
        if (!opResult) {
            System.out.println("Received error " + napiError.errorString + " for band with pid: " + pid);
            return;
        }
		System.out.println("Received pseudo random number: "  +  random  + " for band with pid: " + pid);
	}

	@Override
	public void onStartStopProvisioning(String newState) {
        System.out.println("Provisioning mode is now " + newState);
	}

	@Override
	public void onSymmetricKey(boolean opResult, String pid,String symmetricKey, NapiError napiError) {
        if (!opResult) {
            System.out.println("Received error " + napiError.errorString + " for band with pid: " + pid);
            return;
        }
		System.out.println("Received symmetric key: "  +  symmetricKey  + " for band with pid: " + pid);
	}

	@Override
	public void onTotpGet(boolean opResult, String pid,String totp, NapiError napiError) {
        if (!opResult) {
            System.out.println("Received error " + napiError.errorString + " for band with pid: " + pid);
            return;
        }
		System.out.println("Received totp key: "  +  totp  + " for band with pid: " + pid);
	}

	@Override
	public void onProvisionModeChange(String provisionState) {
		System.out.println("Provisioning mode is now " + provisionState);
	}
	
	@Override
	public void getProvisionList(List<NymiProvision> provisionList) {
	       System.out.println("Provisions:");
	        for (NymiProvision p : provisionList) {
	            System.out.println(p.getPid());
				bands.add(p);
	        }
	}
}
