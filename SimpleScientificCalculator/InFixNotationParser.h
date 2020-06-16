#pragma once
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <algorithm>
#include "myUtils.h"
#include "MyMath.h"
#include "LinearElement.h"

/**
 * InFixNotationParser
 * Static class used to parse infix notation
 * supports:
 *    - parentheses
 *    - addition
 *    - subtraction
 *    - multiplication
 *    - division
 *    - logarithms (log and ln)
 *    - trigonometric functions (sin, cos, tan, ctan)
 *	  - PI and Euler’s number
 *    - Linear equations solver with single variable
 *    - Uses a recursive language parser
 *    - Doesn't use any standard math library
 *    - Bonus feature - simplfies single variable linear expressions (for example: input: "19(2x + 1) + x" --> output: "39x+19" )
 */

class InFixNotationParser {
	public:
		// Parse whole line, calculation or equation, returns result in string form
		static string parseGetResult(const string& inputLine);

	private:
		// Parse whole line, calculation or equation, returns a linear element
		static LinearElement parseGetResult(const string& inputLine, VariableName& varName);

		// parse whole simple sum of terms, i.e. : A + B + ... + Z and return the sum
		static LinearElement parseSumOfTerms(const string& inputLine, VariableName& varName);

		// parse a single term element, i.e. 'A'
		static LinearElement parseNextTerm(const string& inputLine, VariableName& varName);

		// execute a math function (sin/cos/etc)
		static LinearElement execFuntion(const string& inputLine, int& charactersRead, MyMath* myMathPtr, long double (MyMath::* functionPtr)(long double), VariableName& varName);
};