#pragma once

#include "stdafx.h"
#include "XCore.h"

namespace X {

	class SoftRendererContext;

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

	class SoftRenderer
	{
	public:
		SoftRenderer(HWND hWnd, int w, int h);
		~SoftRenderer();

		virtual void 
			Resize(int width, int height);
		virtual void
			Clear(const Float3 & color = Float3(0, 0, 0), float depth = 1.0f);

		virtual void
			Render(const Vertex * vertexBuffer,int vertexCount, const int * iarray, int primCount, int primType);

		virtual void
			Begin();
		virtual void
			End();
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
			_pixel_shader(RasterizerVertex * vio);

		void 
			_output(int x, int y, const RasterizerVertex * vi);

	protected:
		SoftRendererContext * mContext;
		int mWidth, mHeight;
		Float3 * mColorBuffer;
		float * mDepthBuffer;

		std::vector<RasterizerVertex> mRasterizerVertexBuffer;
	};

}
