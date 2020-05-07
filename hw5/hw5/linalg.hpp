#pragma once
#include "number.hpp"

#include <vector>
#include "cmath"

using number = int;
using number_vector = std::vector<number>;

struct vector
{

	number_vector u;

public:

	vector(int size) : u(number_vector(size)) {}
	
	vector(number_vector v) : u(v) {}
	
	size_t size() const { return u.size(); }
	
	double length() const
	{
		number sum = 0;
		for (size_t i = 0; i < u.size(); i++) {
			sum += u[i] * u[i];
		}
		return std::sqrt(sum);
	}
	
	number_vector get() const { return u; }
	
	number operator[](int index) const { return u[index]; }
	
	vector operator+(const vector& v) const {
		if (u.size() != v.size()) { return 0; }
		number_vector w;
		for (size_t i = 0; i < u.size(); i++) {
			w.emplace_back(u[i] + v[i]);
		}
		return w;
	}
	
	vector operator-(const vector& v) const {
		if (u.size() != v.size()) { return vector(0); }
		number_vector w;
		for (size_t i = 0; i < u.size(); i++) {
			w.emplace_back(u[i] - v[i]);
		}
		return w;
	}

	number_vector operator*(number scalar) const {
		number_vector w;
		for (size_t i = 0; i < u.size(); i++) {
			w.emplace_back(u[i] * scalar);
		}
		return w;
	}

	number operator*(const vector& v) const {
		if (u.size() != v.size()) { return 0; }
		number dot = 0;
		for (size_t i = 0; i < u.size(); i++) {
			dot += u[i] * v[i];
		}
		return dot;
	}

	/*number_vector operator*(const matrix& m) const 
	{
	}*/

	bool operator==(const vector& v) const
	{
		if (u.size() != v.size()) return false;
		
		auto u_length = length();
		auto v_length = v.length();

		for (size_t i = 0; i < u.size(); i++) {
			if (u[i] / u_length != v[i] / v_length) {
				return false;
			}
		}
		return true;
	}
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

};