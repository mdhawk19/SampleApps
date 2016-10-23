package com.nymi.api.wrapper;

import org.json.JSONObject;
import org.json.JSONArray;
import org.json.JSONException;

import java.util.HashMap;
import java.util.concurrent.atomic.AtomicBoolean;

import com.nymi.api.LibNapi;
import com.nymi.api.NapiReturn;

public class Listener implements Runnable {

	private AtomicBoolean quit;
	private NapiReturnObject returnObject;

	@SuppressWarnings("unused")
	private class JsonOutcome {
		public String message;
		public LibNapi.JsonGetOutcome outcome;
	}; 
	
	public Listener (NapiReturnObject retobj)
	{
		returnObject = retobj;
	}
	
    public void run() {
        
        while (!quit.get()) {
            //this is a blocking call. Returns only if napi has sent a message, or quit is set to true
            String message;
            NapiReturn result = LibNapi.INSTANCE.jsonNapiGetD(50);
            quit.set(result.quit);
            message = result.message;

            if (result.outcome == LibNapi.JsonGetOutcome.okay) {
                System.out.printf("received message: " + message + "/n");
                
                try {
                	JSONObject jobj = new JSONObject(message);
                     
	                //handle any passed errors
                	if (jobj.has("errors") || !jobj.getBoolean("successful")) {
	                    handleNapiError(jobj);
	                    continue;
	                }

	                //delegate to proper op handler
	                if (jobj.has("operation")) {
	                    String operation = jobj.getString("operation");
	                    
						switch (operation) {
/*
							case "provision":
								//handleOpProvision(jobj);
								break;
							case "info":
								//handleOpInfo(jobj);
								break;
							case "random":
								//handleOpRandom(jobj);
								break;
							case "symmetricKey":
								//handleOpSymmetric(jobj);
								break;
							case "sign":
								//handleOpSignature(jobj);
								break;
							case "totp":
								//handleOpTotp(jobj);
								break;
							case "buzz":
								//handleOpNotified(jobj);
								break;
							case "notifications":
								//handleOpApiNotifications(jobj);
								break;
							case "revoke":
								//handleOpRevokeProvision(jobj);
								break;
							case "key":
								//handleOpKey(jobj);
								break;
*/
							default:
								logMessage(jobj);
								break;
						}
	                }
                } catch (JSONException e){
                	// TODO: Exception handling and logging
                }
            }
        }
    }

    void logMessage(JSONObject jobj) {
    	System.out.println(jobj.toString());
    }
    
    void handleNapiError(JSONObject jobj) {

    	NapiError nErr = new NapiError();
        nErr.errorString = "ERROR.";
        //error message specifies the operation
        nErr.errorString += " Operation: " + jobj.getString("path");
        
        //extract the array of errors
        if (jobj.has("errors")) {
            JSONArray errors = jobj.getJSONArray("errors");
            HashMap<String,String> errorsMap = new HashMap<String,String>();
            nErr.errorString += ", Error message(s):";
            for(int i=0; i < errors.length(); i++){
            	String errType = errors.getJSONObject(i).getString("type");
                String errMsg = errors.getJSONObject(i).getString("message");
                errorsMap.put(errMsg, errType);
                nErr.errorString += "{" + errType + ":" + "'" +errMsg + "'} ";
            }
        }
    }
        
        //some utility functions
        //----------------------
        String getExchange(JSONObject jobj, Boolean errorIfNoExchange){
            
            if (!jobj.has("exchange")) {
                if (errorIfNoExchange) {
                    String errMsg = "Could not find JSON field \"exchange\" in the JSON obj:\n";
                    errMsg += jobj.toString();
                    NapiError err = new NapiError();
                    err.errorString = errMsg;
                    returnObject.onError(err);
                }
                return "";
            }
            
            return jobj.getString("exchange");
            
        }
        
        String getPid(JSONObject jobj, String pid){
            
        	if (jobj.has("request")) {
        		JSONObject request = jobj.getJSONObject("request");
        		if (request.has("pid"))
        				return request.getString("pid");
            }
            return "";
        }
      
        
        NapiError genMissingJsonKeyErr(String key, JSONObject jobj){
            
            String errMsg = "Could not find JSON field \"" + key + "\" in the JSON obj:\n";
            errMsg += jobj.toString();
            NapiError nErr = new NapiError();
            nErr.errorString = errMsg;
            
            return nErr;
        }
}
