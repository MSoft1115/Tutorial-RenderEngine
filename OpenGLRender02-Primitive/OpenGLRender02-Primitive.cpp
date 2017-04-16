#include "App.h"
#include "GLRenderer.h"
#include "ResourceManager.h"

using namespace X;

struct Vertex
{
	Float3 position;
	Float4 color;
};

class MyApp : public App
{
	ResourceManager * mResourceManager;
	GLRenderer * mRenderer;
	Shader::Ptr mShader;
	RenderOp * mRenderOp;
	IRenderBuffer::Ptr mTriList;
	IRenderBuffer::Ptr mLineList;
	int mPrimType;

public:
	MyApp()
	{
	}

	virtual ~MyApp()
	{
	}

	virtual void OnInit(int w, int h)
	{
		// 创建资源管理器
		mResourceManager = new ResourceManager;
		mResourceManager->AddFloder("../Media");

		// 创建渲染器
		mRenderer = new GLRenderer(mWnd, w, h);

		// 加载Shader
		mShader = mRenderer->LoadShader("tutorial02.shader");

		// 创建一个正方形
		mRenderOp = new RenderOp;
		mRenderOp->vlayout.push(eVertexSemantic::POSITION, eVertexFormat::FLOAT3);
		mRenderOp->vlayout.push(eVertexSemantic::COLOR, eVertexFormat::FLOAT4);
		mRenderOp->vbuffer = mRenderer->CreateBuffer(sizeof(Vertex) * 4);
		mRenderOp->primCount = 2;
		mRenderOp->primType = ePrimType::TRI_LIST;

		Vertex * vb = (Vertex *)mRenderOp->vbuffer->Lock(eLockFlag::WRITE);
		{
			vb[0].position = Float3(-1, +1, 0) * 0.5f;
			vb[1].position = Float3(+1, +1, 0) * 0.5f;
			vb[2].position = Float3(-1, -1, 0) * 0.5f;
			vb[3].position = Float3(+1, -1, 0) * 0.5f;
			vb[0].color = Float4(1, 0, 0, 1);
			vb[1].color = Float4(1, 0, 0, 1);
			vb[2].color = Float4(0, 1, 0, 1);
			vb[3].color = Float4(0, 1, 0, 1);
		}
		mRenderOp->vbuffer->Unlock();

		// 三角形列表
		mTriList = mRenderer->CreateBuffer(6 * sizeof(short));
		short * ib = (short *)mTriList->Lock(eLockFlag::WRITE);
		{
			*ib++ = 0, *ib++ = 1, *ib++ = 2;
			*ib++ = 2, *ib++ = 1, *ib++ = 3;
		}
		mTriList->Unlock();

		// 线列表
		mLineList = mRenderer->CreateBuffer(8 * sizeof(short));
		ib = (short *)mLineList->Lock(eLockFlag::WRITE);
		{
			*ib++ = 0, *ib++ = 1;
			*ib++ = 1, *ib++ = 3;
			*ib++ = 3, *ib++ = 2;
			*ib++ = 2, *ib++ = 0;
		}
		mLineList->Unlock();

		// 点列表直接使用顶点的列表，不用额外定义

		mPrimType = ePrimType::TRI_LIST;
	}

	virtual void OnUpdate()
	{
		// 渲染开始
		mRenderer->Begin();

		// 清屏
		mRenderer->Clear(Float4(0, 0, 0), 1);

		// 随着时间旋转
		Mat4 mWorldMatrix;
		mWorldMatrix.MakeRotation(Float3(0, 0, 1), PI * mTime * 0.3f);

		// 设置变换矩阵
		mRenderer->SetWorldMatrix(mWorldMatrix);

		// 设置Shader Uniform
		float aspect = (float)mHeight / mWidth;
		mShader->SetUniform("u_Asecpt", aspect, 0, 0, 0);

		// 设置Shader
		mRenderer->SetShader(mShader);

		// 渲染图元
		switch (mPrimType)
		{
			case ePrimType::TRI_LIST:
				mRenderOp->ibuffer = mTriList;
				mRenderOp->primCount = 2;
				mRenderOp->primType = ePrimType::TRI_LIST;
				mRenderer->Render(mRenderOp);
				break;

			case ePrimType::LINE_LIST:
				mRenderOp->ibuffer = mLineList;
				mRenderOp->primCount = 4;
				mRenderOp->primType = ePrimType::LINE_LIST;
				mRenderer->Render(mRenderOp);
				break;

			case ePrimType::POINT_LIST:
				mRenderOp->ibuffer = NULL;
				mRenderOp->primCount = 4;
				mRenderOp->primType = ePrimType::POINT_LIST;
				mRenderer->Render(mRenderOp);
				break;
		}

		// 渲染结束
		mRenderer->End();

		// 提交
		mRenderer->Present();
	}

	virtual void OnShutdown()
	{
		mTriList = NULL;
		mLineList = NULL;
		delete mRenderOp;
		delete mRenderer;
		delete mResourceManager;
	}

	virtual void OnResize(int w, int h)
	{
		mRenderer->Resize(w, h);
	}

	virtual void OnKeyUp(int vk)
	{
		if (vk == VK_TAB)
		{
			mPrimType += 1;
			mPrimType %= 3;
		}
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new MyApp;
}
