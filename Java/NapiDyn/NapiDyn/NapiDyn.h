// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NAPIDYN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NAPIDYN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef NAPIDYN_EXPORTS
#define NAPIDYN_API __declspec(dllexport)
#else
#define NAPIDYN_API __declspec(dllimport)
#endif

#include "json-napi.h"

struct NapiReturn {
	std::string message;
	int outcome;
	bool quit;
};

extern NAPIDYN_API int jsonNapiConfigureD(std::string rootDirectory, int logLevel = 0, int port = -1, std::string host = "");
extern NAPIDYN_API int jsonNapiPutD(std::string json_in);
extern NAPIDYN_API NapiReturn jsonNapiGetD();
extern NAPIDYN_API NapiReturn jsonNapiGetD(milliseconds timeout, milliseconds sleep = 100);
extern NAPIDYN_API NapiReturn jsonNapiGetD(long long int sleep = 100);
extern NAPIDYN_API void jsonNapiFinishD();
extern NAPIDYN_API void jsonNapiTerminateD();
