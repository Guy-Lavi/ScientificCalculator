#pragma once
#include <iostream>
#include <string> 
#include <stack>
#include <vector>
#include <sstream>
#include <algorithm>
#include "myUtils.h"
#include "MyMath.h"

using namespace std;

/**
 * PostFixNotationParser
 * Static class used to parse postfix notation (RPN)
 * supports:
 *    - addition
 *    - subtraction
 *    - multiplication
 *    - division
 *    - logarithms (log and ln)
 *    - trigonometric functions (sin, cos, tan, ctan)
 *	  - PI and Euler’s number
 *    - Doesn't use any standard math library
 */

class PostFixNotationParser {
	public:

		// Parse whole line expression, returns the result if line is legal
		static long double parseGetResult(string inputLine);
};

