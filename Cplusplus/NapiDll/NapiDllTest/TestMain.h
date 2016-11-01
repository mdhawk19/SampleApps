#pragma once

#include <Windows.h>
#include <iostream>
#include <exception>
#include <thread>

#include "../NapiDll/NapiDll.h"

typedef int(*f_jsonNapiConfigureD)(char*, int, int, char*);
typedef int(*f_jsonNapiPutD)(char*);
typedef struct NapiReturn(*f_jsonNapiGetD)();
typedef struct NapiReturn(*f_jsonNapiGetTSD)(milliseconds, milliseconds);
typedef struct NapiReturn(*f_jsonNapiGetSD)(bool, long long int);
typedef void(*f_jsonNapiFinishD)();
typedef void(*f_jsonNapiTerminateD)();
