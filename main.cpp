#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <Gl\gl.h>
#include <future>
#include <chrono>
#include <string>
#include <array>
#include <vector>
#include <random>
#include <thread>


LRESULT CALLBACK mymessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam);
void dorgb(bool &rgb, std::chrono::steady_clock &globalclock, HWND &hwnd, int &hz);
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
    
    //set up window class
    LPCWSTR windowname = L"HELLO";
    windowname = L"miauw";
    WNDCLASSEXW windclass = {};
    windclass.cbSize = sizeof(windclass);
    windclass.hInstance = hInstance;
    windclass.lpfnWndProc = &mymessageHandler;
    windclass.lpszClassName = windowname;
    
    int hz = 1;
    bool rgb = false;
    bool commanding = false;
    std::vector<std::string> command = {};
    std::string commandpart = {};
    std::chrono::steady_clock globalclock = std::chrono::steady_clock();

    
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
                    //TODO implement commands
                    if (commanding) {
                        commanding = false;
                        if (msg.wParam = VK_RETURN) {
                            
                        }
                    }else {
                        switch (msg.wParam) {
                            //triggers on r and toggles rgb on
                            case 0x52:
                                rgb = !rgb;
                                if (rgb) {
                                    std::future<void> nothing = std::async(dorgb, std::ref(rgb), std::ref(globalclock), std::ref(window), std::ref(hz));
                                }
                                break;
                            case VK_OEM_MINUS:
                                commanding = true;
                                command = {};
                                commandpart = "";
                            default:
                                switchcursor();
                        }
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

void dorgb(bool &rgb, std::chrono::steady_clock &globalclock, HWND &hwnd, int &hz) {
    std::chrono::time_point untilwaittime = globalclock.now() + std::chrono::seconds(1/hz);
    HDC hdc = GetDC(hwnd);
    HBRUSH hbr;
    RECT rc;
    std::uniform_int_distribution randgen(0,255);
    std::random_device randsource;
    COLORREF color;

    while (rgb) {
        color = RGB(randgen(randsource),randgen(randsource),randgen(randsource));
        hbr = CreateSolidBrush(color);
        GetClientRect(hwnd, &rc); 
        SetMapMode(hdc, MM_ANISOTROPIC); 
        SetWindowExtEx(hdc, 100, 100, NULL); 
        SetViewportExtEx(hdc, rc.right, rc.bottom, NULL); 
        FillRect(hdc, &rc, hbr);
        if (globalclock.now() > untilwaittime) {
            untilwaittime = globalclock.now();
        } else {
            std::this_thread::sleep_until(untilwaittime);
        }
        untilwaittime += std::chrono::seconds(1/hz);
    }
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