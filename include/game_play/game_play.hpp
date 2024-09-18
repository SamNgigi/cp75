#ifndef GAME_PLAY_HPP
#define GAME_PLAY_HPP

#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <functional>


namespace fs = std::filesystem;

struct MethodInfo {
  std::string name;
  std::string filename;
  std::string selector;
};

int init_game();
void clearScreen();
std::map<std::string, std::vector<MethodInfo>> getAvailableMethods();
void displayMenu(const std::map<std::string, std::vector<MethodInfo>> &methodsByFile);
MethodInfo getMethodChoice(const std::map<std::string, std::vector<MethodInfo>>& methodsByFile);
fs::path findFile(const std::string &filename);
void openEditor(const std::string& filename, 
                int cursorLine, 
                std::function<int(const char*)> systemCall = std::system);

void editMethod(const std::string& method, 
                const std::string& filename = "linked_list",
                std::function<void(const std::string&, int, std::function<int(const char*)>)> editorFunc = openEditor);

std::string buildEditorCommands(const std::string& filename, int cursorLine);
int runCommand(const std::string& command, int maxRetries = 5);

#endif // GAME_PLAY_HPP
