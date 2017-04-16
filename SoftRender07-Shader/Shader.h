#pragma once

#include "XCore.h"

namespace X {

	struct Vertex;
	struct RasterizerVertex;
	class SoftRenderer;

	typedef void (*FN_VertexShader)(RasterizerVertex * vo, const Vertex * vi, SoftRenderer * renderer);
	typedef bool(*FN_PixelShader)(RasterizerVertex * vio, SoftRenderer * renderer);

	struct Shader
	{
		FN_VertexShader pfn_vs;
		FN_PixelShader pfn_ps;

		Shader()
		{
			pfn_vs = NULL;
			pfn_ps = NULL;
		}
	};

}