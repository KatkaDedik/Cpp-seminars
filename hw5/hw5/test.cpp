
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


int main(){
	simple_vector();
}