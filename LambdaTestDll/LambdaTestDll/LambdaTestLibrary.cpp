// LambdaTestLibrary.cpp - Defines the exported functions for the DLL

#include "pch.h"
#include <functional>
#include "LambdaTestLibrary.h"
#include <iostream>

bool Test() {

	std::function<int(void)> f2 = []() {return 2;};
	if (f2() == 2) {
		std::cout << "Lamda test returned two, failure case.\n";
		return false;
	};
	return true;
}