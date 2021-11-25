#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <GL\glew.h>
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

    //! setup ipengl context
    HDC hdc = GetDC(window);
    PIXELFORMATDESCRIPTOR pixelformat = {
        sizeof(pixelformat),                                        //size of pixelformat
        1,                                                          //version number
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //support OpenGL + window drawing + double buffered
        PFD_TYPE_RGBA,                                              //pixel color type: rgba
        24,                                                         //color 24 bit depth (r + g + b)
        0, 0, 0, 0, 0, 0,                                           //unused color bits
        8,                                                          //alpha channel 8 bit depth
        0,                                                          //shift bit ignored
        0,                                                          //no accumilation buffer
        0, 0, 0, 0,                                                 //ignored accum bits
        32,                                                         //z-buffer 32 bit depth
        0,                                                          //no stencil buffer
        0,                                                          //no auxiliary buffer
        PFD_MAIN_PLANE,                                             //main layer
        0,                                                          //reserved
        0, 0, 0                                                     //layer masks ignored
    };
    int iPixelformat = ChoosePixelFormat(hdc, &pixelformat);
    SetPixelFormat(hdc, iPixelformat, &pixelformat);
    HGLRC hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);
    GLenum err = glewInit();

    
    
    //! first render
    static const GLfloat buffer[] = {
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
         1.0f,  1.0f,  0.0f,
    };
    
    GLuint bufferid;
    
    glGenBuffers(1, &bufferid);
    glBindBuffer(GL_ARRAY_BUFFER, bufferid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, bufferid);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    glDrawArrays(GL_TRIANGLES, 0,3);
    glDisableVertexAttribArray(0);

    SwapBuffers(hdc);


    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        
        //! message handeling area
        switch (msg.message) {

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
