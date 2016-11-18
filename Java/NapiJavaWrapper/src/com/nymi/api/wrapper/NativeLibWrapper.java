package com.nymi.api.wrapper;

import java.util.Arrays;
import java.util.List;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

public interface NativeLibWrapper extends Library {
	
	public class NapiReturnStruct extends Structure {
		public Pointer message;
		public int outcome;
		public boolean quit;

		public static class ByValue extends NapiReturnStruct implements Structure.ByValue {}

		@SuppressWarnings("rawtypes")
		@Override
		protected List getFieldOrder() {
			return Arrays.asList(new String[] { "message", "outcome", "quit" });
		}
		
		public String getMessage() { return message.getString(0); }
		public int getOutcome() { return outcome; }
		public boolean getQuit() { return quit; }
	};
	
	NativeLibWrapper INSTANCE = (NativeLibWrapper) Native.loadLibrary(Platform.isWindows() ? "NapiDll" : "napidyn", NativeLibWrapper.class);
	
	int jsonNapiConfigureD(String rootDirectory, int logLevel, int port, String host);
	int jsonNapiPutD(String json_in);
	NapiReturnStruct.ByValue jsonNapiGetD();
	NapiReturnStruct.ByValue jsonNapiGetTSD(long timeout, long sleep);
	NapiReturnStruct.ByValue jsonNapiGetSD(boolean _quit, long sleep);
	void jsonNapiFinishD();
	void jsonNapiTerminateD();

}
