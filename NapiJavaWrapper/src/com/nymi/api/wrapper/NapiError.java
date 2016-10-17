package com.nymi.api.wrapper;

import java.util.AbstractMap;
import java.util.HashMap;
import java.util.List;

public class NapiError {
    
    public String errorString;
    public HashMap<String,String> errorList;
    
    public NapiError() {
    	errorString = new String("");
    	errorList = new HashMap<String,String>();
    }
};
