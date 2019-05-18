#include <iostream>
#include <sstream>

// XML files from Professor Fawcett's Libraries. We only use the two modules linked,
//     although there are several more.
#include "XmlDocument/XmlParser/XmlParser.h"
#include "XmlDocument/XmlDocument/XmlDocument.h"


//Our own project files, the Logger and TestHarness
#include "TestHarness.h"
#include "Logger.h"

using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;

int main(void) {

	// Start the logger and harness
	Logger log(info);
	TestHarness harness(log);

	//log.Debug("This is an Debug level statement.");
	//log.Info("This is an info level statement.");
	//log.Warning("This is an warning level statement.");
	//log.Error("This is an error level statement.");
	//log.Critical("This is an critical level statement.");

	// This is where our test xml file is.
	std::string src = "xmlFiles/test1.xml";

	log.Debug("Using the professors library to parse XML:");

	// Instansiate xml parser, parse our xml to a file
	XmlProcessing::XmlParser parser(src);
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
			os << "Dynamically loading and evalutating the dll named: --" << libName <<  "--.";
			log.Info("============================================================================");
			log.Info(os.str());
			os.str("");
			os << "Library --" << libName << "-- test status: ";
			if (harness.TestLibrary(libName)) {
				os << "PASS";
			} else {
				os << "FAIL";
			}
			log.Info(os.str());
			log.Info("============================================================================");
		}
	} else {
		os.str("");
		os << "No element called " << testTag << " found.";
		log.Critical(os.str());
	}
	log.Info("All done!");
	system("pause");
	return 0;
}