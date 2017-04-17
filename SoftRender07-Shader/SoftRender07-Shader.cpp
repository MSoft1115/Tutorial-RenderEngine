#include "App.h"
#include "SoftRenderer.h"
#include "ResourceManager.h"
#include <array>

using namespace X;

void CreateQuad(RenderOp * rop, float width, float height)
{
	static const int LINE_ELEMS = 16;
	static const int LINE_VERTS = LINE_ELEMS + 1;

	rop->vbuffer.resize(LINE_VERTS * LINE_VERTS);
	Vertex * vb = &rop->vbuffer[0];
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

	rop->ibuffer.resize(LINE_ELEMS * LINE_ELEMS * 6);
	short * ib = &rop->ibuffer[0];
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

	rop->primCount = LINE_ELEMS * LINE_ELEMS * 2;
	rop->primType = ePrimType::TRI_LIST;
}

struct MyShader : public Shader
{
	float mTime;

	virtual void VertexShader(RasterizerVertex * vo, const Vertex * vi)
	{
		vo->position = Float4(vi->position);
		vo->normal = vi->normal;
		vo->color = vi->color;
		vo->uv = vi->uv;

		// 波动
		vo->position.y += vo->position.y * sin(vo->position.x * PI + mTime * PI) * 0.15f;

		// 世界变换
		vo->position.TransformA(SoftRenderer::Instance()->GetWorldMatrix());

		// 视图变换
		vo->position.TransformA(SoftRenderer::Instance()->GetViewMatrix());

		// 投影变换
		vo->position.Transform(SoftRenderer::Instance()->GetProjMatrix());
	}

	bool PixelShader(RasterizerVertex * vio)
	{
		Texture * tex = SoftRenderer::Instance()->GetTexture();
		const SamplerState & sstate = SoftRenderer::Instance()->GetSamplerState();

		if (tex != NULL)
		{
			vio->color = vio->color * tex->Tex2D(vio->uv.x, vio->uv.y, sstate);
		}

		return true;
	}
};

class MyApp : public App
{
	ResourceManager * mResourceManager;
	SoftRenderer * mRenderer;
	RenderOp * mRenderOp;
	Texture mTexture;
	MyShader mShader;
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
		mResourceManager = new ResourceManager;
		mRenderer = new SoftRenderer(mWnd, w, h);

		mResourceManager->AddFloder("../Media");

		// 创建正方形
		mRenderOp = new RenderOp;
		CreateQuad(mRenderOp, 1, 1);

		// 加载纹理
		mTexture.Load("X.png");
	}

	virtual void OnUpdate()
	{
		mRenderer->Begin();

		mRenderer->Clear(Float3(0, 0, 0), 1);

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

		// 设置材质
		Material material;
		material.Ambient = Float3(1, 1, 1) * 0.3f;
		material.Diffuse = Float3(1, 1, 1);
		material.Specular = Float3(1, 1, 1) * 0.0f;
		material.SpecularPower = 20;
		mRenderer->SetMaterial(material);

		// 设置纹理
		mRenderer->SetTexture(&mTexture);

		// 设置采样状态
		SamplerState sstate;
		sstate.Filter = eTexFilter::LINEAR;
		sstate.AddressMode = eTexAddressMode::WRAP;
		mRenderer->SetSampleState(sstate);

		// 设置渲染状态
		RenderState rstate;
		rstate.CullMode = eCullMode::BACK; // 背面裁剪
		rstate.BlendMode = eBlendMode::OPACITY; // 不透明
		rstate.DepthMode = eDepthMode::LESS_EQUAL; // 深度测试模式
		rstate.DepthMask = TRUE; // 开启深度写
		mRenderer->SetRenderState(rstate);

		// 设置Shader
		mShader.mTime = mTime;
		mRenderer->SetShader(&mShader);

		// 渲染
		mRenderer->Render(mRenderOp);

		//
		mRenderer->End();

		// 提交
		mRenderer->Present();
	}

	virtual void OnShutdown()
	{
		delete mRenderOp;
		delete mRenderer;
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
