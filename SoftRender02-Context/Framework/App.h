
#pragma once

#include "stdafx.h"

class App
{
public:
	App();
	virtual ~App();

	virtual void 
		Init(HWND hWnd, int w, int h);
	virtual void 
		Update();
	virtual void 
		Shutdown();
	virtual void
		Resize(int w, int h);

	virtual void 
		OnInit(int w, int h) = 0;
	virtual void 
		OnUpdate() = 0;
	virtual void 
		OnShutdown() = 0;
	virtual void
		OnResize(int w, int h) = 0;
	virtual void
		OnMouseDown(int btn) {}
	virtual void
		OnMouseUp(int btn) {}
	virtual void
		OnMouseMove(int x, int y) {}
	virtual void
		OnKeyDown(int vk) {}
	virtual void
		OnKeyUp(int vk) {}

protected:
	HWND mWnd;
	int mWidth, mHeight;

	float mTime;
	float mFrameTime;
};

extern "C" {

	void CreateApplication(App ** ppApp);

	typedef void (*CREATE_APPLICATION_FUNCTION)(App ** ppApp);
};


