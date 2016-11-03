package com.nymi.api.wrapper;

import java.util.HashMap;
import java.util.List;

public interface NapiCallbacks {
	
	public void onDeviceInfo(boolean opResult, String pid, TransientNymiBandInfo transientInfo,NapiError napiError);
	public void onEcdsaSign(boolean opResult, String pid, String signature, String verificationKey, NapiError napiError);
	public void onAgreement (List<String> patterns);
	public void onProvision (NymiProvision newProvision);
	public void onError (NapiError napiError);
	public void onGetProvisions (List<NymiProvision> provisions);
	public void onKeyCreation(boolean opResult, String pid, NymiJavaApi.KeyType keytype,NapiError napiError);
	public void onKeyRevocation(boolean opResult, String pid, NymiJavaApi.KeyType keytype,NapiError napiError);
	public void onNewProvision (NymiProvision newProvision);
	public void onNotification(boolean opResult, String pid, NymiJavaApi.HapticNotification notification,NapiError napiError);
	public void onNotificationsGetState (HashMap<String,Boolean> notificationsState);
	public void onNymiBandFoundStatusChange (String pid, NymiJavaApi.FoundStatus before, NymiJavaApi.FoundStatus after);
	public void onNymiBandPresenceChange (String pid, NymiJavaApi.PresenceStatus before, NymiJavaApi.PresenceStatus after, boolean authenticated);
	public void onProvisionRevoked(boolean opResult, String pid,NapiError napiError);
	public void onRandom(boolean opResult, String pid, String random, NapiError napiError);
	public void onStartStopProvisioning (String newState);
	public void onSymmetricKey(boolean opResult, String pid, String symmetricKey, NapiError napiError);
	public void onTotpGet(boolean opResult, String pid, String totp,NapiError napiError);
	public void onProvisionModeChange(String provisionState);
	public void getProvisionList(List<NymiProvision> provisionList);

}
