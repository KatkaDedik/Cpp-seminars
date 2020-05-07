/* An s-expression is a tree in which each node has an arbitrary
 * number of children. To make things a little more interesting, our
 * s-expression nodes will own their children. */
#include <memory>
#include <cassert>
#include <vector>
 /* The base class will be called ‹node› (again) and it will have
  * single (virtual) method: ‹value›, with no arguments and an ‹int›
  * return value. */
class node 
{
public: 
	virtual int value() = 0;
};
using node_ptr = std::unique_ptr< node >;
/* There will be two types of internal nodes: ‹sum› and ‹product›,
 * and in this case, they will compute the sum or the product of all
 * their children, regardless of their number. A sum with no
 * children should evaluate to 0 and a product with no children
 * should evaluate to 1.
 *
 * Both will have an additional method: ‹add_child›, which accepts
 * (by value) a single ‹node_ptr› and both should have default
 * constructors. It is okay to add an intermediate class to the
 * hierarchy. */
class sum : public node
{
	std::vector<node_ptr> children;
	int children_count;

public:
	sum() {}

	int value()
	{ 
		int s = 0;
		for (int i = 0; i < children_count; i++) {
			s += children[i].get()->value();
		}
		return s; 
	}

	void add_child(node_ptr child) 
	{
		children_count++;
		children.push_back(std::move(child));
	}

};

class product : public node
{
	std::vector<node_ptr> children;
	int children_count;

public:
	product() {}

	int value() 
	{
		int p = 1;
		for (int i = 0; i < children_count; i++) {
			p = p * children[i].get()->value();
		}
		return p;
	}

	void add_child(node_ptr child)
	{
		children_count++;
		children.push_back(std::move(child));
	}
};

/* Leaf nodes carry an integer constant, given to them via a
 * constructor. */
class constant : public node
{
	int val;
public:

	constant(int v) : val(v) {}
	
	int value()
	{
		return val;
	}
};


int main()
{

	auto mk_const = [](int n)
	{
		return std::make_unique< constant >(n);
	};
	auto mk_sum = [] { return std::make_unique< sum >(); };
	auto mk_prod = [] { return std::make_unique< product >(); };
	auto s = mk_sum();
	auto p = mk_prod();
	s->add_child(mk_const(2));
	s->add_child(mk_const(3));
	assert(s->value() == 5);
	p->add_child(mk_const(2));
	p->add_child(std::move(s));
	assert(p->value() == 10);
	p->add_child(mk_const(7));
	assert(p->value() == 70);
	s = mk_sum();
	assert(s->value() == 0);
	s->add_child(std::move(p));
	assert(s->value() == 70);
	s->add_child(mk_const(30));
	assert(s->value() == 100);
}

