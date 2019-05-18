#ifndef _DIVIDETEST_H_
#define _DIVIDETEST_H_

#include <iostream>

class DivideTest {
	int dividend = 30; // Hard-coded dividend number
	int divisor;
public:
	DivideTest() { divisor = 1; }
	void setValue(int x)
	{
		divisor = x;
	}
	int getValue()
	{
		return divisor;
	}
	DivideTest(int a)
	{
		divisor = a;
	}
	void operator () ()
	{
		if (divisor == 0)
		{
			throw "Divided by zero exception!";
		}
		else
		{
			std::cout << "Done, the quotient is: " << dividend / divisor << std::endl;
		}
	}

};

#endif // !_DIVIDETEST_H_
