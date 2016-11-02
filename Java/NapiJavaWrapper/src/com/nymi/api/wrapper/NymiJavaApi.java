package com.nymi.api.wrapper;

// Singleton manager for NAPI object

public class NymiJavaApi {

	public enum HapticNotification {
		ERROR(-1),
		NOTIFY_NEGATIVE(0),
		NOTIFY_POSITIVE(1);
		private final int val;
		private HapticNotification(int v) { val = v; }
		public int getVal() { return val; }
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
	private Listener listener = null;
	
	
	public static NymiJavaApi getInstance() {
		if(instance == null) {
			instance = new NymiJavaApi();
		}
		return instance;
	}
	
	private NativeLibWrapper napiInstance = NativeLibWrapper.INSTANCE;
	
    public int init(NapiCallbacks cbobj, String rootDirectory, int log, int nymulatorPort, String nymulatorHost)
    {
    	System.out.println("Initializing NAPI");
    	int initResult = napiInstance.jsonNapiConfigureD(rootDirectory, log, nymulatorPort, nymulatorHost);
    	
    	if (initResult == 0) // 0 is LibNapi.ConfigOutcome.okay
    	{
    		listener = new Listener(cbobj);
    		listener.start();
    	}
    	
    	return initResult;
    }
    
    public void terminate() {
    	napiInstance.jsonNapiFinishD();
    	napiInstance.jsonNapiTerminateD();
    }
	
    public void startProvisioning()
	{
    	System.out.println("Starting provision mode");
    	napiInstance.jsonNapiPutD(GenJson.start_prov());
	}
	
    public void acceptPattern(String pattern)
	{
    	napiInstance.jsonNapiPutD(GenJson.accept_pattern(pattern));		
	}
	
    public void stopProvisioning()
	{
		napiInstance.jsonNapiPutD(GenJson.stop_prov());
	}
	
    public void getProvisions(ProvisionListType type)
	{
	    String exchange = (type == ProvisionListType.ALL) ? "provisions" : "provisionsPresent";
	    napiInstance.jsonNapiPutD(GenJson.get_info(exchange));
	}
	
    public void enableOnFoundChange()
    {
		napiInstance.jsonNapiPutD(GenJson.enable_notification(true,"onFoundChange"));
    }
    
    public void enableOnPresenceChange()
    {
		napiInstance.jsonNapiPutD(GenJson.enable_notification(true,"onPresenceChange"));
    	
    }

    public void disableOnFoundChange()
    {
		napiInstance.jsonNapiPutD(GenJson.enable_notification(false,"onFoundChange"));
    }
    
    public void disableOnPresenceChange()
    {
		napiInstance.jsonNapiPutD(GenJson.enable_notification(false,"onPresenceChange"));
    	
    }
    
    public void getApiNotificationState()
    {
    	if (listener != null) {
			napiInstance.jsonNapiPutD(GenJson.get_state_notifications());
    	}
    }
}
