
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
#include <random>

static std::vector<int> primes = {
    2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,
    53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
    199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281};

std::vector<number> prime_nvec(long seed, int dim) {
  std::mt19937 r(seed);
  std::vector<number> ret;
  for (int i = 0; i < dim; i++) {
    int index = r() % 60;
    ret.emplace_back(primes[index]);
    if (r() % 2) {
      ret[i].change_sgn();
    }
  }
  return ret;
}

vector prime_vector(long seed, int dim) {
  return vector(prime_nvec(seed, dim));
}

matrix prime_matrix(long seed, int cols, int rows, int defect = 0) {
  std::mt19937 r(seed);
  std::vector<vector> mat;
  for (int i = 0; i < rows - defect; i++) {
    mat.emplace_back(prime_vector(r(), cols));
  }
  for (int i = 0; i < defect; i++) {
    mat.push_back(mat[r() % mat.size()] * (i + 1));
  }
  return matrix(mat);
}

matrix transpose(const matrix &m, int dim) {
  std::vector<vector> mat;
  for (int i = 0; i < dim; i++) {
    mat.emplace_back(m.col(i));
  }
  return matrix(mat);
}

matrix prime_extended(long seed, int rows, int cols,
                      const std::vector<number> nvec) {
  std::mt19937 r(seed);
  std::vector<vector> mat;
  for (int i = 0; i < rows; i++) {
    std::vector<number> vec = prime_nvec(r(), cols);
    vec.emplace_back(nvec[i]);
    mat.emplace_back(vec);
  }
  return matrix(mat);
}

matrix subst_col(matrix mat, int col, const std::vector<number> &vec) {
  for (size_t i = 0; i < mat.row_count(); i++) {
    mat.set_element(col, i, vec[i]);
  }
  return mat;
}

/*composite(i, j) je souèin prime(50 + j) a prime(20) ..prime(20 + i / 2)*/
number composite(int i, int j) {
  number ret = primes[50 + j];
  for (int k = 0; k <= i / 2; k++) {
    ret = ret + primes[20 + k];
  }
  return ret;
}

/*mul_rows( mat, rf ) vynásobí kadı øádek matice mat odpovídajícím èíslem z
 * rf*/
matrix mul_rows(const matrix &m, const std::vector<number> &rf) {
  std::vector<vector> mat;
  for (size_t i = 0; i < rf.size(); i++) {
    mat.emplace_back(m.row(i) * rf[i]);
  }
  return matrix(mat);
}

number product(const std::vector<number> &rf) {
  number ret(1);
  for (size_t i = 0; i < rf.size(); i++) {
    ret = ret * rf[i];
  }
  return ret;
}

/*subst_row(mat, n, j, vec) nahradí j - tı øádek matice o n øádcích vektorem
 * vec*/
matrix subst_row(const matrix &mat, int n, int j, const vector &vec) {
  std::vector<vector> m;
  for (int i = 0; i < n; i++) {
    if (i == j) {
      m.emplace_back(vec);
      continue;
    }
    m.emplace_back(mat.row(i));
  }
  return matrix(m);
}

/*basis(j, n) vytvoøí j - tı kanonickı bázovı vektor dimenze n*/
vector basis(int j, int n) {
  std::vector<number> v;
  for (int i = 0; i < n; i++) {
    if (i == j) {
      v.emplace_back(1);
      continue;
    }
    v.emplace_back(0);
  }
  return vector(v);
}

void number_test() {
  assert(number(1) + number(2) == number(3));
  assert(
      (((number(417444192) / (number(1000000000) + 994455584)) * number(1089)) /
       2662) == (number(9) / 43) * (number(1089) / 2662));
  assert((number(7623) / 23958) -
             (number(417444192) / (number(1000000000) + 994455584)) *
                 number(1089) / 2662 ==
         (number(7623) / 23958) - (number(9) / 43) * (number(1089) / 2662));
  assert(number(31) * -7 == -217);
  assert(number(0) + (-217) == -217);
  assert(number(0) * -2 == 0);
  assert(number(0) * 2 == 0);
  assert(number(-2) * 0 == 0);
  assert(number(2) * -2 == -4);
  assert(number(-2) * 2 == -4);
  assert(number(2) * 2 == 4);
  assert(number(-2) * -2 == 4);
  assert(number(999999999) + 1 == number(1000000000));
  assert(number(0) - 2 == -2);
  assert(number(2) - 2 == 0);
  assert(number(2) - 0 == 2);
  assert(number(-2) - number(-2) == 0);
  assert(number(0) - number(-2) == 2);
  assert(number(-6) - (number(-7)) == 1);
  assert(number(-6) - number(7) == -13);
  assert(number(-2) != 2);
  number ss = number(0) / (number(-1) / number(2));
  assert(number(0) / (number(-1) / number(2)) == 0);
  assert(number(-6) + 5 == -1);
  assert(number(6) - 5 == 1);
  assert(number(0) - 1 == -1);
  assert(number(-1) + 0 == -1);
  assert(number(1) - 0 == 1);

  integer_number f(1);
  integer_number s(0xfffffff);
  integer_number t = s * integer_number(0xfffffff);
  f / t;
}

void simple_vector() {
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

  using nv = std::vector<number>;

  number zero(0);
  number one(1);
  number two(2);
  number tree(3);
  number four(4);

  const vector v_1(nv{zero, zero, one, four});
  const vector v_2(nv{four, zero, two, one});
  assert(v_1 * v_2 == 6);
  assert(v_2 * v_1 == 6);

  const vector v1(nv{zero, one, two});
  const vector v2 = v1 * 2;
  assert(v2 == 2 * v1);
  assert(v1 + v1 == v2);
  assert(v2 - v1 == v1);

  {
    const vector u(nv{one, two});
    const vector v(nv{-two, one});
    assert(u * v == 0);
    assert(u + v == vector(nv{-one, tree}));
    assert(u - v == vector(nv{tree, one}));
    assert((u + v) * (u - v) == 0);
  }
}

void inv() {
  using nv = std::vector<number>;
  using vv = std::vector<vector>;
  const number zero(0);
  const number one(1);
  const number two(2);
  const vector e_x(nv{two, zero});
  const vector e_y(nv{zero, one});
  const matrix m(vv{e_x, e_y});
  assert(m.det() == two);
}

void mat() {
  using nv = std::vector<number>;
  using vv = std::vector<vector>;
  const number zero(0);
  const number one(1);
  const vector v_0(3);
  const vector e_x(nv{one, zero, zero});
  const vector e_y(nv{zero, one, zero});
  const vector e_z(nv{zero, zero, one});
  const matrix m_id(vv{e_x, e_y, e_z});
  const matrix m_0(3, 3);
  const matrix m_1(vv{e_x, e_x, e_y});
  const matrix m_2(vv{e_x, e_y});
  assert(m_1 != m_id);
  assert(m_0.rank() == 0);
  assert(m_1 + m_0 == m_1);
}

void mul() {
  using nv = std::vector<number>;
  using vv = std::vector<vector>;
  const number zero(0);
  const number one(1);
  const vector v_0(3);
  const vector e_x(nv{one, zero, zero});
  const vector e_y(nv{zero, one, zero});
  const vector e_z(nv{zero, zero, one});
  const matrix m_id(vv{e_x, e_y, e_z});
  const matrix m_0(3, 3);
  assert(m_id * v_0 == v_0);
  assert(v_0 * m_id == v_0);
  const vector v_1(nv{one, zero});
  const matrix m_1(vv{e_x, e_y});
  assert(v_1 * m_1 == e_x);
}

void vec() {
  using nv = std::vector<number>;
  const number zero(0);
  const number one(1);
  const number two(2);
  const vector v_0(3);
  const vector e_x(nv{one, zero, zero});
  const vector e_y(nv{zero, one, zero});
  const vector e_z(nv{zero, zero, one});
  assert(e_x != e_y);
}

void sanity() {
  inv();
  mat();
  mul();
  vec();
}

void gauss() {
  using nv = std::vector<number>;
  using vv = std::vector<vector>;

  const vector e_x(nv{number(1), number(1), number(2)});
  const vector e_y(nv{number(2), number(4), number(8)});
  const vector e_z(nv{number(3), number(2), number(4)});
  const vector e_a(nv{number(9), number(3), number(5)});

  matrix m_e(vv{e_x, e_y, e_z, e_a});

  const vector s_x(nv{number(1), number(0), number(0)});
  const vector s_y(nv{number(0), number(1), number(0)});
  const vector s_z(nv{number(0), number(0), number(1)});
  const vector s_a(nv{number(0), number(0), number(0)});

  const matrix m_s(vv{s_x, s_y, s_z, s_a});
  m_e.gauss();
  assert(m_e == m_s);

  {
    const vector v1(nv{number(1), number(0), number(0), number(2), number(2),
                       number(0), number(0), number(0)});
    const vector v2(nv{number(0), number(1), number(2), number(0), number(0),
                       number(0), number(0), number(0)});
    const vector v3(nv{number(0), number(0), number(1), number(2), number(0),
                       number(0), number(0), number(0)});
    const vector v4(nv{number(3), number(0), number(0), number(1), number(0),
                       number(0), number(0), number(0)});
    const vector v5(nv{number(4), number(0), number(0), number(0), number(1),
                       number(0), number(0), number(0)});
    const vector v6(nv{number(0), number(0), number(0), number(0), number(0),
                       number(1), number(0), number(0)});
    const vector v7(nv{number(0), number(0), number(0), number(0), number(0),
                       number(0), number(1), number(0)});
    const vector v8(nv{number(0), number(0), number(0), number(0), number(0),
                       number(0), number(0), number(1)});

    matrix m(vv{v1, v2, v3, v4, v5, v6, v7, v8});
    assert(m.det() == -13);

    const vector v01(nv{number(-1) / number(13), number(0), number(0),
                        number(2) / number(13), number(2) / number(13),
                        number(0), number(0), number(0)});
    const vector v02(nv{number(12) / number(13), number(1), number(-2),
                        number(28) / number(13), number(-24) / number(13),
                        number(0), number(0), number(0)});
    const vector v03(nv{number(-6) / number(13), number(0), number(1),
                        number(-14) / number(13), number(12) / number(13),
                        number(0), number(0), number(0)});
    const vector v04(nv{number(3) / number(13), number(0), number(0),
                        number(7) / number(13), number(-6) / number(13),
                        number(0), number(0), number(0)});
    const vector v05(nv{number(4) / number(13), number(0), number(0),
                        number(-8) / number(13), number(5) / number(13),
                        number(0), number(0), number(0)});
    const vector v06(nv{number(0), number(0), number(0), number(0), number(0),
                        number(1), number(0), number(0)});
    const vector v07(nv{number(0), number(0), number(0), number(0), number(0),
                        number(0), number(1), number(0)});
    const vector v08(nv{number(0), number(0), number(0), number(0), number(0),
                        number(0), number(0), number(1)});

    matrix inv_m(vv{v01, v02, v03, v04, v05, v06, v07, v08});

    assert(m.inv() == inv_m);
    assert(inv_m.inv() == m);
    assert(inv_m.det() == number(-1) / number(13));
  }
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

  vector row13(std::vector<number>{number(3), number(3), number(3), number(3),
                                   number(2)});
  vector row14(std::vector<number>{number(2), number(7), number(0), number(2),
                                   number(1)});
  vector row15(std::vector<number>{number(4), number(3), number(3), number(1),
                                   number(0)});
  vector row16(std::vector<number>{number(0), number(3), number(3), number(6),
                                   number(7)});
  vector row17(std::vector<number>{number(2), number(3), number(6), number(6),
                                   number(6)});
  matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});
  assert(m4.rank() == 5);

  vector row10(std::vector<number>{number(1), number(2), number(3)});
  vector row11(std::vector<number>{number(4), number(5), number(6)});
  vector row12(std::vector<number>{number(7), number(8), number(9)});
  matrix m3(std::vector<vector>{row10, row11, row12});
  assert(m3.rank() == 2);

  matrix mat1({vector({number(5), number(), number(1), number(14)}),
               vector({number(2), number(), number(3), number(15)}),
               vector({number(4), number(), number(6), number(30)})});
  assert(mat1.rank() == 2);

  matrix mat({vector({number(5), number(-3), number(1), number(14)}),
              vector({number(2), number(3), number(3), number(15)}),
              vector({number(4), number(6), number(6), number(30)})});
  assert(mat.rank() == 2);
  {
    vector row1(std::vector<number>{number(1), number(2), number(3)});
    vector row2(std::vector<number>{number(0), number(0), number(0)});
    vector row3(std::vector<number>{number(-1), number(-2), number(-3)});
    matrix m(std::vector<vector>{row1, row2, row3});
    assert(m.rank() == 1);

    vector row6(
        std::vector<number>{number(1), number(0), number(2), number(-1)});
    vector row7(
        std::vector<number>{number(3), number(0), number(0), number(5)});
    vector row8(
        std::vector<number>{number(2), number(1), number(4), number(-3)});
    vector row9(
        std::vector<number>{number(1), number(0), number(5), number(0)});
    matrix m2(std::vector<vector>{row6, row7, row8, row9});
    assert(m2.rank() == 4);

    vector row13(std::vector<number>{number(3), number(3), number(3), number(3),
                                     number(2)});
    vector row14(std::vector<number>{number(2), number(7), number(0), number(2),
                                     number(1)});
    vector row15(std::vector<number>{number(4), number(3), number(3), number(1),
                                     number(0)});
    vector row16(std::vector<number>{number(0), number(3), number(3), number(6),
                                     number(7)});
    vector row17(std::vector<number>{number(2), number(3), number(6), number(6),
                                     number(6)});
    matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});
    assert(m4.rank() == 5);

    vector row10(std::vector<number>{number(1), number(2), number(3)});
    vector row11(std::vector<number>{number(4), number(5), number(6)});
    vector row12(std::vector<number>{number(7), number(8), number(9)});
    matrix m3(std::vector<vector>{row10, row11, row12});
    assert(m3.rank() == 2);

    for (size_t i = 0; i < primes.size(); i++) {
      for (size_t j = 0; j < primes.size(); j++) {
        vector row4(std::vector<number>{0 * primes.at(i), 0 * primes.at(j)});
        vector row5(
            std::vector<number>{number(-1) * primes.at(i), primes.at(j)});
        matrix m1(std::vector<vector>{row4, row5});
        assert(m1.rank() == 1);
      }
    }

    vector row18(std::vector<number>{number(3), number(5), number(7),
                                     number(11), number(13)});
    vector row19(std::vector<number>{number(-3), number(5), number(-7),
                                     number(11), number(-13)});
    vector row20(std::vector<number>{number(19), number(3), number(3),
                                     number(-3), number(3)});
    vector row21(std::vector<number>{number(5), number(5), number(-5),
                                     number(5), number(-19)});
    vector row22(std::vector<number>{number(-5), number(-5), number(5),
                                     number(-5), number(19)});
    vector row23(std::vector<number>{number(-5), number(-5), number(5),
                                     number(-5), number(19)});
    vector row24(std::vector<number>{number(-5), number(-5), number(5),
                                     number(-5), number(19)});
    vector row25(std::vector<number>{number(-5), number(-5), number(5),
                                     number(-5), number(19)});
    matrix m5(std::vector<vector>{row18, row19, row20, row21, row22, row23,
                                  row24, row25});
    assert(m5.rank() == 4);
  }
}

void determinant() {

  // 2x2 det = 0
  vector row1(std::vector<number>{number(1), number(0)});
  vector row2(std::vector<number>{number(0), number(0)});
  matrix m0(std::vector<vector>{row1, row2});
  assert(m0.det() == 0);

  // 2x2 det
  vector row4(std::vector<number>{number(1), number(2)});
  vector row5(std::vector<number>{number(2), number(1)});
  matrix m1(std::vector<vector>{row4, row5});
  assert(m1.det() == -3);

  // 4x4 det
  vector row6(std::vector<number>{number(1), number(0), number(2), number(-1)});
  vector row7(std::vector<number>{number(3), number(0), number(0), number(5)});
  vector row8(std::vector<number>{number(2), number(1), number(4), number(-3)});
  vector row9(std::vector<number>{number(1), number(0), number(5), number(0)});
  matrix m2(std::vector<vector>{row6, row7, row8, row9});
  assert(m2.det() == 30);

  // 3x3 det
  vector row10(std::vector<number>{number(6), number(1), number(1)});
  vector row11(std::vector<number>{number(4), number(-2), number(5)});
  vector row12(std::vector<number>{number(2), number(8), number(7)});
  matrix m3(std::vector<vector>{row10, row11, row12});
  assert(m3.det() == -306);

  // 5x5 det
  vector row13(std::vector<number>{number(3), number(3), number(3), number(3),
                                   number(2)});
  vector row14(std::vector<number>{number(2), number(7), number(0), number(2),
                                   number(1)});
  vector row15(std::vector<number>{number(4), number(3), number(3), number(1),
                                   number(0)});
  vector row16(std::vector<number>{number(0), number(3), number(3), number(6),
                                   number(7)});
  vector row17(std::vector<number>{number(2), number(3), number(6), number(6),
                                   number(6)});
  matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});
  assert(m4.det() == -129);

  // 8x8
  vector row18(std::vector<number>{number(3), number(3), number(3), number(3),
                                   number(2), number(6), number(1), number(1)});
  vector row19(std::vector<number>{number(2), number(7), number(0), number(2),
                                   number(1), number(6), number(1), number(1)});
  vector row20(std::vector<number>{number(4), number(3), number(3), number(1),
                                   number(0), number(6), number(1), number(1)});
  vector row21(std::vector<number>{number(0), number(3), number(3), number(6),
                                   number(7), number(6), number(1), number(1)});
  vector row22(std::vector<number>{number(2), number(3), number(6), number(6),
                                   number(6), number(6), number(1), number(1)});
  vector row23(std::vector<number>{number(2), number(7), number(0), number(2),
                                   number(1), number(3), number(1), number(4)});
  vector row24(std::vector<number>{number(4), number(3), number(3), number(1),
                                   number(0), number(6), number(9), number(1)});
  vector row25(std::vector<number>{number(0), number(3), number(3), number(6),
                                   number(7), number(6), number(1), number(5)});

  matrix m5(std::vector<vector>{row18, row19, row20, row21, row22, row23, row24,
                                row25});

  matrix tmp = m5;
  assert(m5.det() == 12384);
}

void inverse() {

  using nv = std::vector<number>;
  using vv = std::vector<vector>;

  const vector e_x(nv{number(1), number(1), number(2)});
  const vector e_y(nv{number(2), number(4), number(10)});
  const vector e_z(nv{number(3), number(5), number(4)});

  const matrix m_e(vv{e_x, e_y, e_z});

  vector row4(std::vector<number>{number(1), number(2)});
  vector row5(std::vector<number>{number(2), number(1)});
  matrix m1(std::vector<vector>{row4, row5});

  vector inv_row(
      std::vector<number>{number(-1) / number(3), number(2) / number(3)});
  vector inv_row1(
      std::vector<number>{number(2) / number(3), number(-1) / number(3)});
  matrix inv_m1(std::vector<vector>{inv_row, inv_row1});
  assert(m1.inv() == inv_m1);

  vector row6(std::vector<number>{number(1), number(0), number(2), number(-1)});
  vector row7(std::vector<number>{number(3), number(0), number(0), number(5)});
  vector row8(std::vector<number>{number(2), number(1), number(4), number(-3)});
  vector row9(std::vector<number>{number(1), number(0), number(5), number(0)});
  matrix m2(std::vector<vector>{row6, row7, row8, row9});

  vector inv_row6(std::vector<number>{number(5) / number(6),
                                      number(1) / number(6), number(0),
                                      number(-1) / number(3)});
  vector inv_row7(std::vector<number>{number(-5) / number(2),
                                      number(1) / number(10), number(1),
                                      number(1) / number(5)});
  vector inv_row8(std::vector<number>{number(-1) / number(6),
                                      number(-1) / number(30), number(0),
                                      number(4) / number(15)});
  vector inv_row9(std::vector<number>{number(-1) / number(2),
                                      number(1) / number(10), number(0),
                                      number(1) / number(5)});
  matrix inv_m2(std::vector<vector>{inv_row6, inv_row7, inv_row8, inv_row9});

  assert(m2.inv() == inv_m2);

  vector row10(std::vector<number>{number(6), number(1), number(1)});
  vector row11(std::vector<number>{number(4), number(-2), number(5)});
  vector row12(std::vector<number>{number(2), number(8), number(7)});
  matrix m3(std::vector<vector>{row10, row11, row12});

  vector inv_row10(std::vector<number>{number(3) / number(17),
                                       number(-1) / number(306),
                                       number(-7) / number(306)});
  vector inv_row11(std::vector<number>{number(1) / number(17),
                                       number(-20) / number(153),
                                       number(13) / number(153)});
  vector inv_row12(std::vector<number>{number(-2) / number(17),
                                       number(23) / number(153),
                                       number(8) / number(153)});
  matrix inv_m3(std::vector<vector>{inv_row10, inv_row11, inv_row12});
  assert(m3.inv() == inv_m3);

  vector row13(std::vector<number>{number(3), number(3), number(3), number(3),
                                   number(2)});
  vector row14(std::vector<number>{number(2), number(7), number(0), number(2),
                                   number(1)});
  vector row15(std::vector<number>{number(4), number(3), number(3), number(1),
                                   number(0)});
  vector row16(std::vector<number>{number(0), number(3), number(3), number(6),
                                   number(7)});
  vector row17(std::vector<number>{number(2), number(3), number(6), number(6),
                                   number(6)});
  matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});

  vector inv_row13(
      std::vector<number>{number(13) / number(43), number(-12) / number(43),
                          number(21) / number(43), number(22) / number(43),
                          number(-28) / number(43)});
  vector inv_row14(std::vector<number>{
      number(-18) / number(43), number(10) / number(43), number(4) / number(43),
      number(-4) / number(43), number(9) / number(43)});
  vector inv_row15(std::vector<number>{
      number(-70) / number(129), number(5) / number(43), number(2) / number(43),
      number(-49) / number(129), number(26) / number(43)});
  vector inv_row16(std::vector<number>{
      number(72) / number(43), number(3) / number(43), number(-59) / number(43),
      number(-27) / number(43), number(7) / number(43)});
  vector inv_row17(
      std::vector<number>{number(-44) / number(43), number(-9) / number(43),
                          number(48) / number(43), number(38) / number(43),
                          number(-21) / number(43)});
  matrix inv_m4(std::vector<vector>{inv_row13, inv_row14, inv_row15, inv_row16,
                                    inv_row17});
  assert(m4.inv() == inv_m4);
  {
    vector row4(std::vector<number>{number(1), number(2)});
    vector row5(std::vector<number>{number(2), number(1)});
    matrix m1(std::vector<vector>{row4, row5});

    vector inv_row(
        std::vector<number>{number(-1) / number(3), number(2) / number(3)});
    vector inv_row1(
        std::vector<number>{number(2) / number(3), number(-1) / number(3)});
    matrix inv_m1(std::vector<vector>{inv_row, inv_row1});
    assert(m1.inv() == inv_m1);

    vector row6(
        std::vector<number>{number(1), number(0), number(2), number(-1)});
    vector row7(
        std::vector<number>{number(3), number(0), number(0), number(5)});
    vector row8(
        std::vector<number>{number(2), number(1), number(4), number(-3)});
    vector row9(
        std::vector<number>{number(1), number(0), number(5), number(0)});
    matrix m2(std::vector<vector>{row6, row7, row8, row9});

    vector inv_row6(std::vector<number>{number(5) / number(6),
                                        number(1) / number(6), number(0),
                                        number(-1) / number(3)});
    vector inv_row7(std::vector<number>{number(-5) / number(2),
                                        number(1) / number(10), number(1),
                                        number(1) / number(5)});
    vector inv_row8(std::vector<number>{number(-1) / number(6),
                                        number(-1) / number(30), number(0),
                                        number(4) / number(15)});
    vector inv_row9(std::vector<number>{number(-1) / number(2),
                                        number(1) / number(10), number(0),
                                        number(1) / number(5)});
    matrix inv_m2(std::vector<vector>{inv_row6, inv_row7, inv_row8, inv_row9});
    assert(m2.inv() == inv_m2);

    vector row10(std::vector<number>{number(6), number(1), number(1)});
    vector row11(std::vector<number>{number(4), number(-2), number(5)});
    vector row12(std::vector<number>{number(2), number(8), number(7)});
    matrix m3(std::vector<vector>{row10, row11, row12});

    vector inv_row10(std::vector<number>{number(3) / number(17),
                                         number(-1) / number(306),
                                         number(-7) / number(306)});
    vector inv_row11(std::vector<number>{number(1) / number(17),
                                         number(-20) / number(153),
                                         number(13) / number(153)});
    vector inv_row12(std::vector<number>{number(-2) / number(17),
                                         number(23) / number(153),
                                         number(8) / number(153)});
    matrix inv_m3(std::vector<vector>{inv_row10, inv_row11, inv_row12});
    assert(m3.inv() == inv_m3);

    vector row13(std::vector<number>{number(3), number(3), number(3), number(3),
                                     number(2)});
    vector row14(std::vector<number>{number(2), number(7), number(0), number(2),
                                     number(1)});
    vector row15(std::vector<number>{number(4), number(3), number(3), number(1),
                                     number(0)});
    vector row16(std::vector<number>{number(0), number(3), number(3), number(6),
                                     number(7)});
    vector row17(std::vector<number>{number(2), number(3), number(6), number(6),
                                     number(6)});
    matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});

    vector inv_row13(
        std::vector<number>{number(13) / number(43), number(-12) / number(43),
                            number(21) / number(43), number(22) / number(43),
                            number(-28) / number(43)});
    vector inv_row14(
        std::vector<number>{number(-18) / number(43), number(10) / number(43),
                            number(4) / number(43), number(-4) / number(43),
                            number(9) / number(43)});
    vector inv_row15(
        std::vector<number>{number(-70) / number(129), number(5) / number(43),
                            number(2) / number(43), number(-49) / number(129),
                            number(26) / number(43)});
    vector inv_row16(
        std::vector<number>{number(72) / number(43), number(3) / number(43),
                            number(-59) / number(43), number(-27) / number(43),
                            number(7) / number(43)});
    vector inv_row17(
        std::vector<number>{number(-44) / number(43), number(-9) / number(43),
                            number(48) / number(43), number(38) / number(43),
                            number(-21) / number(43)});
    matrix inv_m4(std::vector<vector>{inv_row13, inv_row14, inv_row15,
                                      inv_row16, inv_row17});
    assert(m4.inv() == inv_m4);

    vector row18(std::vector<number>{number(-7), number(5)});
    vector row19(std::vector<number>{number(23), number(3)});
    vector v({31, 37});
    matrix m5(std::vector<vector>{row18, row19});
    matrix m5_inv = m5.inv();
    assert(m5_inv * (m5 * v) == v);
  }
}

void gauss_2() {
  vector row13(std::vector<number>{number(3), number(3), number(3), number(3),
                                   number(2)});
  vector row14(std::vector<number>{number(2), number(7), number(0), number(2),
                                   number(1)});
  vector row15(std::vector<number>{number(4), number(3), number(3), number(1),
                                   number(0)});
  vector row16(std::vector<number>{number(0), number(3), number(3), number(6),
                                   number(7)});
  vector row17(std::vector<number>{number(2), number(3), number(6), number(6),
                                   number(6)});
  matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});
  m4.gauss();

  assert(m4.row(0) == vector({1, 0, 0, 0, 0}));
  assert(m4.row(1) == vector({0, 1, 0, 0, 0}));
  assert(m4.row(2) == vector({0, 0, 1, 0, 0}));
  assert(m4.row(3) == vector({0, 0, 0, 1, 0}));
  assert(m4.row(4) == vector({0, 0, 0, 0, 1}));

  vector row10(std::vector<number>{number(6), number(1), number(1)});
  vector row11(std::vector<number>{number(4), number(-2), number(5)});
  vector row12(std::vector<number>{number(2), number(8), number(7)});
  matrix m3(std::vector<vector>{row10, row11, row12});
  m3.gauss();

  assert(m3.row(0) == vector({1, 0, 0}));
  assert(m3.row(1) == vector({0, 1, 0}));
  assert(m3.row(2) == vector({0, 0, 1}));

  vector row6(std::vector<number>{number(1), number(0), number(2), number(-1),
                                  number(9)});
  vector row7(std::vector<number>{number(3), number(0), number(0), number(5),
                                  number(2)});
  vector row8(std::vector<number>{number(2), number(1), number(4), number(-3),
                                  number(-7)});
  vector row9(std::vector<number>{number(1), number(0), number(5), number(0),
                                  number(1)});
  matrix m2(std::vector<vector>{row6, row7, row8, row9});
  m2.gauss();

  assert(m2.row(0) == vector({1, 0, 0, 0, number(15) / 2}));
  assert(m2.row(1) == vector({0, 1, 0, 0, number(-291) / 10}));
  assert(m2.row(2) == vector({0, 0, 1, 0, number(-13) / 10}));
  assert(m2.row(3) == vector({0, 0, 0, 1, number(-41) / 10}));
  {
    matrix mat1({vector({number(5), number(), number(1), number(14)}),
                 vector({number(2), number(), number(3), number(15)}),
                 vector({number(4), number(), number(6), number(30)})});
    mat1.gauss();
    assert(mat1.row(0) == vector({1, 0, 0, number(27) / 13}));
    assert(mat1.row(1) == vector({0, 0, 1, number(47) / 13}));
    assert(mat1.row(2) == vector({0, 0, 0, 0}));

    matrix mat2({vector({number(), number(), number(), number()}),
                 vector({number(2), number(76), number(3), number(15)}),
                 vector({number(4), number(77), number(6), number(30)})});
    mat2.gauss();
    assert(mat2.row(0) == vector({1, 0, number(3) / 2, number(15) / 2}));
    assert(mat2.row(1) == vector({0, 1, 0, 0}));
    assert(mat2.row(2) == vector({0, 0, 0, 0}));

    vector row13(std::vector<number>{number(3), number(3), number(3), number(3),
                                     number(2)});
    vector row14(std::vector<number>{number(2), number(7), number(0), number(2),
                                     number(1)});
    vector row15(std::vector<number>{number(4), number(3), number(3), number(1),
                                     number(0)});
    vector row16(std::vector<number>{number(0), number(3), number(3), number(6),
                                     number(7)});
    vector row17(std::vector<number>{number(2), number(3), number(6), number(6),
                                     number(6)});
    matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});
    m4.gauss();

    assert(m4.row(0) == vector({1, 0, 0, 0, 0}));
    assert(m4.row(1) == vector({0, 1, 0, 0, 0}));
    assert(m4.row(2) == vector({0, 0, 1, 0, 0}));
    assert(m4.row(3) == vector({0, 0, 0, 1, 0}));
    assert(m4.row(4) == vector({0, 0, 0, 0, 1}));

    vector row10(std::vector<number>{number(6), number(1), number(1)});
    vector row11(std::vector<number>{number(4), number(-2), number(5)});
    vector row12(std::vector<number>{number(2), number(8), number(7)});
    matrix m3(std::vector<vector>{row10, row11, row12});
    m3.gauss();

    assert(m3.row(0) == vector({1, 0, 0}));
    assert(m3.row(1) == vector({0, 1, 0}));
    assert(m3.row(2) == vector({0, 0, 1}));

    vector row6(std::vector<number>{number(1), number(0), number(2), number(-1),
                                    number(9)});
    vector row7(std::vector<number>{number(3), number(0), number(0), number(5),
                                    number(2)});
    vector row8(std::vector<number>{number(2), number(1), number(4), number(-3),
                                    number(-7)});
    vector row9(std::vector<number>{number(1), number(0), number(5), number(0),
                                    number(1)});
    matrix m2(std::vector<vector>{row6, row7, row8, row9});
    m2.gauss();

    assert(m2.row(0) == vector({1, 0, 0, 0, number(15) / 2}));
    assert(m2.row(1) == vector({0, 1, 0, 0, number(-291) / 10}));
    assert(m2.row(2) == vector({0, 0, 1, 0, number(-13) / 10}));
    assert(m2.row(3) == vector({0, 0, 0, 1, number(-41) / 10}));

    vector row(std::vector<number>{number(2), number(3), number(4), number(7)});
    vector row1(
        std::vector<number>{number(-6), number(2), number(4), number(5)});
    matrix m(std::vector<vector>{row, row1});
    m.gauss();

    assert(m.row(0) == vector({1, 0, number(-2) / 11, number(-1) / 22}));
    assert(m.row(1) == vector({0, 1, number(16) / 11, number(26) / 11}));

    vector row2(
        std::vector<number>{number(2), number(0), number(4), number(7)});
    vector row3(
        std::vector<number>{number(3), number(0), number(4), number(5)});
    matrix m5(std::vector<vector>{row2, row3});
    m5.gauss();

    assert(m5.row(0) == vector({1, 0, 0, -2}));
    assert(m5.row(1) == vector({0, 0, 1, number(11) / 4}));
  }
}

void col() {
  vector row13(std::vector<number>{number(3), number(3), number(3), number(3),
                                   number(2)});
  vector row14(std::vector<number>{number(2), number(7), number(0), number(2),
                                   number(1)});
  vector row15(std::vector<number>{number(4), number(3), number(3), number(1),
                                   number(0)});
  vector row16(std::vector<number>{number(0), number(3), number(3), number(6),
                                   number(7)});
  vector row17(std::vector<number>{number(2), number(3), number(6), number(6),
                                   number(6)});
  matrix m4(std::vector<vector>{row13, row14, row15, row16, row17});

  vector col0({3, 2, 4, 0, 2});
  vector col1({3, 7, 3, 3, 3});
  vector col2({3, 0, 3, 3, 6});
  vector col3({3, 2, 1, 6, 6});
  vector col4({2, 1, 0, 7, 6});

  assert(m4.col(0) == col0);
  assert(m4.col(1) == col1);
  assert(m4.col(2) == col2);
  assert(m4.col(3) == col3);
  assert(m4.col(4) == col4);
}

void cramer() {

  std::mt19937 r(727);

  for (int n = 2; n < 8; n++) {
    for (int i = 4; i < 6; i++) {
      long seed = r();

      auto mat = transpose(prime_matrix(seed, n, n), n);
      auto nvec = prime_nvec(i, n);
      auto vec = vector(nvec);
      auto ext = prime_extended(seed, n, n, nvec);
      auto det = mat.det();
      assert(det != 0);
      ext.gauss();
      auto sol = ext.col(n);
      for (int j = 0; j < n; j++) {
        auto x = subst_row(mat, n, j, vec).det() / det;
        auto bas = basis(j, n);
        assert(sol * bas == x);
      }
    }
  }

  for (int n = 5; n < 8; n++) {

    int i = n;
    long seed = r();

    auto mat = transpose(prime_matrix(seed, n, n), n); // n = 4
    auto nvec = prime_nvec(i, n);
    auto vec = vector(nvec);
    auto ext = prime_extended(seed, n, n, nvec);
    auto det = mat.det();

    std::vector<number> qxyz;
    for (int j = 0; j < n; j++) {
      qxyz.emplace_back(subst_col(mat, j, nvec).det() / det);
    }

    for (int j = 0; j < n; j++) {
      number control(0);
      for (int k = 0; k < n; k++) {
        control = control + (mat.get_element(k, j) * qxyz[k]);
      }
      assert(control == nvec[j]);
    }
  }

  {
    vector row10(std::vector<number>{number(6), number(1), number(1)});
    vector row11(std::vector<number>{number(4), number(-2), number(5)});
    matrix m3(std::vector<vector>{row10, row11});
    matrix x1({vector({1, 1}), vector({5, -2})});

    matrix x2({vector({6, 1}), vector({4, -2})});

    assert(x1.det() / x2.det() == number(7) / 16);

    matrix y1({vector({6, 1}), vector({4, 5})});

    matrix y2({vector({6, 1}), vector({4, -2})});

    assert(y1.det() / y2.det() == number(-13) / 8);
  }
}

void det() {

  for (int n = 3; n < 9; n++) {
    for (int i = 4; i < 6; i++) {
      auto m_1 = prime_matrix(i, n, n);
      std::vector<number> rf;
      for (int j = 0; j < n; j++) {
        rf.emplace_back(composite(i, j));
      }
      auto m_2 = mul_rows(m_1, rf);
      assert(m_1.det() * product(rf) == m_2.det());
    }
  }
}

void inv_verity() {

  std::mt19937 r(727);

  for (int n = 4; n < 9; n++) {
    for (int i = n; i < 9; i++) {

      long seed = r();

      auto mat = prime_matrix(seed, n, n); // n =	3
      auto vec = prime_vector(seed, n);
      auto inv = mat.inv();
      assert(mat.det() * inv.det() == 1);
      assert(mat * (inv * vec) == vec);
    }
  }
}

void rank_verity() {

  std::mt19937 r(727);

  for (int n = 2; n < 9; n++) {
    for (int m = n; m < 9; m++) {
      for (int defect = 0; defect < n; defect++) {
        long seed = r();
        auto mat = prime_matrix(seed, m, n, defect);
        int r = mat.rank();
        assert(r == n - defect);
      }
    }
  }
}

void verity() {
  inv_verity();
  cramer();
  det();
  rank_verity();
}

int main() {
  verity();
  number_test();
  sanity();
  simple_vector();
  gauss();
  inverse();
  determinant();
  gauss_2();
  col();
  rank();

  return 0;
}
