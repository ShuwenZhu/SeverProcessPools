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


bool TestDLL() {

	return false;
}



int main(int argc, char* argv[]) {
    std::cout << "Worker starting\n"; 
	SocketSystem ss;
	if (argc != 3) {
		std::cout << "Wrong number of arguments specified to the worker.\n";
		std::cout << "Usage: Worker.exe <server port> <my port>";
		return 1;
	}

	int serverPort = atoi(argv[1]);
	int clientPort = atoi(argv[2]);

	std::ostringstream name;
	name.str("");
	name << "client" << clientPort;

	EndPoint serverEP("localhost", serverPort);
	EndPoint clientEP("localhost", clientPort);
	Comm comm(clientEP, name.str());
	comm.start();
	Message readyMsg(serverEP, clientEP);
	readyMsg.name("READY");
	comm.postMessage(readyMsg);


	Message request, passFailReply;  // blocks until message arrives
	while (true) {
		std::cout << comm.name() << ": Waiting for message:\n";
		request = comm.getMessage();
		std::cout << "========================================================\n";
		std::cout << comm.name() + ": received message: " << request.name();
		request.show();
		std::cout << "========================================================\n";
	}
	comm.stop();
	return 0;
}
