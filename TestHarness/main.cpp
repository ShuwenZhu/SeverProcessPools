#include <iostream>
#include <sstream>
//#include <Windows.h>



// XML files from Professor Fawcett's Libraries. We only use the two modules linked,
//     although there are several more.



//Our own project files, the Logger and TestHarness
#include "TestHarness.h"
#include "LoggerTH.h"


// Comm libraries!
#include "Comm/MsgPassingComm/Comm.h"
//#include "Comm/Logger/Logger.h"
//#include "Comm/Utilities/Utilities.h"
#include "Comm/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "Comm/Message/Message.h"

using namespace MsgPassingCommunication;
using namespace Sockets;
using SUtils = Utilities::StringHelper;

int main(void) {

	/* Okay, here are the set of *probably* changes we need to make from the requirements:"
	 *    1) New "Client" project executable.
	 *        a) Our Executive will create a client, (or multiples) which will read in an XML file, and send that file out to our server.
	 *    2) As the Executive starts, he instansiates our TestHarness.
	          a) TestHarness spawns several child worker processes, and a thread to parse the comm and place messages on the right queues.
	          b) TestHarness will be checking the queues, and when a READY message hits the queue, he pulls a message off of the Requests queue,
			     and sends the message to the associated child process.
	 *
	 */

	/*
	   Flow of main (the executive) should be
	     1) Instansiate the Test Harness
			1.1) Test Harness will create worker subprocesses who report "READY"
		 2) Start up at least two "clients" who will take a fileName and a target server address, and its own port 
			2.1) Clients will read in xml, then send requests to the TestHarness server
			2.2) Client exits
		  3) Server is listening for requests!
			3.1) Request comes in, worker thread adds to queues
			3.2) Server removes request from queue, and then passes off to worker process
			3.3) Worker process runs the test and returns pass/fail
			3.4) TestHarness gets pass/fail back
	*/

	// Spin up a reciever in the the main, client will connect and send a message
	SocketSystem ss;
	EndPoint serverEP("localhost", 9091);
	Comm comm(serverEP, "serverComm");
	comm.start();

	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;

	ZeroMemory(&info, sizeof(info));
	info.cb = sizeof(info);
	ZeroMemory(&processInfo, sizeof(processInfo));

	if (CreateProcess(TEXT("..\\Debug\\Client.exe"), NULL,
		NULL, NULL, FALSE, 0, NULL,
		NULL, &info, &processInfo)) {

		//WaitForSingleObject(processInfo.hProcess, INFINITE);
		//CloseHandle(processInfo.hProcess);
		//CloseHandle(processInfo.hThread);
	}

	Message msg;  // blocks until message arrives
	while (true) {
		std::cout << "Waiting for message:\n";
		msg = comm.getMessage();
		std::cout << "========================================================\n";
		std::cout << comm.name() + ": received message: " << msg.name();
		msg.show();
		std::cout << "========================================================\n";
	}


	system("pause");
	return 0; // Just for now while we figure out socket communication


	//===================================================================================================================================
	//===================================================================================================================================
	//===================================================================================================================================

	// Amount of worker threads to create!
	const int workerCount = 5;

	// Start the logger and harness
	LoggerTH log(::Level::info);
	TestHarness harness(log, workerCount);

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


	log.Info("oops");
	log.Info("All done!");
	return 0;
}