#include "DisplayManager.h"

using namespace std;

DisplayManager::DisplayManager(int screenSize):	_screenSize(screenSize) {
}

void DisplayManager::addPersistentLine(string line) {
	_persistantLines.push_back(line);
}

void DisplayManager::addRollingLine(string line) {
	_rollingLines.push_back(line);
	while (_persistantLines.size() + _rollingLines.size() > _screenSize) {
		_rollingLines.erase(_rollingLines.begin());
	}
}

void DisplayManager::display() {
	system("cls");
	for (auto line : _persistantLines) {
		cout << line << endl;
	}

	for (auto line : _rollingLines) {
		cout << ">> " << line << endl;
	}
	cout << flush;
}

void DisplayManager::clearRollingLine() {
	_rollingLines.clear();
}

char DisplayManager::readMenu(vector<char> possibleChices) {
	bool instructionAdded = false;
	char choice;
	do {
		try {
			display();
			string str;
			getline(cin, str);

			if (1 == str.size()) {
				choice = str[0];
				if (find(possibleChices.begin(), possibleChices.end(), choice) != possibleChices.end()) {
					break;
				}
			}
			if (!instructionAdded) {
				string text = "Enter one of the following options: ";
				for (auto option : possibleChices) text = text + option + "/";
				text.pop_back();
				addRollingLine(text);

				instructionAdded = true;
			}
		}
		catch (...)	{ }
	} while (1);

	return choice;
}