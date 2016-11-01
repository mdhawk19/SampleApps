#NapiCppWrapper Sample App

The Nymi JSON API 4.0 runs as an in-process static library. To demonstrate the functionality exposed through the JSON API, we have provided a sample application (`NapiCppWrapper`). The sample app is an NEA written in C++, over a wrapper layer that provides a C++ interface to the JSON API. The wrapper layer handles most of the lower level asynchronous message passing to and from the JSON API.

In the sample app, the NEA is represented by the `main.cpp` file. This file (the NEA) consumes the wrapper in order to interact with the JSON API. The wrapper layer is a reusable component that you can use or modify for writing your own NEA. The sample app has a command-line interface for demonstrating input and output of commands.

 
* Download the Nymi SDK: http://github.com/Nymi/JSON-API/releases
* Developer Quick Start: http://downloads.nymi.com/sdkDoc/latest/index.html#developer-quick-start
* Sample App Walkthrough: http://downloads.nymi.com/sdkDoc/latest/index.html#sample-app-walkthrough

#NapiDll Dynamic Library Wrapper (NapiDll/NapiDynLib)

In order to make porting of the existing Nymi API (NAPI) C++ library to other platforms, we've included a sample wrapper that encloses the existing NAPI static library in a Windows DLL and Mac OS X dynamic library. These artifacts are used in the Java wrapper elsewhere in this repository but may be used in any way needed.  Please note that the actual interfaces have been modified to enforce a pass-by-value design (also to handle the requirements of Java native interfaces) so carefully review the new interface definitions in the wrapper before attempting to use this for your own projects.

