#include "Buffer.h"

namespace X {

	int eVertexFormat::GetFormatSize(int fmt)
	{
		switch (fmt)
		{
			case BYTE1:
			case BYTE2:
			case BYTE3:
			case BYTE4:
				return (fmt - BYTE1 + 1) * 1;

			case UBYTE1:
			case UBYTE2:
			case UBYTE3:
			case UBYTE4:
				return (fmt - UBYTE1 + 1) * 1;

			case SHORT1:
			case SHORT2:
			case SHORT3:
			case SHORT4:
				return (fmt - SHORT1 + 1) * 2;

			case USHORT1:
			case USHORT2:
			case USHORT3:
			case USHORT4:
				return (fmt - USHORT1 + 1) * 2;

			case INT1:
			case INT2:
			case INT3:
			case INT4:
				return (fmt - INT1 + 1) * 4;

			case UINT1:
			case UINT2:
			case UINT3:
			case UINT4:
				return (fmt - UINT1 + 1) * 4;

			case FLOAT1:
			case FLOAT2:
			case FLOAT3:
			case FLOAT4:
				return (fmt - FLOAT1 + 1) * 4;
		}

		d_assert(0);
		return 0;
	}

	//
	VertexLayout::VertexLayout()
	{
		mNumElem = 0;
	}

	VertexLayout::~VertexLayout()
	{
	}

	void VertexLayout::push(int semantic, int format)
	{
		d_assert(mNumElem + 1 < MAX_ELEMS);

		mElems[mNumElem].semantic = semantic;
		mElems[mNumElem].format = format;
		mElems[mNumElem].offset = stride();

		++mNumElem;
	}

	int VertexLayout::size()
	{
		return mNumElem;
	}

	int VertexLayout::stride()
	{
		int offset = 0;
		for (int i = 0; i < mNumElem; ++i)
		{
			offset += eVertexFormat::GetFormatSize(mElems[i].format);
		}

		return offset;
	}

	const VertexLayout::Element & VertexLayout::operator[](int i) const
	{
		d_assert(i < MAX_ELEMS);
		return mElems[i];
	}

}