# Nymi Sample Applications

This repository contains code in various languages for sample applications that consume the Nymi API.  Primarily these apps are meant to act as examples of how to wrap the JSON-based NAPI with other types of data structures or using different languages.

Currently there is a single sample app for C++, four for Java, and one for Python. The following describes each app/project:

Cplusplus  
-	NapiCppWrapper: *A simple command-line application that wraps the NAPI. Both Windows and Mac.*

Java (platform independent)
-	JavaNapiSample: *Java version of the NapiCppWrapper app.*
-	NapiJavaWrapper: *Wrapper library for the dynamic NAPI library. Consumed by the JavaNapiSample app.*
-	RoamingNEASample: *A sample application that consumes the RoamingService server to authenticate a Nymi Band using roaming authentication. Note: this has not yet been updated for use with the v4.1 API. It will shortly.*
-	RoamingService: *A sample JavaSE web service that fulfills the role of the Relying Party Server in the roaming authentication model.*

Python (platform independent)

- Apy: *A Python implementation of the Roaming Authentication Server.  Equivalent to the Java RoamingService project.*

Note: The Nymi SDK, and therefore these sample apps, are supported on macOS Sierra 10.12.2 and later. Version 10.12 and 10.12.1 are not supported.
