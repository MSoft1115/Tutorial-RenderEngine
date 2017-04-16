#include "ImageBMP.h"

namespace X {

	struct BMP_Header
	{
		word type;
		dword sizefile;
		word reserved1;
		word reserved2;
		dword offbytes;

		union BMP_HeaderInfo
		{
			struct {
				int size;
				int width;
				int height;
				short plane;
				short bitcount;
				int compression;
				int sizeimage;
				int xpelspermeter;
				int ypelspermeter;
				int clrused;
				int clrimportant;
			};
			
			byte m[40];

		} info;
	};

	struct BMP_RGBQuad
	{
		byte b;
		byte g;
		byte r;
		byte a;
	};

	bool BMP_Test(DataStream::Ptr stream)
	{
		d_assert (stream != NULL);
		
		word type = 0;
		
		int nreads = stream->Read(&type, sizeof(type));

		stream->Seek(-nreads, SEEK_CUR);

		return type == 0x4d42;
	}

	bool BMP_Load(Image & image, DataStream::Ptr stream)
	{
		d_assert (stream != NULL);

		DataStream & IS = *stream.c_ptr();

		BMP_Header header;
		header.type = 0;

		IS >> header.type;
		IS >> header.sizefile;
		IS >> header.reserved1;
		IS >> header.reserved2;
		IS >> header.offbytes;

		if (header.type != 0x4d42)
			return false;

		header.info.size = 0;
		IS.Read(&header.info, 40);
	
		if (header.info.size != 40)
		{
			d_assert(0 && "?: BMP format not supported");
			return false;
		}

		if (header.info.compression == 1 || header.info.compression == 2)
		{
			d_assert(0 && "?: BMP compression 'RLE', not supported");
			return false;
		}

		bool flip_vertically = header.info.height > 0;
		header.info.height = abs(header.info.height);

		image.width = header.info.width;
		image.height = header.info.height;
		image.depth = 1;
		image.faces = 1;

		if (header.info.bitcount == 8)
		{
			BMP_RGBQuad palette[256];
			IS.Read(palette, sizeof(BMP_RGBQuad) * 256);

			byte * data = new byte[image.width * image.height];
			IS.Read(data, image.width * image.height);

			image.pixels = new byte[image.width * image.height * 3];
			for (int i = 0; i < image.width * image.height; ++i)
			{
				image.pixels[i * 3 + 0] = palette[data[i]].r;
				image.pixels[i * 3 + 1] = palette[data[i]].g;
				image.pixels[i * 3 + 2] = palette[data[i]].b;
			}

			delete[] data;

			image.format = ePixelFormat::R8G8B8;
		}
		else if (header.info.bitcount == 16)
		{
			d_assert(0 && "?: BMP format not supported");
		}
		else if (header.info.bitcount == 24)
		{
			image.pixels = new byte[image.width * image.height * 3];

			IS.Read(image.pixels, image.width * image.height * 3);
			for (int i = 0; i < image.width * image.height; ++i)
			{
				std::swap(image.pixels[i * 3 + 0], image.pixels[i * 3 + 2]);
			}
			
			image.format = ePixelFormat::R8G8B8;
		}
		else if (header.info.bitcount == 32)
		{
			image.pixels = new byte[image.width * image.height * 4];
			IS.Read(image.pixels, image.width * image.height * 4);
			for (int i = 0; i < image.width * image.height; ++i)
			{
				std::swap(image.pixels[i * 4 + 0], image.pixels[i * 4 + 2]);
			}

			image.format = ePixelFormat::R8G8B8A8;
		}
		else
		{
			d_assert(0 && "?: BMP format not supported");
			return false;
		}

		if (flip_vertically)
		{
			int line_bytes = image.width * header.info.bitcount / 8;

			byte * buffer = new byte[line_bytes];
			for (int i = 0, j = image.height - 1; i < j; ++i, --j)
			{
				memcpy(buffer, &image.pixels[i * line_bytes], line_bytes);
				memcpy(&image.pixels[i * line_bytes], &image.pixels[j * line_bytes], line_bytes);
				memcpy(&image.pixels[j * line_bytes], buffer, line_bytes);
			}
			delete[] buffer;
		}

		return true;
	}

}