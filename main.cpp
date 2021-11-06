#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <Gl\gl.h>
#include <chrono>
#include <string>
#include <array>
#include <vector>
#include <random>
#include <thread>


LRESULT CALLBACK mymessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam);
int dorgb(bool &rgb, std::chrono::steady_clock &globalclock, HWND &hwnd, int &hz, HBRUSH &backgroundbrush);
void switchcursor();

/**
 * @brief win main function
 * 
 * @param hInstance 
 * @param hPrevInstance 
 * @param pCmdLine 
 * @param nCmdShow 
 * @return int 
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    //variables
    std::thread nothing;
    HBRUSH backgroundbrush = CreateSolidBrush(RGB(0,0,0));
    int hz = 1;
    bool rgb = false;
    std::chrono::steady_clock globalclock = std::chrono::steady_clock();

    //set up window class
    LPCWSTR windowname = L"HELLO";
    windowname = L"miauw";
    WNDCLASSEXW windclass = {};
    windclass.cbSize = sizeof(windclass);
    windclass.hInstance = hInstance;
    windclass.lpfnWndProc = &mymessageHandler;
    windclass.lpszClassName = windowname;
    windclass.hbrBackground = std::ref(backgroundbrush);

    
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
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        
        //! message handeling area
        switch (msg.message) {
            //toggles between arrow pointer and cross pointer
            //also manages rgb state
            case WM_KEYUP:
            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
                {   
                    switch (msg.wParam) {
                        //triggers on r and toggles rgb on
                        case 0x52:
                            rgb = !rgb;
                            if (rgb) {
                                nothing = std::thread(dorgb, std::ref(rgb), std::ref(globalclock), std::ref(window), std::ref(hz), std::ref(backgroundbrush));
                            }
                            break;
                        default:
                            switchcursor();
                    }
                }
        }

        DispatchMessage(&msg);
    }
    return 0;
}

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

    //triggers when window destroy is called
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uint, wparam, lparam);
}

/**
 * @brief makes the background of the window go rgb if reference rgb param is true;
 * 
 * @param rgb refrence to a bool used for turning rgb on and off
 * @param globalclock 
 * @param hwnd 
 * @param hz 
 */
int dorgb(bool &rgb, std::chrono::steady_clock &globalclock, HWND &hwnd, int &hz, HBRUSH &backgroundbrush) {
    std::chrono::time_point untilwaittime = globalclock.now() + std::chrono::seconds(1/hz);
    HDC hdc = GetDC(hwnd);
    RECT rc;
    std::uniform_int_distribution randgen(0,255);
    std::random_device randsource;
    COLORREF color;

    while (rgb) {
        color = RGB(randgen(randsource),randgen(randsource),randgen(randsource));
        backgroundbrush = CreateSolidBrush(color);
        GetClientRect(hwnd, &rc); 
        SetMapMode(hdc, MM_ANISOTROPIC); 
        SetWindowExtEx(hdc, 100, 100, NULL); 
        SetViewportExtEx(hdc, rc.right, rc.bottom, NULL); 
        FillRect(hdc, &rc, backgroundbrush);
        if (globalclock.now() > untilwaittime) {
            untilwaittime = globalclock.now();
        } else {
            std::this_thread::sleep_until(untilwaittime);
        }
        untilwaittime += std::chrono::seconds(1/hz);
    }

    return 0;
}

/**
 * @brief toggles between arrow and cross pointer, if not one of these it switches to cross
 * 
 */
void switchcursor() {
    CURSORINFO currentcursor = {};
    currentcursor.cbSize = sizeof(currentcursor);
    GetCursorInfo(&currentcursor);
    //checks if the current cursor is the cross, if not sets it to cross, otherwise sets it to pointer
    currentcursor.hCursor == (HCURSOR)LoadCursor(NULL, IDC_CROSS) ? SetCursor(LoadCursor(NULL, IDC_ARROW)) : SetCursor(LoadCursor(NULL, IDC_CROSS));
}
