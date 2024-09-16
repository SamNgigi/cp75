#include <ios>
#include <iostream>
#include <string>
#include <limits>
#include <filesystem>
#include <fstream>
#include <regex>
#include <stdexcept>
#include <functional>

#include "game_play/game_play.hpp"


namespace fs = std::filesystem;



int init_game(){
    
    while(true){
        clearScreen();
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
    return -1;
}

void clearScreen(){
    return;
}

void displayMenu(){
    std::cout << "Cpp75 Practice Menu:\n";
    std::cout << "push_front\n";
    std::cout << "push_back\n";
    std::cout << "q. Quit\n";
    std::cout << "t. Run Tests and Exit\n";
    std::cout << "Choose a method to implement, 'q' to quit or 't' to run tests and Exit\n";
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

fs::path findFile(const std::string &filename){
    fs::path srcPath = "./src";
    
    if (!fs::exists(srcPath) || !fs::is_directory(srcPath)) {
        throw std::runtime_error("src directory not found or is not a directory");
    }

    for (const auto& entry : fs::recursive_directory_iterator(srcPath)) {
        if (entry.is_regular_file() && entry.path().filename() == filename) {
            // Check if the file is not in the game_play directory
            if (entry.path().parent_path().filename() != "game_play") {
                return entry.path();
            }
        }
    }

    throw std::runtime_error("File not found: " + filename);
}

void openEditor(const std::string& filename, 
                int cursorLine, 
                std::function<int(const char*)> systemCall) {
    #ifdef _WIN32
        // For Windows using GitBash and Neovim
        std::string command = "bash -c \"nvim '+call cursor(" + std::to_string(cursorLine) + ",0)' '" + filename + "'\"";
    #else
        std::string editor = std::getenv("EDITOR") ? std::getenv("EDITOR") : "nvim";
        command = editor + " '+call cursor(" + std::to_string(cursorLine) + ",5)' '" + filename + "'";        system(command.c_str());
    #endif  
    systemCall(command.c_str());
}


void editMethod(const std::string& method, 
                const std::string& filename,
                std::function<void(const std::string&, int, std::function<int(const char*)>)> editorFunc){
    OpenEditorSys openEditorSys;
    fs::path sourcePath;

    try {
        sourcePath = findFile(filename + ".cpp");
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return;
    }
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
    editorFunc(tempPath.string(), methodStartLine, std::system);
    
    // Replace the original file with the edited temp file
    std::filesystem::remove(sourcePath);
    std::filesystem::rename(tempPath,sourcePath); 

    return;
}


// New function to build the editor command
std::string buildEditorCommands(const std::string& filename, int cursorLine) {
    #ifdef _WIN32
        // For Windows using GitBash and Neovim
        std::string command = "bash -c \"nvim '+call cursor(" + std::to_string(cursorLine) + ",0)' '" + filename + "'\"";
    #else
        std::string editor = std::getenv("EDITOR") ? std::getenv("EDITOR") : "nvim";
        command = editor + " '+call cursor(" + std::to_string(cursorLine) + ",5)' '" + filename + "'";        system(command.c_str());
    #endif
    return command;
}




int runTests(){
    std::cout << "Running tests...\n";
    return 0;
}




