#include <gtest/gtest.h>

TEST(HelloWorld, TestSetup) {
  EXPECT_STREQ("Hello World", "Hello World");
  EXPECT_STRNE("Hello World", "Hello World!");
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
