/* The first thing we will do is implement a simple class which
 * represents complex numbers using their polar form. This form
 * makes multiplication and division easier, so that is what we will
 * do here (see also ‹cartesian.cpp› for definition of addition).
 *
 *  • the constructor takes the modulus and the argument (angle)
 *  • add ‹abs› and ‹arg› methods to access the attributes
 *  • implement multiplication and division on ‹polar›
 *  • implement equality for ‹polar›; keep in mind that if the
 *    modulus is zero, the argument (angle) is irrelevant
 *
 * NB. The argument is «periodic»: either normalize it to fall
 * within [0, 2π), or otherwise make sure that ‹polar( 1, x ) ==
 * polar( 1, x + 2π )›. The equality operator you implement should
 * be tolerant of imprecision: use ‹std::fabs( x - y ) < 1e-10›
 * instead of ‹x == y› when dealing with real numbers.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#define M_PI 3.14159265358979323846

 /* reference implementation: 29 lines */
class polar {

	int modulus;
	double angle;

public:

	explicit polar(int mod, double angl) : modulus(mod), angle(angl) {
		while (angle >= 2 * M_PI) {
			angle -= 2 * M_PI;
		}
		while (angle < 0) {
			angle += 2 * M_PI;
		}
	}

	void abs(){
		while (angle >= 2 * M_PI) {
			angle -= 2 * M_PI;
		}
		while (angle <= 0) {
			angle += 2 * M_PI;
		}
	}

	int getMod() const {
		return modulus;
	}

	double getAngle() const {
		return angle;
	}

	friend polar operator*(polar first, polar second) {
		return polar(first.getMod() * second.getMod(), first.getAngle() + second.getAngle());
	}

	friend polar operator/(polar first, polar second) {
		if (second.getMod() != 0) {
			return polar(first.getMod() / second.getMod(), first.getAngle() - second.getAngle());
		}
		return first;
	}

	friend bool operator==(polar first, polar second) {
		if (first.getMod() != second.getMod()) {
			return false;
		}
		if (first.getMod() == 0) {
			return true;
		}
		if (std::fabs(first.getAngle() - second.getAngle()) > 0.001) {
			return false;
		}
		
		return true;
	}
};

int main()
{
	polar zero(0, 0), one(1, 0), i(1, 2 * std::atan(1));
	polar minus_one(1, 4 * std::atan(1));

	assert(zero * one == zero);
	assert(zero * i == zero);
	assert(one * i == i);
	assert(i * i == minus_one);
	assert(i / i == one);
	assert(one / i == minus_one * i);
	assert(minus_one * minus_one == one);
	assert(minus_one / minus_one == one);
	std::cout << "testy presli";
}