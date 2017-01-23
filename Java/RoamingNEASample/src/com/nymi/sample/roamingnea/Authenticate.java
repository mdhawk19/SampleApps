package com.nymi.sample.roamingnea;

import java.security.SecureRandom;

import javax.xml.bind.DatatypeConverter;

import org.json.JSONArray;
import org.json.JSONObject;

public class Authenticate {

	NymiJavaApi _napi = null;
	ServiceConnector _service = null;
	
	public Authenticate (NymiJavaApi napi, ServiceConnector sc) {
		_napi = napi;
		_service = sc;
	}
	
	public String run() {
		Listener listener = new Listener();

		// Get a list of PIDs
		_napi.getInfo();
		// Wait for a response
		JSONObject jResponse = listener.listen();
		
		Integer tid = null;
 
		if (jResponse.has("response")) {
        	JSONObject resp = jResponse.getJSONObject("response");
        	System.out.println("Got response: " + resp.toString());
        	if (resp.has("tidIndex")) {
				JSONArray tids = resp.getJSONArray("tidIndex");
				System.out.println("Got tidIndex: " + tids.toString());
				if (tids.length() == 0)
					fail("Unable to find any bands to authenticate");

				for (int i=0; i < tids.length(); i++)
				{
					if (!tids.isNull(i)) {
						tid = tids.getInt(i);
						break;
					}
				}
				
        		if (tid == null)
    				fail("Unable to find any bands to authenticate");

        		_napi.startRun(tid);

        		jResponse = listener.listen();
        		if (!jResponse.getBoolean("successful"))
        			fail ("Failed to start authentication on selected Nymi Band");
        		
        		jResponse = listener.listen();
        		if (!jResponse.getBoolean("successful"))
        			fail ("Failed to start authentication on selected Nymi Band");
        		
        		JSONObject jEvent;
        		if (!jResponse.has("event"))
        			fail("Got bad response for Nymi Band nonce");
        		
        		jEvent = jResponse.getJSONObject("event");
        		String nbNonce = jEvent.getString("nymibandNonce");
 
        		// Create exchange value to uniquely identify this transaction
        		SecureRandom random = new SecureRandom();
        		byte[] bytes = new byte[16];
        		random.nextBytes(bytes);
        		String exchange = DatatypeConverter.printHexBinary(bytes) + "roamingauth";

        		JSONObject authReturn = _service.startAuth(nbNonce, exchange);
        		
        		if (authReturn == null)
        			fail ("First stage of roaming auth on roaming service failed");
        		
        		String partnerPublicKey = authReturn.getString("partnerPublicKey");
        		String serverNonce = authReturn.getString("serverNonce");
        		String serverSignature = authReturn.getString("serverSignature");
        		
        		_napi.completeRun(serverNonce, partnerPublicKey, serverSignature);

        		jResponse = listener.listen();

        		System.out.println("Got final respoonse from band to authentication request");
        		
        		String keyId = "", nbSig = "";
        		
        		if (jResponse.has("response")) {
                	resp = jResponse.getJSONObject("response");
                	System.out.println("Got response: " + resp.toString());
        			keyId = resp.getString("raKeyId");
        			nbSig = resp.getString("nymibandSig");
        		}
            	else
            		fail("Error reading final response from band");
            		
        		
        		return _service.completeAuth(nbSig, exchange, keyId);
        	}
        	else
        		fail("Unable to find any bands to authenticate");
	    }
		
		return "";
	}
	
	void fail (String msg) {
		System.err.println(msg);
		System.exit(1);
	}

}
