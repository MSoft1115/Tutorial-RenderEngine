#pragma once

#include <windows.h>
#include "XCore.h"

namespace X {

	class SoftRendererContext
	{
	public:
		SoftRendererContext(HWND hWnd, int w, int h);
		~SoftRendererContext();

		void
			Resize(int w, int h);
		void
			Present(const Float3 * buffer);

	protected:
		void
			_build();

	protected:
		HWND mWnd;
		int mWidth, mHeight;
		int mLinePitch;
		HDC mOffscreenDC;
		HBITMAP mOffscreenBitmap;
		void * mBackBuffer;
	};

}