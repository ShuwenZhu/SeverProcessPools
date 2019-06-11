// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>


// Read in XML, parse the XML
#include "..\TestHarness\XmlDocument\XmlParser\XmlParser.h"
#include "..\TestHarness\XmlDocument\XmlDocument\XmlDocument.h"

// Comm libraries!
#include "../TestHarness/Comm/MsgPassingComm/Comm.h"
#include "../TestHarness/Comm/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../TestHarness/Comm/Message/Message.h"

using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;
using namespace MsgPassingCommunication;
using namespace Sockets;
using SUtils = Utilities::StringHelper;

int main(int argc, char* argv[]) {
	//std::cout << "Client.exe starting\n";
	int myPort = 9999;
	int serverPort = 9091;
	std::string xmlFileName = "../TestHarness/xmlfiles/test1.xml";

	//Default our arguments for easy testing (just call Client.exe)
	if (argc == 2) {
		serverPort = atoi(argv[0]);
		xmlFileName = argv[1];
	}

	// Socket/Comm interface spin up!
	SocketSystem ss;
	EndPoint serverEP("localhost", serverPort);
	EndPoint clientEP("localhost", myPort);
	Comm comm(clientEP, "clientComm");
	comm.start();

	// Read and process the xmlFile we passed in!
	XmlProcessing::XmlParser parser(xmlFileName, XmlProcessing::XmlParser::sourceType::file);
	XmlProcessing::XmlDocument* pDoc = parser.buildDocument();

	std::string testTag = "TestRequest";
	std::vector<sPtr> found = pDoc->element(testTag).descendents().select();
	std::ostringstream os;
	std::string libName;

	Message msg(serverEP, clientEP);
	msg.name("REQUEST");
	if (found.size() > 0) {
		for (auto pElem : found) {
			libName = pElem->value();
			// "test" are the outside containers of our dll names. Skip em.
			if (libName == "test") { continue; }
			//std::cout << "Sending DLL Library name: --" << libName << "-- to server\n";
			msg.command(libName);
			msg.attribute("requesterIp", "localhost");
			msg.attribute("requesterPort", std::to_string(myPort));
			comm.postMessage(msg);
		}
	}

	Message responseMessage;  // blocks until message arrives
	while (true) {
		//std::cout << "Waiting for message...\n";
		responseMessage = comm.getMessage();
		std::cout << "=======================================================\n";
		std::cout << "Client: Got a response message!" << "\n";
		std::cout << "\tDLL: --" << responseMessage.getAttribute("libname") << "--\n";
		std::cout << "\tResult: --" << responseMessage.command() << "--\n";
		std::cout << "=======================================================\n";
		//responseMessage.show();
	}
	//std::cout << "All Done!\n";
	comm.stop();
	return 0;
}