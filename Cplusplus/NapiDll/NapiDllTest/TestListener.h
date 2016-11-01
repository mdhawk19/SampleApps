#pragma once
#include "TestMain.h"

class TestListener {
	f_jsonNapiGetSD jsonNapiGetSD;
	std::thread t_listen;

public:
	TestListener(HMODULE dllHandle) {
		jsonNapiGetSD = (f_jsonNapiGetSD)GetProcAddress(dllHandle, "jsonNapiGetSD");
	}

	~TestListener() {
		if (t_listen.joinable())
			t_listen.join();
	}

	void waitForMessage();
	bool start();
};