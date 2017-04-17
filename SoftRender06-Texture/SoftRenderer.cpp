#include "SoftRenderer.h"
#include "SoftRendererContext.h"

namespace X {

	SoftRenderer::SoftRenderer(HWND hWnd, int w, int h)
	{
		IMG_Reg(BMP_Test, BMP_Load);
		IMG_Reg(PNG_Test, PNG_Load);

		mContext = new SoftRendererContext(hWnd, w, h);

		mWidth = w, mHeight = h;
		mColorBuffer = new Float3[mWidth * mHeight];
		mDepthBuffer = new float[mWidth * mHeight];

		mWorldMatrix = Mat4::Identity;
		mViewMatrix = Mat4::Identity;
		mProjMatrix = Mat4::Identity;

		mLight = NULL;
		mTexture = NULL;
	}

	SoftRenderer::~SoftRenderer()
	{
		delete[] mColorBuffer;
		delete[] mDepthBuffer;
	}

	void SoftRenderer::Resize(int w, int h)
	{
		d_assert(w > 1 && h > 1);

		mContext->Resize(w, h);

		delete[] mColorBuffer;
		delete[] mDepthBuffer;

		mWidth = w;
		mHeight = h;

		mColorBuffer = new Float3[mWidth * mHeight];
		mDepthBuffer = new float[mWidth * mHeight];
	}

	void SoftRenderer::SetWorldMatrix(const Mat4 & m)
	{
		mWorldMatrix = m;
	}

	void SoftRenderer::SetViewMatrix(const Mat4 & m)
	{
		mViewMatrix = m;
	}

	void SoftRenderer::SetProjMatrix(const Mat4 & m)
	{
		mProjMatrix = m;
	}

	void SoftRenderer::SetCamera(const Float3 & pos, const Float3 & dir)
	{
		mCameraPos = pos;
		mCameraDir = dir;
	}

	void SoftRenderer::SetLight(Light * l)
	{
		mLight = l;
	}

	void SoftRenderer::SetMaterial(const Material & m)
	{
		mMaterial = m;
	}

	void SoftRenderer::SetTexture(Texture * t)
	{
		mTexture = t;
	}

	void SoftRenderer::SetSampleState(const SamplerState & state)
	{
		mSamplerState = state;
	}

	void SoftRenderer::SetRenderState(const RenderState & rstate)
	{
		mRenderState = rstate;
	}

	void SoftRenderer::Clear(const Float3 & color, float depth)
	{
		if (color == Float3::Zero)
		{
			memset(mColorBuffer, 0, mWidth * mHeight * 12);
		}
		else
		{
			for (int i = 0; i < mWidth * mHeight; ++i)
				mColorBuffer[i] = color;
		}

		for (int i = 0; i < mWidth * mHeight; ++i)
			mDepthBuffer[i] = depth;
	}

	void SoftRenderer::Render(RenderOp * rop)
	{
		// 顶点处理
		mRasterizerVertexBuffer.resize(rop->vbuffer.size());
		for (int i = 0; i < (int)rop->vbuffer.size(); ++i)
		{
			_vertex_shader(&mRasterizerVertexBuffer[i], &rop->vbuffer[i]);
		}

		// 图元处理
		RasterizerVertex A, B, C, D;
		for (int i = 0; i < rop->primCount; ++i)
		{
			if (rop->primType == ePrimType::TRI_LIST)
			{
				if (rop->ibuffer.size() > 0)
				{
					// 使用索引列表
					A = mRasterizerVertexBuffer[rop->ibuffer[i * 3 + 0]];
					B = mRasterizerVertexBuffer[rop->ibuffer[i * 3 + 1]];
					C = mRasterizerVertexBuffer[rop->ibuffer[i * 3 + 2]];
				}
				else
				{
					// 使用顶点列表
					A = mRasterizerVertexBuffer[i * 3 + 0];
					B = mRasterizerVertexBuffer[i * 3 + 1];
					C = mRasterizerVertexBuffer[i * 3 + 2];
				}

				// 背面裁剪
				if (_backface_cull(&A, &B, &C))
					continue;

				// 裁剪

				// 视口变换
				A.position.x /= A.position.w;
				A.position.y /= A.position.w;
				B.position.x /= B.position.w;
				B.position.y /= B.position.w;
				C.position.x /= C.position.w;
				C.position.y /= C.position.w;

				A.position.x = (A.position.x + 1) / 2 * mWidth;
				A.position.y = (1 - A.position.y) / 2 * mHeight;
				B.position.x = (B.position.x + 1) / 2 * mWidth;
				B.position.y = (1 - B.position.y) / 2 * mHeight;
				C.position.x = (C.position.x + 1) / 2 * mWidth;
				C.position.y = (1 - C.position.y) / 2 * mHeight;

				// 光栅化
				_rasterizeTri(&A, &B, &C);
			}
			else if (rop->primType == ePrimType::LINE_LIST)
			{
				if (rop->ibuffer.size() > 0)
				{
					// 使用索引列表
					A = mRasterizerVertexBuffer[rop->ibuffer[i * 2 + 0]];
					B = mRasterizerVertexBuffer[rop->ibuffer[i * 2 + 1]];
				}
				else
				{
					// 使用顶点列表
					A = mRasterizerVertexBuffer[i * 2 + 0];
					B = mRasterizerVertexBuffer[i * 2 + 1];
				}

				// 裁剪

				// 视口变换
				A.position.x /= A.position.w;
				A.position.y /= A.position.w;
				B.position.x /= B.position.w;
				B.position.y /= B.position.w;

				A.position.x = (A.position.x + 1) / 2 * mWidth;
				A.position.y = (1 - A.position.y) / 2 * mHeight;
				B.position.x = (B.position.x + 1) / 2 * mWidth;
				B.position.y = (1 - B.position.y) / 2 * mHeight;

				// 光栅化
				_rasterizeLine(&A, &B);
			}
			else if (rop->primType == ePrimType::POINT_LIST)
			{
				if (rop->ibuffer.size() > 0)
				{
					// 使用索引列表
					A = mRasterizerVertexBuffer[rop->ibuffer[i]];
				}
				else
				{
					// 使用顶点列表
					A = mRasterizerVertexBuffer[i];
				}

				// 视口变换
				A.position.x /= A.position.w;
				A.position.y /= A.position.w;

				A.position.x = (A.position.x + 1) / 2 * mWidth;
				A.position.y = (1 - A.position.y) / 2 * mHeight;

				// 光栅化
				_rasterizePoint(&A);
			}
		}
	}

	void SoftRenderer::_rasterizePoint(const RasterizerVertex * PA)
	{
		RasterizerVertex v = *PA;

		int x = (int)v.position.x, y = (int)v.position.y;

		if (x >= 0 && x < mWidth && y >= 0 && y <= mHeight)
		{
			if (_pixel_shader(&v))
			{
				_output(x, y, &v);
			}
		}
	}

	void SoftRenderer::_rasterizeLine(const RasterizerVertex * PA, const RasterizerVertex * PB)
	{
		if (PA->position.x > PB->position.x)
			std::swap(PA, PB);

		RasterizerVertex v;

		Int2 A = Int2((int)PA->position.x, (int)PA->position.y);
		Int2 B = Int2((int)PB->position.x, (int)PB->position.y);
		Int2 AB = B - A;

		if (AB.x > abs(AB.y))
		{
			float k = (float)AB.y / AB.x;
			int x = A.x, y = A.y;

			if (k > 0)
			{
				float e = -0.5f;

				for (int i = 0; i < AB.x; ++i)
				{
					x = x + 1;
					e = e + k;
					if (e > 0)
					{
						y = y + 1;
						e = e - 1.0f;
					}

					if (x >= 0 && x < mWidth && y >= 0 && y <= mHeight)
					{
						float d = AB.x > 0 ? (float)i / AB.x : 1;

						RasterizerVertex::Lerp(v, *PA, *PB, d);
						if (_pixel_shader(&v))
						{
							_output(x, y, &v);
						}
					}
				}
			}
			else if (k == 0)
			{
				for (int i = 0; i < AB.x; ++i)
				{
					x = x + 1;

					if (x >= 0 && x < mWidth && y >= 0 && y <= mHeight)
					{
						float d = AB.x > 0 ? (float)i / AB.x : 1;

						RasterizerVertex::Lerp(v, *PA, *PB, d);
						if (_pixel_shader(&v))
						{
							_output(x, y, &v);
						}
					}
				}
			}
			else
			{
				float e = -0.5f;

				for (int i = 0; i < AB.x; ++i)
				{
					x = x + 1;
					e = e - k;
					if (e > 0)
					{
						y = y - 1;
						e = e - 1.0f;
					}

					if (x >= 0 && x < mWidth && y >= 0 && y <= mHeight)
					{
						float d = AB.x > 0 ? (float)i / AB.x : 1;

						RasterizerVertex::Lerp(v, *PA, *PB, d);
						if (_pixel_shader(&v))
						{
							_output(x, y, &v);
						}
					}
				}
			}
		}
		else
		{
			float k = (float)AB.x / AB.y;
			int x = A.x, y = A.y;

			if (k > 0)
			{
				float e = -0.5f;

				for (int i = 0; i < AB.y; ++i)
				{
					y = y + 1;
					e = e + k;
					if (e > 0)
					{
						x = x + 1;
						e = e - 1.0f;
					}

					if (x >= 0 && x < mWidth && y >= 0 && y <= mHeight)
					{
						float d = AB.y > 0 ? (float)i / AB.y : 1;

						RasterizerVertex::Lerp(v, *PA, *PB, d);
						if (_pixel_shader(&v))
						{
							_output(x, y, &v);
						}
					}
				}
			}
			else if (k == 0)
			{
				if (AB.y > 0)
				{
					for (int i = 0; i < AB.y; ++i)
					{
						y = y + 1;

						if (x >= 0 && x < mWidth && y >= 0 && y <= mHeight)
						{
							float d = AB.y > 0 ? (float)i / AB.y : 1;

							RasterizerVertex::Lerp(v, *PA, *PB, d);
							if (_pixel_shader(&v))
							{
								_output(x, y, &v);
							}
						}
					}
				}
				else
				{
					for (int i = 0; i < -AB.y; ++i)
					{
						y = y - 1;

						if (x >= 0 && x < mWidth && y >= 0 && y <= mHeight)
						{
							float d = -AB.y > 0 ? (float)i / -AB.y : 1;

							RasterizerVertex::Lerp(v, *PA, *PB, d);
							if (_pixel_shader(&v))
							{
								_output(x, y, &v);
							}
						}
					}
				}
			}
			else
			{
				float e = -0.5f;

				for (int i = 0; i < -AB.y; ++i)
				{
					y = y - 1;
					e = e - k;
					if (e > 0)
					{
						x = x + 1;
						e = e - 1.0f;
					}

					if (x >= 0 && x < mWidth && y >= 0 && y <= mHeight)
					{
						float d = -AB.y > 0 ? (float)i / -AB.y : 1;

						RasterizerVertex::Lerp(v, *PA, *PB, d);
						if (_pixel_shader(&v))
						{
							_output(x, y, &v);
						}
					}
				}
			}
		}
	}

	void SoftRenderer::_rasterizeTri(const RasterizerVertex * PA, const RasterizerVertex * PB, const RasterizerVertex * PC)
	{
		int _width = mWidth, _height = mHeight;

		// 确保点A是最上面的点
		if (PA->position.y > PB->position.y)
			std::swap(PA, PB);
		if (PA->position.y > PC->position.y)
			std::swap(PA, PC);

		// 确保B在C的左边
		if (PB->position.x > PC->position.x)
			std::swap(PB, PC);

		Int2 A = Int2((int)PA->position.x, (int)PA->position.y);
		Int2 B = Int2((int)PB->position.x, (int)PB->position.y);
		Int2 C = Int2((int)PC->position.x, (int)PC->position.y);
		Int2 AB = B - A, AC = C - A;

		RasterizerVertex v1, v2, v;

		if (B.y > C.y)
		{
			// 三角形类型1 (B.y < C.y)
			//		A*
			//        
			//			*C
			//	B*
			//

			// 画上半部分
			int cy = A.y, ey = C.y;
			cy = std::max(cy, 0);
			ey = std::min(ey, _height);

			while (cy < ey)
			{
				float kab = AB.y > 0 ? (float)(cy - A.y) / (float)AB.y : 1;
				float kac = AC.y > 0 ? (float)(cy - A.y) / (float)AC.y : 1;

				int x1 = (int)(A.x + AB.x * kab);
				int x2 = (int)(A.x + AC.x * kac);

				RasterizerVertex::Lerp(v1, *PA, *PB, kab);
				RasterizerVertex::Lerp(v2, *PA, *PC, kac);

				if (x1 > x2)
				{
					std::swap(x1, x2);
					std::swap(v1, v2);
				}

				int sx = std::max(x1, 0);
				int ex = std::min(x2, _width);

				for (int x = sx; x < ex; x += 1)
				{
					float k = (x2 - x1) > 0 ? (float)(x - x1) / (float)(x2 - x1) : 1;

					RasterizerVertex::Lerp(v, v1, v2, k);
					if (_pixel_shader(&v))
					{
						_output(x, cy, &v);
					}
				}

				cy += 1;
			}

			// 画下半部分
			Int2 CB = Int2(B.x - C.x, B.y - C.y);
			ey = std::min(B.y, _height);

			while (cy < ey)
			{
				float kab = AB.y > 0 ? (float)(cy - A.y) / (float)AB.y : 1;
				float kcb = CB.y > 0 ? (float)(cy - C.y) / (float)CB.y : 1;

				int x1 = (int)(A.x + AB.x * kab);
				int x2 = (int)(C.x + CB.x * kcb);

				RasterizerVertex::Lerp(v1, *PA, *PB, kab);
				RasterizerVertex::Lerp(v2, *PC, *PB, kcb);

				if (x1 > x2)
				{
					std::swap(x1, x2);
					std::swap(v1, v2);
				}

				int sx = std::max(x1, 0);
				int ex = std::min(x2, _width);

				for (int x = sx; x < ex; x += 1)
				{
					float k = (x2 - x1) > 0 ? (float)(x - x1) / (float)(x2 - x1) : 1;

					RasterizerVertex::Lerp(v, v1, v2, k);
					if (_pixel_shader(&v))
					{
						_output(x, cy, &v);
					}
				}

				cy += 1;
			}
		}
		else
		{
			// 三角形类型2 (B在C上面)
			//       A*
			//        
			//	 B*    
			//          *C
			//

			// 画上半部分
			int cy = A.y, ey = B.y;
			cy = std::max(cy, 0);
			ey = std::min(ey, _height);

			while (cy < ey)
			{
				float kab = AB.y > 0 ? (float)(cy - A.y) / (float)AB.y : 1;
				float kac = AC.y > 0 ? (float)(cy - A.y) / (float)AC.y : 1;

				int x1 = (int)(A.x + AB.x * kab);
				int x2 = (int)(A.x + AC.x * kac);

				RasterizerVertex::Lerp(v1, *PA, *PB, kab);
				RasterizerVertex::Lerp(v2, *PA, *PC, kac);

				if (x1 > x2)
				{
					std::swap(x1, x2);
					std::swap(v1, v2);
				}

				int sx = std::max(x1, 0);
				int ex = std::min(x2, _width);

				for (int x = sx; x < ex; x += 1)
				{
					float k = (x2 - x1) > 0 ? (float)(x - x1) / (float)(x2 - x1) : 1;

					RasterizerVertex::Lerp(v, v1, v2, k);
					if (_pixel_shader(&v))
					{
						_output(x, cy, &v);
					}
				}

				cy += 1;
			}

			// 画下半部分
			Int2 BC = Int2(C.x - B.x, C.y - B.y);
			ey = std::min(C.y, _height);

			while (cy < ey)
			{
				float kbc = BC.y > 0 ? (float)(cy - B.y) / (float)BC.y : 1;
				float kac = AC.y > 0 ? (float)(cy - A.y) / (float)AC.y : 1;

				int x1 = (int)(B.x + BC.x * kbc);
				int x2 = (int)(A.x + AC.x * kac);

				RasterizerVertex::Lerp(v1, *PB, *PC, kbc);
				RasterizerVertex::Lerp(v2, *PA, *PC, kac);

				if (x1 > x2)
				{
					std::swap(x1, x2);
					std::swap(v1, v2);
				}

				int sx = std::max(x1, 0);
				int ex = std::min(x2, _width);

				for (int x = sx; x < ex; x += 1)
				{
					float k = (x2 - x1) > 0 ? (float)(x - x1) / (float)(x2 - x1) : 1;

					RasterizerVertex::Lerp(v, v1, v2, k);
					if (_pixel_shader(&v))
					{
						_output(x, cy, &v);
					}
				}

				cy += 1;
			}
		}
	}

	void SoftRenderer::_vertex_shader(RasterizerVertex * vo, const Vertex * vi)
	{
		vo->position = Float4(vi->position);
		vo->normal = vi->normal;
		vo->color = vi->color;
		vo->uv = vi->uv;

		// 世界变换
		vo->position.TransformA(mWorldMatrix);
		vo->normal.TransformN(mWorldMatrix);

		// 计算光照
		_lighting(vo);

		// 视图变换
		vo->position.TransformA(mViewMatrix);

		// 投影变换
		vo->position.Transform(mProjMatrix);
	}

	void SoftRenderer::_lighting(RasterizerVertex * vio)
	{
		if (mLight == NULL)
			return;

		Float3 P = Float3(vio->position.x, vio->position.y, vio->position.z);
		Float3 N, L, V, H;
		float kd, ks, ka;

		Float3 emssive = mMaterial.Emissive;
		Float3 ambient = mMaterial.Ambient;
		Float3 diffuse = mMaterial.Diffuse * mLight->Diffuse;
		Float3 specular = mMaterial.Specular * mLight->Specular;

		switch (mLight->Type)
		{
			case Light::DIRECTION:
			{
				N = vio->normal;
				L = -mLight->Direction;
				V = mCameraPos - P;
				V.Normalize();
				H = L + V;
				H.Normalize();
				
				kd = std::max(N.Dot(L), 0.0f);
				ks = std::max(N.Dot(H), 0.0f);
				ks = pow(ks, mMaterial.SpecularPower);
				ka = 1;

				vio->color = emssive + (ambient + (diffuse * kd + specular * ks) * ka);
			}
			break;

			case Light::POINT:
			{
				N = vio->normal;
				L = mLight->Position - P;
				V = mCameraPos - P;

				float dist = L.Normalize();
				V.Normalize();
				H = L + V;
				H.Normalize();

				kd = std::max(N.Dot(L), 0.0f);
				ks = std::max(N.Dot(H), 0.0f);
				ka = (dist - mLight->AttenStart) / (mLight->AttenEnd - mLight->AttenStart);
				ka = pow(1 - clamp<float>(ka, 0, 1), mLight->AttenFallOff);

				vio->color = emssive + (ambient + (diffuse * kd + specular * ks) * ka);
			}
			break;

			case Light::SPOT:
			{
				Float3 S = mLight->Position - P;
				N = vio->normal;
				L = -mLight->Direction;
				V = mCameraPos - P;
				V.Normalize();
				H = L + V;
				H.Normalize();

				float dist = S.Normalize();

				kd = std::max(N.Dot(L), 0.0f);
				ks = std::max(N.Dot(H), 0.0f);
				ka = (dist - mLight->AttenStart) / (mLight->AttenEnd - mLight->AttenStart);
				ka = pow(1 - clamp<float>(ka, 0, 1), mLight->AttenFallOff);
				
				float cosInner = cos(mLight->SpotInner * DEG_TO_RAD);
				float cosOuter = cos(mLight->SpotOuter * DEG_TO_RAD);
				float kspot = (S.Dot(L) - cosOuter) / (cosInner - cosOuter);
				kspot = pow(clamp<float>(ks, 0, 1), mLight->SpotFallOff);
				ka *= kspot;

				vio->color = emssive + (ambient + (diffuse * kd + specular * ks) * ka);
			}
			break;
		}
	}

	bool SoftRenderer::_pixel_shader(RasterizerVertex * vio)
	{
		if (mTexture != NULL)
		{
			vio->color = vio->color * mTexture->Tex2D(vio->uv.x, vio->uv.y, mSamplerState);
		}

		return true;
	}

	bool SoftRenderer::_backface_cull(const RasterizerVertex * PA, const RasterizerVertex * PB, const RasterizerVertex * PC)
	{
		if (mRenderState.CullMode == eCullMode::NONE)
			return false;

		Float2 AB, AC;
		AB.x = PB->position.x - PA->position.x;
		AB.y = PB->position.y - PA->position.y;
		AC.x = PC->position.x - PA->position.x;
		AC.y = PC->position.y - PA->position.y;

		float cross = AB.x * AC.y - AB.y * AC.x;

		if (mRenderState.CullMode == eCullMode::BACK)
			return cross > 0;
		else
			return cross < 0;
	}

	bool SoftRenderer::_depth_test(float old_depth, float new_depth)
	{
		switch (mRenderState.DepthMode)
		{
			case eDepthMode::ALWAYS:
				return true;
			case eDepthMode::LESS:
				return new_depth < old_depth;
			case eDepthMode::LESS_EQUAL:
				return new_depth <= old_depth;
			case eDepthMode::GREATER:
				return new_depth > old_depth;
			case eDepthMode::GREATER_EQUAL:
				return new_depth >= old_depth;
			case eDepthMode::EQUAL:
				return new_depth == old_depth;
			case eDepthMode::NOT_EQUAL:
				return new_depth != old_depth;
		}

		return false;
	}

	void SoftRenderer::_output(int x, int y, const RasterizerVertex * vi)
	{
		int index = y * mWidth + x;
		float depth = vi->position.z / vi->position.w;

		if (!_depth_test(mDepthBuffer[index], depth))
			return;

		if (mRenderState.DepthMask == TRUE)
		{
			mDepthBuffer[index] = depth;
		}

		Float3 * fragColor = &mColorBuffer[index];
		switch (mRenderState.BlendMode)
		{
			case eBlendMode::OPACITY:
				fragColor->r = vi->color.r;
				fragColor->g = vi->color.g;
				fragColor->b = vi->color.b;
				break;

			case eBlendMode::ADD:
				fragColor->r += vi->color.r;
				fragColor->g += vi->color.g;
				fragColor->b += vi->color.b;
				break;

			case eBlendMode::ALPHA_BLEND:
				fragColor->r = fragColor->r + (vi->color.r - fragColor->r) * vi->color.a;
				fragColor->g = fragColor->g + (vi->color.g - fragColor->g) * vi->color.a;
				fragColor->b = fragColor->b + (vi->color.b - fragColor->b) * vi->color.a;
				break;
		}

		fragColor->Saturate();
	}

	void SoftRenderer::Begin()
	{
		mLight = NULL;
		mTexture = NULL;
	}

	void SoftRenderer::End()
	{
	}

	void SoftRenderer::Present()
	{
		mContext->Present(mColorBuffer);
	}

}

