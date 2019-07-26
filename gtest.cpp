#include <gtest/gtest.h>

#include "calc.cpp"

Calculator calc;

TEST(Calculator_tests_set, test1) {
  ASSERT_DOUBLE_EQ(2. + 2., calc.calculate("2+2"));
}

TEST(Calculator_tests_set, test2) {
  ASSERT_DOUBLE_EQ((5. - 7.) / 2., calc.calculate("(5-7)/2"));
}

TEST(Calculator_tests_set, test3) {
  ASSERT_DOUBLE_EQ(10. / 3., calc.calculate("10/3"));
}

TEST(Calculator_tests_set, test4) {
  ASSERT_DOUBLE_EQ(-1.1111 + (10. - 2.) / 3.,
      calc.calculate("-1.1111+(10-2)/3"));
}

TEST(Calculator_tests_set, test5) {
  ASSERT_DOUBLE_EQ(2. * ((11. + 1.) / (9. - 3.)),
      calc.calculate("2*((11+1)/(9-3))"));
}

TEST(Calculator_tests_set, test6) {
  ASSERT_DOUBLE_EQ(2. * ((11. + 1.) / (9. - 3.)),
      calc.calculate("2*[(11+1)/(9-3)]"));
}

TEST(Calculator_tests_set, test7_empty) {
  try {
    calc.calculate("");
    FAIL() << "Expected Calculator::precheck_empty_exception";
  }
  catch (Calculator::precheck_empty_exception& e) {
    SUCCEED();
  }
}

TEST(Calculator_tests_set, test8_superfluous_pluses) {
  try {
    calc.calculate("2+++2");
    FAIL() << "Expected Calculator::precheck_unary_plus_exception";
  }
  catch (Calculator::precheck_unary_plus_exception& e) {
    SUCCEED();
  }
}

TEST(Calculator_tests_set, test9_superfluous_minuses) {
  try {
    calc.calculate("2---2");
    FAIL() << "Expected Calculator::precheck_unary_minus_exception";
  }
  catch (Calculator::precheck_unary_minus_exception& e) {
    SUCCEED();
  }
}

TEST(Calculator_tests_set, test10_unidentified_strings) {
  try {
    calc.calculate("dog+cat");
    FAIL() << "Expected Calculator::precheck_unidentified_string_exception";
  }
  catch (Calculator::precheck_unidentified_string_exception& e) {
    SUCCEED();
  }
}

TEST(Calculator_tests_set, test11_brackets) {
  try {
    calc.calculate("2*(2+2))");
    FAIL() << "Expected Calculator::reverse_polish_notation_brackets_exception";
  }
  catch (Calculator::reverse_polish_notation_brackets_exception& e) {
    SUCCEED();
  }
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
