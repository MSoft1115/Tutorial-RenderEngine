#pragma once

#include "XStdafx.h"
#include "SharedPtr.h"

namespace X {

	struct eVertexSemantic
	{
		enum {
			POSITION,
			NORMAL,
			COLOR,
			BONE_INDEX,
			BONE_WEIGHT,
			TEXCOORD0,
			TEXCOORD1,
			TEXCOORD2,
			TEXCOORD3,

			MAX
		};
	};

	struct eVertexFormat
	{
		enum {
			BYTE1,
			BYTE2,
			BYTE3,
			BYTE4,

			UBYTE1,
			UBYTE2,
			UBYTE3,
			UBYTE4,

			SHORT1,
			SHORT2,
			SHORT3,
			SHORT4,

			USHORT1,
			USHORT2,
			USHORT3,
			USHORT4,

			INT1,
			INT2,
			INT3,
			INT4,

			UINT1,
			UINT2,
			UINT3,
			UINT4,

			FLOAT1,
			FLOAT2,
			FLOAT3,
			FLOAT4,
		};

		static int
			GetFormatSize(int fmt);
	};

	struct eLockFlag
	{
		enum enum_t {
			READ = 1,
			WRITE = 2,
			READ_WRITE = READ | WRITE,
		};
	};

	class VertexLayout
	{
	public:
		struct Element
		{
			int semantic;
			int format;
			int offset;
		};

		static const int MAX_ELEMS = 16;

	public:
		VertexLayout();
		~VertexLayout();

		void 
			push(int semantic, int format);
		int 
			size();
		int 
			stride();

		const Element &
			operator[](int i) const;

	protected:
		Element mElems[MAX_ELEMS];
		int mNumElem;
	};

	//
	class IRenderBuffer
	{
		DECLARE_REF_COUNTER();

	public:
		typedef SharedPtr<IRenderBuffer> Ptr;

	public:
		IRenderBuffer() {}
		virtual ~IRenderBuffer() {}

		virtual void
			Release() = 0;

		virtual void *
			Lock(int flag) = 0;
		virtual void
			Unlock() = 0;

	protected:
		int mSize;
	};
	
}