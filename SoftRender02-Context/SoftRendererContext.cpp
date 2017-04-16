#include "SoftRendererContext.h"

namespace X {

	SoftRendererContext::SoftRendererContext(HWND hWnd, int w, int h)
	{
		mWnd = hWnd;
		mWidth = w, mHeight = h;

		_build();
	}

	SoftRendererContext::~SoftRendererContext()
	{
		DeleteObject(mOffscreenBitmap);
		DeleteDC(mOffscreenDC);
	}

	void SoftRendererContext::_build()
	{
		HDC hdc = GetDC(mWnd);
		mOffscreenDC = CreateCompatibleDC(hdc);

		BITMAPINFO bi;
		memset(&bi, 0, sizeof(bi));
		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biWidth = mWidth;
		bi.bmiHeader.biHeight = -mHeight;
		bi.bmiHeader.biPlanes = 1;
		bi.bmiHeader.biBitCount = 24;
		bi.bmiHeader.biCompression = BI_RGB;
		mOffscreenBitmap = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, (void **)&mBackBuffer, NULL, 0);

		SelectObject(mOffscreenDC, mOffscreenBitmap);

		ReleaseDC(mWnd, hdc);

		// 32-bit aligned
		mLinePitch = mWidth * 24 / 8;
		mLinePitch = ((mLinePitch + 3) / 4) * 4;
	}

	void SoftRendererContext::Resize(int w, int h)
	{
		DeleteObject(mOffscreenBitmap);
		DeleteDC(mOffscreenDC);

		mWidth = w, mHeight = h;

		_build();
	}

	void SoftRendererContext::Present(const Float3 * buffer)
	{
		byte * pixels = (byte *)mBackBuffer;

		for (int j = 0; j < mHeight; ++j)
		{
			for (int i = 0; i < mWidth; ++i)
			{
				byte r = (unsigned char)(buffer[i].r * 255);
				byte g = (unsigned char)(buffer[i].g * 255);
				byte b = (unsigned char)(buffer[i].b * 255);

				pixels[i * 3 + 0] = b;
				pixels[i * 3 + 1] = g;
				pixels[i * 3 + 2] = r;
			}

			pixels += mLinePitch;
			buffer += mWidth;
		}

		HDC hdc = GetDC(mWnd);

		BitBlt(hdc, 0, 0, mWidth, mHeight,
			   mOffscreenDC, 0, 0, SRCCOPY);

		ReleaseDC(mWnd, hdc);
	}

}