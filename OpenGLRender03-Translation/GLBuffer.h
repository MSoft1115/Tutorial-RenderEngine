#pragma once

#include "Buffer.h"
#include "GLUtil.h"

namespace X {

	class GLRenderBuffer : public IRenderBuffer
	{
	public:
		GLRenderBuffer(int size);
		virtual ~GLRenderBuffer();

		virtual void
			Release() { delete this;  }

		virtual void *
			Lock(int flag);
		virtual void
			Unlock();

		GLuint
			GetGLBuffer() { return mGLBuffer; }

	protected:
		GLuint mGLBuffer;

		byte * mData;
		int mLockFlag;
	};

}