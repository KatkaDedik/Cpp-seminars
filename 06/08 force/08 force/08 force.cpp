/* This week in the physics department, we will deal with formatting
 * and parsing vectors (forces, just to avoid confusion with
 * ‹std::vector›... for now). */

#include <cassert>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cmath>

 /* The class will be called ‹force›, and it should have a
  * constructor which takes 3 values of type ‹double› and a default
  * constructor which constructs a 0 vector. In addition to that, it
  * should have a (fuzzy) comparison operator and formatting
  * operators, both for input and for output. Use the following
  * format: ‹[F_x F_y F_z]›, that is, a left square bracket, then the
  * three components of the force separated by spaces, and a closing
  * square bracket. Do not forget to set ‹failbit› in the input
  * stream if the format does not match expectations. */

bool fuzzyCompare(double first, double second) {
		return std::fabs(first - second) <= std::numeric_limits<double>::epsilon() * std::max({ 1.0, std::fabs(first) , std::fabs(second) });
}

class force {
	double x = 0, y = 0, z = 0;

public:

	force() = default;

	force(double a_x, double a_y, double a_z) : x(a_x), y(a_y), z(a_z) {}

	friend std::istream& operator>>(std::istream& input, force& f) {
		char c;
		for (;;) {
			input.get(c);
			if (!isspace(c)) {
				break;
			}
		}
		if (c != '[') {
			input.setstate(std::ios::failbit);
			return input;
		}

		input >> f.x >> f.y >> f.z;
 
		input.get(c);
		if (c != ']') {
			input.setstate(std::ios::failbit);
		}
		return input;
	}

	friend std::ostream& operator<<(std::ostream& output, force& f) {
		return output << '[' << f.x << ' ' << f.y << ' ' << f.z << ']';
	}

	friend bool operator==(const force& left, const force& right) {
		return fuzzyCompare(left.x, right.x) && fuzzyCompare(left.y, right.y) && fuzzyCompare(left.z, right.z);
	}
};

int main()
{
	force e_x(1, 0, 0), e_y(0, 1, 0);
	force f_1(3.1415, 0, 1), f_2(0, 1, 5);

	std::ostringstream ostr;
	ostr << e_x;
	assert(ostr.str() == "[1 0 0]");
	ostr << " " << e_y;
	assert(ostr.str() == "[1 0 0] [0 1 0]");

	ostr << " " << f_1 << " " << f_2;

	force r_1, r_2;
	std::istringstream istr("[0 0 0]");
	istr >> r_1;
	assert(r_1 == force(0, 0, 0));

	istr.clear(); /* clear status flags (!) */
	istr.str(ostr.str());

	istr >> r_1 >> r_2;
	assert(r_1 == e_x);
	assert(r_2 == e_y);
	istr >> r_1 >> r_2;
	assert(r_1 == f_1);
	assert(r_2 == f_2);
}
