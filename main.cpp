#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

//#include <iostream>

/**
 * @brief window message handler, used by windows.
 * 
 * @param hwnd 
 * @param uint 
 * @param wparam 
 * @param lparam 
 * @return LRESULT 
 */
LRESULT CALLBACK mymessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam) {
    switch (uint) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    //toggles between arrow pointer and cross pointer
    case WM_KEYUP:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    {
        CURSORINFO currentcursor = {};
        currentcursor.cbSize = sizeof(currentcursor);
        GetCursorInfo(&currentcursor);
        if (currentcursor.hCursor == (HCURSOR)LoadCursor(NULL, IDC_CROSS)) {
            SetCursor(LoadCursor(NULL, IDC_ARROW));
        } else {
            SetCursor(LoadCursor(NULL, IDC_CROSS));
        }
        return 0;
        }
    }
    return DefWindowProc(hwnd, uint, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    
    //set up window class
    LPCWSTR windowname = L"HELLO";
    windowname = L"miauw";
    WNDCLASSEXW windclass = {};
    windclass.cbSize = sizeof(windclass);
    windclass.hInstance = hInstance;
    windclass.lpfnWndProc = &mymessageHandler;
    windclass.lpszClassName = windowname;
    
    //registers window class
    RegisterClassExW(&windclass);

    //create window
    HWND window = CreateWindowExW(
        WS_EX_APPWINDOW,
        windowname,
        windowname,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    
    //error checks and window loop
    if (window == NULL) {
        return 404;
    }
    ShowWindow(window, nCmdShow);
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}