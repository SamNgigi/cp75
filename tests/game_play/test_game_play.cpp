#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <istream>
#include <fstream>

#include "game_play/game_play.hpp"


class MockCin : public std::istringstream {
public:
  MockCin(std::string s) : std::istringstream(s) {}
};

class MockCout : public std::ostringstream {
public:
  MockCout(){}
};

class MockSystem: public SystemInterface {
public:
  MOCK_METHOD(void, executeCommand, (const std::string &command), (const override));
};

class GamePlayTests : public ::testing::Test{
protected:
  void SetUp() override {
    // Redirecting cin to our stringstream
    oldCoutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(mockCout.rdbuf());
  }

  void TearDown() override {
    // Restoring original buffer
    std::cout.rdbuf(oldCoutBuffer);
  }

  MockCout mockCout;
  std::streambuf* oldCoutBuffer;
};

TEST_F(GamePlayTests, TestDisplayMenuShowsAllOptions){
  displayMenu();
  std::string output = mockCout.str();

  EXPECT_THAT(output, testing::HasSubstr("push_front"));
  EXPECT_THAT(output, testing::HasSubstr("push_back"));
  EXPECT_THAT(output, testing::HasSubstr("q. Quit"));
  EXPECT_THAT(output, testing::HasSubstr("t. Run Tests and Exit"));
}

TEST_F(GamePlayTests, TestGetMethodChoiceReturnsUserInput){
  MockCin mockCin("7\n");
  std::cin.rdbuf(mockCin.rdbuf());
  EXPECT_EQ(getMethodChoice(), "7");
  std::cin.rdbuf(std::cin.rdbuf()); // Resetting Cin
}

TEST_F(GamePlayTests, TestEditMethodModifiesFile){
  // Creating a temp file for testing
  std::string tempFileName = "temp_test_file.cpp";
  std::ofstream tempFile(tempFileName);
  tempFile << "void testMethod(){\n   // Original content\n}\n";
  tempFile.close();

  // Mocking the openEditor function
  auto openEditorMock = [](const std::string& filename, int cursorLine) {
    std::ofstream file(filename, std::ios::app);
    file << "   // New content\n";
    file.close();
  };

  // Calling editMethod with our mock
  editMethod("testMethod");

  // Checking if file was modified
  std::ifstream modifiedFile(tempFileName);
  std::string content((std::istreambuf_iterator<char>(modifiedFile)),
                      std::istreambuf_iterator<char>());
  EXPECT_THAT(content, testing::HasSubstr("// New content"));

  // Cleaning up
  modifiedFile.close();
  std::remove(tempFileName.c_str());

}













