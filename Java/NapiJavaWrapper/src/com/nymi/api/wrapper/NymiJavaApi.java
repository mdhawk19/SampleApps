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
	
	private LibNapi napiInstance = LibNapi.INSTANCE;
	
    public int init(NapiCallbacks cbobj, String neaName, String rootDirectory, String provisions, int log, int port, String host)
    {
    	System.out.println("Initializing NAPI");
    	
    	int initResult = napiInstance.napiConfigure(neaName,rootDirectory, provisions, log, port, host);
    	
    	if (initResult == 0) // 0 is LibNapi.ConfigOutcome.okay
    	{
    		listener = new Listener(cbobj);
    		listener.start();
    	}
    	return initResult;
    }
    
    public void terminate() {
    	napiInstance.napiTerminate();
    }
	
    public void startProvisioning()
	{
    	System.out.println("Starting provision mode");
    	napiInstance.napiPut(GenJson.start_prov());
	}
	
    public void acceptPattern(String pattern)
	{
    	napiInstance.napiPut(GenJson.accept_pattern(pattern));		
	}
	
    public void stopProvisioning()
	{
		napiInstance.napiPut(GenJson.stop_prov());
	}
	
    public void getProvisions(ProvisionListType type)
	{
	    String exchange = (type == ProvisionListType.ALL) ? "provisions" : "provisionsPresent";
	    napiInstance.napiPut(GenJson.get_info(exchange));
	}
	
    public void enableOnFoundChange()
    {
		napiInstance.napiPut(GenJson.enable_notification(true,"onFoundChange"));
    }
    
    public void enableOnPresenceChange()
    {
		napiInstance.napiPut(GenJson.enable_notification(true,"onPresenceChange"));
    	
    }

    public void disableOnFoundChange()
    {
		napiInstance.napiPut(GenJson.enable_notification(false,"onFoundChange"));
    }
    
    public void disableOnPresenceChange()
    {
		napiInstance.napiPut(GenJson.enable_notification(false,"onPresenceChange"));
    	
    }
    
    public void getApiNotificationState()
    {
    	if (listener != null) {
			napiInstance.napiPut(GenJson.get_state_notifications());
    	}
    }
}
