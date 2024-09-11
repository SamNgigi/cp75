#include <ios>
#include <iostream>
#include <string>
#include <limits>
#include <filesystem>
#include <fstream>
#include <regex>

#include "game_play/game_play.hpp"

void clearScreen(){
    return;
}

void displayMenu(){
    std::cout << "Cpp75 Practice Menu:\n";
    std::cout << "push_front\n";
    std::cout << "push_back\n";
    std::cout << "q. Quit\n";
    std::cout << "t. Run Tests and Exit\n";
    std::cout << "Choose a method to implement, 'q' to quit or 't' to run tests and Exit";
    return;
}

std::string getMethodChoice(){
    std::string choice = "";
    std::cin >> choice;
    

    #pragma push_macro("max")
    #undef max
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    #pragma pop_macro("max")

    return choice;
}

void editMethod(const std::string& method){
    std::filesystem::path sourcePath = "../linked_list/linked_list.cpp";
    std::filesystem::path tempPath = sourcePath.parent_path() / 
                                     std::filesystem::path("temp" + sourcePath.filename().string());

    std::ifstream inFile(sourcePath);
    std::ofstream outFile(tempPath);
    
    if(!inFile.is_open()){
        std::cerr << "Error: Unable to open source file " << sourcePath << std::endl;
        return;
    }

    if(!outFile.is_open()){
        std::cerr << "Error: Unable to create temp file " << tempPath << std::endl;
        return;
    }
    
    std::string line;
    bool methodFound = false;
    int lineCount = 0;
    int methodStartLine = 0;
    std::regex methodRegex(method + R"(\s*\([^)]*\)\s*\{)");

    while(std::getline(inFile, line)){
        lineCount++;
        if(std::regex_search(line, methodRegex)){
            methodFound = true;
            methodStartLine = lineCount + 1; // For positioning cursor within the function body
            outFile << line << "\n";
            outFile << "    // TODO: IMPLEMENT " << method << " METHOD HERE\n";
            outFile << "    // DESCRIPTION: [ Delete this comment and replace with " << method << " implementation\n";
            while(std::getline(inFile, line) && line.find("}") == std::string::npos){
                // Skip the existing implementation
                // This allows our temp file that we'll edit to only have the above comments
                // To write a fresh implementation
            }
            outFile << "}\n";
        } else {
            outFile << line << "\n"; // Rest of the code is copied to our temp file;
        }
    }
    

    inFile.close();
    outFile.close();

    if(!methodFound){
        std::cout << "Method not found in the file.\n";
        std::filesystem::remove(tempPath);
        return;
    }

    openEditor(tempPath.string(), methodStartLine);

    // Replace original file with edited temp file
    std::filesystem::remove(sourcePath);
    std::filesystem::rename(tempPath, sourcePath);

    return;
}

void openEditor(const std::string& filename, int cursorLine){
    return;
}

int runTests(){
    return -1;
}





