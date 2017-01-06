package com.nymi.sample.roamingnea;

import java.util.HashMap;

public class NapiError {
    
    public String errorString;
    public HashMap<String,String> errorList;
    
    public NapiError() {
    	errorString = new String("");
    	errorList = new HashMap<>();
    }
    
    public NapiError(String message) {
    	errorString = message;
    	errorList = new HashMap<>();
    }
};
