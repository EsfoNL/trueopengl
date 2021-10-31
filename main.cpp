#include <GL/glew.h>
#include <iostream>
#include <windows.h>

void display();

int main(int argc, char** argv) {
    HMODULE hello = GetModuleHandle(NULL);
    std::cout << hello;
}