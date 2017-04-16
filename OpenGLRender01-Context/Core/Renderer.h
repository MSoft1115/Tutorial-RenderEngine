#pragma once

#include "XCore.h"

namespace X {

	class IRenderer
	{
	public:
		IRenderer();
		virtual ~IRenderer();

		virtual void
			Resize(int width, int height) = 0;
		
		virtual void
			Clear(const Float4 & color, float depth = 1.0f) = 0;
		
		virtual void
			Begin() = 0;
		virtual void
			End() = 0;
		virtual void
			Present() = 0;
	};

}