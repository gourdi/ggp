#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/linear_algebra/ggo_sparse_matrix.h>
#include <kernel/math/linear_algebra/ggo_linear_algebra.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(sparse_matrix, sparse_matrix)
{
	//ggo::sparse_matrix<float> m1(3);
	//m1.set(0, 2, 7);
	//m1.set(1, 2, 8);
	//m1.set(0, 1, 4);
	//m1.set(1, 1, 5);
	//m1.set(2, 1, 6);
	//m1.set(0, 0, 1);
	//m1.set(2, 0, 3);
	//m1.set(2, 2, 9);
	//m1.set(1, 0, 2);
	//GGO_CHECK_FLOAT_EQ(m1.get(0, 0), 1);
	//GGO_CHECK_FLOAT_EQ(m1.get(1, 0), 2);
	//GGO_CHECK_FLOAT_EQ(m1.get(2, 0), 3);
	//GGO_CHECK_FLOAT_EQ(m1.get(0, 1), 4);
	//GGO_CHECK_FLOAT_EQ(m1.get(1, 1), 5);
	//GGO_CHECK_FLOAT_EQ(m1.get(2, 1), 6);
	//GGO_CHECK_FLOAT_EQ(m1.get(0, 2), 7);
	//GGO_CHECK_FLOAT_EQ(m1.get(1, 2), 8);
	//GGO_CHECK_FLOAT_EQ(m1.get(2, 2), 9);
	//
	//ggo::sparse_matrix<float> m2(3);
	//m2.set(0, 0, 2);
	//m2.set(0, 1, -1);
	//m2.set(1, 0, -1);
	//m2.set(1, 1, 2);
	//m2.set(1, 2, -1);
	//m2.set(2, 1, -1);
	//m2.set(2, 2, 2);
 // std::function<bool(float, float)> compare_func = [](float v1, float v2) { return std::abs(v1 - v2) < 0.0001f; };
	//GGO_CHECK(ggo::is_matrix_symmetric(m2, m2.get_size(), compare_func));
	//
	//ggo::array_float b(3), r(3);
	//ggo::sparse_matrix<float> a(3);
	//b(0) = 3;
	//b(1) = -5;
	//b(2) = 4;
	//a.set(0, 0, 1);
	//a.set(0, 2, -1);
	//a.set(1, 1, 2);
	//a.set(2, 2, -2);
	//a.apply(b.data(), r.data());
	//GGO_CHECK(std::fabs(r(0) + 1) < 0.0001);
	//GGO_CHECK(std::fabs(r(1) + 10) < 0.0001);
	//GGO_CHECK(std::fabs(r(2) + 8) < 0.0001);
}
