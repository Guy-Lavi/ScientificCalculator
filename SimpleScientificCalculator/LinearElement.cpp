#include "LinearElement.h"

ostream& operator<<(ostream& out, const LinearElement& elm) {
	long double a = elm._linearCoefficient;
	long double b = elm._freeCoefficient;
	if (a != 0) {
		if (a != 1) {
			out << a << "x";
		}
		else {
			out << "x";
		}
		if (b != 0) {
			if (b > 0) {
				out << "+";
			}
			out << b;
		}
	}
	else {
		if (b != 0) {
			out << b;
		}
		else {
			out << "0";
		}
	}
	return out;
}

LinearElement operator+(const LinearElement& lhs, const LinearElement& rhs) {
	LinearElement result(lhs._linearCoefficient + rhs._linearCoefficient, lhs._freeCoefficient + rhs._freeCoefficient);
	return result;
}

LinearElement operator-(const LinearElement& lhs, const LinearElement& rhs) {
	LinearElement result(lhs._linearCoefficient - rhs._linearCoefficient, lhs._freeCoefficient - rhs._freeCoefficient);
	return result;
}

LinearElement operator*(const LinearElement& lhs, const LinearElement& rhs) {
	if (lhs._linearCoefficient != 0 && rhs._linearCoefficient != 0) {
		throw runtime_error("An attempt of 'X^2' detected, only linear operations allowed on variables");
	}
	long double a1, a2, b1, b2;
	a1 = lhs._linearCoefficient;
	b1 = lhs._freeCoefficient;
	a2 = rhs._linearCoefficient;
	b2 = rhs._freeCoefficient;
	LinearElement result(a1*b2 + b1*a2, b1*b2);
	return result;
}

LinearElement operator/(const LinearElement& lhs, const LinearElement& rhs) {
	if (rhs._linearCoefficient != 0) {
		throw runtime_error("An attempt of type '1/X' detected, only linear operations allowed on variables");
	}
	long double a1, b1, b2;
	a1 = lhs._linearCoefficient;
	b1 = lhs._freeCoefficient;
	b2 = rhs._freeCoefficient;
	LinearElement result(a1 / b2, b1 / b2);
	return result;
}