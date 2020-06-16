#include <iostream>
#include <string> 
#include "MyMath.h"
#include "DisplayManager.h"
#include "PostFixNotationParser.h"
#include <exception>
#include <typeinfo>
#include <sstream>
#include "myUtils.h"
#include "InFixNotationParser.h"

using namespace std;

/**
 * SimpleScientificCalculator
 * This file has main function, manages all the basic consule UI
 */

string displayAndReadLine(DisplayManager &displayManager) {
	displayManager.display();
	// read input
	cout << ">> ";
	string line;
	getline(cin, line);
	displayManager.addRollingLine(line);
	return line;
}

void infixNotationScreen() {
	DisplayManager inFixScreen(28);

	inFixScreen.addPersistentLine("<--- Infix Notation and linear equations solver --->");
	inFixScreen.addPersistentLine("");
	inFixScreen.addPersistentLine("-> Enter a calculation or equation in infix notation or 'q' to go back:");

	string inputLine;
	do {
		// read input
		inputLine = displayAndReadLine(inFixScreen);
		// quit
		if (0 == inputLine.compare("q")) break;
		// blank line
		if (0 == inputLine.size()) continue;
		try {
			string result = InFixNotationParser::parseGetResult(inputLine);
			inFixScreen.addRollingLine(result);
		}
		catch (exception & e) {
			inFixScreen.addRollingLine(MyUtils::getExceptionMessage("Error for input: " + inputLine, e));
		}
	} while (1);
}

void postfixNotationScreen() {
	DisplayManager postFixScreen(28);

	postFixScreen.addPersistentLine("<--- PostFix Notation (RPN) --->");
	postFixScreen.addPersistentLine("");
	postFixScreen.addPersistentLine("-> Enter a calculation in postfix notation (use spaces) or 'q' to go back:");

	string inputLine;
	do {
		// read input
		inputLine = displayAndReadLine(postFixScreen);
		// quit
		if (0 == inputLine.compare("q")) break;
		// blank line
		if (0 == inputLine.size()) continue;
		try {
			long double result = PostFixNotationParser::parseGetResult(inputLine);
			postFixScreen.addRollingLine((stringstream() << result).str());
		}
		catch (exception & e) {
			postFixScreen.addRollingLine(MyUtils::getExceptionMessage("Error for input: " + inputLine, e));
		}
	} while (1);
}

void manageWelcomeScreen() {
	DisplayManager welcomeScreen(28);

	welcomeScreen.addPersistentLine("<--- Welcome to SimpleScientificCalculator! --->");
	welcomeScreen.addPersistentLine("");
	welcomeScreen.addPersistentLine("-> Main menu:");
	welcomeScreen.addPersistentLine("---> 1 - for infix notation with linear equations solver");
	welcomeScreen.addPersistentLine("---> 2 - for postfix (RPN) notation");
	welcomeScreen.addPersistentLine("---> q - to exit");
	
	char choice;
	do {
		choice = welcomeScreen.readMenu(vector<char>{'1', '2', 'q'});
		switch (choice) {
		case '1': {
			infixNotationScreen();
			break;
		}
		case '2': {
			postfixNotationScreen();
			break;
		}
		default:
			break;
		}
	} while (choice != 'q');
}

int main() {

	manageWelcomeScreen();

	cout << "Thanks for using SimpleScientificCalculator!" << endl << "Goodbye!" << endl;

	return 0;
}
