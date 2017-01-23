package com.nymi.sample.roaming;


import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

import org.json.JSONObject;

@Path("/sign")
public class SigningService {

	@GET
    @Produces(MediaType.APPLICATION_JSON)
	public String handleGet() {
		return makeError("This service does not accept GET requests");
	}
	
	@POST
    @Consumes(MediaType.APPLICATION_FORM_URLENCODED)
    @Produces(MediaType.APPLICATION_JSON)
    public String handleIncoming(String incoming) {
		System.err.println("******************************************");
		System.err.println("  Starting nonce signing service");
		System.err.println("******************************************");

		JSONObject jrequest = new JSONObject(incoming);
    	if (jrequest.has("request")) {
    		JSONObject jsonRequest = jrequest.getJSONObject("request");
    		if (!jsonRequest.has("nymibandNonce"))
    			return makeError("Missing nymibandNonce field");
    		if (!jsonRequest.has("exchange"))
    			return makeError("Missing exchange field");
    		String bandNonce = jsonRequest.getString("nymibandNonce");
    		String exchange = jsonRequest.getString("exchange");
    		
    		Signer signer = new Signer();
        	JSONObject response = new JSONObject();
        	response.put("response", signer.sign(bandNonce, exchange));
        	response.put("request", jsonRequest);

        	return response.toString();
    	}
    	else
    		return makeError("Missing request field");
    }
    
	
    String makeError(String errMsg) {
    	String err = "{\"response\":\"error: general error: " + errMsg + "\",\"successful\":false}";
    	return err;
    }
}
