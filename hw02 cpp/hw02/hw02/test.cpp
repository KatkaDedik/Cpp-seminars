/* You can put any private test cases into this file. It will *not* be part     +++of
 * the submission. */

#include "gttt.hpp"
#include <cassert>
#include <iostream>

void basic_test() {
	gttt game(2, 3);
	assert(game.play(0, 0, 0)); //0
	assert(!game.play(0, 0, 0));
	assert(game.play(1, 0, 0)); //1
	assert(!game.may_win());
	assert(!game.empty(0, 0, 0));
	assert(game.owner(0, 0, 0) == 0);
	assert(!game.finished());
	assert(game.play(0, 1, 0)); //0
	assert(!game.may_win());
	assert(!game.finished());
	assert(game.play(0, 50, 64)); //1
	assert(game.may_win());
	assert(game.play(0, 2, 0)); //0
	assert(game.finished());
	assert(game.winner() == 0);
}

int main() {

	basic_test();

	gttt g(2, 2);
	assert(g.next() == 0);
	assert(!g.finished());
	g.play(0, 0, 0);
	assert(g.next() == 1);
	assert(g.owner(0, 0, 0) == 0);
	assert(!g.play(0, 0, 0));
	g.play(0, 0, 1);
	assert(g.owner(0, 0, 1) == 1);
	assert(!g.finished());
	assert(g.may_win());
	g.play(1, 0, 0);
	assert(g.owner(1, 0, 0) == 0);
	assert(g.finished());
	assert(g.winner() == 0);

	/*TEST CASE 2*/

	gttt g1(4, 5);
	g1.play(0, 0, 0);
	g1.play(10, 0, 0);
	g1.play(0, 10, 0);
	g1.play(0, 0, 10);

	g1.play(-1, 1, 1);
	g1.play(10, 10, 0);
	g1.play(0, 10, 10);
	g1.play(10, 0, 10);

	g1.play(-2, 2, 2);
	g1.play(20, 0, 0);
	g1.play(0, 20, 0);
	g1.play(0, 0, 20);

	g1.play(-3, 3, 3);
	g1.play(20, 20, 0);
	g1.play(0, 20, 20);
	g1.play(20, 0, 20);

	assert(g1.may_win());
	g1.play(-4, 4, 4);
	assert(g1.finished());
	assert(g1.winner() == 0);

	/* POZNAMKOVY BLOK TEST CASES*/

	gttt g2(2, 3);
	g2.play(0, 0, 0);
	g2.play(10, 0, 0);
	assert(!g2.may_win());

	gttt g3(2, 3);
	g3.play(0, 0, 0);
	g3.play(10, 0, 0);
	assert(!g3.may_win());

	gttt g4(2, 3);
	g4.play(0, 0, 0);
	g4.play(1, 0, 0);
	assert(!g4.may_win());

	gttt g5(2, 3);
	const gttt &h = g5;
	g.play(0, 0, 0);
	g.play(1, 0, 0);
	assert(!h.may_win());

	gttt g6(2, 2);
	g6.play(0, 0, 0);
	g6.play(100, 100, 100);
	assert(g6.may_win());
	g6.play(200, 200, 200);
	assert(g6.may_win());

	gttt g7(3, 3);   // players = 3 len = 2
	g7.play(0, 0, -1); // dx = 0 dy = 0 dz = 1 i = -1
	g7.play(0, -1, 0); // dx = 0 dy = 1 dz = 0 i = -1
	g7.play(0, 0, 0);  // i = -1
	g7.play(0, 0, 1); // dx = 0 dy = 0 dz = 1 i = 1
	g7.play(0, 1, 0); // dx = 0 dy = 1 dz = 0 i = 1
	g7.play(0, 0, 100 * (1 + 1));  // i = 1
	assert(!g7.may_win());


	gttt gg(2, 3);
	gg.play(0, 0, 0); // o => 0,0,0
	gg.play(10, 0, 0); // 1 => 10,0,0
	gg.play(0, 0, 1); // 0 => 0,0,1
	gg.play(10, 0, 1); // 1 => 10,0,0	10,0,1
	gg.play(0, -5, 0);	// o => 0,0,0	0,0,1	0,-5,0
	gg.play(10, 0, 2);	// 1 => 10,0,0	10,0,1	10,0,2
	assert(gg.winner() == 1);


	gttt hra(2, 3);   // players = 2 len = 2 
	hra.play(0, 0, -1); // dx = 0 dy = 0 dz = 1 i = -1 
	hra.play(0, -1, 0); // dx = 0 dy = 1 dz = 0 i = -1 
	hra.play(0, 0, 1); // dx = 0 dy = 0 dz = 1 i = 1 
	hra.play(0, 1, 0); // dx = 0 dy = 1 dz = 0 i = 1 
	assert(hra.may_win());
}
