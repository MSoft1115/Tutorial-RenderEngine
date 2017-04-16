#include "Image.h"
#include <array>

namespace X {

	int ePixelFormat::GetPixelSize(int format)
	{
		switch (format)
		{
			case ePixelFormat::A8:
				return 1;
			case ePixelFormat::L8:
				return 1;
			case ePixelFormat::L8A8:
				return 2;

			case ePixelFormat::R8G8B8:
				return 3;
			case ePixelFormat::R8G8B8A8:
				return 4;
		}

		d_assert(0);

		return 0;
	}

	int ePixelFormat::GetPixelSize(int format, int width, int height, int mipmaps)
	{
		int size = 0;
		for (int i = 0; i <= mipmaps; ++i)
		{
			switch (format)
			{
				case A8:
					size += 1 * width * height;
					break;
				case L8:
					size += 1 * width * height;
					break;
				case L8A8:
					size += 2 * width * height;
					break;
				case R8G8B8:
					size += 3 * width * height;
					break;
				case R8G8B8A8:
					size += 4 * width * height;
					break;
			}

			if ((width /= 2) == 0) width = 1;
			if ((height /= 2) == 0) height = 1;
		}

		d_assert(size != 0);

		return size;
	}


	//
	struct IMG_Factory
	{
		FN_ImageTest fn_test;
		FN_ImageLoad fn_load;
	};

	int _img_factory_size = 0;
	std::array<IMG_Factory, 16> _img_factory;

	void IMG_Reg(FN_ImageTest fn_test, FN_ImageLoad fn_load)
	{
		IMG_Factory factory = { fn_test, fn_load };

		_img_factory[_img_factory_size++] = factory;
	}

	void IMG_RegClear()
	{
		_img_factory_size = 0;
	}

	bool IMG_Load(Image & image, DataStream::Ptr stream)
	{
		for (int i = 0; i < _img_factory_size; ++i)
		{
			if (_img_factory[i].fn_test(stream))
			{
				return _img_factory[i].fn_load(image, stream);
			}
		}

		return false;
	}
	
}