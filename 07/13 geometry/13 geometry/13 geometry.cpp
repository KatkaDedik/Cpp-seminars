/* We will go back to a bit of geometry, this time with circles and
 * lines: in this exercise, we will be interested in planar
 * intersections. We will consider two objects to intersect when
 * they have at least one common point. On the C++ side, we will use
 * a bit of a trick with �virtual� method overloading (in a slightly
 * more general setting, the trick is known as the �visitor
 * pattern�). */
#include <cmath>
#include <cassert>
#include <tuple>
 /* First some definitions: the familiar �point�. */
using point = std::pair< double, double >;
/* Check whether two floating-point numbers are 'essentially' the
 * same� (i.e. fuzzy equality). */
bool close(double a, double b)
{
	return std::fabs(a - b) < 1e-10;
}
/* We will need to use forward declarations in this exercise, since
 * methods of the base class will refer to the derived types. */
struct circle;
struct line;

/* These two helper functions are already defined in this file and
 * may come in useful (like the �slope� class above). */
double dist(point, point);
double dist(const line &, point);

/* Now we can define the class �object�, which will have a �virtual�
 * method �intersects� with two overloads: one that accepts a
 * �const� reference to a �circle� and another that accepts a
 * �const� reference to a �line�. */
class object 
{
public:
	virtual bool intersects(const circle & c) = 0;
	virtual bool intersects(const line & l) = 0;
};

/* Put definitions of the classes �circle� and �line� here. A
 * �circle� is given by a �point� and a radius (�double�), while a
 * �line� is given by two points. NB. Make the �line� attributes
 * �public� and name them �p� and �q� to make the �dist� helper
 * function work. */

struct circle : public object 
{
	point s;
	double radius;
	circle(point p, double r) : s(p), radius(r) {}

	bool intersects(const line & l)
	{
		return (dist(l, s) <= radius);
	}

	bool intersects(const circle & c) {

		if (dist(s, c.s) > radius + c.radius) 
		{
			return false;
		}
		if (dist(s, c.s) == 0) 
		{
			return radius == c.radius;
		}
		if (dist(s, c.s) < radius) {
			if (dist(s, c.s) + c.radius < radius) {
				return false;
			}
		}

		return true;
	}
};/* ref: 18 lines */

struct line : public object
{
public :
	point p;
	point q;
	line(point a, point b) : p(a), q(b) {}

	bool intersects(const line & l)
	{
		if (dist(l, p) == dist(l, q) && dist(l, p) != 0) {
			point a = { q.first - p.first, q.second - p.second };
			point b = { l.q.second - l.p.second, -(l.q.first - l.p.first) };
			return (a.first * b.first + a.second * b.second != 0);
		}
		return true;
	}

	bool intersects(const circle & c) {
		return (dist({p, q}, c.s) <= c.radius);
	}
};/* ref: 18 lines */


/* Definitions of the helper functions. */
double dist(point p, point q)
{ 
	double px = p.first;
	double py = p.second;
	double qx = q.first;
	double qy = q.second;
	return std::sqrt(std::pow(px - qx, 2) +
		std::pow(py - qy, 2));
}
double dist(const line &l, point p)
{
	double x0 = p.first;
	double y0 = p.second;
	double x2 = l.q.first;
	double y2 = l.q.second;
	double x1 = l.p.first;
	double y1 = l.p.second;
	return std::fabs((y2 - y1) * x0 - (x2 - x1) * y0 +
		x2 * y1 - y2 * x1) /
		dist(l.p, l.q);
}

int main()
{
	circle c_unit({ 0, 0 }, 1),
		c_shifted({ 1, 0 }, 1),
		c_big({ 0, 0 }, 2),
		c_distant({ 3, 0 }, 1),
		c_touchy({ 3, 0 }, 2);
	line l1({ 0, 0 }, { 1, 0 }),     // horizontal
		l2({ 1, 0 }, { 1, 1 }),     // vertical at x = 1
		l3({ 0.5, 0 }, { 0.5, 1 }), // vertical at x = 0.5
		l4({ 4, 0 }, { 4, 1 }),     // vertical at x = 4
		l5({ 0, 0 }, { 1, 1 }),     // at 45� angle through [0, 0]
		l6({ 1, 0 }, { 2, 1 });     // at 45� angle through [1, 0]
   /* circles vs circles */
	assert(c_unit.intersects(c_unit));
	assert(c_unit.intersects(c_shifted));
	assert(c_shifted.intersects(c_unit));
	assert(!c_unit.intersects(c_big));
	assert(!c_big.intersects(c_unit));
	assert(c_shifted.intersects(c_big));
	assert(c_big.intersects(c_shifted));
	assert(!c_unit.intersects(c_distant));
	assert(!c_distant.intersects(c_unit));
	assert(c_unit.intersects(c_touchy));
	assert(c_touchy.intersects(c_unit));
	assert(!c_distant.intersects(c_touchy));
	assert(!c_touchy.intersects(c_distant));
	/* lines vs lines */
	assert(l1.intersects(l1));
	/* vertical intersects horizontal */
	assert(l1.intersects(l2) && l2.intersects(l1));
	/* distinct verticals do not intersect */
	assert(!l2.intersects(l3) && !l3.intersects(l2));
	assert(!l2.intersects(l4) && !l4.intersects(l2));
	/* the diagonal line intersects all the vertical/horizontal */
	assert(l5.intersects(l1));
	assert(l5.intersects(l2));
	assert(l5.intersects(l3));
	assert(l5.intersects(l4));
	assert(!l6.intersects(l5) && !l5.intersects(l6));
	/* circles vs lines */
	assert(l1.intersects(c_unit));
	assert(c_unit.intersects(l1));
	assert(c_unit.intersects(l2));
	assert(c_unit.intersects(l3));
	assert(!c_unit.intersects(l4));
	assert(c_unit.intersects(l5));
	assert(c_unit.intersects(l6));
	assert(c_shifted.intersects(l1));
	assert(c_shifted.intersects(l2));
	assert(c_shifted.intersects(l3));
	assert(!c_shifted.intersects(l4));
	assert(c_shifted.intersects(l5));
	assert(c_shifted.intersects(l6));
	assert(c_big.intersects(l1));
	assert(c_big.intersects(l2));
	assert(c_big.intersects(l3));
	assert(!c_big.intersects(l4));
	assert(c_big.intersects(l5));
	assert(c_big.intersects(l6));
	assert(c_distant.intersects(l1));
	assert(!c_distant.intersects(l2));
	assert(!c_distant.intersects(l3));
	assert(c_distant.intersects(l4));
	assert(!c_distant.intersects(l5));
	assert(!c_distant.intersects(l6));
	assert(c_touchy.intersects(l1));
	assert(c_touchy.intersects(l2));
	assert(!c_touchy.intersects(l3));
	assert(c_touchy.intersects(l4));
	assert(!c_touchy.intersects(l5));
	assert(c_touchy.intersects(l6));
}
