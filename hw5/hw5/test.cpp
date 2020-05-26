
/*In this assignment, we will implement a few bits of basic linear
algebra on top of the ‹number› class from assignment 3. We will add
2 additional data types : ‹vector› and ‹matrix›(please try to avoid
	confusing ‹vector› with ‹std::vector›).

	Implement the following basic operations :

• vector addition and subtraction(operators ‹ + › and ‹ - ›),
• multiplication of a vector by a scalar(from both sides, ‹*›),
• dot product of two vectors(operator ‹*›),
• matrix addition(operator ‹ + ›),
• multiplication of a vector by a matrix(again ‹*›, both sides),
• equality on both vectors and matrices.

Implement these additional operations(‹m› is a ‹matrix›) :

	• ‹m.gauss()› performs in - place Gaussian elimination : after the
	call, ‹m› should be in a reduced row echelon form
	• ‹m.rank()› returns the rank of the matrix(as ‹int›)
	• ‹m.det()› returns the determinant of the matrix
	• ‹m.inv()› returns the inverse of the matrix
	• ‹m.row(int n)› returns the ‹n› - th row as a ‹vector›
	• ‹m.col(int n)› returns the ‹n› - th column as a ‹vector›

	With the exception of ‹gauss›, all methods should be ‹const›.

	Object construction :

• ‹vector› from a single ‹int›(dimension) to get a zero vector
• ‹vector› from an ‹std::vector› of ‹number›
• ‹matrix› from 2 ‹int› values(columns and rows, gives zero matrix)
• ‹matrix› from an ‹std::vector› of ‹vector›(rows)

The one - argument constructor variants should be ‹explicit›.

The behaviour is undefined if the ‹vector› instances passed to a
‹matrix› constructor are not all of the same dimension and when
‹det› or ‹inv› are called on a non - square matrix or ‹inv› on a
singular matrix.Likewise, operations on dimensionally mismatched
arguments are undefined.All dimensions must be positive.*/



#include "linalg.hpp"
#include <cassert>

void simple_vector()
{
	number_vector uu;
	uu.push_back(1);
	uu.push_back(0);
	uu.push_back(0);

	number_vector nu;
	nu.push_back(0);
	nu.push_back(2);
	nu.push_back(0);

	number_vector vv;
	vv.push_back(2);
	vv.push_back(0);
	vv.push_back(0);

	number_vector ww;
	ww.push_back(2);
	ww.push_back(0);
	ww.push_back(3);

	vector u(uu);
	vector n(nu);
	vector v(vv);
	vector w(ww);

	assert((n * u) == 0);
	assert((w * u) == 2);
	assert((w * n) == 0);
	assert(u == v);


}

void simple_determinant() {
	matrix ate_mat(8);
	assert(ate_mat.det() == 1);
	assert(true);
}

void inv() {
	using nv = std::vector< number >;
	using vv = std::vector< vector >;
	const number zero(0);
	const number one(1);
	const number two(2);
	const vector e_x(nv{ two, zero });
	const vector e_y(nv{ zero, one });
	const matrix m(vv{ e_x, e_y });
	assert(m.det() == two);
}

void mat() {
	using nv = std::vector< number >;
	using vv = std::vector< vector >;
	const number zero(0);
	const number one(1);
	const vector v_0(3);
	const vector e_x(nv{ one, zero, zero });
	const vector e_y(nv{ zero, one, zero });
	const vector e_z(nv{ zero, zero, one });
	const matrix m_id(vv{ e_x, e_y, e_z });
	const matrix m_0(3, 3);
	const matrix m_1(vv{ e_x, e_x, e_y });
	const matrix m_2(vv{ e_x, e_y });
	assert(m_1 != m_id);
	m_0.print();
	assert(m_0.rank() == 0);
}

void mul() {
	using nv = std::vector< number >;
	using vv = std::vector< vector >;
	const number zero(0);
	const number one(1);
	const vector v_0(3);
	const vector e_x(nv{ one, zero, zero });
	const vector e_y(nv{ zero, one, zero });
	const vector e_z(nv{ zero, zero, one });
	const matrix m_id(vv{ e_x, e_y, e_z });
	const matrix m_0(3, 3);
	assert(m_id * v_0 == v_0);
	assert(v_0* m_id == v_0);
}

void vec() {
	using nv = std::vector< number >;
	const number zero(0);
	const number one(1);
	const number two(2);
	const vector v_0(3);
	const vector e_x(nv{ one, zero, zero });
	const vector e_y(nv{ zero, one, zero });
	const vector e_z(nv{ zero, zero, one });
	assert(e_x != e_y);
}

void sanity() {
	inv();
	mat();
	mul();
	vec();
}

void gauss() {
	using nv = std::vector< number >;
	using vv = std::vector< vector >;

	const vector e_x(nv{ number(1), number(1), number(2) });
	const vector e_y(nv{ number(2), number(4), number(8) });
	const vector e_z(nv{ number(3), number(2), number(4) });
	const vector e_a(nv{ number(9), number(3), number(5) });

	matrix m_e(vv{ e_x, e_y, e_z , e_a});

	const vector s_x(nv{ number(1), number(0), number(0)});
	const vector s_y(nv{ number(0), number(1), number(0)});
	const vector s_z(nv{ number(4), number(-1) / number(2), number(0)});
	const vector s_a(nv{ number(0), number(0), number(1)});


	const matrix m_s(vv{ s_x, s_y, s_z , s_a});
	m_e.print();
	
	m_e.gauss();
	m_e.print();
	assert(m_e == m_s);
}

void rank() {
	vector row1(std::vector<number>{number(1), number(2), number(3)});
	vector row2(std::vector<number>{number(0), number(0), number(0)});
	vector row3(std::vector<number>{number(-1), number(-2), number(-3)});
	matrix m(std::vector<vector>{row1, row2, row3});
	assert(m.rank() == 1);

	vector row6(std::vector<number>{number(1), number(0), number(2), number(-1)});
	vector row7(std::vector<number>{number(3), number(0), number(0), number(5)});
	vector row8(std::vector<number>{number(2), number(1), number(4), number(-3)});
	vector row9(std::vector<number>{number(1), number(0), number(5), number(0)});
	matrix m2(std::vector<vector>{row6, row7, row8, row9});
	assert(m2.rank() == 4);

	vector row13(std::vector<number>{number(3), number(3), number(3), number(3), number(2)});
	vector row14(std::vector<number>{number(2), number(7), number(0), number(2), number(1)});
	vector row15(std::vector<number>{number(4), number(3), number(3), number(1), number(0)});
	vector row16(std::vector<number>{number(0), number(3), number(3), number(6), number(7)});
	vector row17(std::vector<number>{number(2), number(3), number(6), number(6), number(6)});
	matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});
	assert(m4.rank() == 5);

	vector row10(std::vector<number>{number(1), number(2), number(3)});
	vector row11(std::vector<number>{number(4), number(5), number(6)});
	vector row12(std::vector<number>{number(7), number(8), number(9)});
	matrix m3(std::vector<vector>{row10, row11, row12});
	assert(m3.rank() == 2);
}

void determinant() {
	//2x2 det
	vector row4(std::vector<number>{number(1), number(2)});
	vector row5(std::vector<number>{number(2), number(1)});
	matrix m1(std::vector<vector>{row4, row5});
	assert(m1.det() == -3);

	//4x4 det
	vector row6(std::vector<number>{number(1), number(0), number(2), number(-1)});
	vector row7(std::vector<number>{number(3), number(0), number(0), number(5)});
	vector row8(std::vector<number>{number(2), number(1), number(4), number(-3)});
	vector row9(std::vector<number>{number(1), number(0), number(5), number(0)});
	matrix m2(std::vector<vector>{row6, row7, row8, row9});
	assert(m2.det() == 30);

	//3x3 det
	vector row10(std::vector<number>{number(6), number(1), number(1)});
	vector row11(std::vector<number>{number(4), number(-2), number(5)});
	vector row12(std::vector<number>{number(2), number(8), number(7)});
	matrix m3(std::vector<vector>{row10, row11, row12});
	assert(m3.det() == -306);

	//5x5 det
	vector row13(std::vector<number>{number(3), number(3), number(3), number(3), number(2)});
	vector row14(std::vector<number>{number(2), number(7), number(0), number(2), number(1)});
	vector row15(std::vector<number>{number(4), number(3), number(3), number(1), number(0)});
	vector row16(std::vector<number>{number(0), number(3), number(3), number(6), number(7)});
	vector row17(std::vector<number>{number(2), number(3), number(6), number(6), number(6)});
	matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});
	assert(m4.det() == -129);
}

void inverse() {

	using nv = std::vector< number >;
	using vv = std::vector< vector >;

	const vector e_x(nv{ number(1), number(1), number(2) });
	const vector e_y(nv{ number(2), number(4), number(10) });
	const vector e_z(nv{ number(3), number(5), number(4) });

	const matrix m_e(vv{ e_x, e_y, e_z });

	m_e.print();
	m_e.inv().print();

	vector row4(std::vector<number>{number(1), number(2)});
	vector row5(std::vector<number>{number(2), number(1)});
	matrix m1(std::vector<vector>{row4, row5});

	vector inv_row(std::vector<number>{number(-1) / number(3), number(2) / number(3)});
	vector inv_row1(std::vector<number>{number(2) / number(3), number(-1) / number(3)});
	matrix inv_m1(std::vector<vector>{inv_row, inv_row1});
	assert(m1.inv() == inv_m1);

	vector row6(std::vector<number>{number(1), number(0), number(2), number(-1)});
	vector row7(std::vector<number>{number(3), number(0), number(0), number(5)});
	vector row8(std::vector<number>{number(2), number(1), number(4), number(-3)});
	vector row9(std::vector<number>{number(1), number(0), number(5), number(0)});
	matrix m2(std::vector<vector>{row6, row7, row8, row9});

	vector inv_row6(
		std::vector<number>{number(5) / number(6), number(1) / number(6), number(0), number(-1) / number(3)});
	vector inv_row7(
		std::vector<number>{number(-5) / number(2), number(1) / number(10), number(1), number(1) / number(5)});
	vector inv_row8(std::vector<number>{number(-1) / number(6), number(-1) / number(30), number(0),
		number(4) / number(15)});
	vector inv_row9(
		std::vector<number>{number(-1) / number(2), number(1) / number(10), number(0), number(1) / number(5)});
	matrix inv_m2(std::vector<vector>{inv_row6, inv_row7, inv_row8, inv_row9});
	m2.print();
	m2.inv().print();
	inv_m2.print();

	assert(m2.inv() == inv_m2);

	vector row10(std::vector<number>{number(6), number(1), number(1)});
	vector row11(std::vector<number>{number(4), number(-2), number(5)});
	vector row12(std::vector<number>{number(2), number(8), number(7)});
	matrix m3(std::vector<vector>{row10, row11, row12});

	vector inv_row10(
		std::vector<number>{number(3) / number(17), number(-1) / number(306), number(-7) / number(306)});
	vector inv_row11(
		std::vector<number>{number(1) / number(17), number(-20) / number(153), number(13) / number(153)});
	vector inv_row12(
		std::vector<number>{number(-2) / number(17), number(23) / number(153), number(8) / number(153)});
	matrix inv_m3(std::vector<vector>{inv_row10, inv_row11, inv_row12});
	assert(m3.inv() == inv_m3);

	vector row13(std::vector<number>{number(3), number(3), number(3), number(3), number(2)});
	vector row14(std::vector<number>{number(2), number(7), number(0), number(2), number(1)});
	vector row15(std::vector<number>{number(4), number(3), number(3), number(1), number(0)});
	vector row16(std::vector<number>{number(0), number(3), number(3), number(6), number(7)});
	vector row17(std::vector<number>{number(2), number(3), number(6), number(6), number(6)});
	matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});

	vector inv_row13(std::vector<number>{number(13) / number(43), number(-12) / number(43), number(21) / number(43),
		number(22) / number(43), number(-28) / number(43)});
	vector inv_row14(std::vector<number>{number(-18) / number(43), number(10) / number(43), number(4) / number(43),
		number(-4) / number(43), number(9) / number(43)});
	vector inv_row15(std::vector<number>{number(-70) / number(129), number(5) / number(43), number(2) / number(43),
		number(-49) / number(129), number(26) / number(43)});
	vector inv_row16(std::vector<number>{number(72) / number(43), number(3) / number(43), number(-59) / number(43),
		number(-27) / number(43), number(7) / number(43)});
	vector inv_row17(std::vector<number>{number(-44) / number(43), number(-9) / number(43), number(48) / number(43),
		number(38) / number(43), number(-21) / number(43)});
	matrix inv_m4(std::vector<vector>{inv_row13, inv_row14, inv_row15, inv_row16, inv_row17});
	assert(m4.inv() == inv_m4);
}

void gauss_2() {
	vector row13(std::vector<number>{number(3), number(3), number(3), number(3), number(2)});
	vector row14(std::vector<number>{number(2), number(7), number(0), number(2), number(1)});
	vector row15(std::vector<number>{number(4), number(3), number(3), number(1), number(0)});
	vector row16(std::vector<number>{number(0), number(3), number(3), number(6), number(7)});
	vector row17(std::vector<number>{number(2), number(3), number(6), number(6), number(6)});
	matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});
	m4.gauss();

	assert(m4.row(0) == vector({ 1, 0, 0, 0, 0 }));
	assert(m4.row(1) == vector({ 0, 1, 0, 0, 0 }));
	assert(m4.row(2) == vector({ 0, 0, 1, 0, 0 }));
	assert(m4.row(3) == vector({ 0, 0, 0, 1, 0 }));
	assert(m4.row(4) == vector({ 0, 0, 0, 0, 1 }));

	vector row10(std::vector<number>{number(6), number(1), number(1)});
	vector row11(std::vector<number>{number(4), number(-2), number(5)});
	vector row12(std::vector<number>{number(2), number(8), number(7)});
	matrix m3(std::vector<vector>{row10, row11, row12});
	m3.gauss();

	assert(m3.row(0) == vector({ 1, 0, 0 }));
	assert(m3.row(1) == vector({ 0, 1, 0 }));
	assert(m3.row(2) == vector({ 0, 0, 1 }));

	vector row6(std::vector<number>{number(1), number(0), number(2), number(-1), number(9)});
	vector row7(std::vector<number>{number(3), number(0), number(0), number(5), number(2)});
	vector row8(std::vector<number>{number(2), number(1), number(4), number(-3), number(-7)});
	vector row9(std::vector<number>{number(1), number(0), number(5), number(0), number(1)});
	matrix m2(std::vector<vector>{row6, row7, row8, row9});
	m2.gauss();

	assert(m2.row(0) == vector({ 1, 0, 0, 0, number(15) / 2 }));
	assert(m2.row(1) == vector({ 0, 1, 0, 0, number(-291) / 10 }));
	assert(m2.row(2) == vector({ 0, 0, 1, 0, number(-13) / 10 }));
	assert(m2.row(3) == vector({ 0, 0, 0, 1, number(-41) / 10 }));
}

void col() {
	vector row13(std::vector<number>{number(3), number(3), number(3), number(3), number(2)});
	vector row14(std::vector<number>{number(2), number(7), number(0), number(2), number(1)});
	vector row15(std::vector<number>{number(4), number(3), number(3), number(1), number(0)});
	vector row16(std::vector<number>{number(0), number(3), number(3), number(6), number(7)});
	vector row17(std::vector<number>{number(2), number(3), number(6), number(6), number(6)});
	matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});

	vector col0({ 3, 2, 4, 0, 2 });
	vector col1({ 3, 7, 3, 3, 3 });
	vector col2({ 3, 0, 3, 3, 6 });
	vector col3({ 3, 2, 1, 6, 6 });
	vector col4({ 2, 1, 0, 7, 6 });

	assert(m4.col(0) == col0);
	assert(m4.col(1) == col1);
	assert(m4.col(2) == col2);
	assert(m4.col(3) == col3);
	assert(m4.col(4) == col4);
}

int main(){
	sanity();
	simple_vector();
	simple_determinant();
	gauss();
	inverse();
	determinant();
	gauss_2();
	col();
	rank();
}