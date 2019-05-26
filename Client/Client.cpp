// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

// Read in XML, parse the XML
#include "..\TestHarness\XmlDocument\XmlParser\XmlParser.h"
#include "..\TestHarness\XmlDocument\XmlDocument\XmlDocument.h"

using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;

int main() {
	std::cout << "Hello from Client.exe!\n";

	// This just proves we CAN read in an xml file! Need to shift some of the processing to here.
	std::string xmlFileName = "../TestHarness/xmlfiles/test1.xml";
	XmlProcessing::XmlParser parser(xmlFileName, XmlProcessing::XmlParser::sourceType::file);
	XmlProcessing::XmlDocument* pDoc = parser.buildDocument();
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
