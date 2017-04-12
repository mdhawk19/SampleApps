package com.nymi.api.sample;

import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Vector;

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
	private Vector<NymiProvision> bands = new Vector<NymiProvision>();
	
	public Vector<NymiProvision> getBands() { return bands; }

	@Override
	public void onDeviceInfo(Boolean opResult, String pid, TransientNymiBandInfo transientinfo,NapiError nErr) {
        if (!opResult) {
            System.out.println("Received error " + nErr.errorString + " for band with pid: " + pid);
            return;
        }

        System.out.println("Info for Nymi band with provision id: " + transientinfo.getPid());
        System.out.println("  Authentication status: " + transientinfo.getFoundState());
        System.out.println("  Presence status: " + transientinfo.getPresenceState());
        System.out.println("  Last RSSI: " + transientinfo.getRssiLast());
        System.out.println("  Smoothed RSSI: " + transientinfo.getRssiSmoothed());
        System.out.println("  Firmware version: " + transientinfo.getFirmwareVersion());
        System.out.println("  Provisioned: " + transientinfo.isProvisioned().toString());
        System.out.println("  Time since last contact: " + transientinfo.getSinceLastContact());
        System.out.println("  Authentication window remaining: " + transientinfo.getAuthenticationWindowRemaining());
        System.out.println("  Number of commands queued: " + transientinfo.getNumCommandsQueued());
        Vector<String> commands;
        commands = transientinfo.getCommandsQueued();
        if (!commands.isEmpty()) {
            System.out.println("  Commands queued: ");
            for (String cmd : commands){ System.out.println("    " + cmd); }
        }
	}

	@Override
	public void onEcdsaSign(Boolean opResult, String pid, String sig, String vk, NapiError nErr) {
        if (!opResult) {
            System.out.println("Received error " + nErr.errorString + " for band with pid: " + pid);
            return;
        }
        System.out.println("Received signature: "  +  sig  + ", with verification key: " + vk  + " for band with pid: " + pid);
	}

	@Override
	public void onAgreement(Vector<String> patterns) {
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
	public void onProvision(String pid) {
        System.out.println("Successfully provisioned with pid: " + pid);
        napi.stopProvisioning();
	}
	
	@Override
	public void onProvisions(Vector<NymiProvision> provList) {
		bands = provList;
	}
	
	
	@Override
	public void onError(NapiError err) {
		System.out.println("Got unknown or unhandled ERROR: " + err.errorString);
	}

	@Override
	public void onGetProvisions(Vector<NymiProvision> provisions) {
        System.out.println("Provisions:");
        for (NymiProvision p : provisions) {
            System.out.println(p.getPid() );
			bands.add(p);
        }
	}

	@Override
	public void onKeyCreation(Boolean opResult, String pid, KeyType keyType, NapiError err) {
        if (!opResult) {
            System.out.println("Received error in key creation " + err.errorString + " for band with pid: " + pid);
            return;
        }
        System.out.println("Created key type " + keyType.toString() + " for band with pid: " + pid);
	}

	@Override
	public void onKeyRevocation(Boolean opResult, String pid, KeyType keyType, NapiError err) {
        if (!opResult) {
            System.out.println("Received error in key revocation " + err.errorString + " for band with pid: " + pid);
            return;
        }
        System.out.println(keyType.toString() + " revoked on Nymi Band with pid " + pid);
	}

	@Override
	public void onNewProvision(NymiProvision newprov) {
        System.out.println("Successfully provisioned with pid: "  +  newprov.getPid());
        napi.stopProvisioning();
		bands.add(newprov);
	}

	@Override
	public void onNotification(Boolean opResult, String pid, HapticNotification type, NapiError err) {
		if (!opResult) {
	        System.out.println("Received error " + err.errorString + " for band with pid: " + pid);
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
	public void onNymiBandPresenceChange(String pid, PresenceStatus before, PresenceStatus after, Boolean authenticated) {
        System.out.println("onPresenceChange, pid: " + pid + ", before: "  +  before.toString()  +  ", after: "  +  after.toString()  + ", authenticated: " + authenticated.toString());
	}

	@Override
	public void onProvisionRevoked(Boolean opResult, String pid, NapiError err) {
		if (err.errorList.size() != 0)
			System.out.println("ERROR revoking provision on Nymi Band with pid "  +  pid  +  ": "  +  err.errorString );
		else
	        System.out.println("Provision revoked on Nymi Band with pid "  +  pid );
	}

	@Override
	public void onRandom(Boolean opResult, String pid,String prand, NapiError err) {
        if (!opResult) {
            System.out.println("Received error " + err.errorString + " for band with pid: " + pid);
            return;
        }
		System.out.println("Received pseudo random number: "  +  prand  + " for band with pid: " + pid);
	}

	@Override
	public void onStartStopProvisioning(String newState) {
        System.out.println("Provisioning mode is now " + newState);
	}

	@Override
	public void onSymmetricKey(Boolean opResult, String pid,String sk, NapiError err) {
        if (!opResult) {
            System.out.println("Received error " + err.errorString + " for band with pid: " + pid);
            return;
        }
		System.out.println("Received symmetric key: "  +  sk  + " for band with pid: " + pid);
	}

	@Override
	public void onTotpGet(Boolean opResult, String pid,String totp, NapiError err) {
        if (!opResult) {
            System.out.println("Received error " + err.errorString + " for band with pid: " + pid);
            return;
        }
		System.out.println("Received totp key: "  +  totp  + " for band with pid: " + pid);
	}

	@Override
	public void onProvisionModeChange(String provState) {
		System.out.println("Provisioning mode is now " + provState);
	}
	
	@Override
	public void getProvisionList(Vector<NymiProvision> provList) {
	       System.out.println("Provisions:");
	        for (NymiProvision p : provList) {
	            System.out.println(p.getPid());
				bands.add(p);
	        }
	}

	@Override
	public void onEcdsaSignSetup(Boolean opResult, String pid, NapiError err) {
		if (!opResult) {
            System.out.println("Received error " + err.errorString + " for band with pid: " + pid);
            return;
		}
		System.out.println("Succesfully set up signing keys for PID " + pid);
	}
}
