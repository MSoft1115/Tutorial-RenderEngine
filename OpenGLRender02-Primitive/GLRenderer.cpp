#include "GLRenderer.h"
#include "GLContext.h"
#include "GLUtil.h"
#include "GLBuffer.h"
#include "GLShader.h"
#include "ResourceManager.h"

namespace X {

	GLRenderer::GLRenderer(HWND hWnd, int w, int h)
	{
		mContext = new GLContext(hWnd, w, h);
		mWidth = w, mHeight = h;
	}

	GLRenderer::~GLRenderer()
	{
	}

	IRenderBuffer::Ptr GLRenderer::CreateBuffer(int size)
	{
		return new GLRenderBuffer(size);
	}

	Shader::Ptr GLRenderer::LoadShader(const String & source)
	{
		DataStream::Ptr stream = ResourceManager::Instance()->OpenResource(source);
		d_assert (stream != NULL);

		return new GLShader(stream);
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

#define VBO_BUFFER_OFFSET(buff, i) ((const char *)buff + (i))
	void GLRenderer::Render(RenderOp * rop)
	{
		if (rop->primCount == 0)
			return;

		// bind shader
		GLShader * shader = (GLShader *)mShader.c_ptr();
		glUseProgram(shader->GetGLShaderProgram());
		shader->UploadUniform();

		// bind render state
		switch (mRenderState.CullMode)
		{
			case eCullMode::NONE:
				glDisable(GL_CULL_FACE);
				break;

			case eCullMode::FRONT:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				break;

			case eCullMode::BACK:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
				break;
		}
		switch (mRenderState.DepthMode)
		{
			case eDepthMode::ALWAYS:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_ALWAYS);
				glDepthMask(GL_TRUE);
				break;

			case eDepthMode::LESS:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
				glDepthMask(GL_TRUE);
				break;

			case eDepthMode::LESS_EQUAL:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
				glDepthMask(GL_TRUE);
				break;

			case eDepthMode::GREATER:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_GREATER);
				glDepthMask(GL_TRUE);
				break;

			case eDepthMode::GREATER_EQUAL:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_GEQUAL);
				glDepthMask(GL_TRUE);
				break;

			case eDepthMode::EQUAL:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_EQUAL);
				glDepthMask(GL_TRUE);
				break;

			case eDepthMode::NOT_EQUAL:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_NOTEQUAL);
				glDepthMask(GL_TRUE);
				break;
		}
		switch (mRenderState.BlendMode)
		{
			case eBlendMode::OPACITY:
				glDisable(GL_BLEND);
				break;

			case eBlendMode::ALPHA_BLEND:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;

			case eBlendMode::ADD:
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				break;
		}

		d_assert(glGetError() == 0);

		GLenum GLprimType = GLUtil::GetGLPrimType(rop->primType);
		int drawCount = GLUtil::GetGLDrawCount(rop->primType, rop->primCount);
		int stride = rop->vlayout.stride();

		// bind vertex buffer
		GLRenderBuffer * pGLVertexBuffer = (GLRenderBuffer *)rop->vbuffer.c_ptr();
		glBindBuffer(GL_ARRAY_BUFFER, pGLVertexBuffer->GetGLBuffer());

		for (int i = 0; i < rop->vlayout.size(); ++i)
		{
			const VertexLayout::Element & elem = rop->vlayout[i];

			GLenum type = GLUtil::GetGLVertexFormat(elem.format);
			GLsizei size = GLUtil::GetGLVertexElemCount(elem.format);
			GLint handle = shader->GetAttrib(elem.semantic);
			const char * buffer = NULL;

			if (handle != -1)
			{
				glEnableVertexAttribArray(handle);
				glVertexAttribPointer(handle, size, type, GL_FALSE, stride, VBO_BUFFER_OFFSET(buffer, elem.offset));

				d_assert(glGetError() == 0);
			}
		}

		// draw
		if (rop->ibuffer != NULL)
		{
			GLRenderBuffer * pGLIndexBuffer = (GLRenderBuffer *)rop->ibuffer.c_ptr();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pGLIndexBuffer->GetGLBuffer());
			d_assert(glGetError() == 0);

			glDrawElements(GLprimType, drawCount, GL_UNSIGNED_SHORT, NULL);
		}
		else
		{
			glDrawArrays(GLprimType, 0, drawCount);
		}

		d_assert(glGetError() == 0);

		for (int i = 0; i < eVertexSemantic::MAX; ++i)
		{
			glDisableVertexAttribArray(i);
		}
	}

	void GLRenderer::Present()
	{
		mContext->Present();
	}

}

