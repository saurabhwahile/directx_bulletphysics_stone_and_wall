// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include "Renderer.h"
#include "BulletPhysics.h"

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    // wc.hbrBackground = (HBRUSH)COLOR_WINDOW;    
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL,
                          L"WindowClass",
                          L"DirectX Project",
                          WS_EX_TOPMOST | WS_POPUP,    // fullscreen values
                          0, 0,    // the starting x and y positions should be 0
                          SCREEN_WIDTH, SCREEN_HEIGHT,    // set the window to 640 x 480
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    initD3D(hWnd);
	InitBulletPhysics();
    // enter the main loop:

    MSG msg;

    while(TRUE)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
            break;
		
        render_frame();
		StepBulletPhysics();
    }

    // clean up DirectX and COM
    cleanD3D();
	ExitBulletPhysics();

    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}
