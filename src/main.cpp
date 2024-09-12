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
    return 0;

}


