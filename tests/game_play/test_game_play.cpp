#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <istream>
#include <fstream>
#include <filesystem>

#include "game_play/game_play.hpp"

namespace fs = std::filesystem;

class MockCin : public std::istringstream {
public:
  MockCin(std::string s) : std::istringstream(s) {}
};

class MockCout : public std::ostringstream {
public:
  MockCout(){}
};



class GamePlayTests : public ::testing::Test{
protected:
  void SetUp() override {
    
    // Create a test file
    testFile = "./src/linked_list/temp_test.cpp";
    std::ofstream(testFile) << "void test_method(){\n    // Original content\n}\n";

    // Redirecting cin to our stringstream
    oldCoutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(mockCout.rdbuf());
  }

  void TearDown() override {

    fs::remove(testFile);

    // Restoring original buffer
    std::cout.rdbuf(oldCoutBuffer);
  }
  fs::path testFile;
  MockCout mockCout;
  std::streambuf* oldCoutBuffer;
};

// TEST_F(GamePlayTests, TestDisplayMenuShowsAllOptions){
//   displayMenu();
//   std::string output = mockCout.str();
//
//   EXPECT_THAT(output, testing::HasSubstr("push_front"));
//   EXPECT_THAT(output, testing::HasSubstr("push_back"));
//   EXPECT_THAT(output, testing::HasSubstr("q. Quit"));
//   EXPECT_THAT(output, testing::HasSubstr("t. Run Tests and Exit"));
// }
//
// TEST_F(GamePlayTests, TestGetMethodChoiceReturnsUserInput){
//   MockCin mockCin("7\n");
//   std::cin.rdbuf(mockCin.rdbuf());
//   EXPECT_EQ(getMethodChoice(), "7");
//   std::cin.rdbuf(std::cin.rdbuf()); // Resetting Cin
// }


TEST_F(GamePlayTests, TestFindFile) {
    // Test finding an existing file
    EXPECT_EQ(findFile("linked_list.cpp").filename(), "linked_list.cpp");
    EXPECT_EQ(findFile("binary_tree.cpp").filename(), "binary_tree.cpp");

    // Test that it doesn't find files in game_play directory
    EXPECT_THROW(findFile("game_play.cpp"), std::runtime_error);

    // Test finding a non-existent file
    EXPECT_THROW(findFile("nonexistent.cpp"), std::runtime_error);
}

TEST_F(GamePlayTests, TestEditMethod) {

    std::string capturedFilename = "";
    int capturedCursorLine = 0;
    
    auto mockEditorFunc = [&](const std::string& filename,
                              int cursorLine, std::function<int(const char*)>){
      capturedFilename = filename;
      capturedCursorLine = cursorLine;
    };
     
    editMethod("test_method", "temp_test", mockEditorFunc);

    EXPECT_THAT(capturedFilename, testing::EndsWith("temp_test.cpp"));
    EXPECT_GT(capturedCursorLine, 0);

    // Read the edited file
    std::ifstream file(testFile);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());


    // Check if the content has been modified as expected
    EXPECT_THAT(content, testing::HasSubstr( "void test_method(){\n    // TODO: IMPLEMENT test_method METHOD HERE"));
    EXPECT_THAT(content, testing::Not(testing::HasSubstr("Original content")));
}















