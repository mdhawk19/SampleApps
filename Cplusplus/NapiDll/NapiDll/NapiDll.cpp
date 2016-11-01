// NapiDll.cpp : Defines the exported functions for the DLL application.
//

#include "NapiDll.h"
#include <iostream>


extern "C" NAPIDLL_API int jsonNapiConfigureD(char* rootDirectory, int logLevel, int port, char* host)
{
	std::string rootdirstr(rootDirectory);
	std::string hoststr(host);
	return (int)nymi::jsonNapiConfigure(rootdirstr, (nymi::LogLevel)logLevel, port, hoststr);
}

extern "C" NAPIDLL_API int jsonNapiPutD(char* json_in)
{

	return (int)nymi::jsonNapiPut(json_in);
}

extern "C" NAPIDLL_API NapiReturn jsonNapiGetD()
{ 
	std::string json;
	NapiReturn ret;
	ret.outcome = ((int)nymi::jsonNapiGet(json));
	ret.quit = false;
	ret.message = (char*)malloc(sizeof(char) * ((json.end() - json.begin()) + 1));
	std::copy(json.begin(), json.end(), ret.message);
	return ret;
}

extern "C" NAPIDLL_API NapiReturn jsonNapiGetTSD(milliseconds timeout, milliseconds sleep)
{
	std::string json;
	NapiReturn ret;
	ret.outcome = ((int)nymi::jsonNapiGet(json, timeout, sleep));
	ret.quit = false;
	ret.message = (char*)malloc(sizeof(char) * ((json.end() - json.begin()) + 1));
	std::copy(json.begin(), json.end(), ret.message);
	return ret;
}

extern "C" NAPIDLL_API NapiReturn jsonNapiGetSD(bool _quit, long long int sleep)
{
	std::string json;
	NapiReturn ret;
	std::atomic< bool > quit{ _quit };
	ret.outcome = ((int)nymi::jsonNapiGet(json, quit, sleep));
	ret.quit = quit.load();
	ret.message = (char*)malloc(sizeof(char) * ((json.end() - json.begin()) + 1));
	std::copy(json.begin(), json.end(), ret.message);
	return ret;
}

extern "C" NAPIDLL_API void jsonNapiFinishD()
{
	nymi::jsonNapiFinish();
}

extern "C" NAPIDLL_API void jsonNapiTerminateD()
{
	nymi::jsonNapiTerminate();
}

