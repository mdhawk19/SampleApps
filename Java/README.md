# Java Sample Applications

All projects in this directory were created as Eclipse projects and were built using the Neon version of Eclipse.

## NapiJavaWrapper Sample

This project isn't strictly an application.  It produces a single JAR file that acts as a Java library wrapping the Nymi API in a purely Java implementation. 

Building the JAR requires a copy of the NAPI dynamic library wrapper (see the Cplusplus directory for that code). A copy of the pre-built library is included in the source tree (under `/src/win32-x86-64` for Windows and `/src/darwin` for OS X) but can be updated by building a new version at any time.  The project has been configured as a Maven project and should be built using Maven for easiest dependency-resolution.  If you prefer not to use Maven you will need to modify the project to manually load JAR files for JNA (https://github.com/java-native-access/jna) and JSON-Java (https://github.com/stleary/JSON-java) on the build path for the project.

To use the library, create your own singleton copy of the `NymiJavaApi` class (using the `getInstance()` method) and implement the `NapiCallbacks` interface (which is passed to the `init()` method of the `NymiJavaApi` object) to register methods to handle responses from the API.  For a short example of how to do this, take a look at the `JavaNapiSample` project.

## JavaNapiSample Sample Application

This short project is a direct port of the `NapiCpp` application from the Cplusplus sample app.  It provides a simple command-line interface to pass commands to a Nymi Band and receive responses.  Most of the interface code is contained in the `Util` class which also handles a few other features of the app.  Feel free to use code for this app to build or extend your own Java applications. 

## Roaming Authentication Service

This J2EE service provides the customer-required components for a full Nymi roaming authentication system. It needs to be paired with an NEA that provides the functional link (via the Nymi SDK) between this service and Nymi Bands.  It has been tested on Apache Tomcat 8.2 with Java JDK 1.8 but should run on any equivalent J2EE server or middleware platform.  Note that the service required a small MySQL database to maintain some state and store user/band information.  A MySQL script for building the database can be found in the /mysql directory.

## Roaming Authentication Sample NEA

This Java application is a very simple example of the use of roaming authentication.  It does not provide the ability to provision bands (just launch the application in a directory with an existing `provisions.json` file and it will use those).  It provides two functions - setup and auth.  When run in setup mode it will set up roaming authentication with the first provisioned band it finds and associates it with the hardcoded username 'testuser.'  In auth mode it will similarly try to authenticate with the first band it finds, provisioned *or* un-provisioned (which is normally what roaming authentication will expect).  To fully exercise the app, run setup with a `provisions.json` file in the working directory and then authenticate without it.

## Nymi SDK
 
* Download the Nymi SDK here: http://github.com/Nymi/JSON-API/releases



