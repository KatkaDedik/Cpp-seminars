#pragma once
#include "number.hpp"

#include <vector>
#include "cmath"

class matrix;
using number_vector = std::vector<number>;

struct vector
{
	number_vector u;
public:
	vector(int size) : u(number_vector(size)) {}
	vector(number_vector v) : u(v) {}

	size_t size() const { return u.size(); }
	number_vector get() const { return u; }
	number operator[](int index) const { return u[index]; }
	
	vector operator+(const vector& v) const;
	vector operator-(const vector& v) const;
	number_vector operator*(number scalar) const;
	number operator*(const vector& v) const;
	vector operator*(const matrix& mat) const;
	bool operator==(const vector& v) const;
};

class matrix
{
	std::vector<vector> mat;
public: 

	matrix(int columns, int rows) {
		for (int i = 0; i < columns; i++) {
			mat.push_back(vector(rows));
		}
	}
	matrix(std::vector<vector> m) : mat(m) {}

	matrix& gauss();
	int rank() const;
	int det() const;
	matrix inv() const;
	vector row(int n) const;
	vector col(int n) const;

	matrix operator+(const matrix& right) const;
	bool operator==(const matrix& right) const;
	vector operator*(const vector& v) const;
};

