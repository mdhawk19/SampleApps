#NapiCppWrapper Sample App

The Nymi API (NAPI) 4.0 is  JSON-based API that runs as an in-process static library. To demonstrate the functionality exposed through NAPI, we have provided a sample application (`NapiCppWrapper`). The sample app is an NEA written in C++, over a wrapper layer that provides a C++ interface to NAPI. The wrapper layer handles most of the lower level asynchronous message passing to and from NAPI.

In the sample app, the NEA is represented by the `main.cpp` file. This file (the NEA) consumes the C++ wrapper in order to interact with NAPI. The wrapper layer is a reusable component that you can use or modify for writing your own NEA. The sample app has a command-line interface for demonstrating input and output of commands.

 
* Download the Nymi SDK: http://github.com/Nymi/JSON-API/releases
* Developer Quick Start: http://downloads.nymi.com/sdkDoc/latest/index.html#developer-quick-start
* Sample App Walkthrough: http://downloads.nymi.com/sdkDoc/latest/index.html#sample-app-walkthrough

#NapiDll Dynamic Library Wrapper (NapiDll/NapiDynLib)

In order to make porting of the existing Nymi API (NAPI) library to other platforms easier, we've included a sample wrapper that encloses the existing NAPI static library in a Windows DLL and Mac OS X dynamic library. These artifacts are used in the Java wrapper elsewhere in this repository but may be used in any way needed.  Please note that the actual interfaces have been modified to enforce a pass-by-value design (also to handle the requirements of Java native interfaces), so review the new interface definitions in the wrapper carefully before attempting to use this for your own projects.
