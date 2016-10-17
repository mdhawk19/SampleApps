package com.nymi.api.wrapper;

import java.util.List;
import java.util.Map;

import com.nymi.api.wrapper.NymiApi;

public interface NapiReturnObject {
	
	public void onError (NapiError nErr);
	public void onEgreement (List<String> patterns);
	public void onNewProvision (NymiProvision newprov);
	public void onGetProvisions (List<NymiProvision> provisions);
	public void onNotificationsGetState (Map<String,Boolean> notificationsState);
	public void onStartStopProvisioning (String newState);
	public void onRandom(Boolean opResult, String pid, String rand, NapiError nErr);
	public void onSymmetricKey(Boolean opResult, String pid, String sk, NapiError nErr);
	public void ecdsaSign(Boolean opResult, String pid, String sig, String vk, NapiError nErr);
	public void totpGet(Boolean opResult, String pid, String totp,NapiError nErr);
	public void onNotification(Boolean opResult, String pid, NymiApi.HapticNotification notification,NapiError nErr);
	public void deviceInfo(Boolean opResult, String pid, TransientNymiBandInfo transientinfo,NapiError nErr);
	public void createdKey(Boolean opResult, String pid, NymiApi.KeyType keytype,NapiError nErr);
	public void revokedKey(Boolean opResult, String pid, NymiApi.KeyType keytype,NapiError nErr);
	public void onProvisionRevoked(Boolean opResult, String pid,NapiError nErr);
	public void onNymiBandFoundStatusChange (String pid, NymiApi.FoundStatus before, NymiApi.FoundStatus after);
	public void onNymiBandPresenceChange (String pid, NymiApi.PresenceStatus before, NymiApi.PresenceStatus after, Boolean authenticated);

}
