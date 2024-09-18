#ifndef GAME_PLAY_HPP
#define GAME_PLAY_HPP

#include <cstdlib>
#include <string>
#include <filesystem>
#include <functional>


namespace fs = std::filesystem;


int init_game();
void clearScreen();
void displayMenu();
fs::path findFile(const std::string &filename);
std::string getMethodChoice();
void openEditor(const std::string& filename, 
                int cursorLine, 
                std::function<int(const char*)> systemCall = std::system);

void editMethod(const std::string& method, 
                const std::string& filename = "linked_list",
                std::function<void(const std::string&, int, std::function<int(const char*)>)> editorFunc = openEditor);

std::string buildEditorCommands(const std::string& filename, int cursorLine);
int runCommand(const std::string& command, int maxRetries = 5);

#endif // GAME_PLAY_HPP
