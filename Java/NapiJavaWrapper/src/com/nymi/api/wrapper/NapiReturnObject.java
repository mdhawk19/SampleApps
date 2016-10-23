package com.nymi.api.wrapper;

import java.util.Map;
import java.util.Vector;

import com.nymi.api.wrapper.NymiApi;

public interface NapiReturnObject {
	
	public void onDeviceInfo(Boolean opResult, String pid, TransientNymiBandInfo transientinfo,NapiError nErr);
	public void onEcdsaSign(Boolean opResult, String pid, String sig, String vk, NapiError nErr);
	public void onEgreement (Vector<String> patterns);
	public void onError (NapiError nErr);
	public void onGetProvisions (Vector<NymiProvision> provisions);
	public void onKeyCreation(Boolean opResult, String pid, NymiApi.KeyType keytype,NapiError nErr);
	public void onKeyRevocation(Boolean opResult, String pid, NymiApi.KeyType keytype,NapiError nErr);
	public void onNewProvision (NymiProvision newprov);
	public void onNotification(Boolean opResult, String pid, NymiApi.HapticNotification notification,NapiError nErr);
	public void onNotificationsGetState (Map<String,Boolean> notificationsState);
	public void onNymiBandFoundStatusChange (String pid, NymiApi.FoundStatus before, NymiApi.FoundStatus after);
	public void onNymiBandPresenceChange (String pid, NymiApi.PresenceStatus before, NymiApi.PresenceStatus after, Boolean authenticated);
	public void onProvisionRevoked(Boolean opResult, String pid,NapiError nErr);
	public void onRandom(Boolean opResult, String pid, String rand, NapiError nErr);
	public void onStartStopProvisioning (String newState);
	public void onSymmetricKey(Boolean opResult, String pid, String sk, NapiError nErr);
	public void onTotpGet(Boolean opResult, String pid, String totp,NapiError nErr);

}
