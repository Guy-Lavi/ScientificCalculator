#include "MyMath.h"

const long double MyMath::pi = 3.141592653589793238462643383279502884L;
const long double MyMath::e  = 2.718281828459045235360287471352662497L;

using namespace std;

MyMath::MyMath(int precision) {
	// Set precision after the decimal point
	_precision = 0.1;
	for (auto i = 1; i < precision; i++) 	{
		_precision *= 0.1;
	}
}

long double MyMath::mySin(long double rad) {
	// get from any radian range to only (-pi, pi) range, using the fact that sine is 2pi periodic
	rad -= (floor((rad + MyMath::pi) / (2*MyMath::pi))) * (2*MyMath::pi);

	// Calc sine Taylor series
	long double sinRad = rad;
	long double nextTerm = rad;
	long n = 0L;

	do {
		// Next term in Taylor series
		nextTerm *= -(rad * rad) / ((2.0L*n+2L)*(2.0L*n+3L));
		++n;
		sinRad += nextTerm;
	} while (abs(nextTerm) > _precision); // Check precision

	return sinRad;
}

long double MyMath::myCos(long double rad) {
	return mySin(MyMath::pi /2.0L - rad);
}

long double MyMath::myTan(long double rad) {
	auto cosResult = myCos(rad);
	if (0 == cosResult) {
		throw runtime_error("Math error: division by zero in Tan function");
	}
	return mySin(rad) / cosResult;
}

long double MyMath::myCot(long double rad) {
	auto sinResult = mySin(rad);
	if (0 == sinResult) {
		throw runtime_error("Math error: division by zero in Cot function");
	}
	return myCos(rad) / sinResult;
}

long double MyMath::myLn(long double z) {
	if (!(z > 0)) {
		throw runtime_error("Math error: real logarithm is defined only for z > 0");
	}

	// reduce z to the form z = a*e^b -> ln(z) = b + ln(a) where a is less than e
	int b = 0;
	long double eToB = 1;

	while ((eToB*MyMath::e) < z) {
		eToB *= MyMath::e;
		++b;
	}

	// Calculate using area of hyperbolic tangent method (see wikipedia link)
	long double a = z / eToB;
	long double lna = 0, nextTerm;
	int k = 0;

	do {
		nextTerm = (2.0L / (2.0L * k + 1)) * myPow((a - 1)/(a + 1), 2*k+1);
		++k;
		lna += nextTerm;
	} while (abs(nextTerm) > _precision); // Check precision

	return (lna + b);
}


long double MyMath::myLog(long double z, long double base) {
	if (!(base > 0)) {
		throw runtime_error("Math error: real logarithm is defined only for base > 0");
	}
	return myLn(z) / myLn(base);
}

long double MyMath::myPow(long double a, int n) {
	if (n < 0) {
		throw runtime_error("Math error: simple power defined on n >= 0 only");
	}

	long double power = 1;
	for (int i = 0; i < n; ++i) {
		power *= a;
	}
	
	return power;
}

long double MyMath::smothify(long double result) {

	const long double significantDigits = 1000000.0L;

	if (abs(result * significantDigits - round(result * significantDigits)) < 1.0L / significantDigits) {
		result = static_cast<long double>(static_cast<long long int>(round(result * significantDigits))) / significantDigits;
	}

	return result;
}