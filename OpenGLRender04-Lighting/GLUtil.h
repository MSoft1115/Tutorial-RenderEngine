#pragma once

#include <glew.h>
#include <glut.h>
#include <wglew.h>

#include "Buffer.h"
#include "RenderOp.h"

namespace X {

	struct GLUtil
	{
		static GLenum GetGLPrimType(int type)
		{
			switch (type)
			{
				case ePrimType::TRI_LIST:
					return GL_TRIANGLES;

				case ePrimType::LINE_LIST:
					return GL_LINES;

				case ePrimType::POINT_LIST:
					return GL_POINTS;
			}

			d_assert(0);

			return GL_TRIANGLES;
		}

		static int GetGLDrawCount(int type, int primCount)
		{
			switch (type)
			{
				case ePrimType::TRI_LIST:
					return primCount * 3;

				case ePrimType::LINE_LIST:
					return primCount * 2;

				case ePrimType::POINT_LIST:
					return primCount;
			}

			d_assert(0);

			return 0;
		}

		static GLenum GetGLVertexFormat(int format)
		{
			switch (format)
			{
				case eVertexFormat::BYTE1:
				case eVertexFormat::BYTE2:
				case eVertexFormat::BYTE3:
				case eVertexFormat::BYTE4:
					return GL_BYTE;

				case eVertexFormat::UBYTE1:
				case eVertexFormat::UBYTE2:
				case eVertexFormat::UBYTE3:
				case eVertexFormat::UBYTE4:
					return GL_UNSIGNED_BYTE;

				case eVertexFormat::SHORT1:
				case eVertexFormat::SHORT2:
				case eVertexFormat::SHORT3:
				case eVertexFormat::SHORT4:
					return GL_SHORT;

				case eVertexFormat::USHORT1:
				case eVertexFormat::USHORT2:
				case eVertexFormat::USHORT3:
				case eVertexFormat::USHORT4:
					return GL_UNSIGNED_SHORT;

				case eVertexFormat::INT1:
				case eVertexFormat::INT2:
				case eVertexFormat::INT3:
				case eVertexFormat::INT4:
					return GL_INT;

				case eVertexFormat::UINT1:
				case eVertexFormat::UINT2:
				case eVertexFormat::UINT3:
				case eVertexFormat::UINT4:
					return GL_UNSIGNED_INT;

				case eVertexFormat::FLOAT1:
				case eVertexFormat::FLOAT2:
				case eVertexFormat::FLOAT3:
				case eVertexFormat::FLOAT4:
					return GL_FLOAT;
			}

			d_assert(0);

			return 0;
		}

		static int GetGLVertexElemCount(int format)
		{
			switch (format)
			{
				case eVertexFormat::BYTE1:
				case eVertexFormat::BYTE2:
				case eVertexFormat::BYTE3:
				case eVertexFormat::BYTE4:
					return format - eVertexFormat::BYTE1 + 1;

				case eVertexFormat::UBYTE1:
				case eVertexFormat::UBYTE2:
				case eVertexFormat::UBYTE3:
				case eVertexFormat::UBYTE4:
					return format - eVertexFormat::UBYTE1 + 1;

				case eVertexFormat::SHORT1:
				case eVertexFormat::SHORT2:
				case eVertexFormat::SHORT3:
				case eVertexFormat::SHORT4:
					return format - eVertexFormat::SHORT1 + 1;

				case eVertexFormat::USHORT1:
				case eVertexFormat::USHORT2:
				case eVertexFormat::USHORT3:
				case eVertexFormat::USHORT4:
					return format - eVertexFormat::USHORT1 + 1;

				case eVertexFormat::INT1:
				case eVertexFormat::INT2:
				case eVertexFormat::INT3:
				case eVertexFormat::INT4:
					return format - eVertexFormat::INT1 + 1;

				case eVertexFormat::UINT1:
				case eVertexFormat::UINT2:
				case eVertexFormat::UINT3:
				case eVertexFormat::UINT4:
					return format - eVertexFormat::UINT1 + 1;

				case eVertexFormat::FLOAT1:
				case eVertexFormat::FLOAT2:
				case eVertexFormat::FLOAT3:
				case eVertexFormat::FLOAT4:
					return format - eVertexFormat::FLOAT1 + 1;
			}

			d_assert(0);

			return 0;
		}

	};

}