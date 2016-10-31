package com.nymi.api.wrapper;

import java.util.HashMap;
import java.util.Vector;

import com.nymi.api.wrapper.NymiJavaApi;

public interface NapiCallbacks {
	
	public void onDeviceInfo(Boolean opResult, String pid, TransientNymiBandInfo transientinfo,NapiError nErr);
	public void onEcdsaSign(Boolean opResult, String pid, String sig, String vk, NapiError nErr);
	public void onAgreement (Vector<String> patterns);
	public void onProvision (NymiProvision newprov);
	public void onError (NapiError nErr);
	public void onGetProvisions (Vector<NymiProvision> provisions);
	public void onKeyCreation(Boolean opResult, String pid, NymiJavaApi.KeyType keytype,NapiError nErr);
	public void onKeyRevocation(Boolean opResult, String pid, NymiJavaApi.KeyType keytype,NapiError nErr);
	public void onNewProvision (NymiProvision newprov);
	public void onNotification(Boolean opResult, String pid, NymiJavaApi.HapticNotification notification,NapiError nErr);
	public void onNotificationsGetState (HashMap<String,Boolean> notificationsState);
	public void onNymiBandFoundStatusChange (String pid, NymiJavaApi.FoundStatus before, NymiJavaApi.FoundStatus after);
	public void onNymiBandPresenceChange (String pid, NymiJavaApi.PresenceStatus before, NymiJavaApi.PresenceStatus after, Boolean authenticated);
	public void onProvisionRevoked(Boolean opResult, String pid,NapiError nErr);
	public void onRandom(Boolean opResult, String pid, String rand, NapiError nErr);
	public void onStartStopProvisioning (String newState);
	public void onSymmetricKey(Boolean opResult, String pid, String sk, NapiError nErr);
	public void onTotpGet(Boolean opResult, String pid, String totp,NapiError nErr);
	public void onProvisionModeChange(String provState);
	public void getProvisionList(Vector<NymiProvision> provList);

}
