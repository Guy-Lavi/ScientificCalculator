#include "pch.h"
#include <time.h> 
#include <iostream>
#include "CppUnitTest.h"
#include "../SimpleScientificCalculator/MyMath.h"
#include "../SimpleScientificCalculator/PostFixNotationParser.h"
#include "../SimpleScientificCalculator/InFixNotationParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

const long double smallPrecision = 0.0000000001;

bool checkSin(MyMath &myMath, const long double checkRad) {
	return (abs(myMath.mySin(checkRad) - sin(checkRad)) < smallPrecision);
}

bool checkCos(MyMath& myMath, const long double checkRad) {
	return (abs(myMath.myCos(checkRad) - cos(checkRad)) < smallPrecision);
}

bool checkTan(MyMath& myMath, const long double checkRad) {
	return (abs(myMath.myTan(checkRad) - tan(checkRad)) < smallPrecision);
}

bool checkCot(MyMath& myMath, const long double checkRad) {
	return (abs(myMath.myCot(checkRad) - 1.0L/tan(checkRad)) < smallPrecision);
}

bool checkLn(MyMath& myMath, const long double checkNum) {
	return (abs(myMath.myLn(checkNum) - log(checkNum)) < smallPrecision);
}

bool checkLog(MyMath& myMath, const long double checkNum, const long double base) {
	return (abs(myMath.myLog(checkNum, base) - log(checkNum)/ log(base)) < smallPrecision);
}

void AssertEqualStrings(string s1, string s2) {
	Assert::IsTrue(0 == s1.compare(s2));
}

void AssertExpectedResultInFix(string input, string result) {
	AssertEqualStrings(InFixNotationParser::parseGetResult(input), result);
}

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		
		TEST_METHOD(SinUniTest)
		{
			MyMath myMathHighPercision(20);
			MyMath myMathLowPercision(2);

			// Check a range of angles
			for (int i = -20; i < 21; ++i) {
				long double rad = 0.1L + i;
				Assert::IsTrue(checkSin(myMathHighPercision, rad));
				Assert::IsFalse(checkSin(myMathLowPercision, rad));
			}

			// High precision random check in range (-pi, pi)
			srand(static_cast<int>(time(0)));
			for (int i = 0; i < 1000; ++i) {
				long double rad = ((1.0L+(rand() % 9999))/9999.0L) * 2.0L * MyMath::pi - MyMath::pi;
				Assert::IsTrue(checkSin(myMathHighPercision, rad));
			}
		}

		TEST_METHOD(CosUniTest)
		{
			MyMath myMathHighPercision(20);
			MyMath myMathLowPercision(2);

			// Check a range of angles
			for (int i = -20; i < 21; ++i) {
				long double rad = 0.1L + i;
				Assert::IsTrue(checkCos(myMathHighPercision, rad));
				Assert::IsFalse(checkCos(myMathLowPercision, rad));
			}

			// High precision random check in range (-pi, pi)
			srand(static_cast<int>(time(0)));
			for (int i = 0; i < 1000; ++i) {
				long double rad = ((1.0L + (rand() % 9999)) / 9999.0L) * 2.0L * MyMath::pi - MyMath::pi;
				Assert::IsTrue(checkCos(myMathHighPercision, rad));
			}
		}

		TEST_METHOD(TanUniTest)
		{
			MyMath myMathHighPercision(20);
			MyMath myMathLowPercision(2);

			// Check a range of angles
			for (int i = -20; i < 21; ++i) {
				long double rad = 0.1L + i;
				Assert::IsTrue(checkTan(myMathHighPercision, rad));
				Assert::IsFalse(checkTan(myMathLowPercision, rad));
			}

			// High precision random check around range (-pi/2, pi/2)*0.99 (avoid singularity - where precision is low)
			srand(static_cast<int>(time(0)));
			for (int i = 0; i < 1000; ++i) {
				long double rad = (((1.0L + (rand() % 9999)) / 9999.0L) * MyMath::pi - MyMath::pi/2.0L) * 0.99L;
				Assert::IsTrue(checkTan(myMathHighPercision, rad));
			}

			// Check for division by 0 exception
			bool assertionCought = false;
			try {
				myMathHighPercision.myTan(MyMath::pi / 2);
			}
			catch (const std::exception&) {
				assertionCought = true;
			}
			Assert::IsTrue(assertionCought);
		}

		TEST_METHOD(CotUniTest)
		{
			MyMath myMathHighPercision(20);
			MyMath myMathLowPercision(2);

			// Check a range of angles
			for (int i = -20; i < 21; ++i) {
				long double rad = 0.1L + i;
				Assert::IsTrue(checkCot(myMathHighPercision, rad));
				Assert::IsFalse(checkCot(myMathLowPercision, rad));
			}

			// High precision random check around range (0, pi) * 0.98 (avoid singularity - where precision is low)
			srand(static_cast<int>(time(0)));
			for (int i = 0; i < 1000; ++i) {
				long double rad = (((1.0L + (rand() % 9999)) / 9999.0L) * MyMath::pi) * 0.98L + 0.01L;
				Assert::IsTrue(checkCot(myMathHighPercision, rad));
			}

			// Check for division by 0 exception
			bool assertionCought = false;
			try {
				myMathHighPercision.myCot(0);
			}
			catch (const std::exception &) {
				assertionCought = true;
			}
			Assert::IsTrue(assertionCought);
		}

		TEST_METHOD(LnUniTest)
		{
			MyMath myMathHighPercision(20);
			MyMath myMathLowPercision(1);

			// Check a large range of numbers
			for (int i = 2; i < 1000; ++i) {
				Assert::IsTrue(checkLn(myMathHighPercision, i));
				Assert::IsFalse(checkLn(myMathLowPercision, i));
			}

			// High precision random check in range (0, 20)
			srand(static_cast<int>(time(0)));
			for (int i = 0; i < 1000; ++i) {
				long double checkNum = 20.0L * (1.0L + (rand() % 19999))/20000.0L;
				Assert::IsTrue(checkLn(myMathHighPercision, checkNum));
			}

			// Check for negative number exception
			bool assertionCought = false;
			try {
				myMathHighPercision.myLn(-5);
			}
			catch (const std::exception&) {
				assertionCought = true;
			}
			Assert::IsTrue(assertionCought);
		}

		TEST_METHOD(LogUniTest)
		{
			MyMath myMathHighPercision(20);

			// Check a large range of numbers and bases
			for (int base = 2; base < 100; base += 4) {
				for (int i = 2; i < 1000; i += 2) {
					Assert::IsTrue(checkLog(myMathHighPercision, i, base));
				}
			}

			// High precision random check in range (0, 20), bases in range (0, 50)
			srand(static_cast<int>(time(0)));
			for (int j = 0; j < 50; ++j) {
				long double base = 50.0L * (1.0L + (rand() % 29999)) / 30000.0L;
				for (int i = 0; i < 1000; ++i) {
					long double checkNum = 20.0L * (1.0L + (rand() % 19999)) / 20000.0L;
					Assert::IsTrue(checkLog(myMathHighPercision, checkNum, base));
				}
			}

			// Check for negative number exception
			bool assertionCought = false;
			try {
				myMathHighPercision.myLog(-5, 2);
			}
			catch (const std::exception&) {
				assertionCought = true;
			}
			Assert::IsTrue(assertionCought);

			assertionCought = false;
			try {
				myMathHighPercision.myLog(5, -2);
			}
			catch (const std::exception&) {
				assertionCought = true;
			}
			Assert::IsTrue(assertionCought);
		}

		TEST_METHOD(InFixNotationParserUniTest)
		{
			string exceptionMsg;

			// Tests from Email
			/*
			* input: (3+(4-1))*5
			* output: 30
			*/
			AssertExpectedResultInFix("(3+(4-1))*5", "30");

			/*
			* input: 2 * x + 0.5 = 1
			* output: x = 0.25
			*/
			AssertExpectedResultInFix("2 * x + 0.5 = 1", "x = 0.25");

			/*
			* input: 2x + 1 = 2(1-x)
			* output: x = 0.25
			*/
			AssertExpectedResultInFix("2x + 1 = 2(1-x)", "x = 0.25");

			/*
			* input: Log(10)
			* output: 1
			*/
			AssertExpectedResultInFix("Log(10)", "1");
			
			/*
			* input: Log10
			* output: 1
			*/
			AssertExpectedResultInFix("Log10", "1");

			/*
			* input: Log100(10)
			* output : 0.5
			*/
			AssertExpectedResultInFix("Log100(10)", "0.5");

			/*
			* input: sin(pi) or sinpi
			* output : 0
			*/
			AssertExpectedResultInFix("sin(pi)", "0");
			AssertExpectedResultInFix("sinpi", "0");

			/*
			* input : sin(1.5pi) or sin(1.5 * pi)
			* output : -1
			*/
			AssertExpectedResultInFix("sin(1.5pi)", "-1");
			AssertExpectedResultInFix("sin(1.5 * pi)", "-1");

			// Equations
			AssertExpectedResultInFix("1 = x", "x = 1");
			AssertExpectedResultInFix("x + x + x = x", "x = 0");
			AssertExpectedResultInFix("y + y + y = y + y + 100", "y = 100");
			AssertExpectedResultInFix("1 = 2y", "y = 0.5");
			AssertExpectedResultInFix("pix = 1", "x = 0.31831");
			AssertExpectedResultInFix("sin(pi/2) + xlog9(81) = -ln(e*e*e)x + log2(32)", "x = 0.8");
		
			// addition test
			AssertExpectedResultInFix("1 + 2", "3");

			// subtraction
			AssertExpectedResultInFix("-1 - 2", "-3");

			// multipication
			AssertExpectedResultInFix("11.5 * (- 2)", "-23");

			// division
			AssertExpectedResultInFix("15/2", "7.5");

			// logs
			AssertExpectedResultInFix("LOG10", "1");
			AssertExpectedResultInFix("Log100", "2");
			AssertExpectedResultInFix("Log100(10)", "0.5");
			AssertExpectedResultInFix("loge(e)", "1");
			AssertExpectedResultInFix("log3(9)", "2");
			AssertExpectedResultInFix("log25(5)", "0.5");
			AssertExpectedResultInFix("log19(1)", "0");

			// ln
			AssertExpectedResultInFix("lne", "1");
			AssertExpectedResultInFix("ln(e*e)", "2");
			AssertExpectedResultInFix("ln(1)", "0");

			// sin
			AssertExpectedResultInFix("sin0", "0");
			AssertExpectedResultInFix("sin(pi/2)", "1");
			AssertExpectedResultInFix("sin(-pi/2)", "-1");
			AssertExpectedResultInFix("sin(2pi)", "0");

			// cos
			AssertExpectedResultInFix("cos0", "1");
			AssertExpectedResultInFix("cos(pi/2)", "0");
			AssertExpectedResultInFix("COS(-pi/2)", "0");
			AssertExpectedResultInFix("Cos(2pi)", "1");

			// tan
			AssertExpectedResultInFix("tan0", "0");
			AssertExpectedResultInFix("tan(pi/4)", "1");
			AssertExpectedResultInFix("tan(-pi/4)", "-1");
			try {
				auto result = InFixNotationParser::parseGetResult("tan(pi/2)");
			}
			catch (const std::exception& e) {
				exceptionMsg = e.what();
			}
			AssertEqualStrings(exceptionMsg, "Math error: division by zero in Tan function");

			// cot
			AssertExpectedResultInFix("Cot(pi/2)", "0");
			AssertExpectedResultInFix("cot(pi/4)", "1");
			AssertExpectedResultInFix("cot(-pi/4)", "-1");
			try {
				auto result = InFixNotationParser::parseGetResult("cot0");
			}
			catch (const std::exception & e) {
				exceptionMsg = e.what();
			}
			AssertEqualStrings(exceptionMsg, "Math error: division by zero in Cot function");

			// complex test
			AssertExpectedResultInFix("((15 / (7 - (1 + 1))) * 3) - (2 + (1 + 1))", "5");
			AssertExpectedResultInFix("Log9(Log100((sin(pi/2) + cos(0)) * 5) * 7 - 0.5) + Ln(e*e)", "2.5");
			AssertExpectedResultInFix("24/log10 = 2x", "x = 12");
			AssertExpectedResultInFix("2sinpi + cospi = 3x", "x = -0.333333");
			AssertExpectedResultInFix("lne + log10 + log100(10) + x = 0", "x = -2.5");
			AssertExpectedResultInFix("2(1 + (2 + (3 + (4*5)/10)))", "16");
		}

		TEST_METHOD(PostFixNotationParserUniTest)
		{
			double result;
			bool assertionCought;

			// addition test
			result = PostFixNotationParser::parseGetResult("1 2 +");
			Assert::AreEqual(result, 3.0);

			// subtraction
			result = PostFixNotationParser::parseGetResult("-1 2 -");
			Assert::AreEqual(result, -3.0);

			// multipication
			result = PostFixNotationParser::parseGetResult("11 -2 *");
			Assert::AreEqual(result, -22.0);

			// division
			result = PostFixNotationParser::parseGetResult("15 2 /");
			Assert::AreEqual(result, 7.5);

			// logs
			result = PostFixNotationParser::parseGetResult("10 log");
			Assert::AreEqual(result, 1.0);
			result = PostFixNotationParser::parseGetResult("100 log");
			Assert::AreEqual(result, 2.0);
			result = PostFixNotationParser::parseGetResult("10 log100");
			Assert::AreEqual(result, 0.5);
			result = PostFixNotationParser::parseGetResult("e LogE");
			Assert::AreEqual(result, 1.0);
			result = PostFixNotationParser::parseGetResult("9 Log3");
			Assert::AreEqual(result, 2.0);
			result = PostFixNotationParser::parseGetResult("5 Log25");
			Assert::AreEqual(result, 0.5);
			result = PostFixNotationParser::parseGetResult("1 LOG19");
			Assert::AreEqual(result, 0.0);

			// ln
			result = PostFixNotationParser::parseGetResult("e ln");
			Assert::AreEqual(result, 1.0);
			result = PostFixNotationParser::parseGetResult("e e * ln");
			Assert::AreEqual(result, 2.0);
			result = PostFixNotationParser::parseGetResult("1 ln");
			Assert::AreEqual(result, 0.0);

			// sin
			result = PostFixNotationParser::parseGetResult("0 sin");
			Assert::AreEqual(result, 0.0);
			result = PostFixNotationParser::parseGetResult("pi 2 / sin");
			Assert::AreEqual(result, 1.0);
			result = PostFixNotationParser::parseGetResult("-pi 2 / sin");
			Assert::AreEqual(result, -1.0);
			result = PostFixNotationParser::parseGetResult("pi 2 * sin");
			Assert::AreEqual(result, 0.0);

			// cos
			result = PostFixNotationParser::parseGetResult("0 cos");
			Assert::AreEqual(result, 1.0);
			result = PostFixNotationParser::parseGetResult("pi 2 / cos");
			Assert::AreEqual(result, 0.0);
			result = PostFixNotationParser::parseGetResult("-pi 2 / cos");
			Assert::AreEqual(result, 0.0);
			result = PostFixNotationParser::parseGetResult("pi 2 * cos");
			Assert::AreEqual(result, 1.0);

			// tan
			result = PostFixNotationParser::parseGetResult("0 tan");
			Assert::AreEqual(result, 0.0);
			result = PostFixNotationParser::parseGetResult("pi 4 / tan");
			Assert::AreEqual(result, 1.0);
			result = PostFixNotationParser::parseGetResult("-pi 4 / tan");
			Assert::AreEqual(result, -1.0);
			assertionCought = false;
			try {
				result = PostFixNotationParser::parseGetResult("pi 2 / tan");
			}
			catch (const std::exception&) {
				assertionCought = true;
			}
			Assert::IsTrue(assertionCought);

			// cot
			result = PostFixNotationParser::parseGetResult("pi 2 / cot");
			Assert::AreEqual(result, 0.0);
			result = PostFixNotationParser::parseGetResult("pi 4 / cot");
			Assert::AreEqual(result, 1.0);
			result = PostFixNotationParser::parseGetResult("-pi 4 / cot");
			Assert::AreEqual(result, -1.0);
			assertionCought = false;
			try {
				result = PostFixNotationParser::parseGetResult("0 cot");
			}
			catch (const std::exception&) {
				assertionCought = true;
			}
			Assert::IsTrue(assertionCought);

			// complex test - ((15 / (7 - (1 + 1))) * 3) - (2 + (1 + 1)) == 5
			result = PostFixNotationParser::parseGetResult("15 7 1 1 + - / 3 * 2 1 1 + + -");
			Assert::AreEqual(result, 5.0);

			// complex test - Log9(Log100((sin(pi/2) + cos(0)) * 5) * 7 - 0.5) + Ln(e*e) == 2.5
			result = PostFixNotationParser::parseGetResult("pi 2 / sin 0 cos + 5 * Log100 7 * 0.5 - LOG9 e E * LN +");
			Assert::AreEqual(result, 2.5);
		}
	};
}
