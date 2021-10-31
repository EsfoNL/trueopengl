#include <GL/glew.h>
#include <iostream>
#include <windows.h>
#include <string>

LRESULT mymessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam) {
    return LRESULT(0);
}

int main(int argc, char** argv) {
    LPCWSTR windowname = L"HELLO";
    windowname = L"miauw";
    HMODULE instance = GetModuleHandle(NULL);
    
    WNDCLASSEXW windclass = {};
    windclass.hInstance = instance;
    windclass.lpfnWndProc = &mymessageHandler;
    windclass.lpszClassName = windowname;
    RegisterClassExW(&windclass);
    HWND window = CreateWindowExW(0, windowname, windowname, WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, instance, NULL);
    ShowWindow(window, SW_NORMAL);
}