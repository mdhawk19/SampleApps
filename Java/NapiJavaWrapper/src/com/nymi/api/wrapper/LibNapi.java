package com.nymi.api.wrapper;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Pointer;

public interface LibNapi extends Library {
	
	LibNapi INSTANCE = (LibNapi) Native.loadLibrary(Platform.isWindows() ? "napi.dll" : "napi", LibNapi.class);
	
	int napiConfigure(String neaName, String rootDirectory, String provisions, int logLevel, int port, String host);
	int napiPut(String json_in);
	int napiGet(Pointer buffer, int max, Pointer len);  // blocking - in the C API buffer is a char* and len is an int*
	int napiTryGet(Pointer buffer, int max, Pointer len); // non-blocking - in the C API buffer is a char* and len is an int*
	void napiTerminate();
}
