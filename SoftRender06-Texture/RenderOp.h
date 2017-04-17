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

	struct Light
	{
		enum {
			POINT,
			SPOT,
			DIRECTION,
		};

		int Type;
		Float3 Diffuse;
		Float3 Specular;
		Float3 Position;
		Float3 Direction;
		float AttenStart, AttenEnd, AttenFallOff;
		float SpotInner, SpotOuter, SpotFallOff;

		Light()
		{
			Type = POINT;
			Diffuse = Float3(1, 1, 1);
			Specular = Float3(0, 0, 0);
			Position = Float3(0, 0, 0);
			Direction = Float3(0, 0, 1);
			AttenStart = 0, AttenEnd = 1, AttenFallOff = 1;
			SpotInner = 0, SpotOuter = 90, SpotFallOff = 1;
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