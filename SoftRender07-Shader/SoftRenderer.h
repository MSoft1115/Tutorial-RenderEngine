#pragma once

#include "stdafx.h"
#include "XCore.h"
#include "Texture.h"
#include "Shader.h"

namespace X {

	class SoftRendererContext;

	// 顶点结构
	//
	struct Vertex
	{
		Float3 position;	// 位置
		Float3 normal;		// 法线
		Float4 color;		// 颜色
		Float2 uv;			// 纹理坐标
	};

	struct RasterizerVertex
	{
		Float4 position;	// 位置
		Float3 normal;		// 法线
		Float4 color;		// 颜色
		Float2 uv;			// 纹理坐标

		static void Lerp(RasterizerVertex & v, const RasterizerVertex & a, const RasterizerVertex & b, float k)
		{
			v.position = a.position + (b.position - a.position) * k;
			v.normal = a.normal + (b.normal - a.normal) * k;
			v.color = a.color + (b.color - a.color) * k;
			v.uv = a.uv + (b.uv - a.uv) * k;
		}
	};

	struct ePrimType
	{
		enum {
			POINT_LIST,
			LINE_LIST,
			TRI_LIST,
		};
	};

	struct eCullMode
	{
		enum {
			NONE,
			FRONT,
			BACK,
		};
	};

	struct eBlendMode
	{
		enum {
			OPACITY,
			ADD,
			//ALPHA_TEST,
			ALPHA_BLEND,
		};
	};

	struct eDepthMode
	{
		enum {
			ALWAYS,
			LESS,
			LESS_EQUAL,
			GREATER,
			GREATER_EQUAL,
			EQUAL,
			NOT_EQUAL,
		};
	};

	struct RenderState
	{
		int CullMode;
		int BlendMode;
		int DepthMode;
		int DepthMask; // TRUE : Enable, FALSE: Disable

		RenderState()
		{
			CullMode = eCullMode::BACK;
			BlendMode = eBlendMode::OPACITY;
			DepthMode = eDepthMode::LESS_EQUAL;
			DepthMask = TRUE;
		}
	};

	struct Light
	{
		enum {
			POINT,
			SPOT,
			DIRECTION,
		};

		int Type;
		Float3 Diffuse;
		Float3 Specular;
		Float3 Position;
		Float3 Direction;
		float AttenStart, AttenEnd, AttenFallOff;
		float SpotInner, SpotOuter, SpotFallOff;

		Light()
		{
			Type = POINT;
			Diffuse = Float3(1, 1, 1);
			Specular = Float3(0, 0, 0);
			Position = Float3(0, 0, 0);
			Direction = Float3(0, 0, 1);
			AttenStart = 0, AttenEnd = 1, AttenFallOff = 1;
			SpotInner = 0, SpotOuter = 90, SpotFallOff = 1;
		}
	};

	struct Material
	{
		Float3 Emissive; // 自发光
		Float3 Ambient;  // 环境光
		Float3 Diffuse;  // 漫反射
		Float3 Specular; // 镜面反射
		float SpecularPower; // 镜面反射强度

		Material()
		{
			Emissive = Float3(0, 0, 0);
			Ambient = Float3(1, 1, 1);
			Diffuse = Float3(1, 1, 1);
			Specular = Float3(0, 0, 0);
			SpecularPower = 40;
		}
	};

	class SoftRenderer
	{
	public:
		SoftRenderer(HWND hWnd, int w, int h);
		~SoftRenderer();

		virtual void 
			Resize(int width, int height);
		virtual void
			Clear(const Float3 & color = Float3(0, 0, 0), float depth = 1.0f);

		void
			SetWorldMatrix(const Mat4 & m);
		void
			SetViewMatrix(const Mat4 & m);
		void
			SetProjMatrix(const Mat4 & m);
		void
			SetCamera(const Float3 & pos, const Float3 & dir);
		void
			SetLight(Light * l);
		void
			SetMaterial(const Material & m);
		void
			SetTexture(Texture * t);
		void
			SetSampleState(const SamplerState & state);
		void
			SetRenderState(const RenderState & rstate);
		void
			SetShader(Shader * shader);

		const Mat4 &
			GetWorldMatrix() { return mWorldMatrix; }
		const Mat4 &
			GetViewMatrix() { return mViewMatrix; }
		const Mat4 &
			GetProjMatrix() { return mProjMatrix; }
		const Float3 &
			GetCameraPos() { return mCameraPos; }
		const Float3 &
			GetCameraDir() { return mCameraDir; }
		Light *
			GetLight() { return mLight; }
		const Material &
			GetMaterial() { return mMaterial; }
		Texture *
			GetTexture() { return mTexture; }
		const SamplerState &
			GetSamplerState() { return mSamplerState; }
		const RenderState &
			GetRenderState() { return mRenderState; }

		virtual void
			Begin();
		virtual void
			End();
		virtual void
			Render(const Vertex * vertexBuffer,int vertexCount, const int * iarray, int primCount, int primType);
		virtual void
			Present();
	
	protected:
		void
			_rasterizePoint(const RasterizerVertex * PA);
		void
			_rasterizeLine(const RasterizerVertex * PA, const RasterizerVertex * PB);
		void 
			_rasterizeTri(const RasterizerVertex * PA, const RasterizerVertex * PB, const RasterizerVertex * PC);
		
		void
			_vertex_shader(RasterizerVertex * vo, const Vertex * vi);
		void
			_lighting(RasterizerVertex * vio);
		bool
			_pixel_shader(RasterizerVertex * vio);

		bool
			_backface_cull(const RasterizerVertex * PA, const RasterizerVertex * PB, const RasterizerVertex * PC);
		bool
			_depth_test(float old_depth, float new_depth);

		void 
			_output(int x, int y, const RasterizerVertex * vi);

	protected:
		SoftRendererContext * mContext;
		int mWidth, mHeight;
		Float3 * mColorBuffer;
		float * mDepthBuffer;

		Mat4 mWorldMatrix;
		Mat4 mViewMatrix;
		Mat4 mProjMatrix;
		Float3 mCameraPos;
		Float3 mCameraDir;
		std::vector<RasterizerVertex> mRasterizerVertexBuffer;

		Light * mLight;
		Material mMaterial;
		Texture * mTexture;
		SamplerState mSamplerState;
		RenderState mRenderState;
		Shader * mShader;
	};

}
