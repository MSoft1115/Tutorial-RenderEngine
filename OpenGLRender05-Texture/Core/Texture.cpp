#include "Texture.h"
#include "Renderer.h"

namespace X {

	Texture::Texture(const String & name, const String & source)
		: mName(name)
		, mSource(source)
	{
		mFormat = ePixelFormat::UNKNOWN;
		mWidth = 0;
		mHeight = 0;
		mMipmaps = -1;
	}

	Texture::~Texture()
	{
	}

	void Texture::Release()
	{
		IRenderer::Instance()->_destroyTexture(this);
	}

}
