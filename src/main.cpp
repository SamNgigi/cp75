#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>

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
bool runTests();
void clearScreen();
void openEditor(const std::string& filename);

int main(){

  
  return 0;
}

void displayMenu() {
    std::cout << "LinkedList Practice Menu:\n";
    std::cout << "1. push_front\n";
    std::cout << "2. push_back\n";
    std::cout << "q. Quit\n";
    std::cout << "Choose a method to implement (1-10) or 'q' to quit: ";
}


std::string getMethodChoice() {
    std::string choice;
    std::getline(std::cin >> std::ws, choice);
    return choice;
}

void editMethod(const std::string& method) {
    std::string filename = "linked_list.cpp";
    std::string tempFilename = "temp_" + filename;
    std::ifstream inFile(filename);
    std::ofstream outFile(tempFilename);
    
    std::string line;
    bool methodFound = false;
    while (std::getline(inFile, line)) {
        if (line.find(method + "(") != std::string::npos) {
            methodFound = true;
            outFile << line << "\n{\n";
            outFile << "    // TODO: IMPLEMENT " << method << " METHOD HERE\n";
            outFile << "    // DESCRIPTION: [Brief description of what the method should do]\n";
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
        return;
    }
    
    // Open the file in Neovim
    openEditor(tempFilename);
    
    // Replace the original file with the edited temp file
    std::remove(filename.c_str());
    std::rename(tempFilename.c_str(), filename.c_str());
}

bool runTests() {
    // Compile and run the tests
    int result = system("./run.bat");
    
    if (result == 0) {
        std::cout << "All tests passed!\n";
        return true;
    } else {
        std::cout << "Some tests failed. Press Enter to continue editing...";
        std::cin.get();
        return false;
    }
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void openEditor(const std::string& filename) {
    #ifdef _WIN32
        // For Windows using GitBash and Neovim
        std::string command = "bash -c \"nvim '" + filename + "'\"";
        system(command.c_str());
    #else
        std::string editor = std::getenv("EDITOR") ? std::getenv("EDITOR") : "vim";
        std::string command = editor + " " + filename;
        system(command.c_str());
    #endif
}
