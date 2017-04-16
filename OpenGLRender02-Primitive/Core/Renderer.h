#pragma once

#include "XCore.h"
#include "Buffer.h"
#include "Shader.h"
#include "RenderOp.h"

namespace X {

	class IRenderer : public Singleton<IRenderer>
	{
	public:
		IRenderer();
		virtual ~IRenderer();

		virtual IRenderBuffer::Ptr
			CreateBuffer(int size) = 0;
		virtual Shader::Ptr
			LoadShader(const String & source) = 0;

		virtual void
			Resize(int width, int height) = 0;
		
		virtual void
			Clear(const Float4 & color, float depth = 1.0f) = 0;

		void
			SetWorldMatrix(const Mat4 & m);
		void
			SetViewMatrix(const Mat4 & m);
		void
			SetProjMatrix(const Mat4 & m);
		void
			SetCamera(const Float3 & pos, const Float3 & dir);
		void
			SetMaterial(const Material & m);
		void
			SetRenderState(const RenderState & rstate);
		void
			SetShader(Shader::Ptr shader);

		const Mat4 &
			GetWorldMatrix() { return mWorldMatrix; }
		const Mat4 &
			GetViewMatrix() { return mViewMatrix; }
		const Mat4 &
			GetProjMatrix() { return mProjMatrix; }
		const Float4 &
			GetCameraPos() { return mCameraPos; }
		const Float4 &
			GetCameraDir() { return mCameraDir; }
		const Material &
			GetMaterial() { return mMaterial; }
		const RenderState &
			GetRenderState() { return mRenderState; }
		
		virtual void
			Begin() = 0;
		virtual void
			End() = 0;
		virtual void
			Render(RenderOp * rop) = 0;
		virtual void
			Present() = 0;

	protected:
		Mat4 mWorldMatrix;
		Mat4 mViewMatrix;
		Mat4 mProjMatrix;
		Float4 mCameraPos;
		Float4 mCameraDir;
		Material mMaterial;
		RenderState mRenderState;
		Shader::Ptr mShader;
	};

}