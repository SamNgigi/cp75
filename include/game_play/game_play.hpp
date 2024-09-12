#ifndef GAME_PLAY_HPP
#define GAME_PLAY_HPP

#include <string>
#include <filesystem>


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
void editMethod(const std::string& method, const std::string& filename = "linked_list");
void openEditor(const std::string& filename, int cursorLine, const SystemInterface& system );
std::string buildEditorCommands(const std::string& filename, int cursorLine);
int runTests();

#endif // GAME_PLAY_HPP
