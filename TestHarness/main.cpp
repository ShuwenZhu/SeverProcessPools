#include <iostream>
#include <sstream>

// XML files from Professor Fawcett's Libraries. We only use the two modules linked,
//     although there are several more.



//Our own project files, the Logger and TestHarness
#include "TestHarness.h"
#include "Logger.h"



int main(void) {

	// Start the logger and harness
	Logger log(info);
	TestHarness harness(log);

	std::string src = "<?xml version=\"1.0\" encoding=\"utf - 8\"?>\
		<!--XML test case -->\
		<TestRequest>\
		<test>DivideTestDll.dll< / test>\
		<test>LambdaTestDll.dll< / test>\
		<test>WillNotLoadTest.dll< / test>\
		< / TestRequest>";

	log.Debug("Handling passed in Sequence");
	bool result = harness.handleTestSequence(src);
	std::ostringstream os;
	os.str("");
	os << "Result of Sequence:" << result;
	log.Info(os.str());


	log.Info("
	log.Info("All done!");
	system("pause");
	return 0;
}