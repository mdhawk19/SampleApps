package com.nymi.api.wrapper;

import java.security.SecureRandom;

import com.nymi.api.wrapper.NymiJavaApi.HapticNotification;

public class NymiProvision {

	private SecureRandom rng = new SecureRandom();
	private String pid = "";
	
	public NymiProvision() {
		SecureRandom seedRng = new SecureRandom();
		byte[] seed = seedRng.generateSeed(5);
		rng.setSeed(seed);
	}
	
	public NymiProvision(NymiProvision other) {
		this();
		pid = other.getPid();
	}

	public NymiProvision(String nymiPid) {
		this();
		pid = nymiPid;
	}

	public String getPid() {
		return pid;
	}

	public void getRandom() {
		String exchange = "" + rng.nextInt(9999);
		exchange += "random" + getPid();
	    NativeLibWrapper.INSTANCE.jsonNapiPutD(GenJson.getRandom(getPid(),exchange));
	}

	public void createSymmetricKey(boolean guarded) {
		String exchange = "" + rng.nextInt(9999);
	    exchange += "createsymkey" + getPid();
	    String createSymmetricKey = GenJson.createSymmetricKey(getPid(),guarded,exchange);
	    System.out.println("sending msg: " + createSymmetricKey);
	    NativeLibWrapper.INSTANCE.jsonNapiPutD(createSymmetricKey);
	}

	public void getSymmetricKey() {
		String exchange = "" + rng.nextInt(9999);
		exchange += "getsymkey" + getPid();
		NativeLibWrapper.INSTANCE.jsonNapiPutD(GenJson.getSymmetrickey(getPid(),exchange));
	}

	public void signMessage(String msghash) {
		String exchange = "" + rng.nextInt(9999);
		exchange += "sign" + getPid();
		NativeLibWrapper.INSTANCE.jsonNapiPutD(GenJson.signMessage(getPid(), msghash, exchange));
	}

	public void createTotpKey(String totpKey, Boolean guarded) {
		String exchange = "" + rng.nextInt(9999);
		exchange += "createTotp" + getPid();
		NativeLibWrapper.INSTANCE.jsonNapiPutD(GenJson.setTotp(getPid(),totpKey,guarded,exchange));
	}

	public void getTotpKey() {
		String exchange = "" + rng.nextInt(9999);
		exchange += "getTotp" + getPid();
		NativeLibWrapper.INSTANCE.jsonNapiPutD(GenJson.getTotp(getPid(), exchange));
	}

	public void sendNotification(HapticNotification notifyType) {
		String exchange = "" + rng.nextInt(9999);
		exchange += "notify" + getPid();
		NativeLibWrapper.INSTANCE.jsonNapiPutD(GenJson.notify(getPid(), notifyType == NymiJavaApi.HapticNotification.NOTIFY_POSITIVE, exchange));
	}

	public void getDeviceInfo() {
		String exchange = "" + rng.nextInt(9999);
	    exchange += "deviceinfo" + getPid();
	    NativeLibWrapper.INSTANCE.jsonNapiPutD(GenJson.getInfo(exchange));
	}

	public void revokeKey(NymiJavaApi.KeyType keyType) {
		String exchange = "" + rng.nextInt(9999);
	    exchange += "deviceinfo" + getPid();

	    String keyStr = "";
	    switch(keyType) {
	        case SYMMETRIC:
	        	keyStr = "symmetric"; break;
	        case TOTP:
	        	keyStr = "totp"; break;
	        default:
	        	break;
	    }
	    NativeLibWrapper.INSTANCE.jsonNapiPutD(GenJson.deleteKey(getPid(),keyStr,exchange));
	}

	public void revokeProvision(Boolean onlyIfAuthenticated) {
		String exchange = "" + rng.nextInt(9999);
	    exchange += "revokeprovision" + getPid();
	    NativeLibWrapper.INSTANCE.jsonNapiPutD(GenJson.revokeProvision(getPid(),onlyIfAuthenticated,exchange));
	}
	
}
