// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GraphicsProject.h"


#include "3DDateStruct.h"
#include "Camera.h"
#include "transform.h"
#include "Painter.h"
#include "mesh.h"
#include <algorithm>

//#include <gl/GL.h>
//#pragma comment(lib,"opengl32.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);





struct GraphicsWindow
{
    HDC Window_DC = NULL;
    HWND hWnd = NULL;
    HBITMAP BitMap = NULL;
    HBITMAP OldBitMap = NULL;
    unsigned char * FrameBuffer = NULL;
    int Pitch = 0;
    int Width = 0;
    int Height = 0;
    
    void Update()
    {
        HDC hDC = GetDC(hWnd);
        BitBlt(hDC, 0, 0, Width, Height, Window_DC, 0, 0, SRCCOPY);
        ReleaseDC(hWnd, hDC);
        
    }
};

GraphicsWindow GWindow;
Camera cam;
Painter p;

class FPlatformTime
{
    static double SeccondsPerCycle;
public:
    static void InitTime()
    {
        LARGE_INTEGER Frequency;
        QueryPerformanceFrequency(&Frequency);
        SeccondsPerCycle = 1.0 / Frequency.QuadPart;
    }
    static double GetSeconds(void)
    {
        LARGE_INTEGER Cycles;
        QueryPerformanceCounter(&Cycles);

        return Cycles.QuadPart * SeccondsPerCycle + 16777216.0; 
    }
};
double FPlatformTime::SeccondsPerCycle = 0.f;


void DrawPixel()
{
    sort(p.mPixels.begin(), p.mPixels.end(), [](Pixel a, Pixel b) {return a.z > b.z; });//»­¼ÒËã·¨
    for (auto it = p.mPixels.begin(); it != p.mPixels.end(); it++)
    {
        if (it->z < cam.ZNear || it->z > cam.ZFar) continue;
        int x = it->x, y = it->y, r = it->r, g = it->g, b = it->b;
        if (x < 0 || x >= GWindow.Width || y < 0 || y >= GWindow.Height ) continue;
        int PixelIndex = x + GWindow.Width * y;
        int PixelIndex_Blue = 4 * PixelIndex;
        int PixelIndex_Green = 4 * PixelIndex + 1;
        int PixelIndex_Red = 4 * PixelIndex + 2;
        GWindow.FrameBuffer[PixelIndex_Blue] = b;
        GWindow.FrameBuffer[PixelIndex_Green] = g;
        GWindow.FrameBuffer[PixelIndex_Red] = r;
    }
}

void DrwaTriangle(Triangle tri, Camera& cam, const AmbtLight& ambl, const DirectLight& dirl)
{
    Vertex vv[3];

    for (int i = 0; i < 3; i++)
    {
        vv[i] = tri.ver[i];
        Vector4 t;
        t = cam.mViewTransform * tri.ver[i].pos;
        vv[i].pos = Matrix4x4::BuildViewPort(GWindow.Width, GWindow.Height) * (cam.mProjectTransform * t).simplify();
        vv[i].pos.z = t.z;
        //p.AddPixel(Pixel(vv[i].pos.x, vv[i].pos.y, vv[i].pos.z,vv[i].r,vv[i].g,vv[i].b));
    }
    p.CutTriangle(vv[0], vv[1], vv[2], cam, ambl, dirl);
}

void MessageDispatch()
{
    MSG msg;

    // Main message loop:

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GRAPHICSPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    double LastTime = 0;

    p.height = GWindow.Height;
    p.width = GWindow.Width;
    p.pic.mTextHeight = p.pic.mTextWidth = 800;
    p.SetTexture("BlackWhite.bmp");
    cam = Camera(Vector4(0,150,-100,1),Vector4(0,1,0,0),Vector4(0,0,1,0),90,(double)GWindow.Width/GWindow.Height,0.1,2000);
    AmbtLight amb(254, 67, 101);
    DirectLight dir(255, 255, 255, Vector4(0, 0, 1, 0));

    Vertex v1(Vector4(100, 0, 100, 1), Vector4(0, 0, -1, 0).normalize(), Material(0.2, 0.4, 0.6, 2), 0, 1);
    Vertex v2(Vector4(0, 100, 100, 1), Vector4(0, 0, -1, 0).normalize(), Material(0.2, 0.4, 0.6, 2), 1, 0);
    Vertex v3(Vector4(0, 0, 100, 1), Vector4(0, 0, -1, 0).normalize(), Material(0.2, 0.4, 0.6, 2), 0, 0);
    Triangle triangle1(v1, v2, v3);

    Vertex v4(Vector4(100, 100, 100, 1), Vector4(0, 0, -1, 0).normalize(), Material(0.2, 0.4, 0.6, 2), 1, 1);
    Triangle triangle2(v1, v4, v2);

    Vertex v5(Vector4(0, 100, 100, 1), Vector4(0, 1, 0, 0).normalize(), Material(0.2, 0.4, 0.6, 2), 0, 0);
    Vertex v6(Vector4(100, 100, 100, 1), Vector4(0, 1, 0, 0).normalize(), Material(0.2, 0.4, 0.6, 2), 0, 1);
    Vertex v7(Vector4(100, 100, 200, 1), Vector4(0, 1, 0, 0).normalize(), Material(0.2, 0.4, 0.6, 2), 1, 1);
    Triangle triangle3(v5, v6, v7);

    Vertex v8(Vector4(0, 100, 200, 1), Vector4(0, 1, 0, 0).normalize(), Material(0.2, 0.4, 0.6, 2), 1, 0);
    Triangle triangle4(v5, v7, v8);

    Vertex v9(Vector4(100, 0, 100, 1), Vector4(1, 0, 0, 0).normalize(), Material(0.2, 0.4, 0.6, 2), 0, 0);
    Vertex v10(Vector4(100, 0, 200, 1), Vector4(1, 0, 0, 0).normalize(), Material(0.2, 0.4, 0.6, 2), 0, 1);
    Vertex v11(Vector4(100, 100, 100, 1), Vector4(1, 0, 0, 0).normalize(), Material(0.2, 0.4, 0.6, 2), 1, 0);
    Triangle triangle5(v9, v10, v11);

    Vertex v12(Vector4(100, 100, 200, 1), Vector4(0, 1, 0, 0).normalize(), Material(0.2, 0.4, 0.6, 2), 1, 1);
    Triangle triangle6(v11, v10, v12);


    while (true)
    {
        
        MessageDispatch();
        
        double CurrentTime = FPlatformTime::GetSeconds();

        memset(GWindow.FrameBuffer, 0, GWindow.Width * GWindow.Height * 4);
        p.Clear();


        DrwaTriangle(triangle1, cam, amb, dir);
        DrwaTriangle(triangle2, cam, amb, dir);
        DrwaTriangle(triangle3, cam, amb, dir);
        DrwaTriangle(triangle4, cam, amb, dir);
        DrwaTriangle(triangle5, cam, amb, dir);
        DrwaTriangle(triangle6, cam, amb, dir);


        //for (int i = 0; i < 3; i++)
        //{
        //    vv[i] = triangle1.ver[i];
        //    Vector4 t;
        //    t = cam.mViewTransform * triangle1.ver[i].pos;
        //    vv[i].pos = Matrix4x4::BuildViewPort(GWindow.Width, GWindow.Height) * (cam.mProjectTransform * t).simplify();
        //    vv[i].pos.z = t.z;
        //    //p.AddPixel(Pixel(vv[i].pos.x, vv[i].pos.y, vv[i].pos.z,vv[i].r,vv[i].g,vv[i].b));
        //}
        //vv[0].r = vv[0].g = 0;
        //vv[1].g = vv[1].b = 0;
        //vv[2].r = vv[2].b = 0;
        //p.myGouraudFill(vv[0], vv[1], vv[2]);

        
        DrawPixel();

        GWindow.Update();
        LastTime = CurrentTime;

    }

}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICSPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GRAPHICSPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

    FPlatformTime::InitTime();
    GWindow.hWnd = hWnd;
    int Width = 1280;
    int Height = 720;
    HDC hDC = GetDC(hWnd);
    BITMAPINFO BitMapInfo = {
        { sizeof(BITMAPINFOHEADER),
            Width,
            -Height,
            1,
            32,
            BI_RGB,
            Width * Height * 4,
            0,
            0,
            0,
            0 }
    };
    LPVOID BufferPtr;
    GWindow.Width = Width;
    GWindow.Height = Height;
    GWindow.Window_DC = CreateCompatibleDC(hDC);
    GWindow.Pitch = Width * 4;
    ReleaseDC(hWnd, hDC);

    if(!GWindow.Window_DC)
    {
        return FALSE;
    }

    GWindow.BitMap = CreateDIBSection(GWindow.Window_DC, &BitMapInfo, DIB_RGB_COLORS, &BufferPtr, 0,0);

    if(!GWindow.BitMap)
    {
        return FALSE;
    }

    GWindow.OldBitMap = (HBITMAP)SelectObject(GWindow.Window_DC, GWindow.BitMap);
    GWindow.FrameBuffer = (unsigned char*)BufferPtr;
    memset(GWindow.FrameBuffer, 0, GWindow.Width * GWindow.Height * 4);

    RECT Rect = {0, 0, Width, Height};
    AdjustWindowRect(&Rect, GetWindowLong(hWnd, GWL_STYLE), 0);
    int ScreenX = Rect.right - Rect.left;
    int ScreenY = Rect.bottom - Rect.top + 20;
    int CX = (GetSystemMetrics(SM_CXSCREEN) - ScreenX) / 2;
    int CY = (GetSystemMetrics(SM_CYSCREEN) - ScreenY) / 2;

    SetWindowPos(hWnd, NULL, CX, CY, ScreenX, ScreenY, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
    
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void DestroyGraphicWindow()
{
    if(GWindow.Window_DC)
    {
        if(GWindow.OldBitMap)
        {
            SelectObject(GWindow.Window_DC, GWindow.OldBitMap);
            GWindow.OldBitMap = NULL;
        }
        DeleteDC(GWindow.Window_DC);
        GWindow.Window_DC = NULL;
    }
    if(GWindow.BitMap)
    {
        DeleteObject(GWindow.BitMap);
        GWindow.BitMap = NULL;
    }
    GWindow.FrameBuffer = nullptr;
    DestroyWindow(GWindow.hWnd);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //static HGLRC OpenGLHandle;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyGraphicWindow();
                //DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        //wglDeleteContext(OpenGLHandle);
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case 'W':
            cam.move(0, 10, 0);
            break;
        case 'S':
            cam.move(0, -10, 0);
            break;
        case 'A':
            cam.move(-10, 0, 0);
            break;
        case 'D':
            cam.move(10, 0, 0);
            break;
        case 'Q':
            cam.move(0, 0, 10);
            break;
        case 'E':
            cam.move(0, 0, -10);
            break;
        case VK_UP:
            cam.rotate(-5, 0, 0);
            break;
        case VK_DOWN:
            cam.rotate(5, 0, 0);
            break;
        case VK_LEFT:
            cam.rotate(0, -5, 0);
            break;
        case VK_RIGHT:
            cam.rotate(0, 5, 0);
            break;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
