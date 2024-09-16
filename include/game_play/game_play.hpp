#ifndef GAME_PLAY_HPP
#define GAME_PLAY_HPP

#include <cstdlib>
#include <string>
#include <filesystem>
#include <functional>


namespace fs = std::filesystem;



class SystemInterface{
public:
  virtual ~SystemInterface() = default;
  virtual void executeCommand(const std::string &command) const = 0;
};


class OpenEditorSys : public SystemInterface {
  void executeCommand(const std::string &command) const override {
    system(command.c_str());
  }
};


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
int runTests();

#endif // GAME_PLAY_HPP
