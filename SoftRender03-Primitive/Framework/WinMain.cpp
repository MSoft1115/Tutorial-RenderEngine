#include "stdafx.h"
#include "App.h"

App * gApp = NULL;

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam) 
{
	if (gApp == NULL)
		return ::DefWindowProc(hWnd, iMsg, wParam, lParam);

	switch(iMsg)
	{
	case WM_CREATE:
		break;

	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
		{
			RECT rc;

			GetClientRect(hWnd, &rc);

			int w = rc.right - rc.left;
			int h = rc.bottom - rc.top;

			gApp->Resize(w, h);
		}
		break;

	case WM_LBUTTONDOWN:
		if (gApp != NULL)
		{
			gApp->OnMouseDown(0);
		}
		break;

	case WM_LBUTTONUP:
		if (gApp != NULL)
		{
			gApp->OnMouseUp(0);
		}
		break;

	case WM_RBUTTONDOWN:
		if (gApp != NULL)
		{
			gApp->OnMouseDown(1);
		}
		break;

	case WM_RBUTTONUP:
		if (gApp != NULL)
		{
			gApp->OnMouseUp(1);
		}
		break;

	case WM_MOUSEMOVE:
		if (gApp != NULL)
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			gApp->OnMouseMove(x, y);
		}
		break;

	case WM_KEYDOWN:
		if (gApp != NULL)
		{
			gApp->OnKeyDown(wParam);
		}
		break;

	case WM_KEYUP:
		if (gApp != NULL)
		{
			gApp->OnKeyUp(wParam);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return ::DefWindowProc(hWnd, iMsg, wParam, lParam); 
}

static const char * WIN32_CLASS_NAME = "Myway3D";
const int S_WIDTH = 666, S_HEIGHT = 400;
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// 创建窗口
	WNDCLASS wc;
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor= NULL; 
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION); 
	wc.hInstance = hInstance; 
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = WIN32_CLASS_NAME; 
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	DWORD style = WS_OVERLAPPEDWINDOW;//WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;
	RECT rc = { 0, 0, S_WIDTH, S_HEIGHT };

	AdjustWindowRect(&rc, style, FALSE);

	int width = rc.right - rc.left;
	int	height = rc.bottom - rc.top;
	int xpos = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int ypos = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	HWND hWnd = CreateWindow(WIN32_CLASS_NAME, "SoftRender", style, 
							 xpos, ypos, width, height,
							 NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	GetClientRect(hWnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;

	// 创建离屏DC
	BYTE * pBackBuffer = NULL;
	HDC hdc = GetDC(hWnd);
	HDC hOffscreenDC = CreateCompatibleDC(hdc);

	// 初始化程序
	CreateApplication(&gApp);
	gApp->Init(hWnd, width, height);

	// 循环
	MSG msg;
	memset(&msg,0,sizeof(msg));
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			gApp->Update();
		}
	}

	// 退出
	gApp->Shutdown();
	delete gApp;

	UnregisterClass(WIN32_CLASS_NAME, hInstance);

	return 0;
}