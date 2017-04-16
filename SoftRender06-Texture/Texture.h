#pragma once

#include "XCore.h"
#include "Image.h"

namespace X {

	struct eTexFilter
	{
		enum {
			NEAREST, // 最近点
			LINEAR,  // 线性
		};
	};

	struct eTexAddressMode
	{
		enum  {
			WRAP,
			CLAMP,
			BORAD,
		};
	};

	struct SamplerState
	{
		int Filter;
		int AddressMode;
		Float4 BorderColor;

		SamplerState()
		{
			Filter = eTexFilter::LINEAR;
			AddressMode = eTexAddressMode::WRAP;
			BorderColor = Float4(0, 0, 0, 0);
		}
	};

	class Texture
	{
	public:
		Texture();
		~Texture();

		bool
			Load(const String & filename);

		Float4
			GetColor(int u, int v);
		Float4
			Tex2D(float x, float y, const SamplerState & state);

	protected:
		Image mImage;
	};
}