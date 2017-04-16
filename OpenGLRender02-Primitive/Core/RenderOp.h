#pragma once

#include "XStdafx.h"
#include "Float3.h"
#include "Buffer.h"

namespace X {

	struct ePrimType
	{
		enum {
			POINT_LIST,
			LINE_LIST,
			TRI_LIST,
		};
	};

	struct eCullMode
	{
		enum {
			NONE,
			FRONT,
			BACK,
		};
	};

	struct eBlendMode
	{
		enum {
			OPACITY,
			ADD,
			//ALPHA_TEST,
			ALPHA_BLEND,
		};
	};

	struct eDepthMode
	{
		enum {
			ALWAYS,
			LESS,
			LESS_EQUAL,
			GREATER,
			GREATER_EQUAL,
			EQUAL,
			NOT_EQUAL,
		};
	};

	struct RenderState
	{
		int CullMode;
		int BlendMode;
		int DepthMode;
		int DepthMask; // TRUE : Enable, FALSE: Disable

		RenderState()
		{
			CullMode = eCullMode::BACK;
			BlendMode = eBlendMode::OPACITY;
			DepthMode = eDepthMode::LESS_EQUAL;
			DepthMask = TRUE;
		}
	};

	struct Material
	{
		Float3 Emissive; // 自发光
		Float3 Ambient;  // 环境光
		Float3 Diffuse;  // 漫反射
		Float3 Specular; // 镜面反射
		float SpecularPower; // 镜面反射强度

		Material()
		{
			Emissive = Float3(0, 0, 0);
			Ambient = Float3(1, 1, 1);
			Diffuse = Float3(1, 1, 1);
			Specular = Float3(0, 0, 0);
			SpecularPower = 40;
		}
	};

	struct RenderOp
	{
		VertexLayout vlayout;
		IRenderBuffer::Ptr vbuffer;
		IRenderBuffer::Ptr ibuffer;
		int primCount;
		int primType;

		RenderOp() : primCount(0), primType(ePrimType::TRI_LIST) {}
	};

}