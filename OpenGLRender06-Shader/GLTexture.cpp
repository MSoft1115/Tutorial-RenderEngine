#include "GLTexture.h"
#include "ResourceManager.h"

namespace X {

	const byte G_LOG_2[256] = {
		0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
	};

	inline int _log2(int x)
	{
		int l = -1;

		while (x >= 256)
		{
			l += 8;
			x >>= 8;
		}

		return l + G_LOG_2[x];
	}

	GLTexture::GLTexture(const String & name, const String & source)
		: Texture(name, source)
	{
		mGLTexture = 0;
		mPixelData = NULL;
		mLockFlag = 0;
	}

	GLTexture::~GLTexture()
	{
		if (mGLTexture != 0)
			glDeleteTextures(1, &mGLTexture);

		safe_delete_array(mPixelData);
	}

	void * GLTexture::Lock(int flag)
	{
		d_assert(mLockFlag == 0);

		mLockFlag = flag;

		if (mPixelData == NULL)
		{
			int btPixel = ePixelFormat::GetPixelSize(mFormat);
			d_assert(btPixel != 0);

			mPixelData = new byte[btPixel * mWidth * mHeight];
			memset(mPixelData, 0, btPixel * mWidth * mHeight);
		}

		return mPixelData;
	}

	void GLTexture::Unlock()
	{
		d_assert(mLockFlag != 0);

		if (mLockFlag & eLockFlag::WRITE)
		{
			_build();
		}

		mLockFlag = 0;
	}

	void GLTexture::_load()
	{
		DataStream::Ptr stream = ResourceManager::Instance()->OpenResource(mSource);
		if (stream != NULL)
		{
			Image image;
			if (IMG_Load(image, stream))
			{
				mPixelData = image.pixels;
				mFormat = image.format;
				mWidth = image.width;
				mHeight = image.height;

				if (mMipmaps != -1)
					mMipmaps = image.mipmaps;

				image.pixels = NULL;

				_build();
			}
		}
	}

	void GLTexture::_build()
	{
		if (mPixelData != NULL)
		{
			if (mGLTexture == 0)
				glGenTextures(1, &mGLTexture);

			GLenum _internal = GLUtil::GetGLInernalFormat(mFormat);
			GLenum _format = GLUtil::GetGLPixelFormat(mFormat);
			GLenum _type = GLUtil::GetGLPixelType(mFormat);

			glBindTexture(GL_TEXTURE_2D, mGLTexture);

			glTexImage2D(GL_TEXTURE_2D, 0, _internal, mWidth, mHeight, 0, _format, _type, mPixelData);

			if (mMipmaps < 0)
			{
				mMipmaps = std::min(_log2(mWidth), _log2(mHeight));
				if (mMipmaps > 0)
				{
#ifdef M_PLATFORM_WIN32
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mMipmaps);

					glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
#endif
					glGenerateMipmap(GL_TEXTURE_2D);
				}
			}
		}
	}

}