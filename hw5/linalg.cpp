#include "linalg.hpp"

/*--------------------VECTOR--------------------*/

vector vector::operator+(const vector &v) const {
  if (u.size() != v.size()) {
    throw std::logic_error("invalid dimensions!");
  }
  number_vector w;
  for (size_t i = 0; i < u.size(); i++) {
    w.emplace_back(u[i] + v[i]);
  }
  return w;
}

vector vector::operator-(const vector &v) const {
  if (u.size() != v.size()) {
      throw std::logic_error("invalid dimensions!");
  }
  number_vector w;
  for (size_t i = 0; i < u.size(); i++) {
    w.emplace_back(u[i] - v[i]);
  }
  return w;
}

vector vector::operator*(const number &scalar) const {
  number_vector w;
  for (const auto &n : u) {
    w.emplace_back(n * scalar);
  }
  return vector(w);
}

number vector::operator*(const vector &v) const {
  if (u.size() != v.size()) {
    throw std::logic_error("invalid dimensions!");
  }
  number dot = 0;
  for (size_t i = 0; i < u.size(); i++) {
    dot = dot + (u[i] * v[i]);
  }
  return dot;
}

vector vector::operator*(const matrix &mat) const {
  
    if (u.size() != mat.row_count()) {
        throw std::logic_error("invalid dimensions!");
    }

  number_vector ret;

  for (size_t col = 0; col < mat.col_count(); col++) {
    number tmp;
    for (size_t row = 0; row < mat.row_count(); row++) {
      tmp = tmp + (mat.get_element(col, row) * u[row]);
    }
    ret.emplace_back(tmp);
  }

  return vector(ret);
}

bool vector::operator==(const vector &v) const {
  
  if (u.size() != v.size()) {
    return false;
  }

  for (size_t i = 0; i < u.size(); i++) {
    if (u[i] != v[i]) {
      return false;
    }
  }
  return true;
}

bool vector::operator!=(const vector &v) const { return !(*this == v); }

/*--------------------MATRIX--------------------*/

number matrix::gauss_extended() {
  number det(1);
  size_t non_zero_col = 0;
  for (size_t main_row = 0; main_row < row_count(); main_row++) {

    if (non_zero_col >= col_count()) {
      return number(0);
    }

    if (mat[main_row][non_zero_col] == 0) {
      bool col_is_zero = true;
      while (col_is_zero) {
        for (size_t tmp_row = main_row; tmp_row < row_count(); tmp_row++) {
          if (mat[tmp_row][non_zero_col] != 0) {
            mat[main_row].get().swap(mat[tmp_row].get());
            col_is_zero = false;
            det.change_sgn();
            non_zero_col--;
            break;
          }
        }
        non_zero_col++;
        if (non_zero_col >= col_count()) {
          return number(0);
        }
      }
    }
    number denominator_tmp = mat[main_row][non_zero_col];
    det = det * denominator_tmp;
    for (size_t i = non_zero_col; i < col_count(); i++) {
      mat[main_row][i] = mat[main_row][i] / denominator_tmp;
    }
    for (size_t cur_row = 0; cur_row < row_count(); cur_row++) {
      if (cur_row == main_row)
        continue;
      mat[cur_row] =
          mat[cur_row] - (mat[main_row] * mat[cur_row][non_zero_col]);
    }
    non_zero_col++;
  }
  return det;
}

int matrix::rank() const {
  matrix tmp = *this;
  tmp.gauss();
  int r = row_count();
  vector zero_vector(col_count());
  for (size_t i = 0; i < row_count(); i++) {
    if (tmp.row(i) == zero_vector) {
      r--;
    }
  }
  return r;
}

number matrix::det() const {
  if (mat.size() != mat[0].size()) {
    throw std::logic_error("non - square matrix!");
  }
  matrix tmp = *this;
  return tmp.gauss_extended();
}

matrix matrix::inv() const {
  if (mat.size() != mat[0].size()) {
    throw std::logic_error("non - square matrix!");
  }
  if (row_count() != col_count()) {
    return *this;
  }

  std::vector<vector> tmp = mat;

  for (size_t i = 0; i < mat.size(); i++) {
    number_vector v(mat.size());
    v[i] = 1;
    tmp[i].get().insert(tmp[i].get().end(), v.begin(), v.end());
  }

  matrix tmp_matrix(tmp);
  tmp_matrix.gauss();

  for (size_t i = 0; i < tmp_matrix.row_count(); i++) {
    tmp_matrix.get().at(i).get().erase(tmp_matrix.get().at(i).get().begin(),
                                       tmp_matrix.get().at(i).get().begin() +
                                           tmp_matrix.row_count());
  }
  return tmp_matrix;
}

vector matrix::row(int n) const { return mat[n]; }

vector matrix::col(int n) const {
  number_vector v;
  for (const vector &row : mat) {
    v.emplace_back(row[n]);
  }
  return vector(v);
}

matrix matrix::operator+(const matrix &right) const {
  
  if (mat.size() != right.row_count() ||mat[0].size() != right.col_count()) {
    throw std::logic_error("invalid dimensions!!");
  }
    
  matrix ret(col_count(), row_count());

  for (size_t row = 0; row < row_count(); row++) {
    for (size_t col = 0; col < col_count(); col++) {
      ret.set_element(col, row, mat[row][col] + right.get_element(col, row));
    }
  }

  return ret;
}

bool matrix::operator==(const matrix &right) const {
  if (row_count() != right.row_count() || col_count() != right.col_count()) {
    return false;
  }

  for (size_t row = 0; row < mat.size(); row++) {
    for (size_t col = 0; col < mat[0].size(); col++) {
      if (mat[row][col] != right.get_element(col, row)) {
        return false;
      }
    }
  }

  return true;
}

bool matrix::operator!=(const matrix &right) const { return !(*this == right); }

vector matrix::operator*(const vector &v) const {
  if (col_count() != v.size()) {
    throw std::logic_error("invalid dimensions!");
  }

  number_vector ret;

  for (size_t r = 0; r < row_count(); r++) {
    ret.emplace_back(row(r) * v);
  }

  return vector(ret);
}