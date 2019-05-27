#include <iostream>
#include <sstream>

//Our own project files, the Logger and TestHarness
#include "TestHarness.h"
#include "LoggerTH.h"

// Comm libraries!
#include "Comm/MsgPassingComm/Comm.h"
#include "Comm/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "Comm/Message/Message.h"

using namespace MsgPassingCommunication;
using namespace Sockets;
using SUtils = Utilities::StringHelper;


int main(void) {

	// Some constants
	std::string xmlFilePath = "../TestHarness/xmlfiles/test1.xml";
	int serverPort = 9091;
	const int workerCount = 5;

	// Start the logger and harness
	LoggerTH log(::Level::info);
	TestHarness harness(log);



	// =========================================================================
	// Some black magic needed, found here:
	// https://stackoverflow.com/questions/14276425/calling-overloaded-member-functions-using-stdthread
	// Essentially tricks std::thread to behavior properly!
	using memfunc_type = void (TestHarness::*)(int);
	memfunc_type memfunc = &TestHarness::serverThreadFunction;
	// =========================================================================
	// Start the server in a seperate thread!
	log.Info("Starting server thread");
	std::thread serverThread(memfunc, &harness, serverPort);
	serverThread.detach();

	// Start the workers!
	harness.StartWorkers(serverPort, workerCount);

	// Start the Client:
	// ========================================================================
	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&info, sizeof(info));
	info.cb = sizeof(info);
	ZeroMemory(&processInfo, sizeof(processInfo));
	// Make the cmdArgs needed
	std::ostringstream cmdargs;
	cmdargs.str(" ");
	cmdargs << serverPort << " " << xmlFilePath;
	std::string cmdArgs = cmdargs.str();
	// Create the client process with arguments
	if (CreateProcess(
		"..\\Debug\\Client.exe", 
		LPSTR(cmdArgs.c_str()),
		NULL, NULL, FALSE, 0, NULL,
		NULL, &info, &processInfo)) {

		std::cout << "Client Process Launched!\n";
	} else {
		std::cout << "Client process failed to launch!\n";
		return 1;
	}
	int i = 0;
	while (true) {
		//Just block stuff.
		i++;
	}

	serverThread.join();
	return 0;
}