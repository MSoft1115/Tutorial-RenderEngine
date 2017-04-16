#include "GLBuffer.h"

namespace X {

	GLRenderBuffer::GLRenderBuffer(int size)
	{
		glGenBuffers(1, &mGLBuffer);

		mSize = size;
		mData = NULL;
		mLockFlag = 0;
	}

	GLRenderBuffer::~GLRenderBuffer()
	{
	}

	void * GLRenderBuffer::Lock(int flag)
	{
		d_assert(mLockFlag == 0);

		mLockFlag = flag;

		if (mData == NULL && (mLockFlag & eLockFlag::WRITE))
		{
			mData = new byte[mSize];
		}

		d_assert(glGetError() == 0);

		return mData;
	}

	void GLRenderBuffer::Unlock()
	{
		d_assert(mLockFlag != 0);

		if (mLockFlag & eLockFlag::WRITE)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mGLBuffer);
			glBufferData(GL_ARRAY_BUFFER, mSize, mData, GL_STATIC_DRAW);
		}

		d_assert(glGetError() == 0);

		mLockFlag = 0;
	}

}