#include "App.h"
#include "SoftRenderer.h"
#include "ResourceManager.h"
#include <array>

using namespace X;

struct QuadMesh
{
	static const int LINE_ELEMS = 16;
	static const int LINE_VERTS = LINE_ELEMS + 1;

	std::array<Vertex, LINE_VERTS * LINE_VERTS> vertexBuffer;
	std::array<int, LINE_ELEMS * LINE_ELEMS * 6> indexBuffer;

	void Create(float width, float height)
	{
		Vertex * vb = &vertexBuffer[0];
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

		int * ib = &indexBuffer[0];
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
	}
};

float gTime = 0;

void VertexShader(RasterizerVertex * vo, const Vertex * vi, SoftRenderer * renderer)
{
	vo->position = Float4(vi->position);
	vo->normal = vi->normal;
	vo->color = vi->color;
	vo->uv = vi->uv;

	// 波动
	vo->position.y += vo->position.y * sin(vo->position.x * PI + gTime * PI) * 0.15f;

	// 世界变换
	vo->position.TransformA(renderer->GetWorldMatrix());

	// 视图变换
	vo->position.TransformA(renderer->GetViewMatrix());

	// 投影变换
	vo->position.Transform(renderer->GetProjMatrix());
}

bool PixelShader(RasterizerVertex * vio, SoftRenderer * renderer)
{
	Texture * tex = renderer->GetTexture();
	const SamplerState & sstate = renderer->GetSamplerState();

	if (tex != NULL)
	{
		vio->color = vio->color * tex->Tex2D(vio->uv.x, vio->uv.y, sstate);
	}

	return true;
}

class MyApp : public App
{
	ResourceManager * mResourceManager;
	SoftRenderer * mRenderer;
	QuadMesh mMesh;
	Texture mTexture;
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

		// 创建球体
		mMesh.Create(1, 1);

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
		Shader shader;
		shader.pfn_vs = VertexShader;
		shader.pfn_ps = PixelShader;
		mRenderer->SetShader(&shader);

		// 渲染
		mRenderer->Render(&mMesh.vertexBuffer[0],
						  mMesh.vertexBuffer.size(),
						  &mMesh.indexBuffer[0],
						  mMesh.indexBuffer.size() / 3,
						  ePrimType::TRI_LIST);
		//
		mRenderer->End();

		// 提交
		mRenderer->Present();

		//
		gTime = mTime;
	}

	virtual void OnShutdown()
	{
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
