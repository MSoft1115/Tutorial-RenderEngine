#include "Texture.h"
#include "ResourceManager.h"

namespace X {

	Texture::Texture()
	{
	}

	Texture::~Texture()
	{
	}

	bool Texture::Load(const String & filename)
	{
		DataStream::Ptr stream = ResourceManager::Instance()->OpenResource(filename);

		return IMG_Load(mImage, stream);
	}

	Float4 Texture::GetColor(int u, int v)
	{
		d_assert(
			u >= 0 && u < mImage.width &&
			v >= 0 && v < mImage.height);

		Float4 color;
		const byte * data = (const byte *)mImage.pixels;
		int index = v * mImage.height + u;

		switch (mImage.format)
		{
			case ePixelFormat::A8:
			{
				byte a = data[index];

				color.r = 1;
				color.g = 1;
				color.b = 1;
				color.a = a / 255.0f;
			}
			break;

			case ePixelFormat::L8:
			{
				byte r = data[index];

				color.r = r / 255.0f;
				color.g = color.r;
				color.b = color.r;
				color.a = 1;
			}
			break;

			case ePixelFormat::L8A8:
			{
				byte r = data[index * 2];
				byte a = data[index * 2 + 1];

				color.r = r / 255.0f;
				color.g = color.r;
				color.b = color.r;
				color.a = a / 255.0f;
			}
			break;

			case ePixelFormat::R8G8B8:
			{
				byte r = data[index * 3];
				byte g = data[index * 3 + 1];
				byte b = data[index * 3 + 2];

				color.r = r / 255.0f;
				color.g = g / 255.0f;
				color.b = b / 255.0f;
				color.a = 1;
			}
			break;

			case ePixelFormat::R8G8B8A8:
			{
				byte r = data[index * 4];
				byte g = data[index * 4 + 1];
				byte b = data[index * 4 + 2];
				byte a = data[index * 4 + 3];

				color.r = r / 255.0f;
				color.g = g / 255.0f;
				color.b = b / 255.0f;
				color.a = a / 255.0f;
			}
			break;
		}

		return color;
	}

	Float4 Texture::Tex2D(float x, float y, const SamplerState & state)
	{
		if (mImage.pixels == NULL)
			return Float4(0, 0, 0);
		
		switch (state.AddressMode)
		{
			case eTexAddressMode::BORAD:
			{
				if (x < 0 || x > 1 || y < 0 || y > 1)
					return state.BorderColor;
			}
			break;

			case eTexAddressMode::CLAMP:
			{
				if (x < 0)
					x = 0;
				if (x > 1)
					x = 1;
				if (y < 0)
					y = 0;
				if (y > 1)
					y = 1;
			}
			break;

			case eTexAddressMode::WRAP:
			{
				while (x < 0)
					x += 1;
				while (x > 1)
					x -= 1;
				while (y < 0)
					y += 1;
				while (y > 1)
					y -= 1;
			}
			break;
		}

		float fu = x * (mImage.width - 1);
		float fv = y * (mImage.height - 1);

		int iu0 = (int)fu;
		int iv0 = (int)fv;

		if (state.Filter == eTexFilter::NEAREST)
		{
			Float4 c0;
			c0 = GetColor(iu0, iv0);

			return c0;
		}
		else if (state.Filter == eTexFilter::LINEAR)
		{
			int iu1 = iu0 + 1;
			int iv1 = iv0 + 1;

			if (iu1 > mImage.width - 1)
				iu1 = iu0;
			if (iv1 > mImage.height - 1)
				iv1 = iv0;

			Float4 c0, c1, c2, c3;
			c0 = GetColor(iu0, iv0);
			c1 = GetColor(iu1, iv0);
			c2 = GetColor(iu0, iv1);
			c3 = GetColor(iu1, iv1);

			float du = fu - iu0;
			float dv = fv - iv0;

			Float4 c4 = c0 + (c1 - c0) * du;
			Float4 c5 = c2 + (c3 - c2) * du;

			return c4 + (c5 - c4) * dv;
		}

		return Float4(0, 0, 0);
	}

}