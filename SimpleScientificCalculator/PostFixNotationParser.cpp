#include "PostFixNotationParser.h"

using namespace std;

long double getStackTop(stack<long double> &calculationStack, string op) {
	if (calculationStack.empty()) {
		throw runtime_error("Error in parsing operator - not sufficient values supplied for this operator: " + op);
	}
	long double result = calculationStack.top();
	calculationStack.pop();
	return result;
}

long double PostFixNotationParser::parseGetResult(string inputLine) {

	stack<long double> calculationStack;
	MyMath myMath(20);

	// get all terms
	vector<string> termsSplit;
	stringstream ss(inputLine);
	string token;
	while (getline(ss, token, ' ')) {
		termsSplit.push_back(token);
	}

	// parse
	for (string term : termsSplit) {

		// negative number
		long double negIndicator = 1;
		if (term.size() > 1 && '-' == term[0]) {
			negIndicator = -1;
			term.erase(0, 1);
		}

		// double
		if (term.find('.') != std::string::npos) {
			calculationStack.push(MyUtils::extractDouble(term) * negIndicator);
		} // int
		else if (MyUtils::is_number(term)) {
			calculationStack.push(stoll(term) * negIndicator);
		} // pi
		else if (0 == term.compare("pi") || 0 == term.compare("PI") || 0 == term.compare("Pi")) {
			calculationStack.push(MyMath::pi * negIndicator);
		} // e
		else if (0 == term.compare("e") || 0 == term.compare("E")) {
			calculationStack.push(MyMath::e * negIndicator);
		} // sin
		else if (0 == term.compare("sin") || 0 == term.compare("Sin") || 0 == term.compare("SIN")) {
			calculationStack.push(myMath.mySin(getStackTop(calculationStack, "sin")));
		} // cos
		else if (0 == term.compare("cos") || 0 == term.compare("Cos") || 0 == term.compare("COS")) {
			calculationStack.push(myMath.myCos(getStackTop(calculationStack, "cos")));
		} // tan
		else if (0 == term.compare("tan") || 0 == term.compare("Tan") || 0 == term.compare("TAN")) {
			calculationStack.push(myMath.myTan(getStackTop(calculationStack, "tan")));
		} // cot
		else if (0 == term.compare("cot") || 0 == term.compare("Cot") || 0 == term.compare("COT")) {
			calculationStack.push(myMath.myCot(getStackTop(calculationStack, "cot")));
		} // +
		else if (0 == term.compare("+")) {
			auto parm2 = getStackTop(calculationStack, "+");
			auto parm1 = getStackTop(calculationStack, "+");
			calculationStack.push(parm1 + parm2);
		} // -
		else if (0 == term.compare("-")) {
			auto parm2 = getStackTop(calculationStack, "-");
			auto parm1 = getStackTop(calculationStack, "-");
			calculationStack.push(parm1 - parm2);
		} // *
		else if (0 == term.compare("*")) {
			auto parm2 = getStackTop(calculationStack, "*");
			auto parm1 = getStackTop(calculationStack, "*");
			calculationStack.push(parm1 * parm2);
		} // /
		else if (0 == term.compare("/")) {
			auto parm2 = getStackTop(calculationStack, "/");
			auto parm1 = getStackTop(calculationStack, "/");
			calculationStack.push(parm1 / parm2);
		} // Ln
		else if (0 == term.find("Ln") || 0 == term.find("ln") || 0 == term.find("LN")) {
			calculationStack.push(myMath.myLog(getStackTop(calculationStack, "Ln"), myMath.e));
		} // Log
		else if (0 == term.find("Log") || 0 == term.find("log") || 0 == term.find("LOG")) {
			long double base;
			if (3 == term.size()) {
				base = 10;
			} else if (4 == term.size() && (term[3] == 'e' || term[3] == 'E')) {
				base = MyMath::e;
			} else if (!MyUtils::is_number(term.substr(3))) {
				throw runtime_error("Error parsing log base from: " + term);
			} else {
				base = static_cast<long double>(stoll(term.substr(3)));
			}
			calculationStack.push(myMath.myLog(getStackTop(calculationStack, "log"), base));
		} // Error
		else {
			throw runtime_error("Error parsing term: " + term);
		}
	}

	if (calculationStack.size() != 1) {
		throw runtime_error("Error parsing command. Number of remaining terms: " + calculationStack.size());
	}

	long double result = calculationStack.top();

	return MyMath::smothify(result);
}