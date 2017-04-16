#include "App.h"
#include "GLRenderer.h"
#include "ResourceManager.h"

using namespace X;

struct Vertex
{
	Float3 position;
	Float3 normal;
	Float4 color;
	Float2 uv;
};

void CreateBox(RenderOp * rop, float width, float height, float depth)
{
	const float half_w = width * 0.5f;
	const float half_h = height * 0.5f;
	const float half_d = depth * 0.5f;

	rop->vlayout.push(eVertexSemantic::POSITION, eVertexFormat::FLOAT3);
	rop->vlayout.push(eVertexSemantic::NORMAL, eVertexFormat::FLOAT3);
	rop->vlayout.push(eVertexSemantic::COLOR, eVertexFormat::FLOAT4);
	rop->vlayout.push(eVertexSemantic::TEXCOORD0, eVertexFormat::FLOAT2);

#define d_pos(i) vb->position = all_pos[i]
#define d_normal(i) vb->normal = all_normal[i]
#define d_color(i) vb->color = all_color[i]
#define d_uv(i) vb->uv = all_uv[i]

	Float3 all_pos[8] = {
		Float3(-half_w, half_h, -half_d),
		Float3(half_w, half_h, -half_d),
		Float3(-half_w, -half_h, -half_d),
		Float3(half_w, -half_h, -half_d),

		Float3(-half_w, half_h, half_d),
		Float3(half_w, half_h, half_d),
		Float3(-half_w, -half_h, half_d),
		Float3(half_w, -half_h, half_d),
	};

	Float3 all_normal[6] = {
		Float3(0, 0, -1),
		Float3(0, 0, 1),
		Float3(-1, 0, 0),
		Float3(1, 0, 0),
		Float3(0, 1, 0),
		Float3(0, -1, 0)
	};

	Float4 all_color[6] = {
		Float4(1, 0, 0),
		Float4(1, 0, 0),
		Float4(1, 0, 0),
		Float4(1, 0, 0),
		Float4(1, 0, 0),
		Float4(1, 0, 0)
	};

	Float2 all_uv[4] = {
		Float2(0, 0),
		Float2(1, 0),
		Float2(0, 1),
		Float2(1, 1)
	};

	rop->vbuffer = IRenderer::Instance()->CreateBuffer(sizeof(Vertex) * 4 * 6);
	Vertex * vb = (Vertex *)rop->vbuffer->Lock(eLockFlag::WRITE);
	// front
	d_pos(0), d_normal(0), d_color(0), d_uv(0), ++vb;
	d_pos(1), d_normal(0), d_color(0), d_uv(1), ++vb;
	d_pos(2), d_normal(0), d_color(0), d_uv(2), ++vb;
	d_pos(3), d_normal(0), d_color(0), d_uv(3), ++vb;

	// back
	d_pos(5), d_normal(1), d_color(1), d_uv(0), ++vb;
	d_pos(4), d_normal(1), d_color(1), d_uv(1), ++vb;
	d_pos(7), d_normal(1), d_color(1), d_uv(2), ++vb;
	d_pos(6), d_normal(1), d_color(1), d_uv(3), ++vb;

	// left
	d_pos(4), d_normal(2), d_color(2), d_uv(0), ++vb;
	d_pos(0), d_normal(2), d_color(2), d_uv(1), ++vb;
	d_pos(6), d_normal(2), d_color(2), d_uv(2), ++vb;
	d_pos(2), d_normal(2), d_color(2), d_uv(3), ++vb;

	// right
	d_pos(1), d_normal(3), d_color(3), d_uv(0), ++vb;
	d_pos(5), d_normal(3), d_color(3), d_uv(1), ++vb;
	d_pos(3), d_normal(3), d_color(3), d_uv(2), ++vb;
	d_pos(7), d_normal(3), d_color(3), d_uv(3), ++vb;

	// top
	d_pos(4), d_normal(4), d_color(4), d_uv(0), ++vb;
	d_pos(5), d_normal(4), d_color(4), d_uv(1), ++vb;
	d_pos(0), d_normal(4), d_color(4), d_uv(2), ++vb;
	d_pos(1), d_normal(4), d_color(4), d_uv(3), ++vb;

	// bottom
	d_pos(2), d_normal(5), d_color(5), d_uv(0), ++vb;
	d_pos(3), d_normal(5), d_color(5), d_uv(1), ++vb;
	d_pos(6), d_normal(5), d_color(5), d_uv(2), ++vb;
	d_pos(7), d_normal(5), d_color(5), d_uv(3), ++vb;

	rop->vbuffer->Unlock();

#undef d_pos
#undef d_normal
#undef d_uv

	rop->ibuffer = IRenderer::Instance()->CreateBuffer(sizeof(short) * 6 * 6);
	short * ib = (short *)rop->ibuffer->Lock(eLockFlag::WRITE);
	for (short i = 0; i < 6; ++i)
	{
		short idx = i * 4;

		*ib++ = idx + 0;
		*ib++ = idx + 1;
		*ib++ = idx + 2;

		*ib++ = idx + 2;
		*ib++ = idx + 1;
		*ib++ = idx + 3;
	}
	rop->ibuffer->Unlock();

	rop->primCount = 6 * 2;
	rop->primType = ePrimType::TRI_LIST;
}

class MyApp : public App
{
	ResourceManager * mResourceManager;
	GLRenderer * mRenderer;
	Shader::Ptr mShader;
	RenderOp * mRenderOp;
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
		mShader = mRenderer->LoadShader("tutorial03.shader");

		// 创建正方体
		mRenderOp = new RenderOp;
		CreateBox(mRenderOp, 1, 1, 1);
	}

	virtual void OnUpdate()
	{
		// 渲染开始
		mRenderer->Begin();

		// 清屏
		mRenderer->Clear(Float4(0, 0, 0), 1);

		// 世界矩阵
		mWorldMatrix.MakeRotation(Float3(0, 1, 0), PI * mTime * 0.3f);

		// 相机矩阵
		Float3 vEye = Float3(0, 3, -3);
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

		// 设置Shader
		mRenderer->SetShader(mShader);

		// 设置渲染状态
		RenderState rstate;
		rstate.CullMode = eCullMode::BACK; // 背面裁剪
		rstate.BlendMode = eBlendMode::OPACITY; // 不透明
		rstate.DepthMode = eDepthMode::LESS_EQUAL; // 深度测试模式
		rstate.DepthMask = TRUE; // 开启深度写
		mRenderer->SetRenderState(rstate);

		// 渲染图元
		mRenderer->Render(mRenderOp);

		// 渲染结束
		mRenderer->End();

		// 提交
		mRenderer->Present();
	}

	virtual void OnShutdown()
	{
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
