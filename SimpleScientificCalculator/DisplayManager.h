#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

/**
 * DisplayManager
 * This class is used as a simple consule UI with a running lines buffer (keep up to ScreenSize lines on the screen)
 */

using namespace std;

class DisplayManager {
	public:
		// Ctor
		DisplayManager(int screenSize);

		// Add a line that is persistant on the screen
		void addPersistentLine(string line);

		// Add a line that is will be replaced once the screen is full
		void addRollingLine(string line);

		// Clear all rolling lines
		void clearRollingLine();

		// Display lines using current buffers
		void display();

		// get menu choice from the user
		char readMenu(vector<char> possibleChices);
	
	private:
		// number of lines on the screen
		int _screenSize;

		// persistent lines buffer
		vector<string> _persistantLines;
		
		// rolling lines buffer
		vector<string> _rollingLines;
};