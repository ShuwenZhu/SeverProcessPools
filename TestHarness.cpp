#include <iostream>
#include <cstdarg>
#include <initializer_list>
#include <sstream>
#include <windows.h>

#include "TestHarness.h"
#include "Logger.h"

// Default constructor
TestHarness::TestHarness(Logger myLogger) {
	log = myLogger;
}

// iTest function prototype
typedef bool (*iTest)(void);

bool TestHarness::TestLibrary(::std::string libname) {
	std::ostringstream aggString;
	bool result = false;
	aggString << "TestHarness::TestLibrary: called with library --" << libname << "--";
	log.Debug(aggString.str());
	aggString.str("");

	//Load the library in libname
	HINSTANCE hDLL;
	hDLL = LoadLibraryEx(libname.c_str(), NULL, NULL);

	if (hDLL != NULL) {

		aggString << "Library --" << libname << "-- correctly loaded. Extrating Test()..";
		log.Debug(aggString.str());
		aggString.str("");

		// Load the Test() function into "myTest", and check if its loaded.
		iTest myTest = (iTest)GetProcAddress(hDLL, "Test");
		if (myTest != NULL) {

			log.Debug("TestHarness::TestLibrary: Calling extracted Test() function:");
			try {
				result = myTest();
			} catch (const char* msg) {
				log.Error("Caught an exception!");
				log.Error(msg);
			}
		} else {
			aggString << "Library --" << libname << "-- has no Test() function";
			log.Error(aggString.str());
		}
	} else {
		aggString << "Library --" << libname << "-- was unable to be loaded.";
		log.Error(aggString.str());
	}
	FreeLibrary(hDLL);         // Free the library
	aggString.str("");
	aggString << "TestHarness::TestLibrary: Library --" << libname << "-- fully tested. Result: " << result;
	log.Debug(aggString.str());

	return result;
}

// Destructor
TestHarness::~TestHarness() {};