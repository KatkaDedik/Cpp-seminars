/* A Minsky machine has two counters and a fixed program which uses
 * three instructions: ‹halt›, ‹inc i› and ‹jzdec i x y› where ‹i›
 * is the counter (0 or 1) and ‹x› and ‹y› are instruction indices:
 * after ‹jzdec›, the machine continues at index ‹x› if register
 * ‹i›, was zero after the decrement, otherwise it continues at
 * index ‹y›. This machine is Turing-complete if the counters are
 * realized as non-negative integers. Normal (machine) integers will
 * be sufficient for this exercise. Calling ‹jzdec› on a zero
 * counter is undefined. */

#include <cassert>
#include <vector>

struct inst
{
	enum { halt, inc, jzdec } opcode;
	int counter; /* 0 or 1; unused for ‹halt› */
	int x, y; /* only used if opcode == jzdec */
};

/* Write a function ‹minsky› which takes a vector of instructions
 * and returns ‹true› if counter 0 is non-zero at the end of
 * computation. Both counters start at 0. */

bool minsky(const std::vector< inst > & p)
{
	int counter[2] = { 0, 0 };
	for (int pc = 0; pc < p.size(); pc++) {

		switch (p[pc].opcode) {
		case inst::halt:
			pc = p.size();
			break;
		case inst::inc:
			counter[p[pc].counter]++;
			break;
		case inst::jzdec:
			counter[p[pc].counter]--;
			if (counter[p[pc].counter] == 0) {
				pc = p[pc].x;
			}
			else {
				pc = p[pc].y;
			}
			pc--;
			break;
		}
	}
	return (counter[0] != 0);
}/* ref: 13 lines */

int main()
{
	inst halt{ inst::halt,  0,  0,  0 },
		inc0{ inst::inc,   0,  0,  0 },
		inc1{ inst::inc,   1,  0,  0 },
		dec0{ inst::jzdec, 0, -1, -1 },
		dec1{ inst::jzdec, 1, -1, -1 };

	auto run = [](auto... is)
	{
		std::vector< inst > p;
		for (inst i : { is... })
		{
			if (i.x == -1) i.x = p.size() + 1;
			if (i.y == -1) i.y = p.size() + 1;
			p.push_back(i);
		}
		return minsky(p);
	};

	auto jzdec = [](int i, int x, int y)
	{
		return inst{ inst::jzdec, i, x, y };
	};

	assert(!run(halt));
	assert(run(inc0, halt));
	assert(!run(inc1, halt));


	assert(!run(inc0, inc0, jzdec(0, 3, 2), halt));
	assert(!run(inc0, inc0, inc0, inc0, inc0,
		dec0, jzdec(0, 7, 4), halt));
	assert(run(inc0, inc1, inc1, inc1,
		dec1, jzdec(1, 6, 3), halt));
}