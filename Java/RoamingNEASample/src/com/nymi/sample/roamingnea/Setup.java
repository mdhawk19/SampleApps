package com.nymi.sample.roamingnea;

import java.security.SecureRandom;

import javax.xml.bind.DatatypeConverter;

import org.json.JSONArray;
import org.json.JSONObject;

public class Setup {
	NymiJavaApi _napi = null;
	ServiceConnector _service = null;

	public Setup (NymiJavaApi napi, ServiceConnector sc) {
		_napi = napi;
		_service = sc;
	}
	
	public Boolean run() {
		Listener listener = new Listener();

		// Get a list of PIDs
		_napi.getInfo();
		// Wait for a response
		JSONObject jResponse = listener.listen();
		
       	String pid = "";
        if (jResponse.has("response")) {
        	JSONObject resp = jResponse.getJSONObject("response");
        	System.out.println("Got response: " + resp.toString());
        	if (resp.has("provisionsPresent")) {
				JSONArray pids = resp.getJSONArray("provisionsPresent");
				System.out.println("Got provisions: " + pids.toString());
				if (pids.length() == 0) {
					System.err.println("Unable to find any active provisions to set up for roaming authentication");
					System.exit(1);
				}
				pid = pids.getString(0);
        	}
        	else
        	{
				System.err.println("Unable to find any active provisions to set up for roaming authentication");
				System.exit(1);
        	}
        }
		
		// Get public key from roaming authentication service
		String pubkey = _service.getPublicKey();
		if (pubkey == "") {
			System.err.println("Failed to get public key from roaming service");
			return false;
		}
		
		// Create exchange value to uniquely identify this transaction
		SecureRandom random = new SecureRandom();
		byte[] bytes = new byte[2];
		random.nextBytes(bytes);
		String exchange = DatatypeConverter.printHexBinary(bytes) + "roaming" + pid;
		
		_napi.setup(pid, pubkey, exchange);
		
		jResponse = listener.listen();
		if (jResponse.has("successful"))
			if (!jResponse.getBoolean("successful"))
				return false;
		
		// Setup has successfully completed, so add new user to roaming service
		// NOTE: Username is hardcoded to "testuser" here
		JSONObject resp = jResponse.getJSONObject("response");
		if (!_service.createUser("testuser", resp.getString("RAKey"), resp.getString("RAKeyId"))) {
			System.err.println("Failed to create user on roaming service");
			return false;
		}
		return true;
	}

}
