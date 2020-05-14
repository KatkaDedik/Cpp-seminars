#include "number.hpp"
#include <cassert>
#include <iostream>
#include <climits> // INT_MAX
#include <chrono>

void primitive_test() {

	integer_number one(1);
	integer_number two(2);
	integer_number ntwo(-2);
	integer_number none(-1);
	integer_number nthree(-3);

	assert(one + one == two);
	assert(two - one == one);
	assert(one - two == none);
	assert(ntwo + none == nthree);
	assert(ntwo - nthree == one);
	assert(nthree + one == ntwo);
}

void less_primitive_test() {

	std::vector<uint32_t> s;
	s.push_back(0xE);
	s.push_back(1);

	integer_number small(s, false);


	std::vector<uint32_t> l;
	l.push_back(0xffffffff);

	integer_number big(l, false);

	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	integer_number large(l, false);
	
	integer_number nlarge(l, true);

	l.push_back(0xffffffff);

	integer_number very_large(l, false);

	integer_number hentai(69);
	integer_number weed(-420);
	integer_number star_wars(54);

	//assert(large * nlarge < weed);
	//assert(big * hentai > large);
	//assert((big - small) * weed < 0);
}

void primitive_devide() {

	std::vector<uint32_t> l;
	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	integer_number long_number(l, false);

	std::vector<uint32_t> e;
	e.push_back(0x8ce602fb);
	e.push_back(0x1054d);
	integer_number expected(e, false);

	assert(long_number.devide(0xface, 0) == expected);
	
	l.push_back(0xffffffff);
	integer_number very_long_number(l, false);
	
	e[0] = 0x80000000;
	e[1] = 0x8ce602fb;
	integer_number expected2(e, false);

	assert(long_number.devide(0xface, 0) == expected);
}

void simple_devide() {

	std::vector<uint32_t> l;
	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	//l.push_back(0xffffffff);
	integer_number long_number(l, false);

	std::vector<uint32_t> osem;
	osem.push_back(0x88888888);
	osem.push_back(0x88888888);
	integer_number osmicky(osem, false);
	
	std::vector<uint32_t> e;
	//e.push_back(0x00000000);
	e.push_back(0xe0000000);
	e.push_back(0x10000000);
	integer_number expected(e, false);
	//assert(long_number / osmicky == expected);

	integer_number res2 = long_number.devide(2, 0);
	//assert(long_number / res2 == integer_number(2));

	integer_number res8888 = long_number.devide(0x8888, 0);
	//assert(long_number / res8888 == integer_number(0x8888));


	std::vector<uint32_t> smth;
	smth.push_back(0xff);
	smth.push_back(0xffffffff);
	smth.push_back(0xffffffff);
	smth.push_back(0xffffffff);
	smth.push_back(0xffffffff);
	integer_number smth_big(smth, false);
	integer_number resff = smth_big.devide(0xff, 0);
	assert(smth_big / resff == integer_number(0xff));
	
	integer_number hentai(69);

	integer_number r = smth_big + hentai;
	assert(smth_big / resff == integer_number(0xff));

	integer_number vn(999999999);
	integer_number vd(56789);
	assert(vn / vd == integer_number(17609));

	integer_number video_numerator = integer_number(999999999) * integer_number(100000000);
	integer_number video_denominator = integer_number(56789) * integer_number(100000000);
	//assert(video_numerator / video_denominator == integer_number(17609));

}

void simple_power_test() {

	integer_number two(2);
	integer_number ntwo(-2);
	integer_number twototen(1024);
	assert(two.power(10) == twototen);
	assert(ntwo.power(10) == two.power(10));
	assert(ntwo.power(3) == integer_number(-8));
	assert((integer_number(1019).power(500) / integer_number(1019).power(500)).power(500) == integer_number(1));

}

void dont_know_test() {
	std::vector<uint32_t> l;
	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	l.push_back(0xffffffff);
	std::vector<uint32_t> one;
	one.push_back(0x1);
	integer_number long_number(l, true);
	integer_number number_one(one, true);

	std::cout << " + ";
	std::cout << " = ";
	integer_number ll = long_number + number_one;

	std::vector<uint32_t> vv;
	vv.push_back(0x0);
	vv.push_back(0x1);
	integer_number v(vv, true);

	std::cout << std::endl;
	std::cout << " - ";
	std::cout << " = ";
	integer_number sub = v - number_one;

	std::cout << std::endl;
	integer_number tmp = long_number * long_number;

	l.push_back(0xffffffff);
	integer_number tmp2(l, false);
	std::vector<uint32_t> face;
	face.push_back(0xfaceface);
	face.push_back(0xface);
	integer_number tmp3(face, false);
}

void new_devide() {

	std::vector<uint32_t> f;
	f.push_back(0x66666666);
	f.push_back(0x55555555);
	f.push_back(0x44444444);
	f.push_back(0x33333333);
	f.push_back(0x22222222);
	f.push_back(0x11111111);

	integer_number numerator(f, false);

	std::vector<uint32_t> d;
	d.push_back(0xcccccccc);
	d.push_back(0xffffffff);
	d.push_back(0x22222222);
	integer_number denominator(d, false);

	integer_number res = numerator / denominator;

}

void integer_number_tests() {
	less_primitive_test();
	primitive_test();
	primitive_devide();
	simple_devide();
	simple_power_test();
	new_devide();
	//dont_know_test();
}

void spec_test() {
	number a(10), b(25);
	number c = a + b;
	number d = c / a;
	number e = d * a;
	assert(e == a + b);
	assert(e != a);
	assert(c > a);
	assert(a < b);
	assert(c.power(2) > c);
	c = number(10).power(-5);
	assert(c > c.power(2));

	number s = number(145).sqrt(3); /* 3 fractional digits */
	/* the exact result rounded to 3 fractional places is 12.042 */
	number pow_4(10);
	pow_4 = pow_4.power(-4);
	number l(120415);
	number u(120425);
	number lower = l * pow_4;
	number upper = u * pow_4;
	assert(s > lower);
	assert(s < upper);
}


void precision_subtest() {
	number big = number(1000).power(20);
	number small = number(10).power(-10);
	assert(big + small > big);
}


void everything_subtest() {

	assert(number(0) == number(-0));

	number n(number(2).power(31) - 1);
	number x(INT_MAX);
	assert((number(2).power(31) - 1) == INT_MAX);
	assert(n - x == 0);

	number y(INT_MAX);
	y = y.power(400);
	assert(y > (number(INT_MAX).power(399)));
	assert(y < (number(INT_MAX).power(401)));


	number nn(25);
	nn = nn.sqrt(1);
	assert(nn > number(49) / 10);
	assert(nn < number(51) / 10);

	number u = number(2).power(500);      // b = 2 i = 500
	number m1 = number(2).power(-500);
	number one = m1 * u;
	number bp = u * number(2).power(-500 + 1);
	assert(one == 1);

	assert(bp == 2);
	assert(one.power(2) == 1);



	number m(0);
	number s1 = number(10).power(-5); // scale = -5
	m = m + number(331662) * s1;
	s1 = number(10).power(-11); // scale = -11
	m = m + number(479035) * s1;            // chunk = 479035
	s1 = number(10).power(-17); // scale = -17
	m = m + number(539984) * s1;            // chunk = 539984
	s1 = number(10).power(-23); // scale = -23
	m = m + number(911493) * s1;            // chunk = 911493
	s1 = number(10).power(-29); // scale = -29
	m = m + number(273667) * s1;            // chunk = 273667
	s1 = number(10).power(-35); // scale = -35
	m = m + number(68668) * s1;            // chunk = 68668
	s1 = number(10).power(-41); // scale = -41
	m = m + number(392708) * s1;            // chunk = 392708
	s1 = number(10).power(-47); // scale = -47
	m = m + number(854558) * s1;            // chunk = 854558
	s1 = number(10).power(-53); // scale = -53
	m = m + number(935359) * s1;            // chunk = 935359
	s1 = number(10).power(-59); // scale = -59
	m = m + number(705868) * s1;            // chunk = 705868
	s1 = number(10).power(-65); // scale = -65
	m = m + number(214611) * s1;            // chunk = 214611
	s1 = number(10).power(-71); // scale = -71
	m = m + number(648464) * s1;            // chunk = 648464
	s1 = number(10).power(-77); // scale = -77
	m = m + number(260904) * s1;            // chunk = 260904
	s1 = number(10).power(-83); // scale = -83
	m = m + number(384670) * s1;            // chunk = 384670
	s1 = number(10).power(-89); // scale = -89
	m = m + number(884339) * s1;            // chunk = 884339
	s1 = number(10).power(-95); // scale = -95
	m = m + number(912829) * s1;            // chunk = 912829
	s1 = number(10).power(-101); // scale = -101
	m = m + number(65090) * s1;           // chunk = 65090
	s1 = number(10).power(-107); // scale = -107
	m = m + number(424242) * s1;            // chunk = 424242

	number n12 = number(11).sqrt(71);

	number error = m - n12 > 0 ? m - n12 : n12 - m;
	assert(error < number(10).power(-70));
	//


	number f = number(144).sqrt(3);

	number g = number(9).sqrt(0);
	assert(g >= 3);
	assert(g < 4);


	number n5 = number(1019).power(500);    // b = 1019 i = 500
	number m5 = number(1019).power(-500);
	number one5 = m5 * n5;
	number bp5 = n5 * number(1019).power(-500 + 1);
	assert(one5 == 1);
	assert(bp5 == 1019);
	assert(one5.power(500) == 1);

	number w = number(1);
	w = w - 2;
	number z = number(2);
	assert(w * z < 0);

	number big = number(1000).power(20);

	assert(big - 1 < big);
}

void pow_subtest() {

	

	for (int bb = 2; bb < 5; ++bb) {
		number b(bb);
		for (int i = 450; i < 550; i) {
			i += 10;
			number n = b.power(i);
			number m = b.power(-i);
			number one = m * n;
			assert(one == number(1));
			number bp = n * b.power(-i + 1);
			assert(bp == b);
			assert(bp.power(i) / bp.power(i - 1) == bp);
		}
	}
	
	
}


void sqrt_test() {


	/*
	sqrt for i =19996 : 0.009318s
sqrt for i =19997 : 0.010086s
sqrt for i =19998 : 0.00985s
sqrt for i =19999 : 0.010377s
	*/

	number a = number(19996).sqrt(0);
	assert(a > number(1414) / number(10));
	assert(a < number(1415) / number(10));
}

void test_multiplication() {

	std::cout << "Multiplication TIME TESTS:" << std::endl;
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	number a = number(459) * number(968);
	std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	assert(a == number(444312));

	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "TEST 1. Elapsed time: " << elapsed.count() << std::endl;

	start = std::chrono::high_resolution_clock::now();
	number b = number(45) * number(45842546);
	finish = std::chrono::high_resolution_clock::now();
	assert(b == number(2062914570));

	elapsed = finish - start;
	std::cout << "TEST 3. Elapsed time: " << elapsed.count() << std::endl;
}


void static test_sqrt() {
	integer_number one(1);

	std::vector<uint32_t> aa;
	aa.push_back(0x5a1db6a1);
	aa.push_back(0x33096e64);
	aa.push_back(0xdbf);
	integer_number a_tmp(aa, false);

	number a(a_tmp, one); //number a("64917769987331924539041");

	std::vector<uint32_t> a_res_vec;
	a_res_vec.push_back(0x52a5a6d1);
	a_res_vec.push_back(0x3b);
	number a_res(integer_number(a_res_vec, false), integer_number(10000));
	number ar = a.sqrt(3);
	//assert(a.sqrt(3) == a_res);
	
	//number s = number("254856896523254856896523").sqrt(3); /* 3 fractional numerators */
	/* the exact result rounded to 3 fractional places is 504833533477.378 */
	//number lower = number("5048335334773775") * number(10).power(-4);
	//number upper = number("5048335334773785") * number(10).power(-4);
	//assert(s > lower);
	//assert(s < upper);

	//number t = number("254856896523254856896523").sqrt(0); /* 3 fractional numerators */
	/* the exact result rounded to 3 fractional places is 504833533477 */
	//lower = number("5048335334765") * number(10).power(-1);
	//upper = number("5048335334775") * number(10).power(-1);
	//assert(t > lower);
	//assert(t < upper);*/
}


void didn_passed_tests() {

	number m(0);
	number s = number(10).power(-5);// scale = -5 
	m = m + number(331662) * s;            // chunk = 331662 
	s = number(10).power(-11);// scale = -11 
	m = m + number(479035) * s;       // chunk = 479035 
	 s = number(10).power(-17); // scale = -17 
	m = m + number(539984) * s;         // chunk = 539984 
	s = number(10).power(-23);// scale = -23 
	m = m + number(911493) * s;       // chunk = 911493 
	s = number(10).power(-29); // scale = -29 
	m = m + number(273667) * s;      // chunk = 273667 
	s = number(10).power(-35); // scale = -35 
	m = m + number(68668) * s;      // chunk = 68668 
	s = number(10).power(-41);// scale = -41 
	m = m + number(392708) * s;          // chunk = 392708 
	s = number(10).power(-47); // scale = -47 
	m = m + number(854558) * s;          // chunk = 854558 
	s = number(10).power(-53); // scale = -53 
	m = m + number(935359) * s;        // chunk = 935359 
	s = number(10).power(-59); // scale = -59 
	m = m + number(705868) * s;         // chunk = 705868 
	s = number(10).power(-65); // scale = -65 
	m = m + number(214611) * s;          // chunk = 214611 
	s = number(10).power(-71); // scale = -71 
	m = m + number(648464) * s;          // chunk = 648464 
	s = number(10).power(-77); // scale = -77 
	m = m + number(260904) * s;         // chunk = 260904 
	s = number(10).power(-83); // scale = -83 
	m = m + number(384670) * s;           // chunk = 384670 
	s = number(10).power(-89); // scale = -89 
	m = m + number(884339) * s;        // chunk = 884339 
	s = number(10).power(-95); // scale = -95 
	m = m + number(912829) * s;        // chunk = 912829 
	s = number(10).power(-101); // scale = -101 
	m = m + number(65090) * s;          // chunk = 65090 
	s = number(10).power(-107);// scale = -107 
	m = m + number(424242) * s; // chunk = 424242
	number n = number(11).sqrt(71);
	number error = m - n > 0 ? m - n : n - m;
	assert(error > 0);

}

void another_tests() {
	test_multiplication();
	test_sqrt();
}

void number_tests() {
	
	didn_passed_tests();

	spec_test();
	
	sqrt_test();

	everything_subtest();

	pow_subtest();

	precision_subtest();
	
	another_tests();
}


int main()
{
	integer_number_tests();
	number_tests();
}