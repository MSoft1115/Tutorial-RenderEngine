#include "Renderer.h"

namespace X {

	template<> IRenderer * Singleton<IRenderer>::msInstance = NULL;

	IRenderer::IRenderer()
	{
		mLight = NULL;
	}

	IRenderer::~IRenderer()
	{
	}

	void IRenderer::SetWorldMatrix(const Mat4 & m)
	{
		mWorldMatrix = m;
	}

	void IRenderer::SetViewMatrix(const Mat4 & m)
	{
		mViewMatrix = m;
	}

	void IRenderer::SetProjMatrix(const Mat4 & m)
	{
		mProjMatrix = m;
	}

	void IRenderer::SetCamera(const Float3 & pos, const Float3 & dir)
	{
		mCameraPos = pos;
		mCameraDir = dir;
	}

	void IRenderer::SetLight(Light * l)
	{
		mLight = l;
	}

	void IRenderer::SetMaterial(const Material & m)
	{
		mMaterial = m;
	}

	void IRenderer::SetRenderState(const RenderState & rstate)
	{
		mRenderState = rstate;
	}

	void IRenderer::SetShader(Shader::Ptr shader)
	{
		mShader = shader;
	}

	void IRenderer::Begin()
	{
		mLight = NULL;
	}

	void IRenderer::End()
	{
	}

}