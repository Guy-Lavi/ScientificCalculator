#pragma once
#include <iostream>

/**
 * MyMath
 * This class is used to calculate the elementry functions required in the calculator without using any math libraries
 * It takes the desired approx precision after the decimal point as an input
 */

class MyMath {
	public:
		// Ctor with number of digits of precision after the decimal point. For example: 3->0.001
		MyMath(int precision);

		// The pi constant
		static const long double pi;

		// Euler’s constant
		static const long double e;

		// Calculate a sine approximation
		long double mySin(long double rad);

		// Calculate a cosine approximation
		long double myCos(long double rad);

		// Calculate a tangent approximation
		long double myTan(long double rad);

		// Calculate a cotangent approximation
		long double myCot(long double rad);

		// Calculate natural logarithm, based on optimization here: https://en.wikipedia.org/wiki/Logarithm#Calculation
		long double myLn(long double z);

		// Calculate logarith with any base
		long double myLog(long double z, long double base);

		// Simply calculate power
		long double myPow(long double a, int n);

		// due to inaccuracies like pi not being truely pi, this function is used to smoth really
		// small numbers (like when sin(pi) gives ~1*10^-16 instead of 0)
		static long double smothify(long double result);

	private:
		// Minimum number of digits of precision after the decimal point
		long double _precision;
};

