#pragma once

#include "Texture.h"
#include "GLUtil.h"

namespace X {

	class GLTexture : public Texture
	{
		friend class GLRenderer;

	public:
		GLTexture(const String & name, const String & source);
		virtual ~GLTexture();

		virtual void *
			Lock(int flag);
		virtual void
			Unlock();

		GLuint
			GetGLTexture() { return mGLTexture; }

	protected:
		void
			_load();
		void 
			_build();

	protected:
		GLuint mGLTexture;
		byte * mPixelData;
		int mLockFlag;
	};

}