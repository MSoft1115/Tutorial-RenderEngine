#include "GLContext.h"

LRESULT WINAPI WGL_DummyWndProc(HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp)
{
	return DefWindowProc(hwnd, umsg, wp, lp);
}

void WGL_Init()
{
	LPCSTR dummyText = "_WglDummy_";
	HINSTANCE hinst = GetModuleHandle(NULL);

	WNDCLASS dummyClass;
	memset(&dummyClass, 0, sizeof(WNDCLASS));
	dummyClass.style = CS_OWNDC;
	dummyClass.hInstance = hinst;
	dummyClass.lpfnWndProc = WGL_DummyWndProc;
	dummyClass.lpszClassName = dummyText;
	RegisterClass(&dummyClass);

	HWND hwnd = CreateWindow(dummyText, dummyText,
							 WS_POPUP | WS_CLIPCHILDREN,
							 0, 0, 32, 32, 0, 0, hinst, 0);
	HDC hdc = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE;
	pfd.iPixelType = PFD_TYPE_RGBA;

	int format;
	if ((format = ChoosePixelFormat(hdc, &pfd)) != 0)
		SetPixelFormat(hdc, format, &pfd);

	HGLRC hrc = wglCreateContext(hdc);

	wglMakeCurrent(hdc, hrc);

	GLenum err = glewInit();

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hrc);
	ReleaseDC(hwnd, hdc);
	DestroyWindow(hwnd);
	UnregisterClass(dummyText, hinst);

	wglSwapIntervalEXT(0);
}

void WGL_SetPixelFormat(HDC hDC)
{
	int cbits, abits;
	int dbits, sbits;

	cbits = 24, abits = 8; // R8G8B8
	dbits = 24, sbits = 8; // D24S8

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = cbits;
	pfd.cAlphaBits = abits;
	pfd.cDepthBits = dbits;
	pfd.cStencilBits = sbits;

	int PixelFormat = 0;
	{
		int iAttributes[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB, cbits,
			WGL_ALPHA_BITS_ARB, abits,
			WGL_DEPTH_BITS_ARB, dbits,
			WGL_STENCIL_BITS_ARB, sbits,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_SAMPLE_BUFFERS_ARB, GL_FALSE,
			WGL_SAMPLES_ARB, 4,
			0, 0
		};

		float fAttributes[] = {
			0, 0
		};

		UINT numFormats = 0;

		if (WGLEW_GET_FUN(__wglewChoosePixelFormatARB) != NULL)
			wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &PixelFormat, &numFormats);

		if (numFormats == 0)
			PixelFormat = ChoosePixelFormat(hDC, &pfd);
	}

	SetPixelFormat(hDC, PixelFormat, &pfd);
}

namespace X {

	GLContext::GLContext(HWND hWnd, int w, int h)
	{
		//初始化 WGL
		WGL_Init();

		m_hwnd = hWnd;
		m_hdc = GetDC(hWnd);

		// 设置像素格式
		WGL_SetPixelFormat(m_hdc);

		// 创建OpenGL Context
		m_glrc = wglCreateContext(m_hdc);

		// 设置当前 Context
		wglMakeCurrent(m_hdc, m_glrc);
	}

	GLContext::~GLContext()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_glrc);
		ReleaseDC(m_hwnd, m_hdc);
	}

	void GLContext::Resize(int w, int h)
	{
	}

	void GLContext::Present()
	{
		SwapBuffers(m_hdc);
	}

}