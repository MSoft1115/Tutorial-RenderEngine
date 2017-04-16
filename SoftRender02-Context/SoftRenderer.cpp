#include "SoftRenderer.h"
#include "SoftRendererContext.h"

namespace X {

	SoftRenderer::SoftRenderer(HWND hWnd, int w, int h)
	{
		mContext = new SoftRendererContext(hWnd, w, h);

		mWidth = w, mHeight = h;
		mColorBuffer = new Float3[mWidth * mHeight];
		mDepthBuffer = new float[mWidth * mHeight];
	}

	SoftRenderer::~SoftRenderer()
	{
		delete[] mColorBuffer;
		delete[] mDepthBuffer;
	}

	void SoftRenderer::Resize(int w, int h)
	{
		d_assert(w > 1 && h > 1);

		mContext->Resize(w, h);

		delete[] mColorBuffer;
		delete[] mDepthBuffer;

		mWidth = w;
		mHeight = h;

		mColorBuffer = new Float3[mWidth * mHeight];
		mDepthBuffer = new float[mWidth * mHeight];
	}

	void SoftRenderer::Clear(const Float3 & color, float depth)
	{
		if (color == Float3::Zero)
		{
			// fast copy
			ZeroMemory(mColorBuffer, mWidth * mHeight * 12);
		}
		else
		{
			for (int i = 0; i < mWidth * mHeight; ++i)
				mColorBuffer[i] = color;
		}

		for (int i = 0; i < mWidth * mHeight; ++i)
			mDepthBuffer[i] = depth;
	}

	void SoftRenderer::Begin()
	{
	}

	void SoftRenderer::End()
	{
	}

	void SoftRenderer::Present()
	{
		mContext->Present(mColorBuffer);
	}

}

