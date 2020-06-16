#pragma once
#include <iostream>
#include <ostream>
#include <string>

using namespace std;

/**
 * LinearElement
 * Thie class represents a linear element in the form of "ax+b"
 * Supports simple arithmetic operations
 */

class LinearElement {
	public:
		// Ctor
		LinearElement(): _linearCoefficient(0), _freeCoefficient(0) { }
		LinearElement(long double linearCoefficient, long double freeCoefficient):
			_linearCoefficient(linearCoefficient), _freeCoefficient(freeCoefficient){ }

		// the << operator for LinearElement used for printing
		friend ostream& operator<<(ostream& out, const LinearElement& linearElement);

		// Arithmetic operators
		friend LinearElement operator+(const LinearElement& lhs, const LinearElement& rhs);
		friend LinearElement operator-(const LinearElement& lhs, const LinearElement& rhs);
		friend LinearElement operator*(const LinearElement& lhs, const LinearElement& rhs);
		friend LinearElement operator/(const LinearElement& lhs, const LinearElement& rhs);
		LinearElement operator-() {
			_linearCoefficient *= -1;
			_freeCoefficient   *= -1;
			return *this;
		}

		// Accessors shortcuts: a*x + b
		long double& a() { return _linearCoefficient; }
		long double& b() { return _freeCoefficient; }

	private:
		// represents the linear coefficient in a simple single varibale term, i.e. 'C' in "C*X + 7"
		long double _linearCoefficient;
		// represents the free coefficient in a simple single varibale term, i.e. 'C' in "3*X + C"
		long double _freeCoefficient;
};

