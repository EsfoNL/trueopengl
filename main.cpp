#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <Gl\gl.h>
#include <thread>
#include <chrono>


/**
 * @brief window message handler, used by windows.
 * 
 * @param hwnd 
 * @param uint 
 * @param wparam 
 * @param lparam 
 * @return LRESULT 
 */
LRESULT CALLBACK mymessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    
    //set up window class
    LPCWSTR windowname = L"HELLO";
    windowname = L"miauw";
    WNDCLASSEXW windclass = {};
    windclass.cbSize = sizeof(windclass);
    windclass.hInstance = hInstance;
    windclass.lpfnWndProc = &mymessageHandler;
    windclass.lpszClassName = windowname;
    
    
    bool rgb = false;
    
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
        &rgb
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
        
        //! message handeling area
        switch (msg.message) {
            //toggles between arrow pointer and cross pointer
            //also manages rgb state
            case WM_KEYUP:
            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
                {
                    switch (msg.wParam)
                    {
                    case 0x52:
                        rgb = !rgb;
                        break;
                    
                    default:
                        CURSORINFO currentcursor = {};
                        currentcursor.cbSize = sizeof(currentcursor);
                        GetCursorInfo(&currentcursor);
                        //checks if the current cursor is the cross, if not sets it to cross, otherwise sets it to pointer
                        currentcursor.hCursor == (HCURSOR)LoadCursor(NULL, IDC_CROSS) ? SetCursor(LoadCursor(NULL, IDC_ARROW)) : SetCursor(LoadCursor(NULL, IDC_CROSS));
                    }
                }
        }

        DispatchMessage(&msg);
    }
    return 0;
}


LRESULT CALLBACK mymessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam) {

    switch (uint) {

    //triggers when window destroy is called
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uint, wparam, lparam);
}