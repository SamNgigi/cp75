#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <filesystem>
#include <fstream>
#include <regex>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <iomanip>


#include "game_play/game_play.hpp"


namespace fs = std::filesystem;



int init_game(){
    
    while(true){
        clearScreen();
        auto methodsByFile = getAvailableMethods();
        MethodInfo chosenMethod = getMethodChoice(methodsByFile);

        if(chosenMethod.selector == "q"){
            std::cout << "Thanks for practicing! Goodbye.\n";
            break;
        }else if(chosenMethod.selector == "t"){
            std::cout << "Exiting game to run tests. A new window will open with test results\n";
            return 42;
        } else {
            editMethod(chosenMethod.name, chosenMethod.filename, openEditor);
        }
        


    }
    return -1;
}

void clearScreen(){
    return;
}

std::map<std::string, std::vector<MethodInfo>> getAvailableMethods() {
    std::map<std::string, std::vector<MethodInfo>> methodsByFile;
    
    std::regex methodRegex(R"((?:^|\n)\s*(?:virtual\s+)?(?:static\s+)?(?:inline\s+)?(?:explicit\s+)?(?:constexpr\s+)?(?:(?:const\s+)?(?:volatile\s+)?(?:\w+::)*\w+(?:\s*<[^>]*>)?(?:\s*\*|\s*&)?\s+)(?:\w+::)?(\w+)\s*\([^)]*\)(?:\s*const)?(?:\s*noexcept)?(?:\s*override)?(?:\s*final)?(?:\s*=\s*0)?\s*\{)");

    int fileCounter = 1;

    for (const auto& entry : fs::recursive_directory_iterator("./src")) {
        if (entry.path().extension() == ".cpp" && entry.path().filename() != "main.cpp" && entry.path().filename() != "game_play.cpp") {
            std::string fullFilename = entry.path().filename().string();
            std::string filenameNoExt = fullFilename.substr(0, fullFilename.find_last_of('.'));
            std::ifstream file(entry.path());

            if (!file.is_open()) {
                std::cerr << "Error: Unable to open file " << fullFilename << std::endl;
                continue;
            }

            std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            std::sregex_iterator iter(fileContent.begin(), fileContent.end(), methodRegex);
            std::sregex_iterator end;

            int methodCounter = 1;
            while (iter != end) {
                std::smatch match = *iter;
                std::string methodName = match[1].str();
                std::string selector = std::to_string(fileCounter) + "." + std::to_string(methodCounter);
                
                methodsByFile[filenameNoExt].push_back({methodName, filenameNoExt, selector});
                
                methodCounter++;
                ++iter;
            }

            if (methodCounter == 1) {
                std::cout << "No methods found in file: " << filenameNoExt << std::endl;
            }

            fileCounter++;
        }
    }

    // Sort methods within each file
    for (auto& [_, methods] : methodsByFile) {
        std::sort(methods.begin(), methods.end(), 
                  [](const MethodInfo& a, const MethodInfo& b) {
                      return a.name < b.name;
                  });
    }

    return methodsByFile;
}

void displayMenu(const std::map<std::string, std::vector<MethodInfo>>& methodsByFile) {
    std::cout << "Cpp75 Practice Menu:\n";
    std::cout << std::setw(20) << std::left << "File" << "Method\n";
    std::cout << std::string(40, '-') << "\n";
    
    for (const auto& [filename, methods] : methodsByFile) {
        std::cout << filename << "\n";
        for (const auto& method : methods) {
            std::cout << std::setw(4) << "" << std::setw(8) << std::left << method.selector 
                      << method.name << "\n";
        }
        std::cout << "\n";  // Add a blank line between files for better readability
    }
    
    std::cout << "q. Quit\n";
    std::cout << "t. Run Tests and Exit\n";
    std::cout << "Choose a method to implement (e.g., 1.2), 'q' to quit or 't' to run tests and Exit\n";
}


MethodInfo getMethodChoice(const std::map<std::string, std::vector<MethodInfo>>& methodsByFile) {
    while (true) {
        displayMenu(methodsByFile);
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "q" || choice == "t") {
            return {"", "", choice};
        }
        
        for (const auto& [_, methods] : methodsByFile) {
            auto it = std::find_if(methods.begin(), methods.end(),
                                   [&choice](const MethodInfo& mi) { return mi.selector == choice; });
            if (it != methods.end()) {
                return *it;
            }
        }
        
        std::cout << "Invalid choice. Please try again.\n";
    }
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
            
            size_t braceCount = 1; // Count of open braces
            while (std::getline(inFile, line)) {
                // Skip the existing implementation body
                braceCount += std::count(line.begin(), line.end(), '{');
                braceCount -= std::count(line.begin(), line.end(), '}');

                if(braceCount == 0){
                    // We've found the matching closing brace
                    outFile << "}\n";
                    break;
                }
            }
            
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

