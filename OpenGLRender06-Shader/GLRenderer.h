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

		virtual IRenderBuffer::Ptr
			CreateBuffer(int size);
		virtual Shader::Ptr
			LoadShader(const String & source);
		virtual Texture::Ptr
			LoadTexture(const String & source);
		virtual Texture::Ptr
			CreateTexture(const String & name, int format, int w, int h, int mipmaps);

		virtual void 
			Resize(int width, int height);

		virtual void
			Clear(const Float4 & color, float depth = 1.0f);

		virtual void
			Begin();
		virtual void
			End();
		virtual void
			Render(RenderOp * rop);
		virtual void
			Present();
	
	protected:
		GLContext * mContext;
		int mWidth, mHeight;
	};

}
