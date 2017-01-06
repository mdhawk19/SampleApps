package com.nymi.sample.roaming;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

import org.json.JSONObject;

@Path("/provision")
public class ProvisioningService {

	@GET
    @Produces(MediaType.APPLICATION_JSON)
	public String handleGet() {
		return makeError("This service does not accept GET requests");
	}

	@GET @Path("/getpubkey")
    @Produces(MediaType.APPLICATION_JSON)
	public String startProvision() {
		System.err.println("******************************************");
		System.err.println("  Starting user/band provisioning service");
		System.err.println("******************************************");

		KeyHelper kh = new KeyHelper();
		
		String publicEncoded = kh.getEncodedPubKey();
		if (publicEncoded == "")
			return makeError ("Unable to retreive or encode system public key");
		else
			return("{\"response\":{\"partnerPublicKey\":\"" + publicEncoded + "\"},\"successful\":true}");
	}	
	
	@POST @Path("/newuser")
    @Consumes(MediaType.APPLICATION_FORM_URLENCODED)
    @Produces(MediaType.APPLICATION_JSON)
	public String newUser (String incoming) {
		System.err.println("******************************************");
		System.err.println("  Starting user/band provisioning service");
		System.err.println("******************************************");

		JSONObject jrequest = new JSONObject(incoming);
    	if (jrequest.has("request")) {
    		JSONObject jsonRequest = jrequest.getJSONObject("request");
    		if (!jsonRequest.has("nbPublicKey"))
    			return makeError("Request field \"nbPublicKey\" missing");
    		if (!jsonRequest.has("nbPublicKeyID"))
    			return makeError("Request field \"nbPublicKeyID\" missing");
    		if (!jsonRequest.has("userID"))
    			return makeError("Request field \"userID\" missing");
 
    		String nbPublicKey = jsonRequest.getString("nbPublicKey");
    		String nbPublicKeyID = jsonRequest.getString("nbPublicKeyID");
    		String userID = jsonRequest.getString("userID");
    		
    		UserModel um = new UserModel();
    		
    		if (um.getUserIDFromKeyID(nbPublicKeyID) != "")
    			return makeError("User with Key ID " + nbPublicKeyID + " already exists");
    		
    		if (!um.createNewUser(userID, nbPublicKey, nbPublicKeyID))
    			return makeError("Failed to create new user");

        	JSONObject response = new JSONObject();
        	response.put("response", "User " + userID + " successfully added");
        	response.put("successful", true);
        	response.put("request", jsonRequest);
            return response.toString();
    	}
    	else
    	{
    		return makeError("Request field missing");
    	}
	}

	@POST @Path("/deleteuser")
    @Consumes(MediaType.APPLICATION_FORM_URLENCODED)
    @Produces(MediaType.APPLICATION_JSON)
	public String deleteUser (String incoming) {
		JSONObject jrequest = new JSONObject(incoming);
    	if (jrequest.has("request")) {
    		JSONObject jsonRequest = jrequest.getJSONObject("request");
    		if (!jsonRequest.has("userID"))
    			return makeError("Request field \"userID\" missing");
 
    		String userID = jsonRequest.getString("userID");
    		
    		UserModel um = new UserModel();
    		
    		if (!um.deleteUserByUserID(userID))
    			return makeError("Failed to delete user " + userID);
    		
        	JSONObject response = new JSONObject();
        	response.put("response", "User " + userID + " successfully deleted");
        	response.put("successful", true);
        	response.put("request", jsonRequest);
            return response.toString();
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
