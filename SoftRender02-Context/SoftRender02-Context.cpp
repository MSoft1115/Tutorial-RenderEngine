#include "App.h"
#include "SoftRenderer.h"

using namespace X;

class MyApp : public App
{
	SoftRenderer * mRenderer;

public:
	MyApp()
	{
	}

	virtual ~MyApp()
	{
	}

	virtual void OnInit(int w, int h)
	{
		mRenderer = new SoftRenderer(mWnd, w, h);
	}

	virtual void OnUpdate()
	{
		// 开始渲染
		mRenderer->Begin();

		// 清屏
		mRenderer->Clear(Float3(1, 0, 0), 1);

		// 渲染结束
		mRenderer->End();

		// 提交
		mRenderer->Present();
	}

	virtual void OnShutdown()
	{
		delete mRenderer;
	}

	virtual void OnResize(int w, int h)
	{
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new MyApp;
}
