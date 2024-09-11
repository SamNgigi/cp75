#ifndef GAME_PLAY_HPP
#define GAME_PLAY_HPP

#include <string>
#include <filesystem>

class SystemInterface{
public:
  virtual ~SystemInterface() = default;
  virtual void executeCommand(const std::string &command) const = 0;
};


class RunSystemCommands : public SystemInterface {
  void executeCommand(const std::string &command) const override {
    system(command.c_str());
  }
};

void clearScreen();
void displayMenu();
std::filesystem::path findFile(const std::string &filename);
std::string getMethodChoice();
void editMethod(const std::string& method);
void openEditor(const std::string& filename, int cursorLine);
int runTests();

#endif // GAME_PLAY_HPP
