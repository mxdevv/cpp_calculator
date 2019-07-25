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

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
