#pragma once
#include "number.hpp"

#include <array>
#include <iomanip>
#include <iostream>
#include <vector>

class matrix;
using number_vector = std::vector<number>;

struct vector {
  number_vector u;

public:
  explicit vector(int size) : u(number_vector(size)) {}
  vector(number_vector v) : u(v) {}
  vector(const vector &v) = default;
  size_t size() const { return u.size(); }
  number_vector &get() { return u; }
  const number &operator[](int index) const { return u[index]; }
  number &operator[](int index) { return u[index]; }
  vector operator+(const vector &v) const;
  vector operator-(const vector &v) const;
  vector operator*(const number &scalar) const;
  number operator*(const vector &v) const;
  vector operator*(const matrix &mat) const;
  bool operator==(const vector &v) const;
  bool operator!=(const vector &v) const;
  void set(size_t index, const number &value) {
    if (index > u.size()) {
      throw std::logic_error("Index out of range!");
    }
    u[index] = value;
  }
};

class matrix {
  std::vector<vector> mat;

public:
  matrix(int columns, int rows) {
    for (int i = 0; i < rows; i++) {
      mat.emplace_back(columns);
    }
  }
  explicit matrix(std::vector<vector> m) : mat(m) {}
  matrix(const matrix &m) = default;

  number gauss_extended();
  matrix &gauss() {
    gauss_extended();
    return *this;
  }
  int rank() const;
  number det() const;
  matrix inv() const;
  vector row(int n) const;
  vector col(int n) const;
  size_t row_count() const { return mat.size(); }
  size_t col_count() const { return mat[0].size(); }
  matrix operator+(const matrix &right) const;
  bool operator==(const matrix &right) const;
  bool operator!=(const matrix &right) const;
  vector operator*(const vector &v) const;
  number get_element(int col, int row) const { return mat[row][col]; }
  void set_element(int col, int row, const number &val) { mat[row][col] = val; }
  std::vector<vector> &get() { return mat; }
};

inline vector operator*(const number &num, const vector &v) { return v * num; }
