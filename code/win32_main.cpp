#include <windows.h>
#include <Windowsx.h>
#include <glad/glad.h>
#include <GL/wglext.h>
#include <stdio.h>

// my stuff...
#include "game.h"

global_variable bool is_runnig;
global_variable bool should_update_mouse_pos;
global_variable int xOffset;
global_variable int yOffset;

LRESULT CALLBACK WindowProc(
        HWND   hwnd,
        UINT   uMsg,
        WPARAM wParam,
        LPARAM lParam)
{
    LRESULT result;
    switch(uMsg)
    {
        case WM_CLOSE:
        {
            is_runnig = false; 
        }break;
        case WM_DESTROY:
        {
            is_runnig = false; 
        }break;
        case WM_MOVE:
        {
        // TODO(Manuto): handle the case the we move the window
        // we should recalculate de mouse default position...
            xOffset = (int)(short)LOWORD(lParam);   // horizontal position 
            yOffset = (int)(short)HIWORD(lParam);   // vertical position
            should_update_mouse_pos = true; 
        }break;
        default:
        {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }break;
    }
    return result;
}


// function to initialize OpengGL in windows.h
bool InitOpengGLContext(HWND* hWnd, HDC* handle_device_context)
{
     PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,     // flags
        PFD_TYPE_RGBA,     // color frame buffer pixel format
        32,                // color frame buffer depht format 
 		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,   
        24,                // number of bits for the depht buffer
        8,                 // number of bits for the stancil buffer
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    }; 

    *handle_device_context = GetDC(*hWnd);
    int wnd_pixel_format = ChoosePixelFormat(*handle_device_context, &pfd);
    SetPixelFormat(*handle_device_context, wnd_pixel_format, &pfd);

    HGLRC opengl_render_context_temp = wglCreateContext(*handle_device_context);
    wglMakeCurrent(*handle_device_context, opengl_render_context_temp);
    
    int attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    HGLRC opengl_render_context;
    if(wglCreateContextAttribsARB)
    {
        opengl_render_context = wglCreateContextAttribsARB(*handle_device_context, NULL, attributes);
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(opengl_render_context_temp);
        wglMakeCurrent(*handle_device_context, opengl_render_context);

        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 
            (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        if(wglSwapIntervalEXT)
        {
            wglSwapIntervalEXT(1);
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    if(!gladLoadGL())
    {
        OutputDebugString("ERROR::INITIALIZING::GLAD\n");
        return false;
    }
    
    // set up initial configuration for open GL...
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    glViewport(0, 0, WNDWIDTH, WNDHEIGHT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST); 

    return true; 
}


// this is the entry point of our application...
int WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR     lpCmdLine,
        int       nShowCmd)
{
    // register wndClass to decribe the type of window
    // we want to have... 
    WNDCLASS wc      = { };
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance; 
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "BouncingBalls3D";
    if(RegisterClass(&wc) == 0)
    {
        OutputDebugString("FAILED::REGISTERING::CLASS\n");
        return 1;
    }
    
   // We create a RECT and use to give the windows the correct client size 800x600 
    RECT windowRect;
    windowRect.left   = 0;
    windowRect.right  = WNDWIDTH;
    windowRect.bottom = WNDHEIGHT;
    windowRect.top    = 0;
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE); 

    // funtion that create the window whit the wndClass and a position and a dimention
    HWND hWnd = CreateWindow(
                    "BouncingBalls3D",
                    "BouncingBalls3D",
                    WS_OVERLAPPEDWINDOW,
                    0,
                    0,
                    windowRect.right  - windowRect.left,
                    windowRect.bottom - windowRect.top,
                    NULL,
                    NULL,
                    hInstance,
                    NULL);
    if(hWnd == 0)
    {
        OutputDebugString("FAILED::CREATING::WINDOW\n");
        return 1;
    }
    
    // TODO: Initialize OpenGL from windows.h library... 
    HDC handle_device_context;
    if(!InitOpengGLContext(&hWnd, &handle_device_context))
    {
        OutputDebugString("FAILED::INITIALIZING::OPENGL\n");
        return 1;
    }

    // TODO: Initialize the Game...
    MainGame game = {};
    game.input.mouseDefaultPosX = WNDWIDTH  / 2;
    game.input.mouseDefaultPosY = WNDHEIGHT / 2;
    GameInit(&game);
    InitializeGamePads(&game.input);
    ShowWindow(hWnd, SW_SHOW);
    CoInitialize(nullptr);
    is_runnig = true;

    // QueryPerformanceFrequency we need to get this value to the use it with the QueryPerformanceCounter...
    LARGE_INTEGER queryPerformancefrequency;
    QueryPerformanceFrequency(&queryPerformancefrequency);
    uint64_t frequency = queryPerformancefrequency.QuadPart;
    
    LARGE_INTEGER oldCounter;
    QueryPerformanceCounter(&oldCounter);

    
    while(is_runnig == true)
    { 
        
        MSG message;
        if(PeekMessage(&message, hWnd, 0, 0, PM_REMOVE))
        {
            switch(message.message)
            {
                // here we handle keyboard INPUT...
                case WM_KEYDOWN:
                {
                    uint32_t keyCode = (uint32_t)message.wParam;
                    SetKeyDown(&game.input, keyCode);
                }break;
                case WM_KEYUP:
                {
                    uint32_t keyCode = (uint32_t)message.wParam;
                    SetKeyUp(&game.input, keyCode);
                }break;
                case WM_MOUSEMOVE:
                {
                    POINT cursorPos;
                    GetCursorPos(&cursorPos);
                    game.input.mousePosX = cursorPos.x;
                    game.input.mousePosY = cursorPos.y; 

                    if(GetMouseButtonPress(&game.input, RIGHTBUTTON))
                    { 
                        game.input.mouseIncX += cursorPos.x - (float)game.input.mouseDefaultPosX;
                        game.input.mouseIncY += cursorPos.y - (float)game.input.mouseDefaultPosY;
                        SetCursorPos(game.input.mouseDefaultPosX, game.input.mouseDefaultPosY);
                    }

                }break;
                
                // LEFT::MOUSE::BUTTON::HANDLER...
                case WM_LBUTTONDOWN:
                {
                    SetMouseButtonPress(&game.input, 0);

                }break;
                case WM_LBUTTONUP:
                {
                    SetMouseButtonUp(&game.input, 0);                  
                }break;
                // RIGHT::MOUSE::BUTTON::HANDLER...
                case WM_RBUTTONDOWN:
                {
                    SetCursorPos(game.input.mouseDefaultPosX, game.input.mouseDefaultPosY);
                    ShowCursor(FALSE);
                    SetMouseButtonPress(&game.input, 1);
                }break;
                case WM_RBUTTONUP:
                {
                    ShowCursor(TRUE);
                    SetMouseButtonUp(&game.input, 1);                   
                }break;
                default:
                {
                    TranslateMessage(&message);
                    DispatchMessage(&message);    
                }break;
            }
        }
        else
        {    
            LARGE_INTEGER counter;
            QueryPerformanceCounter(&counter);
            // get the delta time...
            uint64_t counterElapse = counter.QuadPart - oldCounter.QuadPart;
            float deltaTime = (counterElapse / (float)frequency);

            // update game...
            if(should_update_mouse_pos == true)
            {
                game.input.mouseDefaultPosX = xOffset + (windowRect.right - windowRect.left) / 2;
                game.input.mouseDefaultPosY = yOffset + (windowRect.bottom - windowRect.top) / 2;
                should_update_mouse_pos = false;
            }

            if(game.input.gamePadConected == true)
            {
                ProcessGamePads(&game.input);            
            }

            glClearColor(0.03f, 0.0f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // render game...
            GameUnpdateAndRender(&game, deltaTime);

            SwapBuffers(handle_device_context);
            game.input.mouseIncX = 0.0f;
            game.input.mouseIncY = 0.0f;     
            oldCounter = counter;
        }
    }

    // TODO: Clear and Colse the Game...
    ReleaseDC(hWnd, handle_device_context); 

    return 0;
}

