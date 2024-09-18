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

// Mock for std::filesystem operations
class MockFilesystem {
public:
    MOCK_METHOD(bool, is_regular_file, (const fs::path&), (const));
    MOCK_METHOD(fs::path, filename, (), (const));
    MOCK_METHOD(std::uintmax_t, file_size, (const fs::path&), (const));
};

// Custom recursive_directory_iterator for testing
class TestRecursiveDirectoryIterator {
public:
    class Iterator {
    public:
        Iterator(const std::vector<fs::path>& paths, size_t index) 
            : paths_(paths), index_(index) {}

        Iterator& operator++() { ++index_; return *this; }
        bool operator!=(const Iterator& other) const { return index_ != other.index_; }
        fs::directory_entry operator*() const { return fs::directory_entry(paths_[index_]); }

    private:
        const std::vector<fs::path>& paths_;
        size_t index_;
    };

    TestRecursiveDirectoryIterator(const std::vector<fs::path>& paths) : paths_(paths) {}

    Iterator begin() { return Iterator(paths_, 0); }
    Iterator end() { return Iterator(paths_, paths_.size()); }

private:
    std::vector<fs::path> paths_;
};

class GamePlayTests : public ::testing::Test{
protected:
  void SetUp() override {

    // Create a temporary directory structure for testing
    temp_dir_ = fs::temp_directory_path() / "game_play_test";
    fs::create_directories(temp_dir_ / "file1");
    fs::create_directories(temp_dir_ / "file2");

    // Create some dummy .cpp files
    std::ofstream(temp_dir_ / "file1/test1.cpp") << "void method1() {}\nvoid method2() {}";
    std::ofstream(temp_dir_ / "file2/test2.cpp") << "int method3() {}\nfloat method4() {}";
    std::ofstream(temp_dir_ / "main.cpp") << "int main() {}";
    std::ofstream(temp_dir_ / "game_play.cpp") << "void game_play() {}";
    
    // Create a test file
    testFile = "./src/linked_list/temp_test.cpp";
    std::ofstream(testFile) << "void test_method(){\n    // Original content\n}\n";

    // Redirecting cin to our stringstream
    oldCoutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(mockCout.rdbuf());
  }

  void TearDown() override {

    // Clean up the temporary directory
    fs::remove_all(temp_dir_);

    fs::remove(testFile);

    // Restoring original buffer
    std::cout.rdbuf(oldCoutBuffer);
  }

  fs::path temp_dir_;
  fs::path testFile;
  MockCout mockCout;
  std::streambuf* oldCoutBuffer;
};

TEST_F(GamePlayTests, TestGetAvailableMethods) {
    // Redirect std::cerr to capture any error messages
    std::stringstream cerr_buffer;
    std::streambuf* old_cerr = std::cerr.rdbuf(cerr_buffer.rdbuf());

    auto methods = getAvailableMethods(temp_dir_);

    // Restore std::cerr
    std::cerr.rdbuf(old_cerr);

    ASSERT_EQ(methods.size(), 2);
    ASSERT_TRUE(methods.find("test1") != methods.end());
    ASSERT_TRUE(methods.find("test2") != methods.end());
    
    ASSERT_EQ(methods["test1"].size(), 2);
    ASSERT_EQ(methods["test2"].size(), 2);

    // Check if methods are sorted alphabetically
    EXPECT_EQ(methods["test1"][0].name, "method1");
    EXPECT_EQ(methods["test1"][1].name, "method2");
    EXPECT_EQ(methods["test2"][0].name, "method3");
    EXPECT_EQ(methods["test2"][1].name, "method4");

    // Check if main.cpp and game_play.cpp are excluded
    EXPECT_TRUE(cerr_buffer.str().empty());
}

// New test case for error handling
TEST_F(GamePlayTests, GetAvailableMethodsNonExistentPath) {
    fs::path non_existent_path = temp_dir_ / "non_existent";
    
    std::stringstream cerr_buffer;
    std::streambuf* old_cerr = std::cerr.rdbuf(cerr_buffer.rdbuf());

    auto methods = getAvailableMethods(non_existent_path);

    std::string error_output = cerr_buffer.str();
    std::cerr.rdbuf(old_cerr);

    EXPECT_TRUE(methods.empty());
    EXPECT_FALSE(error_output.empty());
    EXPECT_THAT(error_output, ::testing::HasSubstr("Error: The specified path does not exist or is not a directory"));
}

TEST_F(GamePlayTests, TestDisplayMenu) {
    std::map<std::string, std::vector<MethodInfo>> methodsByFile = {
        {"file1", {{"method1", "file1", "1.1"}, {"method2", "file1", "1.2"}}},
        {"file2", {{"method3", "file2", "2.1"}, {"method4", "file2", "2.2"}}}
    };

    // Redirect cout to capture the output
    std::stringstream output;
    std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

    displayMenu(methodsByFile);

    // Restore cout
    std::cout.rdbuf(old_cout);

    std::string expected_output = 
        "Cpp75 Practice Menu:\n"
        "File                Method\n"
        "----------------------------------------\n"
        "file1\n"
        "    1.1     method1\n"
        "    1.2     method2\n"
        "\n"
        "file2\n"
        "    2.1     method3\n"
        "    2.2     method4\n"
        "\n"
        "q. Quit\n"
        "t. Run Tests and Exit\n"
        "Choose a method to implement (e.g., 1.2), 'q' to quit or 't' to run tests and Exit\n";

    EXPECT_EQ(output.str(), expected_output);
}

TEST_F(GamePlayTests, TestGetMethodChoice) {
    std::map<std::string, std::vector<MethodInfo>> methodsByFile = {
        {"file1", {{"method1", "file1", "1.1"}, {"method2", "file1", "1.2"}}},
        {"file2", {{"method3", "file2", "2.1"}, {"method4", "file2", "2.2"}}}
    };

    // Test valid method choice
    {
        std::stringstream input("1.2\n");
        std::streambuf* old_cin = std::cin.rdbuf(input.rdbuf());

        MethodInfo result = getMethodChoice(methodsByFile);

        std::cin.rdbuf(old_cin);

        EXPECT_EQ(result.name, "method2");
        EXPECT_EQ(result.filename, "file1");
        EXPECT_EQ(result.selector, "1.2");
    }

    // Test quit option
    {
        std::stringstream input("q\n");
        std::streambuf* old_cin = std::cin.rdbuf(input.rdbuf());

        MethodInfo result = getMethodChoice(methodsByFile);

        std::cin.rdbuf(old_cin);

        EXPECT_EQ(result.name, "");
        EXPECT_EQ(result.filename, "");
        EXPECT_EQ(result.selector, "q");
    }

    // Test run tests option
    {
        std::stringstream input("t\n");
        std::streambuf* old_cin = std::cin.rdbuf(input.rdbuf());

        MethodInfo result = getMethodChoice(methodsByFile);

        std::cin.rdbuf(old_cin);

        EXPECT_EQ(result.name, "");
        EXPECT_EQ(result.filename, "");
        EXPECT_EQ(result.selector, "t");
    }

    // Test invalid input followed by valid input
    {
        std::stringstream input("invalid\n2.1\n");
        std::streambuf* old_cin = std::cin.rdbuf(input.rdbuf());

        MethodInfo result = getMethodChoice(methodsByFile);

        std::cin.rdbuf(old_cin);

        EXPECT_EQ(result.name, "method3");
        EXPECT_EQ(result.filename, "file2");
        EXPECT_EQ(result.selector, "2.1");
    }
}


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















