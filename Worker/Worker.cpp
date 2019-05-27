// Worker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <sstream>

#include "../TestHarness/Comm/MsgPassingComm/Comm.h"
#include "../TestHarness/Comm/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../TestHarness/Comm/Message/Message.h"

using namespace MsgPassingCommunication;
using namespace Sockets;
using SUtils = Utilities::StringHelper;

typedef bool (*iTest)(void);

bool TestDLL(std::string libname) {

	std::wstring stemp = std::wstring(libname.begin(), libname.end());
	LPCWSTR libName = stemp.c_str();

	bool result = false;
	//Load the library in libname
	HINSTANCE hDLL;
	hDLL = LoadLibraryEx(libName, NULL, NULL);
	if (hDLL != NULL) {
		// Load the Test() function into "myTest", and check if its loaded.
		iTest myTest = (iTest)GetProcAddress(hDLL, "Test");
		if (myTest != NULL) {
			try {
				result = myTest();
			}
			catch (const char* msg) {
				std::cout << "Caught an exception! --" << msg << "--\n";
			}
		}
		else {
			std::cout << "Library --" << libname << "-- has no Test() function\n";
		}
	}
	else {
		std::cout << "Library --" << libname << "-- was unable to be loaded.\n";
	}
	FreeLibrary(hDLL);         // Free the library
	return result;
}


inline const char* const BoolToString(bool b) {
	return b ? "true" : "false";
}

int main(int argc, char* argv[]) {
	SocketSystem ss;
	if (argc != 2) {
		std::cout << "Wrong number of arguments specified to the worker.\n";
		std::cout << "Usage: Worker.exe <server port> <my port>";
		return 1;
	}

	int serverPort = atoi(argv[0]);
	int clientPort = atoi(argv[1]);

	std::ostringstream name;
	name.str("");
	name << "Worker#" << clientPort;

	EndPoint serverEP("localhost", serverPort);
	EndPoint clientEP("localhost", clientPort);
	Comm comm(clientEP, name.str());
	comm.start();
	Message readyMsg(serverEP, clientEP);
	readyMsg.name("READY");
	comm.postMessage(readyMsg);
	

	Message request, passFailReply;  // blocks until message arrives
	bool result = false;
	while (true) {
		//std::cout << comm.name() << ": Waiting for message:\n";
		request = comm.getMessage();
		//std::cout << "========================================================\n";
		//std::cout << comm.name() + ": received message: " << request.name();
		if (request.name() == "REQUEST") {
			//  Test the dll, return the result
			result = TestDLL(request.command());
			passFailReply.command(BoolToString(result));
			passFailReply.to(request.from());
			passFailReply.from(request.to());
			passFailReply.name("RESULT");
			comm.postMessage(passFailReply);
		}
		request.show();
		//std::cout << "========================================================\n";
	}
	comm.stop();
	return 0;
}
