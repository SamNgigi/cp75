#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

int main(){

    std::cout << "Hello World\n";
    return 0;

}


