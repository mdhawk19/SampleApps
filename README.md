# Nymi Sample Applications

This repository contains code in various languages for sample applications that consume the Nymi API.  Primarily these apps are meant to act as examples of how to wrap the JSON-based NAPI with other types of data structures or using different languages.

Currently there are two sets of samples apps for C++ and four for Java. The following describes each app/project:


Cplusplus  
-	NapiCppWrapper: *A simple command-line application that wraps the NAPI. Both Windows and Mac.*
-	NapiDll: *A wrapper for the static NAPI library that re-publishes is as a dynamic library. Windows only.*
-	NapDynLib: *A wrapper for the static NAPI library that re-publishes is as a dynamic library. Mac only.*

Java (all platform independent)
-	JavaNapiSample: *Java version of the NapiCppWrapper app.*
-	NapiJavaWrapper: *Wrapper library for the dynamic NAPI library. Consumed by the JavaNapiSample app.*
-	RoamingNEASample: *A sample application that consumes the RoamingService server to authenticate a Nymi Band using roaming authentication.*
-	RoamingService: *A sample JavaSE web service that fulfills the role of the Relying Party Server in the roaming authentication model.*
