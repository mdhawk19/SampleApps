#pragma once
#ifndef JSON_NAPI
#define JSON_NAPI

//#define API __attribute__((visibility("default")))
#define API

#ifdef __cplusplus

#include <atomic>
#include <string>
#include <sstream>
//#include <src/api/command.h>
//#include <src/api/enums.h>

typedef long long int milliseconds;

template< typename DataType >
  std::wostream& add_the_w( std::wostream& out, const DataType data ){
    std::stringstream subout;
    subout << data;
    return out << subout.str().c_str();
  }

namespace nymi{
  enum API class ConfigOutcome{
      okay,
      failedToInit,
      configurationFileNotFound,
      configurationFileNotReadable,
      configurationFileNotParsed
  };
  API std::ostream& operator<<( std::ostream& out, const ConfigOutcome v );

  API inline std::wostream& operator<<( std::wostream& out, const ConfigOutcome data ){ return add_the_w< ConfigOutcome >( out, data ); }

  enum API class JsonGetOutcome{
      okay,
      napiNotRunning,
      timedout,
      quitSignaled,
      napiFinished
  };
  API std::ostream& operator<<( std::ostream& out, const JsonGetOutcome v );

  API inline std::wostream& operator<<( std::wostream& out, const JsonGetOutcome data ){ return add_the_w< JsonGetOutcome >( out, data ); }

  enum API class JsonPutOutcome{
      okay,
      napiNotRunning
  };
  API std::ostream& operator<<( std::ostream& out, const JsonPutOutcome v );

  API inline std::wostream& operator<<( std::wostream& out, const JsonPutOutcome data ){ return add_the_w< JsonPutOutcome >( out, data ); }

  API ConfigOutcome jsonNapiConfigure( std::string rootDirectory, int logLevel = 0, int port = -1, std::string host = "" );
  API JsonPutOutcome jsonNapiPut( std::string json_in );
  API JsonGetOutcome jsonNapiGet( std::string& json );
  API JsonGetOutcome jsonNapiGet( std::string& json, milliseconds timeout, milliseconds sleep = 100 );
  API JsonGetOutcome jsonNapiGet( std::string& json, std::atomic< bool >& quit, long long int sleep = 100 );
  API void jsonNapiFinish();
  API void jsonNapiTerminate();
}

extern "C" {

#endif // __cplusplus

typedef enum{
    CO_OKAY,
    CO_FAILED_TO_INIT,
    CO_CONFIGURATION_FILE_NOT_FOUND,
    CO_CONFIGURATION_FILE_NOT_READABLE,
    CO_CONFIGURATION_FILE_NOT_PARSED,
    CO_IMPOSSIBLE
}                     configOutcome;
typedef enum{
    PO_OKAY,
    PO_NAPI_NOT_RUNNING,
    PO_IMPOSSIBLE
}                     jsonPutOutcome;
typedef enum{
    GO_OKAY,
    GO_NAPI_NOT_RUNNING,
    GO_TIMED_OUT,
    GO_QUIT_SIGNALED,
    GO_NAPI_FINISHED,
    GO_IMPOSSIBLE
}                     jsonGetOutcome;

extern configOutcome jsonNapiConfigure( const char* rootDirectory, int logLevel, int port, const char* host );
extern jsonPutOutcome jsonNapiPut( const char* json_in );
extern jsonGetOutcome jsonNapiGet( char* json_out, unsigned long long max );
extern jsonGetOutcome jsonNapiGetTimeout( char* json_out, unsigned long long max, long long int timeout, long long int sleep );
extern void jsonNapiFinish();
extern void jsonNapiTerminate();

#ifdef __cplusplus
}

#endif // __cplusplus
#endif // JSON_NAPI
