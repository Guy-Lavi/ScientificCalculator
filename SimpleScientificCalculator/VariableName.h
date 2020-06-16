#pragma once
#include "myUtils.h"

/**
 * VariableName 
 * This class holds the variable name 
 * rise an exception when multiple variable names are used in the same equation
 * also used to identify the when an equation is used or not
 */

using namespace std;

class VariableName {
	public:
		// Ctor
		VariableName() : _isSet(false), _isEuation(false),_varName(0) {}

		// set name if wasn't set before, if already set - makes sure the name is the same
		void setName(char varName) {
			if (_isSet) {
				if (varName != _varName) {
					throw runtime_error("Only use of a single variable is allowed, can't use: '" + string(1, varName) + "' since: '" + string(1, _varName) + "' is already used");
				}
			}
			else {
				_varName = varName;
				_isSet = true;
			}
		}

		// was the variable set
		bool isSet() {
			return _isSet;
		}

		// is this variable used in an equation or a simple calculation
		bool isEquation() {
			return _isEuation;
		}

		// get name
		char getName() {
			return _varName;
		}

		// called when this variable is used in an equation
		void setEquation() {
			_isEuation = true;
		}

		// reset variable
		void clear() {
			_isSet = false;
			_isEuation = false;
			_varName = 0;
		}
	private:
		// was the variable name set already or not
		bool _isSet;

		// is this an equation solution
		bool _isEuation;

		// the variable name
		char _varName;
};

