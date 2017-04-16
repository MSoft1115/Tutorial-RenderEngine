#include "Shader.h"

namespace X {

	ShaderUniform::ShaderUniform()
		: type(FLOAT4)
		, count(1)
		, data(NULL)
	{
	}
	ShaderUniform::~ShaderUniform()
	{
		safe_delete_array(data);
	}

	void ShaderUniform::SetData(const Float4 & v)
	{
		d_assert(type <= FLOAT4);

		*(Float4 *)data = v;
	}

	void ShaderUniform::SetData(const Mat4 & v)
	{
		d_assert(type == MATRIX4);

		*(Mat4 *)data = v;
	}

	void ShaderUniform::SetData(const Float4 * v, int _count)
	{
		d_assert(type == FLOAT4 && count >= _count);

		Float4 * d = (Float4 *)data;
		for (int i = 0; i < _count; ++i)
		{
			*d++ = *v++;
		}
	}

	//
	Shader::Shader()
	{
	}

	Shader::~Shader()
	{
	}

	int Shader::GetUniformIndex(const String & name)
	{
		for (int i = 0; i < (int)mUniforms.size(); ++i)
		{
			if (mUniforms[i]->name == name)
				return 1;
		}

		return -1;
	}

	ShaderUniform * Shader::GetUniform(const String & name)
	{
		for (int i = 0; i < (int)mUniforms.size(); ++i)
		{
			if (mUniforms[i]->name == name)
				return mUniforms[i];
		}

		return NULL;
	}

	ShaderUniform * Shader::GetUniform(int i)
	{
		return mUniforms[i];
	}

	int Shader::GetUniformCount() const
	{
		return (int)mUniforms.size();
	}

	void Shader::SetUniform(int index, float x, float y, float z, float w)
	{ 
		SetUniform(index, Float4(x, y, z, w)); 
	}

	void Shader::SetUniform(int index, const Float4 * data, int count)
	{ 
		mUniforms[index]->SetData(data, count); 
	}

	void Shader::SetUniform(int index, const Float4 & data) 
	{ 
		mUniforms[index]->SetData(data); 
	}

	void Shader::SetUniform(int index, const Mat4 & data) 
	{ 
		mUniforms[index]->SetData(data);
	}

	bool Shader::SetUniform(const String & name, float x, float y, float z, float w)
	{
		return SetUniform(name, Float4(x, y, z, w));
	}

	bool Shader::SetUniform(const String & name, const Float4 * data, int count)
	{
		ShaderUniform * u = GetUniform(name);
		if (u != NULL)
		{
			u->SetData(data, count);
			return true;
		}

		return false;
	}

	bool Shader::SetUniform(const String & name, const Float4 & data)
	{
		ShaderUniform * u = GetUniform(name);
		if (u != NULL)
		{
			u->SetData(data);
			return true;
		}

		return false;
	}

	bool Shader::SetUniform(const String & name, const Mat4 & data)
	{
		ShaderUniform * u = GetUniform(name);
		if (u != NULL)
		{
			u->SetData(data);
			return true;
		}

		return false;
	}

}