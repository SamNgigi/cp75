#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <limits>
#include <filesystem>
#include <regex>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

#include "linked_list/linked_list.hpp"



// Function prototypes
void displayMenu();
std::string getMethodChoice();
void editMethod(const std::string& method);
int runTests();
void clearScreen();
void openEditor(const std::string& filename, int cursorLine);

int main(int argc, char* argv[]){    
    while(true){
        // clearScreen();
        displayMenu();
        std::string choice = getMethodChoice();

        if(choice == "q"){
            std::cout << "Thanks for practicing! Goodbye.\n";
            break;
        }

        if(choice == "t"){
            return runTests();
        }
        
        editMethod(choice);


    }
    
    return 0;
}

void displayMenu() {
    std::cout << "LinkedList Practice Menu:\n";
    std::cout << "1. push_front\n";
    std::cout << "2. push_back\n";
    std::cout << "t. Run tests\n";
    std::cout << "q. Quit\n";
    std::cout << "Choose a method to implement (1-10) 't' to run tests and exit, or 'q' to quit: ";
}


std::string getMethodChoice() {
    std::string choice;
    std::cin >> choice;
    // We wrap the cin.ignore with pragma push undef & pop because of the predefined max macro on window.h
    #pragma push_macro("max")
    #undef max
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    #pragma pop_macro("max")
    return choice;
}

void editMethod(const std::string& method) {
    std::filesystem::path sourcePath =  "./src/linked_list/linked_list.cpp";
    std::filesystem::path tempPath = sourcePath.parent_path() / std::filesystem::path("temp_" + sourcePath.filename().string());

    std::ifstream inFile(sourcePath);
    std::ofstream outFile(tempPath);

    if(!inFile.is_open()){
        std::cerr << "Error: Unable to open source file " << sourcePath << std::endl;
        return;
    }
    
    if(!outFile.is_open()){
        std::cerr << "Error: Unable to create temporary file " << tempPath << std::endl;
    }
        
    std::string line;
    bool methodFound = false;
    int lineCount = 0;
    int methodStartLine = 0;
    std::regex methodRegex(method + R"(\s*\([^)]*\)\s*\{)");

    while (std::getline(inFile, line)) {
        lineCount++;
        if (std::regex_search(line,methodRegex)) {
            methodFound = true;
            methodStartLine = lineCount + 1; // Position of cursor inside the function body
            outFile << line << "\n";
            outFile << "    // TODO: IMPLEMENT " << method << " METHOD HERE\n";
            outFile << "    // DESCRIPTION: [Delete this comment and replace with implementation]\n";
            while (std::getline(inFile, line) && line.find("}") == std::string::npos) {
                // Skip the existing implementation
            }
            outFile << "}\n";
        } else {
            outFile << line << "\n";
        }
    }
    
    inFile.close();
    outFile.close();
    
    if (!methodFound) {
        std::cout << "Method not found in the file.\n";
        std::filesystem::remove(tempPath);
        return;
    }
    
    // Open the file in Neovim
    openEditor(tempPath.string(), methodStartLine);
    
    // Give some time for the editor to fully close
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Replace the original file with the edited temp file
    int retries = 5;
    while (retries > 0) {
        try {
            std::filesystem::remove(sourcePath);
            std::filesystem::rename(tempPath, sourcePath);
            break;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error replacing file: " << e.what() << std::endl;
            std::cerr << "Retrying in 1 second... (" << retries << " retries left)" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            retries--;
        }
    }
    if (retries == 0) {
        std::cerr << "Failed to replace the file after multiple attempts." << std::endl;
    }
}


void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void openEditor(const std::string& filename, int cursorLine) {
    #ifdef _WIN32
        // For Windows using GitBash and Neovim
        std::string command = "bash -c \"nvim '+call cursor(" + std::to_string(cursorLine) + ",0)' '" + filename + "'\"";
    #else
        std::string editor = std::getenv("EDITOR") ? std::getenv("EDITOR") : "nvim";
        command = editor + " '+call cursor(" + std::to_string(cursorLine) + ",5)' '" + filename + "'";        system(command.c_str());
    #endif

    system(command.c_str());
}

int runTests(){
    std::cout << "Running tests...\n";
    return 0;
}



