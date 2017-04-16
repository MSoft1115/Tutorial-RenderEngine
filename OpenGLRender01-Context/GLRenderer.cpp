#include "GLRenderer.h"
#include "GLContext.h"
#include "GLUtil.h"

namespace X {

	GLRenderer::GLRenderer(HWND hWnd, int w, int h)
	{
		mContext = new GLContext(hWnd, w, h);
		mWidth = w, mHeight = h;
	}

	GLRenderer::~GLRenderer()
	{
	}

	void GLRenderer::Resize(int w, int h)
	{
		d_assert(w > 1 && h > 1);

		mContext->Resize(w, h);

		mWidth = w;
		mHeight = h;

		glViewport(0, 0, w, h);
	}

	void GLRenderer::Clear(const Float4 & color, float depth)
	{
		glColorMask(true, true, true, true);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glStencilMask(GL_TRUE);

		glClearColor(color.r, color.g, color.b, color.a);
		glClearDepth(depth);
		glClearStencil(0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void GLRenderer::Begin()
	{
	}

	void GLRenderer::End()
	{
		glFinish();
	}

	void GLRenderer::Present()
	{
		mContext->Present();
	}

}

