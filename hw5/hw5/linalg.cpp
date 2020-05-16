#include "linalg.hpp"

/*--------------------VECTOR--------------------*/

vector vector::operator+(const vector& v) const {
	if (u.size() != v.size()) { return 0; }
	number_vector w;
	for (size_t i = 0; i < u.size(); i++) {
		w.emplace_back(u[i] + v[i]);
	}
	return w;
}

vector vector::operator-(const vector& v) const {
	if (u.size() != v.size()) { return vector(0); }
	number_vector w;
	for (size_t i = 0; i < u.size(); i++) {
		w.emplace_back(u[i] - v[i]);
	}
	return w;
}

number_vector vector::operator*(number scalar) const {
	number_vector w;
	for (size_t i = 0; i < u.size(); i++) {
		w.emplace_back(u[i] * scalar);
	}
	return w;
}

number vector::operator*(const vector& v) const {
	if (u.size() != v.size()) { return 0; }
	number dot = 0;
	for (size_t i = 0; i < u.size(); i++) {
		dot = dot + (u[i] * v[i]);
	}
	return dot;
}

vector vector::operator*(const matrix& mat) const {
	return mat.col(0);
}

bool vector::operator==(const vector& v) const
{
	if (u.size() != v.size()) return false;

	vector v_tmp = v * (u[0] / v[0]);

	for (size_t i = 0; i < u.size(); i++) {
		if (u[i] != v_tmp[i]) {
			return false;
		}
	}
	return true;
}



/*--------------------MATRIX--------------------*/

matrix& matrix::gauss() 
{
	return *this;
}

int matrix::rank() const
{
	return 0;
}

int matrix::det() const 
{
	return 0;
}

matrix matrix::inv() const 
{
	return *this;
}

vector matrix::row(int n) const 
{
	return mat[0];
}

vector matrix::col(int n) const 
{
	return mat[0];
}

matrix matrix::operator+(const matrix& right) const 
{
	return right;
}

bool matrix::operator==(const matrix& right) const 
{
	return right.col(0)[0] == 0;
}

vector matrix::operator*(const vector& v) const 
{
	return v;
}