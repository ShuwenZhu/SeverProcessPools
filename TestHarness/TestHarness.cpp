#include <iostream>
#include <cstdarg>
#include <initializer_list>
#include <iostream>
#include <sstream>

#include "TestHarness.h"
#include "LoggerTH.h"

#include "Comm/MsgPassingComm/Comm.h"
#include "Comm/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "Comm/Message/Message.h"

using namespace MsgPassingCommunication;
using namespace Sockets;
using SUtils = Utilities::StringHelper;


bool TestHarness::startWorker(int serverPort, int clientPort) {
	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;

	ZeroMemory(&info, sizeof(info));
	info.cb = sizeof(info);
	ZeroMemory(&processInfo, sizeof(processInfo));

	// Make the cmdArgs needed
	std::ostringstream cmdargs;
	cmdargs.str(" ");
	cmdargs << serverPort << " " << clientPort;
	std::string cmdArgs = cmdargs.str();

	// Create the client process with arguments
	if (CreateProcess(
#ifdef _DEBUG
		"..\\Debug\\Worker.exe",
#else
		"..\\Release\\Worker.exe",
#endif
		LPSTR(cmdArgs.c_str()),
		NULL, NULL, FALSE, 0, NULL,
		NULL, &info, &processInfo)) {
		log.Debug("Worker Process successfully launched!");
		return true;
	}
	else {
		log.Error("Worker Process failed to launched!");
		return false;
	}
	return false;
}

// ProcessQueues handles the queues, dispatching REQUESTS to READY workers.
void TestHarness::ProcessQueues(int serverPort) {
	Message msg;
	Message requestMsg;
	Message readyMsg;
	EndPoint client;
	SocketSystem ss;

	EndPoint serverEP("localhost", serverPort);
	EndPoint sneakyServer("localhost", 8090);// We dont listen, just need a comm
	Comm myComm(sneakyServer, "serverComm2");
	myComm.start();

	while (true) {
		// We have a request message and a client
		//    once this unblocks!
		msg = requestQueue.deQ();
		readyMsg = readyQueue.deQ();
		client = readyMsg.from();
		requestMsg = msg;
		requestMsg.to(client);
		requestMsg.from(msg.to());
		requestMsg.name("REQUEST");
		myComm.postMessage(requestMsg);
	}
	return;
}
// serverThreadfunction is the server thread that handles the 
void TestHarness::serverThreadFunction(int serverPort) {
	std::ostringstream output;
	SocketSystem ss;
	EndPoint serverEP("localhost", serverPort);

	Comm myComm(serverEP, "serverComm");
	myComm.start();



	using memfunc_type = void (TestHarness::*)(int);
	memfunc_type memfunc = &TestHarness::ProcessQueues;
	std::thread QueueProcess(memfunc, this, serverPort);

	QueueProcess.detach();
	int port;
	std::string hostname;

	Message msg;  // blocks until message arrives
	while (true) {
		msg = myComm.getMessage();
		if (msg.name() == "READY") {
			output.str("Processing a READY message from a worker.");
			log.Info(output.str());
			// Push worker endpoint to ready queue!
			log.Info("Adding a worker to the READY queue.");
			readyQueue.enQ(msg);
		}
		else if (msg.name() == "REQUEST") {
			// A request comes from a client. We push the message on the request queue.
			output.str("");
			output << "Processing a REQUEST message for dll --" << msg.command() << "--";
			log.Info(output.str());
			requestQueue.enQ(msg);
		}
		else if (msg.name() == "RESULT") {
			// Pass the message back to the client!
			output.str("");
			output << "RESULT msg received for dll --" << msg.getAttribute("libname") << "--. Result: --" << msg.command() << "--";
			log.Info(output.str());
			// Convert value from string to int. Works idk.
			std::stringstream strValue;
			strValue << msg.getAttribute("requesterPort");
			strValue >> port;

			EndPoint client(msg.getAttribute("requesterIp"), port );
			msg.from(msg.to());
			msg.to(client);
			myComm.postMessage(msg);
		}
		else {
			log.Warning("Unknown message recieved, tossing.");
			// Toss it
		}
	}
}

// Default constructor. Starts comm endpoints and spins up workers.
TestHarness::TestHarness(LoggerTH myLogger) {
	log = myLogger;
}

void TestHarness::StartWorkers( int serverPort, int workerCount) {
	int clientPort = 9092; // Hardcoded starting port? #IndustryStandards
	for (int i = 0; i < workerCount; i++) {
		// spin up worker subprocess
		log.Debug("Spining up a worker subprocess!");
		startWorker(serverPort, clientPort);
		clientPort++;
	}
	return;
}

// Destructor
TestHarness::~TestHarness() {};