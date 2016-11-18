#include "TestListener.h"

using namespace std;


void TestListener::waitForMessage() {

	cout << "TestListener: Waiting for response messages" << endl;

	while (true) {
		//this is a blocking call. Returns only if napi has sent a message, or quit is set to true
		if (jsonNapiGetSD) {
			try {
				NapiReturn retstruct;
				retstruct = jsonNapiGetSD(false, 100);
				if (retstruct.outcome == 0)
					cout << "GET: PASSED [" << retstruct.message << "]" << endl;
				else {
					cout << "GET: FAILED [" << retstruct.outcome << "]" << endl;
					continue;
				}
			}
			catch (exception& e) {
				cout << "GET: FAILED" << endl;
				cout << "Exception: " << e.what() << endl;
			}
		}
		else
			cout << "jsonNapiGetSD not loaded" << endl;
	}
}
	
bool TestListener::start() {
	t_listen = thread([=] { waitForMessage(); });
	if (t_listen.joinable())
		return true;
	else
		return false;
}