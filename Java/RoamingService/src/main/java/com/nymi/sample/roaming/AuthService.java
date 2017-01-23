package com.nymi.sample.roaming;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

import org.json.JSONException;
import org.json.JSONObject;

@Path("/auth")
public class AuthService {

	@GET
    @Produces(MediaType.APPLICATION_JSON)
	public String handleGet() {
		return makeError("This service does not accept GET requests");
	}

	@POST
    @Consumes(MediaType.APPLICATION_FORM_URLENCODED)
    @Produces(MediaType.APPLICATION_JSON)
	public String authUser (String incoming) {
		System.err.println("******************************************");
		System.err.println("  Starting user authentication service");
		System.err.println("******************************************");
		
		JSONObject jrequest = new JSONObject(incoming);
    	if (jrequest.has("request")) {
    		JSONObject jsonRequest = jrequest.getJSONObject("request");
    		if (!jsonRequest.has("nbPublicKeyID"))
    			return makeError("Request field \"nbPublicKeyID\" missing");
    		if (!jsonRequest.has("signedNonces"))
    			return makeError("Request field \"signedNonces\" missing");
    		if (!jsonRequest.has("exchange"))
    			return makeError("Request field \"exchange\" missing");

    		String nbPublicKeyID = "";
    		String signedNonces = "";
    		String exchange = "";
    		
    		try {
	    		nbPublicKeyID = jsonRequest.getString("nbPublicKeyID");
	    		signedNonces = jsonRequest.getString("signedNonces");
	    		exchange = jsonRequest.getString("exchange");
    		} catch (JSONException e) {
    			return makeError("Unable to parse JSON request: " + e.getLocalizedMessage());
    		}

    		UserModel um = new UserModel();
    		
    		String nbPublicKey;
    		if ((nbPublicKey = um.getUserKeyFromKeyID(nbPublicKeyID)) == "")
    			return makeError("User with Key ID " + nbPublicKeyID + " not found");
    		
    		NonceModel nm = new NonceModel();
    		String serverNonce = "";
    		if ((serverNonce = nm.getNonce(exchange)).equals(""))
				return makeError("Could not find saved nonce for exchange " + exchange);
    		
    		System.err.println("Got saved nonce " + serverNonce);
    		
    		Signer signer = new Signer();
    		
    		if (signer.verifySignature(nbPublicKey, signedNonces, serverNonce)) {
    			if (!nm.deleteNoncebyExchange(exchange))
    				System.err.println("WARNING: Unable to remove used nonce from database. It should clean up on its own shortly");
    			String userid = um.getUserIDFromKeyID(nbPublicKeyID);
    			System.err.println("User " + userid + " authenticated with Nymi Band. Handing off to authorization class.");
    			Authorize.User(userid);
            	JSONObject response = new JSONObject("{}");
            	response.put("response", "User " + userid + " successfully authenticated");
            	response.put("userid", userid);
            	response.put("successful", true);
            	response.put("request", jsonRequest);
                return response.toString();
    		}
    		else
    		{
    			if (!nm.deleteNoncebyExchange(exchange))
    				System.err.println("WARNING: Unable to remove used nonce from database. It should clean up on its own shortly");
    			return makeError ("Signature verification failed so user with keyid " + nbPublicKeyID + " is not authenticated");
    		}
    	}
    	else
    	{
    		return makeError("Request field missing");
    	}
	}
	
    String makeError(String errMsg) {
    	String err = "{\"response\":\"error: general error: " + errMsg + "\",\"successful\":false}";
    	return err;
    }
}
