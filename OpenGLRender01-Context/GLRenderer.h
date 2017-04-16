#pragma once

#include "Renderer.h"
#include "GLUtil.h"

namespace X {

	class GLContext;

	class GLRenderer : public IRenderer
	{
	public:
		GLRenderer(HWND hWnd, int w, int h);
		~GLRenderer();

		virtual void 
			Resize(int width, int height);

		virtual void
			Clear(const Float4 & color, float depth = 1.0f);

		virtual void
			Begin();
		virtual void
			End();
		virtual void
			Present();
	
	protected:
		GLContext * mContext;
		int mWidth, mHeight;
	};

}
