#pragma once

#include "XCore.h"

namespace X {

	struct Vertex;
	struct RasterizerVertex;
	class SoftRenderer;

	struct Shader
	{
		virtual ~Shader() {}

		virtual void VertexShader(RasterizerVertex * vo, const Vertex * vi) = 0;
		virtual bool PixelShader(RasterizerVertex * vio) = 0;
	};

}