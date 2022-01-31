#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <GL\glew.h>
#include <chrono>
#include <string>
#include <array>
#include <vector>
#include <variant>
#include <random>
#include <thread>
#include <freetype2/freetype/config/ftheader.h>
#include <memory>
#include <lib/types.h>

//function definitions
LRESULT CALLBACK mymessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam);
void render(HWND &window, types::RenderData &renderdata, const std::chrono::steady_clock &globalclock, bool &running);
types::VertexBuffer mirrorvertexbuffer_y(types::VertexBuffer vertexbuffer);

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
    
    //constants
    const std::chrono::steady_clock globalclock = std::chrono::steady_clock();

    //variables
    HBRUSH backgroundbrush = CreateSolidBrush(RGB(0,0,0));
    HCURSOR cursor = (HCURSOR)LoadCursor(NULL, IDC_CROSS);

    //set up window class
    LPCWSTR windowname = L"TrueOpenGL";
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

    types::RenderData renderdata;
    bool renderrunning = true;

    renderdata.mtx.unlock();
    //! setup rendering thread
    std::thread(render, std::ref(window), std::ref(renderdata), std::ref(globalclock), std::ref(renderrunning)).detach();
    

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        
        //! message handeling area
        switch (msg.message) {
            case WM_KEYDOWN: {
                switch (msg.wParam) {
                    //arrow up
                    case 0x26:
                        renderdata.maxfps++;
                        break;
                    //arrow down
                    case 0x27:
                        renderdata.maxfps--;
                        break;
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
    //keyboard input

    }
    return DefWindowProc(hwnd, uint, wparam, lparam);
}

//!render functions

//render vertex buffer layer
void renderlayer(types::VertexBuffer vertexbuffer) {
    GLuint bufferid;
    glGenBuffers(1, &bufferid);
    glBindBuffer(GL_ARRAY_BUFFER, bufferid);
    glBufferData(GL_ARRAY_BUFFER, vertexbuffer.bytesize, vertexbuffer.storedarray.get(), GL_STATIC_DRAW);

    
    glBindBuffer(GL_ARRAY_BUFFER, bufferid);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    glDrawArrays(GL_TRIANGLES, 0, (int)(vertexbuffer.bytesize/3));
}

void renderlayer(types::PlaceHolder placeholder) {

};

//render int layer

void render(HWND &window, types::RenderData &renderdata, const std::chrono::steady_clock &globalclock, bool &running) {
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

    glEnableVertexAttribArray(0);
    renderdata.mtx.lock();
    
    //! first render
    /*GLfloat buffer[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };*/

    GLfloat buffer[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    types::VertexBuffer workingbuffer(buffer, 9);
    renderdata.appendlayer(workingbuffer);
    renderdata.mtx.unlock();

    //timepoint for next frame
    std::chrono::time_point nextframe = globalclock.now() + std::chrono::milliseconds((int)(1000/renderdata.maxfps));

    while (running) {
        //lock the renderdata
        renderdata.mtx.lock();

            for (int i = 0; i < renderdata.layercount; i++) {
                renderlayer(renderdata.vertexbuffers[i]);
            }
        
        //unlock the renderdata
        renderdata.mtx.unlock();
    
        //swap buffer to display render
        SwapBuffers(hdc);
        //flip buffer
        renderdata.vertexbuffers[0] = mirrorvertexbuffer_y(renderdata.vertexbuffers[0]);

        //timing stuff
        std::this_thread::sleep_until(nextframe);
        nextframe += std::chrono::milliseconds((int)(1000/renderdata.maxfps));
        if (globalclock.now() > nextframe) {
            globalclock.now() + std::chrono::milliseconds((int)(1000/renderdata.maxfps));
        }
    }
}

types::VertexBuffer mirrorvertexbuffer_y(types::VertexBuffer vertexbuffer) {
    for (int i = 0; i < vertexbuffer.size; i++) {
        vertexbuffer.storedarray[i] = -vertexbuffer.storedarray[i];
    }
    return vertexbuffer;
}