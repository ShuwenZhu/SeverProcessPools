// DivideTestLibrary.cpp : defines the exported functions for the DLL

#include "pch.h"
#include "DivideTestLibrary.h"
#include "DivideTest.h"


bool Test(void) {
	DivideTest divideTest;
	divideTest.setValue(5);
	divideTest();
	return true;
}