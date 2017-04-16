#include "App.h"

class MyApp : public App
{
public:
	MyApp()
	{
	}

	virtual ~MyApp()
	{
	}

	virtual void OnInit(int w, int h)
	{
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnShutdown()
	{
	}

	virtual void OnResize(int w, int h)
	{
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new MyApp;
}
