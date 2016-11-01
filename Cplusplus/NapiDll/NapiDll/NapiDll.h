// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NAPIDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NAPIDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef NAPIDLL_EXPORTS
#define NAPIDLL_API __declspec(dllexport)
#else
#define NAPIDLL_API __declspec(dllimport)
#endif

#include "json-napi.h"

struct NapiReturn {
	char* message;
	int outcome;
	bool quit;
};

extern "C" NAPIDLL_API int jsonNapiConfigureD(char* rootDirectory, int logLevel, int port, char* host);
extern "C" NAPIDLL_API int jsonNapiPutD(char* json_in);
extern "C" NAPIDLL_API struct NapiReturn jsonNapiGetD();
extern "C" NAPIDLL_API struct NapiReturn jsonNapiGetTSD(milliseconds timeout, milliseconds sleep);
extern "C" NAPIDLL_API struct NapiReturn jsonNapiGetSD(bool _quit, long long int sleep);
extern "C" NAPIDLL_API void jsonNapiFinishD();
extern "C" NAPIDLL_API void jsonNapiTerminateD();
