#pragma once
#ifndef TESTHARNESS_H
#define TESTHARNESS_H

#include <initializer_list>
#include "Logger.h"

class TestHarness {
private:
	Logger log; //Logger package

public:
	TestHarness(Logger myLogger);
	bool TestLibrary(::std::string libname);
	//destructor
	~TestHarness();
};

#endif