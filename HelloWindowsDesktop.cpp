// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ReadingFile(const wchar_t* filepath);

wstringstream sstream;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    OPENFILENAME ofn;       // common dialog box structure
    TCHAR szFile[260] = { 0 };       // if using TCHAR macros

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = _T("Picture.bmp\0*.bmp\0Text\0*.TXT\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) != TRUE)
    {
        MessageBox(0, L"Ошибка открытия файла", L"", 0);
        exit(1);
        return 1;
    }
    ReadingFile(ofn.lpstrFile);

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 500,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,nCmdShow); 
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
vector<unsigned char> file;
long width;
long height;
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return

//filepath - содержит путь к файлу
void ReadingFile(const wchar_t* filepath) {
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
    unsigned char cur;
    
    fstream image(filepath, ios::in | ios::binary);
    if (image.is_open()) {
        image.read((char*)&file_header, sizeof(file_header));
        image.read((char*)&info_header, sizeof(info_header));
        image.seekp(file_header.bfOffBits, std::ios::beg); //Смещение до конца заголовочного файла
        width = info_header.biWidth;
        height = info_header.biHeight;
        /*const char* target = "@ 7 H"; *///"40 C0 20"
        int counter = 0; 
        while (!image.eof()) {
            int w;
            int h;
            int rgb1;
            int rgb2;
            int rgb3;
            int c = counter / 3;
            int inner;
            
            image.read( (char*) & cur, 1);
            file.push_back(cur);



            /*if (cur == *target) {
                
                color += cur;
                color += ' ';
                inner += 1;
                if (color == " " && inner==3) {
                    h = c / width;
                    w = c - h * width;
                    rgb1 = (int)color[0];
                    rgb2 = (int)color[2];
                    rgb3 = (int)color[4];
                    vec.push_back(rgb1);
                    vec.push_back(rgb2);
                    vec.push_back(rgb3);
                    vec.push_back(h);
                    vec.push_back(w);
                    
                    v.push_back(vec);

                }
                else if (inner > 3) {
                    color = "";
                }
                
                
            }
            counter++;*/
        }
        
        int pixel;
        image.read((char*)&pixel, 3);

        sstream << file_header.bfOffBits << ' ' << info_header.biWidth << ' ' << info_header.biHeight << ' ' << pixel;
    }
    else {
        throw exception("Cant open file");
    }
    
};





LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;   
    switch (message)
    {
    case WM_PAINT:
    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        
        int window_width = rect.right - rect.left;
        int window_height = rect.bottom - rect.top;

        int xo = window_width / 2 - width / 2;
        int yo = window_height - height;

        hdc = BeginPaint(hWnd, &ps);
        PaintDesktop(hdc);

        unsigned char* data = file.data();
        //for (int i = 0; i < file.size() / 3; i++) {
        //    int count;


        //    /*if (s.length()!=3) {
        //        s += file[i];

        //    }
        //    else if (s!=pixel){*/
        //    int t = i / width;

        //    unsigned int h = data[2] | data[1] << 8 | data[0] << 16;

        //    SetPixel(hdc, i % width, i / width, h);
        //    /*}
        //    else if (s == pixel) {
        //        s = "";
        //    }*/
        //    data += 3;
        //}
        /*unsigned char a[] = {file[0], file[1], file[2]};*/


        for (int y = height; y > 0; y--) {
            for (int x = 0; x < width; x++) {
                if (data[2] != file[2] || data[1] != file[1] || data[0] != file[0]) {
                    unsigned int h = data[2] | data[1] << 8 | data[0] << 16;
                    SetPixel(hdc, x+xo, y+yo, h);
                }
                data += 3;
            }
        }

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, Windows desktop!"
        // in the top left corner.

        /*TextOut(hdc,
            5, 5,
            sstream.str().c_str(), _tcslen(sstream.str().c_str()));*/
            // End application-specific layout section.

        EndPaint(hWnd, &ps);
    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}