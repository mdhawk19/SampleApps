package com.nymi.api.wrapper;

public class NapiException extends Exception {

	private static final long serialVersionUID = -6952033194040850505L;

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
