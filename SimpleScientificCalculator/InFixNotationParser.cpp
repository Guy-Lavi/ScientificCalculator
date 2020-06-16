#include "InFixNotationParser.h"

// get the content of a parentheses term, with the enclosing parentheses
string parseParentheses(const string& inputLine, int& charactersRead) {
	string term;

	if ('(' == inputLine[charactersRead]) {
		int parenthesesCount = 1;
		term += inputLine[charactersRead];
		charactersRead += 1;
		while (charactersRead < inputLine.size() && parenthesesCount != 0) {
			term += inputLine[charactersRead];
			if ('(' == inputLine[charactersRead]) {
				++parenthesesCount;
			}
			else if (')' == inputLine[charactersRead]) {
				--parenthesesCount;
			}
			++charactersRead;
		}

		if (parenthesesCount != 0) {
			throw runtime_error("Unbalanced parentheses detected: " + inputLine);
		}
	}
	else {
		throw runtime_error("No parentheses detected in: " + inputLine);
	}

	return term;
}

string InFixNotationParser::parseGetResult(const string& inputLine) {
	VariableName varName;
	LinearElement result = InFixNotationParser::parseGetResult(inputLine, varName);

	if (!varName.isEquation()) {
		// Calculation
		return ((stringstream() << result).str());
	}
	else {
		// Equation
		return ((stringstream() << varName.getName() << " = " << result).str());
	}
}

LinearElement InFixNotationParser::parseGetResult(const string &inputLine, VariableName &varName) {
	string lowerCased;
	int diff = 'A' - 'a';
	for (int i = 0; i < inputLine.size(); ++i) {
		if (inputLine[i] >= 'A' && inputLine[i] <= 'Z') {
			lowerCased.push_back(inputLine[i] - diff);
		} else {
			lowerCased.push_back(inputLine[i]);
		}
	}
	
	LinearElement result;
	// look for '=' sign, which means an equation needs to be solved

	// An expression
	if (lowerCased.find('=') == std::string::npos) {
		result = parseSumOfTerms(lowerCased, varName);
	}
	// An equation
	else {
		varName.setEquation();

		// get both sides of the equation
		vector<string> equationSplit;
		stringstream ss(lowerCased);
		string side;
		while (getline(ss, side, '=')) {
			equationSplit.push_back(side);
		}

		if (equationSplit.size() != 2) {
			throw runtime_error("Only single '=' sign is allowed in euqation");
		}

		auto result1 = parseSumOfTerms(equationSplit[0], varName);
		auto result2 = parseSumOfTerms(equationSplit[1], varName);

		// Coefficients
		if (0 == result1.a()) {
			// both X coefficients are 0
			if (0 == result2.a()) {
				if (result1.b() == result2.b()) {
					throw runtime_error("Equation of type 0=0, solution is all real numbers");
				}
				else {
					throw runtime_error("Equation of type 1=0, there is no solution");
				}
			}
			// only left side X coefficient is 0
			else {
				return LinearElement(0, (result1.b() - result2.b()) / result2.a());
			}
		}
		// left side X coefficient is not 0
		else {
			// both X coefficients are equal 
			if (result1.a() == result2.a()) {
				if (result1.b() == result2.b()) {
					throw runtime_error("Equation of type 0=0, solution is all real numbers");
				}
				else {
					throw runtime_error("Equation of type 1=0, there is no solution");
				}
			}
			// solution exists
			else {
				return LinearElement(0, (result2.b() - result1.b()) / (result1.a() - result2.a()));
			}
		}
	}

	return result;
}

LinearElement InFixNotationParser::parseSumOfTerms(const string& inputLine, VariableName& varName) {
	if (inputLine.size() < 1) {
		throw runtime_error("Empy input parseSumOfTerms");
	}

	// split to terms
	int parenthesesCount = 0;
	vector<string> terms;
	string term;
	for (int i = 0; i < inputLine.size(); ++i) {
		if (' ' == inputLine[i]) continue;
		if ('(' == inputLine[i]) {
			++parenthesesCount;
		}
		else if (')' == inputLine[i]) {
			if (parenthesesCount <= 0) {
				throw runtime_error("Uneven parentheses at location " + i);
			}
			--parenthesesCount;
		}

		if (parenthesesCount != 0) {
			term += inputLine[i];
			continue;
		}

		if ('-' == inputLine[i]) {
			if (term.size() > 0) {
				terms.push_back(term);
				term.clear();
			}
			term += inputLine[i];
		}
		else if ('+' == inputLine[i]) {
			terms.push_back(term);
			term.clear();
		}
		else {
			term += inputLine[i];
		}
	}

	if (term.size() > 0) {
		terms.push_back(term);
		term.clear();
	}

	// sum all terms
	LinearElement sum;
	for (auto termItr: terms) {
		sum = sum + parseNextTerm(termItr, varName);
	}

	return sum;
}

LinearElement InFixNotationParser::execFuntion(const string& inputLine, int &charactersRead, MyMath* myMathPtr, long double (MyMath::* functionPtr)(long double), VariableName& varName) {
	LinearElement result;

	// parentheses inside function
	if ('(' == inputLine[charactersRead]) {
		string term = parseParentheses(inputLine, charactersRead);
		result = parseSumOfTerms(term.substr(1, static_cast<size_t>(term.size()-2)), varName);
	}
	// single term inside function
	else {
		string term;
		int i = charactersRead;
		while (i < inputLine.size() && inputLine[i] != '(' && inputLine[i] != ')' && inputLine[i] != '+' && inputLine[i] != '-' &&
			inputLine[i] != '*' && inputLine[i] != '//' && inputLine[i] != 'x' && inputLine[i] != 'y') {
			term += inputLine[i++];
		}
		charactersRead = i;

		result = parseNextTerm(term, varName);
	}

	if (0 != result.a()) {
		throw runtime_error("Linear equations allowed only, can't have a variable inside a function");
	}

	// execute function on a none-variable expression
	result.b() = (myMathPtr->*functionPtr)(result.b());

	return result;
}

LinearElement InFixNotationParser::parseNextTerm(const string& inputLine, VariableName& varName) {
	if (inputLine.size() < 1) {
		throw runtime_error("Empy input parseNextTerm");
	}

	// used to call single input math functions
	MyMath myMath(20),*myMathPtr;
	myMathPtr = &myMath;
	long double (MyMath:: * functionPtr)(long double);
	functionPtr = nullptr;

	LinearElement result;

	// pre-negative sign (-x)
	if (inputLine[0] == '-') {
		return -parseNextTerm(inputLine.substr(1), varName);
	}

	int charactersRead = 0;
	// a number
	if (MyUtils::is_number(inputLine.substr(0, 1))) {
		while (inputLine[charactersRead] == '.' || (inputLine[charactersRead] >= '0' && inputLine[charactersRead] <= '9')) ++charactersRead;

		string numStr = inputLine.substr(0, charactersRead);

		long double num;
		// double
		if (numStr.find('.') != std::string::npos) {
			num = MyUtils::extractDouble(numStr);
		} // integer
		else {
			num = static_cast<long double>(stoll(numStr));
		}

		result = LinearElement(0 ,num);
	}
	// e
	else if (inputLine[0] == 'e') {
		++charactersRead;
		result = LinearElement(0, MyMath::e);
	}
	// pi
	else if (0 == inputLine.substr(0,2).compare("pi")) {
		charactersRead += 2;
		result = LinearElement(0, MyMath::pi);
	}
	// x / y
	else if (inputLine[0] == 'x' || inputLine[0] == 'y') {
		varName.setName(inputLine[0]);
		++charactersRead;
		result = LinearElement(1, 0);
	}
	// parentheses
	else if (inputLine[0] == '(') {
		int parenthesesCount = 1;
		charactersRead = 1;
		while (charactersRead < inputLine.size() && parenthesesCount != 0) {
			if ('(' == inputLine[charactersRead]) {
				++parenthesesCount;
			} else if (')' == inputLine[charactersRead]) {
				--parenthesesCount;
			}
			++charactersRead;
		}

		if (parenthesesCount != 0) {
			throw runtime_error("Unbalanced parentheses detected: " + inputLine);
		}

		result = parseSumOfTerms(inputLine.substr(1, static_cast<size_t>(charactersRead - 2)), varName);
	}
	// sin
	else if (0 == inputLine.substr(0, 3).compare("sin")) {
		charactersRead += 3;
		functionPtr = &(MyMath::mySin);
		result = execFuntion(inputLine, charactersRead, myMathPtr, functionPtr, varName);
	}
	// cos
	else if (0 == inputLine.substr(0, 3).compare("cos")) {
		charactersRead += 3;
		functionPtr = &(MyMath::myCos);
		result = execFuntion(inputLine, charactersRead, myMathPtr, functionPtr, varName);
	}
	// tan
	else if (0 == inputLine.substr(0, 3).compare("tan")) {
		charactersRead += 3;
		functionPtr = &(MyMath::myTan);
		result = execFuntion(inputLine, charactersRead, myMathPtr, functionPtr, varName);
	}
	// cot
	else if (0 == inputLine.substr(0, 3).compare("cot")) {
		charactersRead += 3;
		functionPtr = &(MyMath::myCot);
		result = execFuntion(inputLine, charactersRead, myMathPtr, functionPtr, varName);
	}
	// ln
	else if (0 == inputLine.substr(0, 2).compare("ln")) {
		charactersRead += 2;
		functionPtr = &(MyMath::myLn);
		result = execFuntion(inputLine, charactersRead, myMathPtr, functionPtr, varName);
	}
	// log - very complicated because all of those are legal: {log10, log(10), log10(20), log1.5, log(1.5), log1.5(1.5)}
	else if (0 == inputLine.substr(0, 3).compare("log")) {
		charactersRead += 3;
		long double base;
		// log with no base with parentheses
		if ('(' == inputLine[3]) {
			base = 10.0L;
			string term = parseParentheses(inputLine, charactersRead);
			result = parseSumOfTerms(term.substr(1, static_cast<size_t>(term.size() - 2)), varName);
		}
		else {
			long double number;

			if (0 == inputLine.substr(charactersRead, 2).compare("pi")) {
				charactersRead += 2;
				number = MyMath::pi;
			}
			else if (0 == inputLine.substr(charactersRead, 1).compare("e")) {
				charactersRead += 1;
				number = MyMath::e;
			}
			else {
				string numStr;
				int i = charactersRead;
				while (i < inputLine.size() && ('.' == inputLine[i] || (inputLine[i] >= '0' && inputLine[i] <= '9'))) {
					numStr += inputLine[i++];
				}
				charactersRead = i;

				if (numStr.find('.') != std::string::npos) {
					number = MyUtils::extractDouble(numStr);
				}
				else if (MyUtils::is_number(numStr)) {
					number = static_cast<long double>(stoll(numStr));
				}
				else {
					throw runtime_error("Failed to parse log base/param :" + inputLine);
				}
			}

			// log with no base and no parentheses
			if ('(' != inputLine[charactersRead]) {
				base = 10.0L;
				result = LinearElement(0, number);
			}
			// log with base and with parentheses
			else {
				base = number;
				string term = parseParentheses(inputLine, charactersRead);
				result = parseSumOfTerms(term.substr(1, static_cast<size_t>(term.size() - 2)), varName);
			}
		}

		if (0 != result.a()) {
			throw runtime_error("Linear equations allowed only, can't have a variable inside a function");
		}

		result.b() = myMath.myLog(result.b(), base);
	}
	// input error
	else {
		throw runtime_error("Unable to parse: " + inputLine);
	}

	/* handle following operand */
	if (charactersRead < inputLine.size()) {
		// '*' - multiplication
		if ('*' == inputLine[charactersRead]) {
			charactersRead += 1;
			result = result * parseNextTerm(inputLine.substr(charactersRead), varName);
		}
		// '/' - division
		else if ('/' == inputLine[charactersRead]) {
			charactersRead += 1;
			result = result / parseNextTerm(inputLine.substr(charactersRead), varName);
		}
		// multiplication with next term, missing/silent '*' sign (i.e. - 2x or 2pi instead of 2*x or 2*pi)
		else {
			result = result * parseNextTerm(inputLine.substr(charactersRead), varName);
		}
	}

	result.a() = MyMath::smothify(result.a());
	result.b() = MyMath::smothify(result.b());
	
	return result;
}