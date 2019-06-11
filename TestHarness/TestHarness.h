#pragma once
#ifndef TESTHARNESS_H
#define TESTHARNESS_H

#include <initializer_list>
#include "LoggerTH.h"
#include "Comm/MsgPassingComm/Comm.h"
using namespace MsgPassingCommunication;

class TestHarness {
private:
	LoggerTH log; //Logger package
	BlockingQueue<Message> readyQueue;
	BlockingQueue<Message> requestQueue;
	bool startWorker(int serverPort, int clientPort);
	void ProcessQueues(int serverPort);
public:
	void serverThreadFunction(int serverPort);

	TestHarness(LoggerTH myLogger);
	bool TestLibrary(::std::string libname);
	void StartHarnessServer(int serverPort);
	void StartWorkers(int serverPort, int workerCount);
	//destructor
	~TestHarness();
};

#endif