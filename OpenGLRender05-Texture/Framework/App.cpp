#include "App.h"

App::App()
{
}

App::~App()
{
}

void App::Init(HWND hWnd, int w, int h)
{
	mWnd = hWnd;
	mWidth = w, mHeight = h;

	OnInit(w, h);

	mTime = 0;
	mFrameTime = 1.0f / 60.0f;
}

void App::Update()
{
	static DWORD last_tick = GetTickCount();
	DWORD cur_tick = GetTickCount();

	OnUpdate();

	mFrameTime = (cur_tick - last_tick) / 1000.0f;
	mTime += mFrameTime;
	last_tick = cur_tick;
}

void App::Shutdown()
{
	OnShutdown();
}

void App::Resize(int w, int h)
{
	mWidth = w, mHeight = h;
	OnResize(w, h);
}

