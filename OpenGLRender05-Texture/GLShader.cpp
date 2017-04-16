#include "GLShader.h"
#include "Renderer.h"
#include "GLTexture.h"

namespace X {

	GLShader::GLShader(DataStream::Ptr stream)
	{
		for (int i = 0; i < (int)mAttribs.size(); ++i)
			mAttribs[i] = -1;

		_load(stream);
	}

	GLShader::~GLShader()
	{
		for (int i = 0; i < (int)mUniforms.size(); ++i)
		{
			delete mUniforms[i];
		}
	}

	void GLShader::UploadUniform()
	{
		IRenderer * r = IRenderer::Instance();
		for (int i = 0; i < (int)mUniforms.size(); ++i)
		{
			GLShaderUniform * u = (GLShaderUniform *)mUniforms[i];
			if (u->handle == -1)
				continue;

			switch (u->type)
			{
				case ShaderUniform::FLOAT1:
					glUniform1fv(u->handle, u->count, (const float *)u->data);
					break;

				case ShaderUniform::FLOAT2:
					glUniform2fv(u->handle, u->count, (const float *)u->data);
					break;

				case ShaderUniform::FLOAT3:
					glUniform3fv(u->handle, u->count, (const float *)u->data);
					break;

				case ShaderUniform::FLOAT4:
					glUniform4fv(u->handle, u->count, (const float *)u->data);
					break;

				case ShaderUniform::MATRIX4:
					glUniformMatrix4fv(u->handle, u->count, GL_FALSE, (const GLfloat *)u->data);
					break;

				case ShaderUniform::WORLD_MATRIX:
					glUniformMatrix4fv(u->handle, 1, GL_FALSE, (const float *)&r->GetWorldMatrix());
					break;

				case ShaderUniform::VIEW_MATRIX:
					glUniformMatrix4fv(u->handle, 1, GL_FALSE, (const float *)&r->GetViewMatrix());
					break;

				case ShaderUniform::PROJ_MATRIX:
					glUniformMatrix4fv(u->handle, 1, GL_FALSE, (const float *)&r->GetProjMatrix());
					break;
			}

			d_assert(glGetError() == 0);
		}
	}

	void GLShader::UploadSampler()
	{
		for (int i = 0; i < (int)mSamplers.size(); ++i)
		{
			GLShaderSampler * s = (GLShaderSampler *)mSamplers[i];
			if (s->handle == -1)
				continue;

			GLTexture * texture = (GLTexture *)IRenderer::Instance()->GetSamplerTexture(s->index);

			glUniform1i(s->handle, s->index);

			glActiveTexture(GL_TEXTURE0 + s->index);

			if (texture)
				glBindTexture(GL_TEXTURE_2D, texture->GetGLTexture());
			else
				glBindTexture(GL_TEXTURE_2D, 0);

			int mipmapLevel = texture ? texture->GetMipmaps() : 0;
			if (mipmapLevel > 0)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmapLevel);

				switch (s->filter)
				{
					case ShaderSampler::TF_LINEAR:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						break;

					case ShaderSampler::TF_NEAREST:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						break;
				}
			}
			else
			{
				switch (s->filter)
				{
					case ShaderSampler::TF_LINEAR:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						break;

					case ShaderSampler::TF_NEAREST:
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						break;
				}
			}

			switch (s->addressMode)
			{
				case ShaderSampler::TA_WRAP:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					break;

				case ShaderSampler::TA_CLAMP:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					break;

				case ShaderSampler::TA_BORAD:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
					glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,s->borderColor.m);
					break;
			}
		}
	}

	void GLShader::_load(DataStream::Ptr stream)
	{
		Rad::rml_document rdoc;
		rdoc.parse((char *)stream->GetData());

		// load uniform
		Rad::rml_node * node = rdoc.first_node("Uniform");
		if (node != NULL)
		{
			Rad::rml_node * child = node->first_node();
			while (child != NULL)
			{
				_loadUniform(child);
				child = child->next_sibling();
			}
		}

		// load shader
		Rad::rml_node * nodeVS = rdoc.first_node("VertexShader");
		Rad::rml_node * nodePS = rdoc.first_node("PixelShader");
		if (nodeVS != NULL && nodePS != NULL)
		{
			_build(nodeVS->value(), nodePS->value());
		}
	}

	const char * _skipwhite(const char * str)
	{
		while (*str == ' ' || *str == '\t')
			++str;

		return str;
	}

	const char * _substring(char * buffer, int maxcount, const char * str, char end = ' ')
	{
		str = _skipwhite(str);

		int n = 0;
		while (n < maxcount && *str)
		{
			if (*str == end)
				break;

			buffer[n++] = *str++;
		}

		buffer[n] = 0;

		return str;
	}

	int _getUniformType(const String & str)
	{
		if (str == "FLOAT1")
			return ShaderUniform::FLOAT1;
		else if (str == "FLOAT2")
			return ShaderUniform::FLOAT2;
		else if (str == "FLOAT3")
			return ShaderUniform::FLOAT3;
		else if (str == "FLOAT4")
			return ShaderUniform::FLOAT4;
		else if (str == "MATRIX4")
			return ShaderUniform::FLOAT4;
		else if (str == "WORLD_MATRIX")
			return ShaderUniform::WORLD_MATRIX;
		else if (str == "VIEW_MATRIX")
			return ShaderUniform::VIEW_MATRIX;
		else if (str == "PROJ_MATRIX")
			return ShaderUniform::PROJ_MATRIX;

		d_assert(0);

		return ShaderUniform::FLOAT4;
	}

	void GLShader::_loadUniform(Rad::rml_node * node)
	{
		GLShaderUniform * u = new GLShaderUniform;
		u->name = node->name();

		char temp[128];
		const char * str = node->value();

		if (*str)
		{
			str = _substring(temp, sizeof(temp), str);
			u->type = _getUniformType(temp);

			str = _skipwhite(str);
		}

		switch (u->type)
		{
			case ShaderUniform::FLOAT1:
			case ShaderUniform::FLOAT2:
			case ShaderUniform::FLOAT3:
			case ShaderUniform::FLOAT4:
				u->data = new byte[sizeof(Float4) * u->count];
				memset(u->data, 0, sizeof(Float4) * u->count);
				break;

			case ShaderUniform::MATRIX4:
				u->data = new byte[sizeof(Mat4) * u->count];
				memset(u->data, 0, sizeof(Mat4) * u->count);
				break;
		}

		mUniforms.push_back(u);
	}

	inline int _getTexFilter(const char * str)
	{
		if (strcmp(str, "LINEAR") == 0)
			return ShaderSampler::TF_LINEAR;
		else if (strcmp(str, "NEAREST") == 0)
			return ShaderSampler::TF_NEAREST;

		return ShaderSampler::TF_LINEAR;
	}

	inline int _getTexAddressMode(const char * str)
	{
		if (strcmp(str, "WRAP") == 0)
			return ShaderSampler::TA_WRAP;
		else if (strcmp(str, "CLAMP") == 0)
			return ShaderSampler::TA_CLAMP;
		else if (strcmp(str, "BORAD") == 0)
			return ShaderSampler::TA_BORAD;

		return ShaderSampler::TA_WRAP;
	}

	static const char * _getFloat(float & v, const char * str)
	{
		char buffer[128];

		str = _substring(buffer, 128, str);

		v = (float)atof(buffer);

		return str;
	}

	inline Float4 _getFloat4(const char * str)
	{
		float r = 0, g = 0, b = 0, a = 0;

		str = _getFloat(r, str);
		str = _getFloat(g, str);
		str = _getFloat(b, str);
		str = _getFloat(a, str);

		return Float4(r, g, b, a);
	}

	void GLShader::_loadSampler(Rad::rml_node * node)
	{
		GLShaderSampler * sampler = new GLShaderSampler;
		sampler->index = (int)mSamplers.size();

		Rad::rml_node * child = node->first_node("Name");
		if (child)
		{
			sampler->name = child->value();
		}

		child = node->first_node("Index");
		if (child != NULL)
		{
			sampler->index = atoi(child->value());
		}

		child = node->first_node("Filter");
		if (child)
		{
			sampler->filter = _getTexFilter(child->value());
		}

		child = node->first_node("AddressMode");
		if (child == NULL) child = node->first_node("Address");
		if (child)
		{
			sampler->addressMode = _getTexAddressMode(child->value());
		}

		child = node->first_node("BorderColor");
		if (child)
		{
			sampler->borderColor = _getFloat4(child->value());
		}

		mSamplers.push_back(sampler);
	}

	void GLShader::_build(const char * vscode, const char * pscode)
	{
		GLuint vertexShader = 0, pixelShader = 0;
		GLint success = 1;
		int vslen = strlen(vscode);
		int pslen = strlen(pscode);

		// create vertex shader
		if (success)
		{
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, (const char **)&vscode, &vslen);
			glCompileShader(vertexShader);
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				char errorLog[1024];
				glGetShaderInfoLog(vertexShader, sizeof(errorLog), NULL, errorLog);

				d_assert(0);
			}
		}

		// create pixel shader
		if (success)
		{
			pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(pixelShader, 1, (const char **)&pscode, &pslen);
			glCompileShader(pixelShader);
			glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				char errorLog[1024];
				glGetShaderInfoLog(pixelShader, sizeof(errorLog), NULL, errorLog);

				d_assert(0);
			}
		}

		// create program
		if (success)
		{
			mGLProgram = glCreateProgram();

			glAttachShader(mGLProgram, vertexShader);
			glAttachShader(mGLProgram, pixelShader);

			glLinkProgram(mGLProgram);

			glGetProgramiv(mGLProgram, GL_LINK_STATUS, &success);
			if (!success)
			{
				char errorLog[1024];
				glGetProgramInfoLog(mGLProgram, sizeof(errorLog), NULL, errorLog);

				glDeleteProgram(mGLProgram);
				mGLProgram = 0;

				d_assert(0);
			}
		}

		if (vertexShader)
			glDeleteShader(vertexShader);

		if (pixelShader)
			glDeleteShader(pixelShader);

		d_assert(glGetError() == 0);

		// 
		if (success)
		{
			// 通过名字索引顶点语义
			int nattribs = 0;
			glGetProgramiv(mGLProgram, GL_ACTIVE_ATTRIBUTES, &nattribs);
			for (int i = 0; i < nattribs; ++i)
			{
				const int MAX_LENGTH = 64;
				char name[MAX_LENGTH];
				int actual_length = 0;
				int size = 0;
				GLenum type;

				glGetActiveAttrib(mGLProgram, i, MAX_LENGTH, &actual_length, &size, &type, name);

				if (strcmp(name, "POSITION") == 0)
					mAttribs[eVertexSemantic::POSITION] = glGetAttribLocation(mGLProgram, name);
				else if (strcmp(name, "NORMAL") == 0)
					mAttribs[eVertexSemantic::NORMAL] = glGetAttribLocation(mGLProgram, name);
				else if (strcmp(name, "COLOR") == 0)
					mAttribs[eVertexSemantic::COLOR] = glGetAttribLocation(mGLProgram, name);
				else if (strcmp(name, "BONE_INDEX") == 0)
					mAttribs[eVertexSemantic::BONE_INDEX] = glGetAttribLocation(mGLProgram, name);
				else if (strcmp(name, "BONE_WEIGHT") == 0)
					mAttribs[eVertexSemantic::BONE_WEIGHT] = glGetAttribLocation(mGLProgram, name);
				else if (strcmp(name, "TEXCOORD0") == 0)
					mAttribs[eVertexSemantic::TEXCOORD0] = glGetAttribLocation(mGLProgram, name);
				else if (strcmp(name, "TEXCOORD1") == 0)
					mAttribs[eVertexSemantic::TEXCOORD1] = glGetAttribLocation(mGLProgram, name);
				else if (strcmp(name, "TEXCOORD2") == 0)
					mAttribs[eVertexSemantic::TEXCOORD2] = glGetAttribLocation(mGLProgram, name);
				else if (strcmp(name, "TEXCOORD3") == 0)
					mAttribs[eVertexSemantic::TEXCOORD3] = glGetAttribLocation(mGLProgram, name);
			}

			// 查找所有Uniform
			for (int i = 0; i < (int)mUniforms.size(); ++i)
			{
				GLShaderUniform * u = (GLShaderUniform *)mUniforms[i];

				u->handle = glGetUniformLocation(mGLProgram, u->name.c_str());
				if (u->handle != -1)
				{
					if (u->data != NULL && u->type != GLShaderUniform::MATRIX4)
					{
						float defaultValue[4] = { 0 };
						glGetUniformfv(mGLProgram, u->handle, defaultValue);

						memcpy(u->data, defaultValue, 4 * sizeof(float));
					}
				}
			}

			// 查找所有Sampler
			for (int i = 0; i < (int)mSamplers.size(); ++i)
			{
				GLShaderSampler * s = (GLShaderSampler *)mSamplers[i];

				s->handle = glGetUniformLocation(mGLProgram, s->name.c_str());
			}
		}

		d_assert(success);
	}

}