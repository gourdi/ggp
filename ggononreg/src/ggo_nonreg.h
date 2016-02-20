#ifndef __GGO_NON_REG__
#define __GGO_NON_REG__

#include <ggo_unit_test.h>
#include <ggo_unit_test_manager.h>
#include <ggo_color_stream.h>
#include <cmath>

#define GGO_UNIT_TEST_CLASS_NAME(test_case_name, unit_test_name) ggo_##test_case_name##_##unit_test_name##_unit_test
#define GGO_UNIT_TEST_INSTANCE_NAME(test_case_name, unit_test_name) instance_##test_case_name##_##unit_test_name##_unit_test

#define GGO_TEST(test_case_name, unit_test_name) class GGO_UNIT_TEST_CLASS_NAME(test_case_name, unit_test_name) : public ggo_unit_test { public: \
                                                 GGO_UNIT_TEST_CLASS_NAME(test_case_name, unit_test_name)() : ggo_unit_test(#test_case_name, #unit_test_name) {} \
                                                 void run_test() override; }; \
                                                 static GGO_UNIT_TEST_CLASS_NAME(test_case_name, unit_test_name) GGO_UNIT_TEST_INSTANCE_NAME(test_case_name, unit_test_name); \
                                                 void GGO_UNIT_TEST_CLASS_NAME(test_case_name, unit_test_name)::run_test()

#define RUN_ALL_TESTS(argc, argv) ggo_unit_test_manager::instance().run_all(argc, argv);

#define GGO_CHECK(cond) if (!(cond)) { ++_failed_count; ggo::color_stream cs(ggo::console_color::RED); cs << "** TEST FAILED ** " << '[' << __FILE__ << ':' << __LINE__ << "\n"; }

#define GGO_CHECK_FABS(v1, v2) {auto eval1 = (v1); auto eval2 = (v2); \
                                if (std::fabs(eval1 - eval2) > 0.0001) { \
                                  ++_failed_count; \
                                  ggo::color_stream cs(ggo::console_color::RED); cs << "** TEST FAILED ** (" << __FILE__ << ':' << __LINE__ << ") " << \
                                  eval1 << " != " << eval2 << std::endl; \
                                } }

#endif