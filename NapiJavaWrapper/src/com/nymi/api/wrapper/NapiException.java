package com.nymi.api.wrapper;

public class NapiException extends Exception {

	public NapiException() {
		super();
	}
	
	public NapiException (String message) {
		super(message);
	}
	
	public NapiException (NapiError error) {
		super(error.errorString);
	}
}
