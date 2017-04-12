package com.nymi.api.wrapper;

import java.security.SecureRandom;

import com.nymi.api.wrapper.NymiJavaApi.HapticNotification;

public class NymiProvision {

	private SecureRandom rng = new SecureRandom();
	private String pid = "";
	private String privateKey = "";
	NymiJavaApi napi;
	
	public NymiProvision() {
		SeedRng();
	}
	
	public NymiProvision(NymiProvision other) {
		pid = other.getPid();
		privateKey = other.getPrivateKey();
		SeedRng();
	}

	public NymiProvision(String npid, String nkey) {
		pid = npid;
		privateKey = nkey;
		SeedRng();
		}

	public String getPid() {
		return pid;
	}
	
	public String getPrivateKey() {
		return privateKey;
	}
	
	public String getProvisionString() {
		return "\"" + pid + "\":{\"pid\":\"" + pid + "\",\"provision\":\"" + privateKey + "\",\"provisioned\":true}";
	}
	
	private void SeedRng() {
		SecureRandom seedrng = new SecureRandom();
		byte[] seed = seedrng.generateSeed(5);
		rng.setSeed(seed);
	}

	public void getRandom() {
		String exchange = ((Integer)rng.nextInt(9999)).toString();;
		exchange += "random" + getPid();
	    LibNapi.INSTANCE.napiPut(GenJson.get_random(getPid(),exchange));
	}

	public void createSymmetricKey(Boolean guarded) {
		String exchange = ((Integer)rng.nextInt(9999)).toString();;
	    exchange += "createsymkey" + getPid();
	    String createsk = GenJson.create_symkey(getPid(),guarded,exchange);
	    System.out.println("sending msg: " + createsk);
	    LibNapi.INSTANCE.napiPut(createsk);
	}

	public void getSymmetricKey() {
		String exchange = ((Integer)rng.nextInt(9999)).toString();;
		exchange += "getsymkey" + getPid();
		LibNapi.INSTANCE.napiPut(GenJson.get_symkey(getPid(),exchange));
	}

	public void signMessageSetup () {
		String exchange = ((Integer)rng.nextInt(9999)).toString();;
		exchange += "signsetup" + getPid();
		LibNapi.INSTANCE.napiPut(GenJson.sign_setup(pid, "NIST256", exchange));
	}
	
	public void signMessage(String msghash) {
		String exchange = ((Integer)rng.nextInt(9999)).toString();;
		exchange += "sign" + getPid();
		LibNapi.INSTANCE.napiPut(GenJson.sign_msg(getPid(), msghash, exchange));
	}

	public void createTotpKey(String totpKey, Boolean guarded) {
		String exchange = ((Integer)rng.nextInt(9999)).toString();;
		exchange += "createTotp" + getPid();
		LibNapi.INSTANCE.napiPut(GenJson.set_totp(getPid(),totpKey,guarded,exchange));
	}

	public void getTotpKey() {
		String exchange = ((Integer)rng.nextInt(9999)).toString();;
		exchange += "getTotp" + getPid();
		LibNapi.INSTANCE.napiPut(GenJson.get_totp(getPid(), exchange));
	}

	public void sendNotification(HapticNotification notifyType) {
		String exchange = ((Integer)rng.nextInt(9999)).toString();;
		exchange += "notify" + getPid();
		LibNapi.INSTANCE.napiPut(GenJson.notify(getPid(), notifyType == NymiJavaApi.HapticNotification.NOTIFY_POSITIVE, exchange));
	}

	public void getDeviceInfo() {
		String exchange = ((Integer)rng.nextInt(9999)).toString();;
	    exchange += "deviceinfo" + getPid();
	    LibNapi.INSTANCE.napiPut(GenJson.get_info(exchange));
	}

	public void revokeKey(NymiJavaApi.KeyType keyType) {
		String exchange = ((Integer)rng.nextInt(9999)).toString();;
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
	    LibNapi.INSTANCE.napiPut(GenJson.delete_key(getPid(),keyStr,exchange));
	}

	public void revokeProvision(Boolean onlyIfAuthenticated) {
		String exchange = ((Integer)rng.nextInt(9999)).toString();;
	    exchange += "revokeprovision" + getPid();
	    LibNapi.INSTANCE.napiPut(GenJson.revoke_provision(getPid(),onlyIfAuthenticated,exchange));
	}
	
}
