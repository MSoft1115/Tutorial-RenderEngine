#pragma once

#include "stdafx.h"
#include "RenderOp.h"

namespace X {

	class SoftRendererContext;

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
			SetRenderState(const RenderState & rstate);

		virtual void
			Render(RenderOp * rop);

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
		std::vector<RasterizerVertex> mRasterizerVertexBuffer;

		RenderState mRenderState;
	};

}
