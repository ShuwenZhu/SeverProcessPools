#ifdef DIVIDETESTLIBRARY_EXPORTS
#define DIVIDETESTLIBRARY_API __declspec(dllexport)
#else 
#define DIVIDETESTLIBRARY_API __declspec(dllimport)
#endif

//extern "C" DIVIDETESTLIBRARY_API bool test(void);
extern "C" __declspec(dllexport) bool Test(void);