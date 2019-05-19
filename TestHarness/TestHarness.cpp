#include <iostream>
#include <cstdarg>
#include <initializer_list>
#include <sstream>
#include <windows.h>

#include "TestHarness.h"
#include "Logger.h"

#include "XmlDocument\XmlParser\XmlParser.h"
#include "XmlDocument/XmlDocument/XmlDocument.h"


using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;

// Default constructor
TestHarness::TestHarness(Logger myLogger) {
	log = myLogger;
}

// iTest function prototype
typedef bool (*iTest)(void);


bool TestHarness::handleTestSequence(::std::string xmlSequence) {
	bool result = true;
	XmlProcessing::XmlParser parser(xmlSequence, XmlProcessing::XmlParser::sourceType::str);
	XmlProcessing::XmlDocument* pDoc = parser.buildDocument();

	// Find all children of "TestRequest", which should be "test" elements with DLL names inside.
	std::string testTag = "TestRequest";
	std::vector<sPtr> found = pDoc->element(testTag).descendents().select();
	std::ostringstream os;
	std::string libName;
	if (found.size() > 0) {
		for (auto pElem : found) {

			libName = pElem->value();
			// "test" are the outside containers of our dll names. Skip em.
			if (libName == "test") { continue; }

			os.str("");
			os << "Dynamically loading and evalutating the dll named: --" << libName << "--.";
			log.Info("============================================================================");
			log.Info(os.str());
			os.str("");
			os << "Library --" << libName << "-- test status: ";
			if (this -> TestLibrary(libName)) {
				os << "PASS";
			}
			else {
				os << "FAIL";
				result = false;
			}
			log.Info(os.str());
			log.Info("============================================================================");
		}
	}
	else {
		os.str("");
		os << "No element called " << testTag << " found.";
		log.Critical(os.str());
	}
	return result;
}

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