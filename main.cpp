#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

LRESULT mymessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam) {
    switch (uint) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uint, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    LPCWSTR windowname = L"HELLO";
    windowname = L"miauw";
    WNDCLASSEXW windclass = {};
    windclass.cbSize = sizeof(windclass);
    windclass.hInstance = hInstance;
    windclass.lpfnWndProc = &mymessageHandler;
    windclass.lpszClassName = windowname;

    RegisterClassExW(&windclass);

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