#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif


#include "game_play/game_play.hpp"

int main(){

    std::cout << "Hello World\n";
    
    int result = init_game();
    if(result == 42) {
        // The game loop exited with a request to run tests
        std::system("start cmd /c \"run.bat run_tests_new_window\"");
        return 0;
    }
    return result;

}

