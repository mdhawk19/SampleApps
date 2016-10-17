package com.nymi.api;

import com.sun.jna.Library;
import com.sun.jna.Native;

public interface LibNapi extends Library {
	public enum LogLevel{
		normal,
		info,
		debug,
		verbose
	}
	
	public enum ConfigOutcome{
		okay,
		failedToInit,
		configurationFileNotFound,
		configurationFileNotReadable,
		configurationFileNotParsed
	}

	public enum JsonGetOutcome{
		okay,
		napiNotRunning,
		timedout,
		quitSignaled,
		napiFinished
	}

	public enum JsonPutOutcome{
		okay,
		napiNotRunning
	}
	
	LibNapi INSTANCE = (LibNapi) Native.loadLibrary("NapiDyn", LibNapi.class);
	
	ConfigOutcome jsonNapiConfigureD(String rootDirectory, LogLevel logLevel, int port, String host);
	JsonPutOutcome jsonNapiPutD(String json_in);
	NapiReturn jsonNapiGetD();
	NapiReturn jsonNapiGetD(long timeout, long sleep);
	NapiReturn jsonNapiGetD(long sleep);
	void jsonNapiFinishD();
	void jsonNapiTerminateD();

}
