
#include "TestMain.h"
#include "TestListener.h"

using namespace std;

int main(int argv, char* argc)
{
	HMODULE dllHandle = NULL;

	dllHandle = LoadLibraryA("NapiDyn.dll");

	if (!dllHandle) {
		cout << "Unable to load NapiDyn.dll" << endl;
		exit (1);
	}
	else
	{
		cout << "Loaded NapiDyn.dll succesfully" << endl;
	}

	f_jsonNapiConfigureD jsonNapiConfigureD = (f_jsonNapiConfigureD)GetProcAddress(dllHandle, "jsonNapiConfigureD");
	f_jsonNapiPutD jsonNapiPutD = (f_jsonNapiPutD)GetProcAddress(dllHandle, "jsonNapiPutD");

	if (jsonNapiConfigureD) {
		try {
			int retcode = jsonNapiConfigureD(".", 0, 9089, "127.0.0.1");
			if (retcode == 0)
				cout << "INIT: PASSED" << endl;
			else
				cout << "INIT: FAILED [" << retcode << "]" << endl;
		}
		catch (exception& e) {
			cout << "PUT: FAILED" << endl;
			cout << "Exception: " << e.what() << endl;
		}
	}
	else
		cout << "jsonNapiConfigureD not loaded" << endl;

	// Start test listener thread
	TestListener* listener = new TestListener(dllHandle);

	if (listener->start())
		cout << "Listener started" << endl;
	else {
		cout << "Listener statup failed" << endl;
		exit(1);
	}

	Sleep(200);

	if (jsonNapiPutD) {
		try {
			char* provstart = "{\"exchange\":\"*provisioning*\",\"path\":\"provision/run/start\"}";
			cout << "Sending JSON message: [" << provstart << "]" << endl;
			int retcode = jsonNapiPutD(provstart);
			if (retcode == 0)
				cout << "PUT: PASSED" << endl;
			else
				cout << "PUT: FAILED [" << retcode << "]" << endl;
		}
		catch (exception& e) {
			cout << "PUT: FAILED" << endl;
			cout << "Exception: " << e.what() << endl;
		}
	}
	else
		cout << "jsonNapiPutD not loaded" << endl;
	
	while (true) {
		Sleep(5000);
	}

	FreeLibrary(dllHandle);
	exit(0);
}
