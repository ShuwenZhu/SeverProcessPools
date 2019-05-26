#pragma once
#ifndef TESTHARNESS_H
#define TESTHARNESS_H

#include <initializer_list>
#include "LoggerTH.h"

class TestHarness {
private:
	LoggerTH log; //Logger package

public:
	TestHarness(LoggerTH myLogger, int workerCount);
	bool TestLibrary(::std::string libname);
	bool handleTestSequence(::std::string xmlSequence);
	//destructor
	~TestHarness();
};

#endif