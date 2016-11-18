//
//  NapiDynlib.h
//  NapiDynlib
//
//  Created by Mike Martin on 2016-10-28.
//  Copyright © 2016 Nymi Inc. All rights reserved.
//

#ifndef NapiDynlib_h
#define NapiDynlib_h

#include <stdio.h>

#include "json-napi.h"

struct NapiReturn {
    char* message;
    int outcome;
    bool quit;
};

extern "C" int jsonNapiConfigureD(char* rootDirectory, int logLevel, int port, char* host);
extern "C" int jsonNapiPutD(char* json_in);
extern "C" struct NapiReturn jsonNapiGetD();
extern "C" struct NapiReturn jsonNapiGetTSD(milliseconds timeout, milliseconds sleep);
extern "C" struct NapiReturn jsonNapiGetSD(bool _quit, long long int sleep);
extern "C" void jsonNapiFinishD();
extern "C" void jsonNapiTerminateD();

#endif /* NapiDynlib_h */
