package com.nymi.api.wrapper;

// Singleton manager for NAPI object
import com.nymi.api.LibNapi;

import java.util.AbstractMap;
import java.util.List;


public class NymiApi {

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
	
	
	private static NymiApi instance = null;
	private Listener listener = null;
	
	
	public static NymiApi getInstance() {
		if(instance == null) {
			instance = new NymiApi();
		}
		return instance;
	}
	
    public LibNapi.ConfigOutcome init(NapiReturnObject retobj, String rootDirectory, LibNapi.LogLevel log, int nymulatorPort, String nymulatorHost)
    {
    	LibNapi.ConfigOutcome initResult = LibNapi.INSTANCE.jsonNapiConfigureD(rootDirectory, log, nymulatorPort, nymulatorHost);

    	if (initResult == LibNapi.ConfigOutcome.okay)
    	{
    		listener = new Listener(retobj);
    	}
    	
    	return initResult;
    }
	
	void startProvisioning()
	{
		LibNapi.INSTANCE.jsonNapiPutD(GenJson.start_prov());
	}
	
	void acceptPattern(String pattern)
	{
		LibNapi.INSTANCE.jsonNapiPutD(GenJson.accept_pattern(pattern));		
	}
	
	void stopProvisioning()
	{
		LibNapi.INSTANCE.jsonNapiPutD(GenJson.stop_prov());
	}
	
	void getProvisions(ProvisionListType type)
	{
	    String exchange = (type == ProvisionListType.ALL) ? "provisions" : "provisionsPresent";
	    LibNapi.INSTANCE.jsonNapiPutD(GenJson.get_info(exchange));
	}
	
    void enableOnFoundChange()
    {
		LibNapi.INSTANCE.jsonNapiPutD(GenJson.enable_notification(true,"onFoundChange"));
    }
    
    void enableOnPresenceChange()
    {
		LibNapi.INSTANCE.jsonNapiPutD(GenJson.enable_notification(true,"onPresenceChange"));
    	
    }

    void disableOnFoundChange()
    {
		LibNapi.INSTANCE.jsonNapiPutD(GenJson.enable_notification(false,"onFoundChange"));
    }
    
    void disableOnPresenceChange()
    {
		LibNapi.INSTANCE.jsonNapiPutD(GenJson.enable_notification(false,"onPresenceChange"));
    	
    }
    
    void getApiNotificationState()
    {
    	if (listener != null) {
			LibNapi.INSTANCE.jsonNapiPutD(GenJson.get_state_notifications());
    	}
    }
}
