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

void paintwindow(HBRUSH brush, HDC &hdc, HWND &hwnd);
LRESULT CALLBACK mymessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam);
int dorgb(bool &rgb, std::chrono::steady_clock &globalclock, HWND &hwnd, float &hz, HBRUSH &backgroundbrush, HDC &hdc);
void switchcursor(HCURSOR &cursor, HWND &hwnd);

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
    float hz = 1;
    bool rgb = false;
    std::chrono::steady_clock globalclock = std::chrono::steady_clock();
    HCURSOR cursor = (HCURSOR)LoadCursor(NULL, IDC_CROSS);

    //set up window class
    LPCWSTR windowname = L"HELLO";
    windowname = L"miauw";
    WNDCLASSEXW windclass = {};
    windclass.cbSize = sizeof(windclass);
    windclass.hInstance = hInstance;
    windclass.lpfnWndProc = &mymessageHandler;
    windclass.lpszClassName = windowname;
    windclass.hbrBackground = std::ref(backgroundbrush);
    windclass.hCursor = cursor;

    
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

    HDC hdc = GetDC(window);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        
        //! message handeling area
        switch (msg.message) {
            //toggles between arrow pointer and cross pointer
            //also manages rgb state
            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
                switchcursor(cursor, window);
            case WM_KEYUP:
                {   
                    switch (msg.wParam) {
                        //triggers on r and toggles rgb on
                        case 0x52:
                            rgb = !rgb;
                            if (rgb) {
                                std::thread(dorgb, std::ref(rgb), std::ref(globalclock), std::ref(window), std::ref(hz), std::ref(backgroundbrush), std::ref(hdc)).detach();
                            }
                            break;
                        //triggers on i, increases hz
                        case 0x49:
                            hz *= 2;
                            break;
                        //triggers on o, increases hz
                        case 0x4F:
                            hz /= 2;
                            break;
                    }
                }
            case WM_PAINT:
                paintwindow(backgroundbrush, hdc, window);
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
 * @param backgroundbrush 
 */
int dorgb(bool &rgb, std::chrono::steady_clock &globalclock, HWND &hwnd, float &hz, HBRUSH &backgroundbrush, HDC &hdc) {
    std::chrono::time_point untilwaittime = globalclock.now() + std::chrono::milliseconds((int)(1000/hz));
    std::uniform_int_distribution randgen(0,255);
    std::random_device randsource;
    COLORREF color;

    while (rgb) {
        color = RGB(randgen(randsource),randgen(randsource),randgen(randsource));
        backgroundbrush = CreateSolidBrush(color);
        paintwindow(backgroundbrush, hdc, hwnd);
        if (globalclock.now() > untilwaittime) {
            untilwaittime = globalclock.now();
        } else {
            std::this_thread::sleep_until(untilwaittime);
        }
        untilwaittime += std::chrono::milliseconds((int)(1000/hz));
    }

    return 0;
}

/**
 * @brief toggles between arrow and cross pointer, if not one of these it switches to cross
 * 
 */
void switchcursor(HCURSOR &cursor, HWND &hwnd) {
    //checks if the current cursor is the cross, if not sets it to cross, otherwise sets it to pointer
    cursor == (HCURSOR)LoadCursor(NULL, IDC_CROSS) ? cursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW) : cursor = (HCURSOR)LoadCursor(NULL, IDC_CROSS);
    SetClassLongPtr(hwnd, -12, (LONG_PTR)cursor);
    SetCursor(cursor);
}

void paintwindow(HBRUSH brush, HDC &hdc, HWND &hwnd) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    SetMapMode(hdc, MM_ANISOTROPIC); 
    SetWindowExtEx(hdc, 100, 100, NULL); 
    SetViewportExtEx(hdc, rc.right, rc.bottom, NULL); 
    FillRect(hdc, &rc, brush);
}