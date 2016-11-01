# Java Sample Applications

All projects in this directory were created primarily as Eclipse projects and were built using the Neon version of Eclipse.

#NapiJavaWrapper Sample

This project isn't strictly an application.  It produces a single JAR file that acts as a Java library wrapping the Nymi API in a purely Java implementation. Building the JAR requires a copy of the NAPI dynamic library wrapper (see the Cplusplus directory for that code). A copy of the pre-built library is included in the source tree (under /src/win32-x86-64 for Windows and /src/darwin for OS X) but can be updated by building a new version at any time.  The project has been configured as a Maven project and should be built using Maven for easiest dependency-resolution.  If you prefer not to use Maven you will need to modify the project to manually load JAR files for JNA (https://github.com/java-native-access/jna) and JSON-Java (https://github.com/stleary/JSON-java) on the build path for the project.

Using the library is fairly simple. Just create your own singleton copy of the NymiJavaApi class (using the getInstance() method) and implement the NapiCallbacks interface (which is passed to the init() method of the NymiJavaApi object) to register methods to handle responses from the API.  For a short example of how to do this, take a look at the JavaNapiSample project.

#JavaNapiSample Sample Application

This short project is a direct port of the NapiCpp application from the Cplusplus sample app.  It provides a simple command-line interface to pass commands to a Nymi Band and receive responses.  Most of the interface code is contained in the Util class which also handles a few other features of the app.  Feel free to use code for this app to build or extend your own Java applications. 


 
* Download the Nymi SDK: http://github.com/Nymi/JSON-API/releases


