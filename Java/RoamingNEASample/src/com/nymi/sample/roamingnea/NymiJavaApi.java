package com.nymi.sample.roamingnea;

import org.json.JSONObject;

// Singleton manager for NAPI object

public class NymiJavaApi {

	public enum HapticNotification {
		ERROR(-1),
		NOTIFY_NEGATIVE(0),
		NOTIFY_POSITIVE(1);
		private final int value;
		private HapticNotification(int v) { value = v; }
		public int getValue() { return value; }
	};

	public enum FoundStatus {
		ERROR,
		ANONYMOUS,
		AUTHENTICATED,
		IDENTIFIED,
		PROVISIONING,
		UNCLASPED,
		UNDETECTED,
		UNPROVISIONABLE,
		DISCOVERED
	};
	
	public enum PresenceStatus {
		ERROR,
		DEVICE_PRESENCE_NO,
		DEVICE_PRESENCE_UNLIKELY,
		DEVICE_PRESENCE_LIKELY,
		DEVICE_PRESENCE_YES
	};
	
	public enum ProximityState {
		ERROR,
		PROXIMITY_STATE_NOT_READY,
		PROXIMITY_STATE_UNDETECTABLE, 
		PROXIMITY_STATE_DETECTABLE, 
		PROXIMITY_STATE_SPHERE1,
		PROXIMITY_STATE_SPHERE2, 
		PROXIMITY_STATE_SPHERE3, 
		PROXIMITY_STATE_SPHERE4
	};
	
	public enum KeyType {
		ERROR,
		SYMMETRIC,
		TOTP
	};
	
	
	public enum ProvisionListType {
		ALL,
		PRESENT
	};
	
	private static NymiJavaApi instance = null;
	
	
	public static NymiJavaApi getInstance() {
		if(instance == null) {
			instance = new NymiJavaApi();
		}
		return instance;
	}
	
	private NativeLibWrapper napiInstance = NativeLibWrapper.INSTANCE;
	
    public int init(String rootDirectory, int log, int nymulatorPort, String nymulatorHost)
    {
    	System.out.println("Initializing NAPI");
    	int initResult = napiInstance.jsonNapiConfigureD(rootDirectory, log, nymulatorPort, nymulatorHost);
    	return initResult;
    }
    
    public void terminate() {
    	napiInstance.jsonNapiFinishD();
    	napiInstance.jsonNapiTerminateD();
    }
	
	public void getInfo() {
		napiInstance.jsonNapiPutD("{\"path\":\"info/get\",\"exchange\":\"provisions\"}");
	}
	
	public void setup(String pid, String pubkey, String exchange) {
		JSONObject json = new JSONObject();
		json.put("path","roaming-auth-setup/run");
		json.put("exchange",exchange);
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("pid", pid);
		jsonrequest.put("partnerPublicKey", pubkey);
		json.put ("request",jsonrequest);
		napiInstance.jsonNapiPutD (json.toString());
	}
	
	public void startRun(Integer tid) {
		JSONObject json = new JSONObject();
		json.put("path","roaming-auth/run");
		json.put("exchange", "*roamingrun*");
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("tid", tid);
		json.put ("request",jsonrequest);
		napiInstance.jsonNapiPutD (json.toString());
	}
	
	public void completeRun(String serverNonce, String pubkey, String serverSignature) {
		JSONObject json = new JSONObject();
		json.put("path","roaming-auth-sig/run");
		json.put("exchange", "*roamingrun*");
		JSONObject jsonrequest = new JSONObject(); // sub-object with request data
		jsonrequest.put("serverNonce", serverNonce);
		jsonrequest.put("partnerPublicKey", pubkey);
		jsonrequest.put("serverSignature", serverSignature);
		json.put ("request",jsonrequest);
		napiInstance.jsonNapiPutD (json.toString());
	}
    
}
