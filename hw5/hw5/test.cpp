
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

void inverse() {
	using nv = std::vector< number >;
	using vv = std::vector< vector >;

	const vector e_x(nv{ number(1), number(1), number(2) });
	const vector e_y(nv{ number(2), number(4), number(10) });
	const vector e_z(nv{ number(3), number(5), number(4) });

	const matrix m_e(vv{ e_x, e_y, e_z });

	m_e.print();
	m_e.inv().print();

}

int main(){
	sanity();
	simple_vector();
	simple_determinant();
	gauss();
	inverse();
}