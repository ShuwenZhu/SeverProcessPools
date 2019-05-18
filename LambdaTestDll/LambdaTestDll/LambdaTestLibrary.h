// LambdaTestLibrary.h - contains declarations of lambda test
#pragma once


#ifdef LAMBDATESTLIBRARY_EXPORTS
#define LAMBDATESTLIBRARY_API __declspec(dllexport)
#else
#define LAMBDATESTLIBRARY_API __declspec(dllimport)
#endif

extern "C" LAMBDATESTLIBRARY_API bool Test();