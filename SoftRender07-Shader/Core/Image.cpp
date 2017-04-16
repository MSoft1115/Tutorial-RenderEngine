#include "Image.h"
#include <array>

namespace X {

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