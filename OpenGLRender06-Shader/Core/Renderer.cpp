#include "Renderer.h"

namespace X {

	template<> IRenderer * Singleton<IRenderer>::msInstance = NULL;

	IRenderer::IRenderer()
	{
		IMG_Reg(BMP_Test, BMP_Load);
		IMG_Reg(PNG_Test, PNG_Load);

		mLight = NULL;
		for (int i = 0; i < (int)mSamplerTextures.size(); ++i)
		{
			mSamplerTextures[i] = NULL;
		}
	}

	IRenderer::~IRenderer()
	{
		d_assert(mTextureMap.empty() && "Texture already destroy??");
	}

	void IRenderer::_destroyTexture(Texture * p)
	{
		auto i = mTextureMap.find(p->GetName());
		if (i != mTextureMap.end())
		{
			mTextureMap.erase(i);
		}

		delete p;
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

	void IRenderer::SetSamplerTexture(int i, Texture::Ptr texture)
	{
		mSamplerTextures[i] = texture.c_ptr();
	}

	void IRenderer::Begin()
	{
		mLight = NULL;
		for (int i = 0; i < (int)mSamplerTextures.size(); ++i)
		{
			mSamplerTextures[i] = NULL;
		}
	}

	void IRenderer::End()
	{
	}

}