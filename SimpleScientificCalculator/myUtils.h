#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <exception>
#include <typeinfo>
#include <algorithm>
#include "VariableName.h"

using namespace std;

/**
 * MyUtils
 * General static class with simple useful utility operations
 */

class MyUtils {

	public:

		// Macro-like function for exeptions
		static string getExceptionMessage(const string& msg, const exception& e) {
			stringstream ss;
			ss << msg << " (" << e.what() << ")";
			return ss.str();
		}

		// extracts a long double from a string in the format: #.#
		static long double extractDouble(const string& input) {
			auto ss = stringstream(input);
			string piece;
			long double resultDouble;

			try {
				if (getline(ss, piece, '.')) {
					if (!is_number(piece)) throw runtime_error("Not a number: " + piece);
					resultDouble = static_cast<long double>(stoll(piece));
				} else {
					throw runtime_error("bad integer part");
				}
				if (getline(ss, piece, '.')) {
					if (!is_number(piece)) throw runtime_error("Not a number: " + piece);
					long double fractionPart = static_cast<long double>(stoll(piece));
					for (auto i = 0; i < piece.size(); ++i) fractionPart /= 10;
					resultDouble += fractionPart;
				} else {
					throw runtime_error("bad fraction part");
				}
				if (getline(ss, piece)) {
					throw runtime_error("not a legal fraction");
				}
			}
			catch (const exception & e)
			{
				throw runtime_error(getExceptionMessage("Unable to parse from: " + input, e));
			}

			return resultDouble;
		}

		// Checks if a string is an integer
		static bool is_number(const string& s) {
			return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
		}
};