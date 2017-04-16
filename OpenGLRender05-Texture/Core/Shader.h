#pragma once

#include "Float4.h"
#include "Mat4.h"
#include "SharedPtr.h"

namespace X {

	struct ShaderUniform
	{
		enum eType {
			FLOAT1,
			FLOAT2,
			FLOAT3,
			FLOAT4,
			MATRIX4,

			// 矩阵参数
			WORLD_MATRIX,
			VIEW_MATRIX,
			PROJ_MATRIX,
		};

		String name;	// 名字
		int type;		// 类型
		int	count;		// 数量
		byte * data;	// 缓存数据

		ShaderUniform();
		virtual ~ShaderUniform();

		void 
			SetData(const Float4 & v);
		void 
			SetData(const Mat4 & v);
		void 
			SetData(const Float4 * v, int _count);
	};

	struct ShaderSampler
	{
		enum eTexFilter
		{
			TF_NEAREST,
			TF_LINEAR,
		};

		enum eTexAddressMode
		{
			TA_WRAP,
			TA_CLAMP,
			TA_BORAD,
		};

		int	index;
		String name;
		int filter;
		int addressMode;
		Float4 borderColor;

		ShaderSampler()
		{
			index = 0;
			filter = TF_LINEAR;
			addressMode = TA_WRAP;
			borderColor = Float4(0, 0, 0, 0);
		}
	};

	class Shader
	{
		DECLARE_REF_COUNTER();

	public:
		typedef SharedPtr<Shader> Ptr;

	public:
		Shader();
		virtual ~Shader();

		virtual void
			Release() = 0;

		int
			GetUniformIndex(const String & name);
		ShaderUniform *
			GetUniform(const String & name);
		ShaderUniform *
			GetUniform(int i);
		int
			GetUniformCount() const;

		void
			SetUniform(int index, float x, float y, float z, float w = 1);
		void
			SetUniform(int index, const Float4 * data, int count);
		void
			SetUniform(int index, const Float4 & data);
		void
			SetUniform(int index, const Mat4 & data);

		bool
			SetUniform(const String & name, float x, float y, float z, float w = 1);
		bool
			SetUniform(const String & name, const Float4 * data, int count);
		bool
			SetUniform(const String & name, const Float4 & data);
		bool
			SetUniform(const String & name, const Mat4 & data);

	protected:
		std::vector<ShaderUniform *> mUniforms;
		std::vector<ShaderSampler *> mSamplers;
	};

}