#pragma once

#include "XCore.h"

namespace X {

	struct Vertex
	{
		Float3 position;	// 位置
		Float3 normal;		// 法线
		Float4 color;		// 颜色
		Float2 uv;			// 纹理坐标
	};

	struct RasterizerVertex
	{
		Float4 position;	// 位置
		Float3 normal;		// 法线
		Float4 color;		// 颜色
		Float2 uv;			// 纹理坐标

		static void Lerp(RasterizerVertex & v, const RasterizerVertex & a, const RasterizerVertex & b, float k)
		{
			v.position = a.position + (b.position - a.position) * k;
			v.normal = a.normal + (b.normal - a.normal) * k;
			v.color = a.color + (b.color - a.color) * k;
			v.uv = a.uv + (b.uv - a.uv) * k;
		}
	};

	struct ePrimType
	{
		enum {
			POINT_LIST,
			LINE_LIST,
			TRI_LIST,
		};
	};

	struct RenderOp
	{
		std::vector<Vertex> vbuffer;
		std::vector<short> ibuffer;
		int primType;
		int primCount;
	};

}