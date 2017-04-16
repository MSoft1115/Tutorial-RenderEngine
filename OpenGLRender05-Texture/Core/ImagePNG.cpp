#include "ImagePNG.h"
#include "lodepng.c"

namespace X {

	union PNG_Chunk
	{
		struct {
			int length;
			int id;
		};

		char ch[8];
	};

	struct PNG_Header
	{
		dword width;
		dword height;
		byte bitdepth;
		byte colortype;
		byte compression;
		byte filter;
		byte interlace;
	};

	bool PNG_Test(DataStream::Ptr stream)
	{
		byte png_magic[8] = { 137,80,78,71,13,10,26,10 };
		byte magic[8];

		int nreads = stream->Read(magic, 8);

		stream->Seek(-nreads, SEEK_CUR);

		return memcmp(magic, png_magic, 8) == 0;
	}

	int PNG_Read8(byte & i8, DataStream & IS)
	{
		return IS.Read(&i8, 1);
	}

	int PNG_Read16(word & i16, DataStream & IS)
	{
		int nreads = IS.Read(&i16, 2);

		i16 = ENDIAN_BIG_TO_HOST(i16);

		return nreads;
	}

	int PNG_Read32(dword & i32, DataStream & IS)
	{
		int nreads = IS.Read(&i32, 4);

		i32 = ENDIAN_BIG_TO_HOST(i32);

		return nreads;
	}

	int PGN_ReadChunk(PNG_Chunk & ck, DataStream & IS)
	{
		int nreads = IS.Read(&ck, 8);

		ck.id = ENDIAN_BIG_TO_HOST(ck.id);
		ck.length = ENDIAN_BIG_TO_HOST(ck.length);

		return nreads;
	}

	bool PNG_Load(Image & image, DataStream::Ptr stream)
	{
		d_assert (stream != NULL);

		DataStream & IS = *stream.c_ptr();

		int nreads = 0;
		byte png_magic[8] = { 137,80,78,71,13,10,26,10 };
		byte magic[8];

		nreads += IS.Read(magic, 8);
		if (memcmp(magic, png_magic, 8) != 0)
			return NULL;

		PNG_Chunk ck;
		nreads += PGN_ReadChunk(ck, IS);

		if (ck.id != MAKE_DWORD('I','H','D','R') && ck.length != 13)
			return NULL;

		PNG_Header header;
		nreads += PNG_Read32(header.width, IS);
		nreads += PNG_Read32(header.height, IS);
		nreads += PNG_Read8(header.bitdepth, IS);
		nreads += PNG_Read8(header.colortype, IS);

		if (header.bitdepth != 8)
			return NULL;

		image.width = header.width;
		image.height = header.height;
		image.depth = 1;
		image.faces = 1;

		LodePNGColorType colortype;
		int chanels = 0;

		switch (header.colortype)
		{
		case 0:
			image.format = ePixelFormat::L8;
			colortype = LCT_GREY;
			chanels = 1;
			break;

		case 2:
			image.format = ePixelFormat::R8G8B8;
			colortype = LCT_RGB;
			chanels = 3;
			break;

		case 3:
			image.format = ePixelFormat::R8G8B8A8;
			colortype = LCT_RGBA;
			chanels = 4;
			break;

		case 4:
			image.format = ePixelFormat::L8A8;
			colortype = LCT_GREY_ALPHA;
			chanels = 2;
			break;

		case 6:
			image.format = ePixelFormat::R8G8B8A8;
			colortype = LCT_RGBA;
			chanels = 4;
			break;
		}

		if (chanels == ePixelFormat::UNKNOWN)
			return false;

		IS.Seek(-nreads, SEEK_CUR);

		//
		unsigned char * data = (unsigned char *)stream->GetData();
		int len = stream->Size();
		unsigned int w = 0, h = 0;

		int err = lodepng_decode_memory(&image.pixels, &w, &h, data, len, colortype, 8);
		
		return err == 0;
	}

	bool PNG_Save(const String & filename, const byte * pixels, int width, int height, int format)
	{
		if (format != ePixelFormat::L8 && format != ePixelFormat::L8A8 &&
			format != ePixelFormat::R8G8B8 && format != ePixelFormat::R8G8B8A8)
		{
			d_assert(0 && "?: PNG format not supported");
			return false;
		}

		FILE * fp = fopen(filename.c_str(), "wb");
		if (fp == NULL)
		{
			d_assert(0 && "File can't open");
			return false;
		}

		LodePNGColorType colortype;
		switch (format)
		{
		case ePixelFormat::L8:
			colortype = LCT_GREY;
			break;

		case ePixelFormat::L8A8:
			colortype = LCT_GREY_ALPHA;
			break;

		case ePixelFormat::R8G8B8:
			colortype = LCT_RGB;
			break;

		case ePixelFormat::R8G8B8A8:
			colortype = LCT_RGBA;
			break;
		}

		byte * data = NULL;
		size_t size = 0;

		lodepng_encode_memory(&data, &size, pixels, width, height, colortype, 8);
		if (data != NULL)
		{
			fwrite(data, size, 1, fp);
			free(data);
		}

		fclose(fp);

		return true;
	}

}