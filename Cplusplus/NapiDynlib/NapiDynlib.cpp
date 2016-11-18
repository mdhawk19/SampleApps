//
//  NapiDynlib.cpp
//  NapiDynlib
//
//  Created by Mike Martin on 2016-10-28.
//  Copyright © 2016 Nymi Inc. All rights reserved.
//

#define EXPORT __attribute__((visibility("default")))

#include "NapiDynlib.h"

#include <iostream>
#include <stdio.h>

EXPORT
int jsonNapiConfigureD(char* rootDirectory, int logLevel, int port, char* host)
{
    std::string rootdirstr(rootDirectory);
    std::string hoststr(host);
    return (int)nymi::jsonNapiConfigure(rootdirstr, (nymi::LogLevel)logLevel, port, hoststr);
}

EXPORT
int jsonNapiPutD(char* json_in)
{
    
    return (int)nymi::jsonNapiPut(json_in);
}

EXPORT
NapiReturn jsonNapiGetD()
{
    std::string json;
    NapiReturn ret;
    ret.outcome = ((int)nymi::jsonNapiGet(json));
    ret.quit = false;
    ret.message = (char*)malloc(sizeof(char) * ((json.end() - json.begin()) + 1));
    std::copy(json.begin(), json.end(), ret.message);
    return ret;
}

EXPORT
NapiReturn jsonNapiGetTSD(milliseconds timeout, milliseconds sleep)
{
    std::string json;
    NapiReturn ret;
    ret.outcome = ((int)nymi::jsonNapiGet(json, timeout, sleep));
    ret.quit = false;
    ret.message = (char*)malloc(sizeof(char) * ((json.end() - json.begin()) + 1));
    std::copy(json.begin(), json.end(), ret.message);
    return ret;
}

EXPORT
NapiReturn jsonNapiGetSD(bool _quit, long long int sleep)
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

EXPORT
void jsonNapiFinishD()
{
    nymi::jsonNapiFinish();
}

EXPORT
void jsonNapiTerminateD()
{
    nymi::jsonNapiTerminate();
}
