#include "linalg.hpp"

/*--------------------VECTOR--------------------*/

vector vector::operator+(const vector& v) const {
	if (u.size() != v.size()) { return *this; }
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

vector vector::operator*(const number& scalar) const {
	number_vector w;
	for (const auto & n : u) {
		w.emplace_back(n * scalar);
	}
	return vector(w);
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

	size_t j = 0;
	while (j != u.size() && v[j] == 0) {
		if (u[j] != 0) {
			return false;
		}
		j++;
	}

	if (j == u.size()) {
		return true;
	}

	vector v_tmp = v * (u[j] / v[j]);

	for (size_t i = 0; i < u.size(); i++) {
		if (u[i] != v_tmp[i]) {
			return false;
		}
	}
	return true;
}

bool vector::operator!=(const vector& v) const {
	return !(*this == v);
}



/*--------------------MATRIX--------------------*/

matrix& matrix::gauss()
{
	for (size_t r = 0; r < mat[0].size(); r++) {
		size_t non_zero = r;
		while (non_zero < mat.size() && mat[non_zero][r] == 0) {
			non_zero++;
		}

		number tmp_denominator = mat[non_zero][r];

		for (size_t i = non_zero; i < mat.size(); i++) {
			mat[i][r] = mat[i][r] / tmp_denominator;
		}


		for (size_t cur_r = 0; cur_r < mat[0].size(); cur_r++) {
			if (cur_r == r) continue;
			number dif = mat[non_zero][cur_r] / mat[non_zero][r];
			for (size_t cur_col = non_zero; cur_col < mat.size(); cur_col++) {
				mat[cur_col][cur_r] = mat[cur_col][cur_r] - (mat[cur_col][r] * dif);
			}
		}
	}
	return *this;
}

int matrix::rank() const
{
	matrix tmp = *this;
	tmp.gauss();
	int r = mat[0].size();
	vector zero_vector(mat.size());
	for (size_t i = 0; i < mat[0].size(); i++) {
		if (tmp.row(i) == zero_vector) {
			r--;
		}
	}
	return r;
}

number matrix::det() const {
	std::vector<std::vector<const number*>> pseudo_matrix;
	for (size_t i = 0; i < mat.size(); i++) {
		std::vector<const number*> tmp_vector;
		for (size_t j = 0; j < mat.size(); j++) {
			tmp_vector.push_back(&(mat[i][j]));
		}
		pseudo_matrix.push_back(tmp_vector);
	}
	return determinant(pseudo_matrix);
}

number matrix::determinant(const std::vector<std::vector<const number*>>& pseudo_matrix) const {
	if (pseudo_matrix.size() == 1) {
		return *pseudo_matrix[0][0];
	}

	number det_sum(0);

	for (size_t i = 0; i < pseudo_matrix.size(); i++) {
		std::vector<std::vector<const number*>> tmp_matrix;
		for (size_t j = 0; j < pseudo_matrix.size(); j++) {
			if (j == i) continue;
			std::vector<const number*> tmp_vector;
			for (size_t k = 1; k < pseudo_matrix.size(); k++) {
				tmp_vector.push_back(pseudo_matrix[j][k]);
			}
			tmp_matrix.push_back(tmp_vector);
		}

		if (i % 2 == 0) {
			det_sum = det_sum + (*pseudo_matrix[i][0] * determinant(tmp_matrix));
		}
		else {
			det_sum = det_sum - (*pseudo_matrix[i][0] * determinant(tmp_matrix));
		}
	}
	return det_sum;
}

matrix matrix::inv() const
{

	if (mat.size() != mat[0].size()) {
		return *this;
	}

	std::vector<vector> tmp = mat;

	for (size_t i = 0; i < mat.size(); i++) {
		vector v(mat.size());
		v.set(i, 1);
		tmp.emplace_back(v);
	}

	matrix tmp_matrix(tmp);
	tmp_matrix.gauss();

	std::vector<vector> inverse;

	for (size_t i = mat.size(); i < tmp_matrix.size(); i++) {
		inverse.emplace_back(tmp_matrix.col(i));
	}

	return matrix(inverse);
}

vector matrix::row(int n) const 
{
	number_vector r;
	for (auto col : mat) {
		r.emplace_back(col[n]);
	}
	return vector(r);
}

vector matrix::col(int n) const 
{
	return mat[n];
}

matrix matrix::operator+(const matrix& right) const 
{
	matrix ret(mat.size(), mat[0].size());

	for (size_t i = 0; i < mat.size(); i++) {
		for (size_t j = 0; j < mat[0].size(); j++) {
			ret.set_element(i, j, mat[i][j] + right.get_element(i, j));
		}
	}

	return ret;
}

bool matrix::operator==(const matrix& right) const 
{

	for (size_t i = 0; i < mat.size(); i++) {
		for (size_t j = 0; j < mat[0].size(); j++) {
			if (mat[i][j] != right.get_element(i, j)) {
				return false;
			}
		}
	}

	return true;
}

bool matrix::operator!=(const matrix& right) const
{
	return !(*this == right);
}

vector matrix::operator*(const vector& v) const 
{
	number_vector ret;

	for (size_t i = 0; i < mat[0].size(); i++) {
		number tmp;
		for (size_t j = 0; j < mat.size(); j++) {
			tmp = tmp + (mat[j][i] * v[j]);
		}
		ret.emplace_back(tmp);
	}

	return vector(ret);
}