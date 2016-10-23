// NapiDyn.cpp : Defines the exported functions for the DLL application.
//

#include "NapiDyn.h"


extern NAPIDYN_API int jsonNapiConfigureD(std::string rootDirectory, int logLevel, int port, std::string host)
{
	return ((int)nymi::jsonNapiConfigure(rootDirectory, (nymi::LogLevel)logLevel, port, host));
}

extern NAPIDYN_API int jsonNapiPutD(std::string json_in)
{
	return ((int)nymi::jsonNapiPut(json_in));
}

extern NAPIDYN_API NapiReturn jsonNapiGetD()
{
	std::string json;
	NapiReturn ret;
	ret.outcome = ((int)nymi::jsonNapiGet(json));
	ret.message = json;
	return ret;
}

extern NAPIDYN_API NapiReturn jsonNapiGetD(milliseconds timeout, milliseconds sleep)
{
	std::string json;
	NapiReturn ret;
	ret.outcome = ((int)nymi::jsonNapiGet(json, timeout, sleep));
	ret.message = json;
	return ret;
}

extern NAPIDYN_API NapiReturn jsonNapiGetD(long long int sleep)
{
	std::string json;
	NapiReturn ret;
	std::atomic< bool > quit;
	ret.outcome = ((int)nymi::jsonNapiGet(json, quit, sleep));
	ret.message = json;
	ret.quit = quit.load();
	return ret;
}

extern NAPIDYN_API void jsonNapiFinishD()
{
	nymi::jsonNapiFinish();
}

extern NAPIDYN_API void jsonNapiTerminateD()
{
	nymi::jsonNapiTerminate();
}

