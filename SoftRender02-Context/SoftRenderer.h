#pragma once

#include "stdafx.h"
#include "XCore.h"

namespace X {

	class SoftRendererContext;

	class SoftRenderer
	{
	public:
		SoftRenderer(HWND hWnd, int w, int h);
		~SoftRenderer();

		virtual void 
			Resize(int width, int height);
		virtual void
			Clear(const Float3 & color = Float3(0, 0, 0), float depth = 1.0f);
		virtual void
			Begin();
		virtual void
			End();
		virtual void
			Present();
	
	protected:
		SoftRendererContext * mContext;
		int mWidth, mHeight;
		Float3 * mColorBuffer;
		float * mDepthBuffer;
	};

}
