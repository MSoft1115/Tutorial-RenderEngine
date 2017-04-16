#pragma once

#include "MRml.h"
#include "Shader.h"
#include "Buffer.h"
#include "DataStream.h"
#include "GLUtil.h"

namespace X {

	struct GLShaderUniform : public ShaderUniform
	{
		GLint handle;

		GLShaderUniform() : handle(-1) {}
		virtual ~GLShaderUniform() {}
	};

	struct GLShaderSampler : public ShaderSampler
	{
		GLint handle;

		GLShaderSampler() : handle(-1) {}
		virtual ~GLShaderSampler() {}
	};

	class GLShader : public Shader
	{
	public:
		GLShader(DataStream::Ptr stream);
		virtual ~GLShader();

		virtual void
			Release() { delete this;  }

		void
			UploadUniform();
		void
			UploadSampler();

		GLuint
			GetGLShaderProgram() { return mGLProgram; }
		GLint
			GetAttrib(int i) { return mAttribs[i];  }

	protected:
		void 
			_load(DataStream::Ptr stream);
		void
			_loadUniform(Rad::rml_node * node);
		void
			_loadSampler(Rad::rml_node * node);
		void
			_build(const char * vscode, const char * pscode);

	protected:
		GLuint mGLProgram;
		std::array<GLint, eVertexSemantic::MAX> mAttribs;
	};

}