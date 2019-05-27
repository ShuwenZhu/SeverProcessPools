#include <iostream>
#include <cstdarg>
#include <initializer_list>
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
		"..\\Debug\\Worker.exe",
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

void TestHarness::ProcessQueues(int serverPort) {
	Message msg;
	Message request;
	EndPoint client;
	SocketSystem ss;

	EndPoint serverEP("localhost", serverPort);
	EndPoint sneakyServer("localhost", 8090);
	Comm myComm(sneakyServer, "serverComm2");
	myComm.start();

	while (true) {
		// We have a request message and a client
		//    once this unblocks!
		msg = requestQueue.deQ();
		client = readyQueue.deQ();

		request.to(client);
		request.from(msg.to());
		request.name("REQUEST");
		request.command(msg.command());
		myComm.postMessage(request);
	}
	return;
}

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

	Message msg;  // blocks until message arrives
	while (true) {
		msg = myComm.getMessage();
		log.Debug("========================================================");
		if (msg.name() == "READY") {
			log.Debug("Processing a READY message..");
			// Push worker endpoint to ready queue!
			readyQueue.enQ(msg.from());
		}
		else if (msg.name() == "REQUEST") {
			log.Debug("Processing a REQUEST message..");
			requestQueue.enQ(msg);
			// pop a worker off the ready queue, pass it a request
		}
		else if (msg.name() == "RESULT") {
			log.Debug("Processing a RESULT message..");
			output.str(""); output << "Received a result from child --";
			output << "--: Test was a --" << msg.command() << "--";
			log.Info(output.str());
		}
		else {
			log.Warning("Unknown message recieved, tossing.");
			// Toss it
		}
		log.Debug("========================================================");
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