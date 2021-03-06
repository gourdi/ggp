#ifndef __GGO_NON_REG__
#define __GGO_NON_REG__

#include <kernel/nonreg/ggo_unit_test.h>
#include <kernel/nonreg/ggo_unit_test_manager.h>
#include <kernel/ggo_log.h>
#include <kernel/ggo_string_helpers.h>
#include <cmath>
#include <ostream>

template <typename data_t>
std::ostream & operator<<(std::ostream & os, const std::vector<data_t> & v)
{
  os << '(';
  ggo::join(os, v, ", ");
  os << ')';
  return os;
}

#define GGO_UNIT_TEST_CLASS_NAME(test_case_name, unit_test_name) ggo_##test_case_name##_##unit_test_name##_unit_test
#define GGO_UNIT_TEST_INSTANCE_NAME(test_case_name, unit_test_name) instance_##test_case_name##_##unit_test_name##_unit_test

#define GGO_TEST(test_case_name, unit_test_name) class GGO_UNIT_TEST_CLASS_NAME(test_case_name, unit_test_name) : public ggo::unit_test { public: \
                                                 GGO_UNIT_TEST_CLASS_NAME(test_case_name, unit_test_name)() : ggo::unit_test(#test_case_name, #unit_test_name) {} \
                                                 void run_test() override; }; \
                                                 static GGO_UNIT_TEST_CLASS_NAME(test_case_name, unit_test_name) GGO_UNIT_TEST_INSTANCE_NAME(test_case_name, unit_test_name); \
                                                 void GGO_UNIT_TEST_CLASS_NAME(test_case_name, unit_test_name)::run_test()

#define RUN_ALL_TESTS(argc, argv) ggo::unit_test_manager::instance().run_all(argc, argv);

#define GGO_TEST_FAILED_COUT std::cout << ggo::red_color << "** TEST FAILED ** " << __FILE__ << ':' << __LINE__ << ggo::default_color

#define GGO_CHECK(cond) {\
if (!((cond))) { \
  ++_failed_count; \
  GGO_TEST_FAILED_COUT << ": " << #cond << std::endl; }}

#define GGO_CHECK_EQ(v1, v2) {\
auto eval1 = (v1); auto eval2 = (v2); \
if (eval1 != eval2) { \
  ++_failed_count; \
  GGO_TEST_FAILED_COUT << ": " << #v1 \
  << " [=" << eval1 << "] != " << #v2 << " [=" << eval2 << "]" << std::endl; }}

#define GGO_CHECK_NE(v1, v2) {\
auto eval1 = (v1); auto eval2 = (v2); \
if (eval1 == eval2) { \
  ++_failed_count; \
  GGO_TEST_FAILED_COUT << ": " << #v1 << " [=" << eval1 << "] == " << #v2 << " [=" << eval2 << "]" << std::endl; }}

#define GGO_CHECK_FLOAT_NEAR(v1, v2, tol) {\
auto eval1 = (v1); auto eval2 = (v2); \
if (std::abs(eval1 - eval2) > tol) { \
  ++_failed_count; \
  GGO_TEST_FAILED_COUT << ": " << #v1 << " [=" << eval1 << "] != " << #v2 << " [=" << eval2 << "]" << std::endl; }}

#define GGO_CHECK_FLOAT_EQ(v1, v2) GGO_CHECK_FLOAT_NEAR(v1, v2, 0.0001)

#define GGO_CHECK_VEC2F_NEAR(v1, v2, tol) {\
auto eval1 = (v1); auto eval2 = (v2); \
auto x1 = eval1.x(); auto x2 = eval2.x(); \
auto y1 = eval1.y(); auto y2 = eval2.y(); \
if (std::abs(x1 - x2) > 0.0001 || std::abs(y1 - y2) > 0.0001) { \
  ++_failed_count; \
  GGO_TEST_FAILED_COUT << ": " << #v1 << " [=" << eval1 << "] != " << #v2 << " [=" << eval2 << "]" << std::endl; }}

#define GGO_CHECK_VEC2F_EQ(v1, v2) GGO_CHECK_VEC2F_NEAR(v1, v2, 0.0001)

#define GGO_CHECK_THROW(code) { \
bool thrown = false; try{ code; } catch(...) { thrown = true; } \
if (thrown == false) {\
  ++_failed_count; \
  GGO_TEST_FAILED_COUT << ": no exception thrown" << std::endl; }}

#endif
