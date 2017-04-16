#include "App.h"
#include "GLRenderer.h"
#include "ResourceManager.h"

using namespace X;

struct Vertex
{
	Float3 position;
	Float4 color;
	Float2 uv;
};

void CreateQuad(RenderOp * rop, float width, float height)
{
	static const int LINE_ELEMS = 16;
	static const int LINE_VERTS = LINE_ELEMS + 1;

	rop->vlayout.push(eVertexSemantic::POSITION, eVertexFormat::FLOAT3);
	rop->vlayout.push(eVertexSemantic::COLOR, eVertexFormat::FLOAT4);
	rop->vlayout.push(eVertexSemantic::TEXCOORD0, eVertexFormat::FLOAT2);

	rop->vbuffer = IRenderer::Instance()->CreateBuffer(sizeof(Vertex) * LINE_VERTS * LINE_VERTS);
	Vertex * vb = (Vertex *)rop->vbuffer->Lock(eLockFlag::WRITE);
	for (int j = 0; j <= LINE_ELEMS; ++j)
	{
		for (int i = 0; i <= LINE_ELEMS; ++i)
		{
			vb->position.x = (i / (float)LINE_ELEMS - 0.5f) * width;
			vb->position.y = (0.5f - j / (float)LINE_ELEMS) * height;
			vb->position.z = 0;
			vb->color = Float4(1, 1, 1);
			vb->uv.x = i / (float)LINE_ELEMS;
			vb->uv.y = j / (float)LINE_ELEMS;

			++vb;
		}
	}
	rop->vbuffer->Unlock();

	rop->ibuffer = IRenderer::Instance()->CreateBuffer(sizeof(short) * LINE_ELEMS * LINE_ELEMS * 6);
	short * ib = (short *)rop->ibuffer->Lock(eLockFlag::WRITE);
	for (int j = 0; j < LINE_ELEMS; ++j)
	{
		for (int i = 0; i < LINE_ELEMS; ++i)
		{
			*ib++ = (j + 0) * LINE_VERTS + (i + 0);
			*ib++ = (j + 0) * LINE_VERTS + (i + 1);
			*ib++ = (j + 1) * LINE_VERTS + (i + 0);

			*ib++ = (j + 1) * LINE_VERTS + (i + 0);
			*ib++ = (j + 0) * LINE_VERTS + (i + 1);
			*ib++ = (j + 1) * LINE_VERTS + (i + 1);
		}
	}
	rop->ibuffer->Unlock();

	rop->primCount = LINE_ELEMS * LINE_ELEMS * 2;
	rop->primType = ePrimType::TRI_LIST;
}

//
class MyApp : public App
{
	ResourceManager * mResourceManager;
	GLRenderer * mRenderer;
	Shader::Ptr mShader;
	RenderOp * mRenderOp;
	Texture::Ptr mTexture;
	Mat4 mWorldMatrix;
	Mat4 mViewMatrix;
	Mat4 mProjMatrix;

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
		mShader = mRenderer->LoadShader("tutorial06.shader");

		// 创建正方形
		mRenderOp = new RenderOp;
		CreateQuad(mRenderOp, 1, 1);

		// 加载纹理
		mTexture = mRenderer->LoadTexture("X.png");
	}

	virtual void OnUpdate()
	{
		// 渲染开始
		mRenderer->Begin();

		// 清屏
		mRenderer->Clear(Float4(0.3f, 0.3f, 0.3f), 1);

		// 世界矩阵
		mWorldMatrix = Mat4::Identity;

		// 相机矩阵
		Float3 vEye = Float3(0, 0, -2);
		Float3 vAt = Float3(0, 0, 0);
		Float3 vUp = Float3(0, 1, 0);
		mViewMatrix.MakeViewTM(vEye, vAt, vUp);

		// 投影矩阵
		float nearClip = 0.1f;
		float farClip = 1000.0f;
		float fovy = PI / 3;
		float aspect = mWidth / (float)mHeight;
		mProjMatrix.MakePerspectiveTM(fovy, aspect, nearClip, farClip);

		// 设置变换矩阵
		mRenderer->SetWorldMatrix(mWorldMatrix);
		mRenderer->SetViewMatrix(mViewMatrix);
		mRenderer->SetProjMatrix(mProjMatrix);

		// 设置相机
		Float3 vDir = vAt - vEye;
		vDir.Normalize();
		mRenderer->SetCamera(vEye, vDir);

		// 设置Shader
		mShader->SetUniform("u_Time", mTime, 0, 0, 0);

		mRenderer->SetShader(mShader);

		// 设置渲染状态
		RenderState rstate;
		rstate.CullMode = eCullMode::BACK; // 背面裁剪
		rstate.BlendMode = eBlendMode::OPACITY; // 不透明
		rstate.DepthMode = eDepthMode::LESS_EQUAL; // 深度测试模式
		rstate.DepthMask = TRUE; // 开启深度写
		mRenderer->SetRenderState(rstate);

		// 设置纹理
		mRenderer->SetSamplerTexture(0, mTexture);

		// 渲染图元
		mRenderer->Render(mRenderOp);

		// 渲染结束
		mRenderer->End();

		// 提交
		mRenderer->Present();
	}

	virtual void OnShutdown()
	{
		mTexture = NULL;
		delete mRenderOp;
		delete mRenderer;
		delete mResourceManager;
	}

	virtual void OnResize(int w, int h)
	{
		mRenderer->Resize(w, h);
	}

};

void CreateApplication(App ** ppApp)
{
	*ppApp = new MyApp;
}
