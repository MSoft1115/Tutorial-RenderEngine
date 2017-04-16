/*
*	Image
*
*	Author: SiZhong.Wang(M-001)
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "XStdafx.h"
#include "DataStream.h"

namespace X {

	struct ePixelFormat
	{
		enum enum_t {
			UNKNOWN,

			// Color format
			A8,
			L8,
			L8A8,
			R8G8B8,
			R8G8B8A8,

			// Float format
			R16F,
			R16G16F,
			R16G16B16A16F,

			R32F,
			R32G32F,
			R32G32B32A32F,

			// Depth format
			D16,
			D24,
			D24S8,
		};

		static bool
			IsCompressed(int format);
		static int
			GetPixelSize(int format);
		static int
			GetPixelSize(ePixelFormat format, int width, int height, int mipmaps = 0);
	};

	struct Image
	{
		byte * pixels;
		int format;
		int width;
		int height;
		int depth;
		int mipmaps;
		int faces;

		Image()
		{
			pixels = NULL;
			format = ePixelFormat::UNKNOWN;
			width = 0;
			height = 0;
			depth = 1;
			mipmaps = 0;
			faces = 1;
		}

		~Image()
		{
			safe_delete_array(pixels);
		}
	};

	typedef bool (*FN_ImageTest)(DataStream::Ptr stream);
	typedef bool (*FN_ImageLoad)(Image & image, DataStream::Ptr stream);

	void
		IMG_Reg(FN_ImageTest fn_test, FN_ImageLoad fn_load);
	void
		IMG_RegClear();
	bool
		IMG_Load(Image & image, DataStream::Ptr stream);
}

#include "ImageBMP.h"
#include "ImagePNG.h"
