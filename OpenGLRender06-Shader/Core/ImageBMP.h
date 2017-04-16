#pragma once

#include "Image.h"
#include "DataStream.h"

namespace X {

	bool 
		BMP_Test(DataStream::Ptr stream);
	bool
		BMP_Load(Image & image, DataStream::Ptr stream);
}