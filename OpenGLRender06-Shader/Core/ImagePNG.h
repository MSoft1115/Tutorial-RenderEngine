#pragma once

#include "Image.h"
#include "DataStream.h"

namespace X {

	bool 
		PNG_Test(DataStream::Ptr stream);
	bool
		PNG_Load(Image & image, DataStream::Ptr stream);
	bool
		PNG_Save(const String & filename, const byte * pixels, int width, int height, int format);

}