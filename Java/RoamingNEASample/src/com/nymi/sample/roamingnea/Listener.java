package com.nymi.sample.roamingnea;

import org.json.JSONObject;

import org.json.JSONArray;
import org.json.JSONException;

public class Listener {
	
	private NativeLibWrapper napiInstance = NativeLibWrapper.INSTANCE;

	public JSONObject listen() {
		JSONObject jobj = null;
        while (true) {
            NativeLibWrapper.NapiReturnStruct.ByValue result = napiInstance.jsonNapiGetTSD(30000, 100);
            
            if (result.getOutcome() == 0) {    // 0 == LibNapi.JsonGetOutcome.okay
                System.out.println("received message: " + result.getMessage());
                
                try {
                	jobj = new JSONObject(result.getMessage());
                     
	                //handle any passed errors
                	if (jobj.has("errors")) {
	                    handleNapiError(jobj);
	                    continue;
	                }
                	if (jobj.has("successful"))
                			if (!jobj.getBoolean("successful")) {
        	                    handleNapiError(jobj);
        	                    continue;
        	                }
                } catch (JSONException e){
                	System.err.println("Caught exception in JSON operation handler: " + e.getMessage());
                	e.printStackTrace();
                }
            	return jobj;
            }
        }
    }

    void handleNapiError(JSONObject jobj) {

    	NapiError nErr = new NapiError();
        //error message specifies the operation
        nErr.errorString += " Operation: " + jobj.getString("path");
        
        //extract the array of errors
        if (jobj.has("errors")) {
            JSONArray errors = jobj.getJSONArray("errors");
            nErr.errorString += ", Error message(s):";
            if (errors.length() > 0)
	            for(int i=0; i < errors.length(); i++){
	            	String errType = errors.getJSONArray(i).getString(1);
	                String errMsg = errors.getJSONArray(i).getString(0);
	                nErr.errorString += "{" + errType + ":" + "'" +errMsg + "'} ";
	            }
        }
        System.err.println(nErr.errorString);
        System.exit(1);
    }
}
