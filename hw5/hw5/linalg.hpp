#pragma once
#include "number.hpp"

#include <vector>
#include <array>
#include "cmath"
#include <iostream>
#include <iomanip>

class matrix;
using number_vector = std::vector<number>;

struct vector
{
	number_vector u;
public:
	explicit vector(int size) : u(number_vector(size)) {}
	vector(number_vector v) : u(v) {}

	size_t size() const { return u.size(); }
	number_vector get() const { return u; }
	const number& operator[](int index) const { return u[index]; }
	number& operator[](int index) { return u[index]; }
	
	vector operator+(const vector& v) const;
	vector operator-(const vector& v) const;
	vector operator*(const number& scalar) const;
	number operator*(const vector& v) const;
	vector operator*(const matrix& mat) const;
	bool operator==(const vector& v) const;
	bool operator!=(const vector& v) const;
	void set(size_t index, const number& value) {
		if (index > u.size()) {
			throw std::logic_error("Index out of range!");
		}
		u[index] = value;
	}
};

class matrix
{
	std::vector<vector> mat;
public: 

	matrix(int columns, int rows) {
		for (int i = 0; i < columns; i++) {
			mat.emplace_back(vector(rows));
		}
	}

	explicit matrix(std::vector<vector> m) : mat(m) {}

	explicit matrix(int dimension) {
		for (int i = 0; i < dimension; i++) {
			vector tmp(dimension);
			tmp.set(i, number(1));
			mat.emplace_back(tmp);
		}
	}

	/*	‹m.gauss()› performs in - place Gaussian elimination : after the
	*	call, ‹m› should be in a reduced row echelon form */
	matrix& gauss();

	int rank() const;

	number det() const;

	number determinant(const std::vector<std::vector<const number*>>& pseudo_matrix) const;
	
	matrix inv() const;
	
	vector row(int n) const;
	
	vector col(int n) const;

	size_t size() const { return mat.size(); }

	matrix operator+(const matrix& right) const;
	bool operator==(const matrix& right) const;
	bool operator!=(const matrix& right) const;
	vector operator*(const vector& v) const;

	number get_element(int col, int row) const {
		return mat[col][row];
	}

	void set_element(int col, int row, const number& val) { mat[col][row] = val; }

	void print() const {

		for (size_t i = 0; i < mat[0].size(); i++) {
			for (size_t j = 0; j < mat.size(); j++) {
				if (mat[j][i].sgn()) {
					std::cout << "-";
				}
				else {
					std::cout << "+";
				}
				std::cout << std::setw(9) << mat[j][i].get_numerator().get(0) << "   ";
			}
			std::cout << std::endl;
			for (size_t j = 0; j < mat.size(); j++) {
				std::cout << "----------   ";
			}
			std::cout << std::endl;
			for (size_t j = 0; j < mat.size(); j++) {
				std::cout << std::setw(10) << mat[j][i].get_denominator().get(0) << "   ";
			}
			std::cout << std::endl;
			std::cout << std::endl;
		}
		std::cout << std::endl << std::endl << std::endl;
	}
};

inline vector operator*(const number& num, const vector& v)
{
    return v * num;
}



