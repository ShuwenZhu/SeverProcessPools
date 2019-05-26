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
	std::cout << "Client.exe starting\n";
	for (int i = 0; i < argc; i++) {
		std::cout << "ClientArgs: --" << argv[i] << "--\n";
	}
	if (argc != 2) {
		std::cout << "Wrong number of arguments specified to the worker.\n";
		std::cout << "Usage: Client.exe <serverPort> <xmlfilPath>\n";
		std::cout << "My args: " << argv << "\n";
		return 1;
	}


	int serverPort = atoi(argv[0]);
	std::string xmlFileName = argv[1];


	// This just proves we CAN read in an xml file! Need to shift some of the processing to here.
	XmlProcessing::XmlParser parser(xmlFileName, XmlProcessing::XmlParser::sourceType::file);
	XmlProcessing::XmlDocument* pDoc = parser.buildDocument();

	// Create a comm, send out to the server (known as 9091 right now)
	SocketSystem ss;
	EndPoint serverEP("localhost", serverPort);
	EndPoint clientEP("localhost", 9999);
	Comm comm(clientEP, "clientComm");
	comm.start();
	std::cout << "Client: Comm started, creating message\n";
	Message msg(serverEP, clientEP);
	msg.name("REQUEST");
	msg.command("Dllname.dll");
	comm.postMessage(msg);
	comm.stop();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
