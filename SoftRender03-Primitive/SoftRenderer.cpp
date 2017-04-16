#include "SoftRenderer.h"
#include "SoftRendererContext.h"

namespace X {

	SoftRenderer::SoftRenderer(HWND hWnd, int w, int h)
	{
		mContext = new SoftRendererContext(hWnd, w, h);

		mWidth = w, mHeight = h;
		mColorBuffer = new Float3[mWidth * mHeight];
		mDepthBuffer = new float[mWidth * mHeight];
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

	void SoftRenderer::Render(const Vertex * varray, int vertexCount, const int * iarray, int primCount, int primType)
	{
		// 顶点处理
		mRasterizerVertexBuffer.resize(vertexCount);
		for (int i = 0; i < vertexCount; ++i)
		{
			_vertex_shader(&mRasterizerVertexBuffer[i], &varray[i]);
		}

		// 图元处理
		RasterizerVertex A, B, C, D;
		for (int i = 0; i < primCount; ++i)
		{
			if (primType == ePrimType::TRI_LIST)
			{
				if (iarray != NULL)
				{
					// 使用索引列表
					A = mRasterizerVertexBuffer[iarray[i * 3 + 0]];
					B = mRasterizerVertexBuffer[iarray[i * 3 + 1]];
					C = mRasterizerVertexBuffer[iarray[i * 3 + 2]];
				}
				else
				{
					// 使用顶点列表
					A = mRasterizerVertexBuffer[i * 3 + 0];
					B = mRasterizerVertexBuffer[i * 3 + 1];
					C = mRasterizerVertexBuffer[i * 3 + 2];
				}

				// 背面裁剪

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
			else if (primType == ePrimType::LINE_LIST)
			{
				if (iarray != NULL)
				{
					// 使用索引列表
					A = mRasterizerVertexBuffer[iarray[i * 2 + 0]];
					B = mRasterizerVertexBuffer[iarray[i * 2 + 1]];
				}
				else
				{
					// 使用顶点列表
					A = mRasterizerVertexBuffer[i * 2 + 0];
					B = mRasterizerVertexBuffer[i * 2 + 1];
				}

				// 背面裁剪

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
			else if (primType == ePrimType::POINT_LIST)
			{
				if (iarray != NULL)
				{
					// 使用索引列表
					A = mRasterizerVertexBuffer[iarray[i]];
				}
				else
				{
					// 使用顶点列表
					A = mRasterizerVertexBuffer[i];
				}

				// 背面裁剪

				// 裁剪

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
			_pixel_shader(&v);
			_output(x, y, &v);
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

						_pixel_shader(&v);
						_output(x, y, &v);
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

						_pixel_shader(&v);
						_output(x, y, &v);
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

						_pixel_shader(&v);
						_output(x, y, &v);
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

						_pixel_shader(&v);
						_output(x, y, &v);
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

							_pixel_shader(&v);
							_output(x, y, &v);
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

							_pixel_shader(&v);
							_output(x, y, &v);
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

						_pixel_shader(&v);
						_output(x, y, &v);
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

					_pixel_shader(&v);
					_output(x, cy, &v);
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

					_pixel_shader(&v);
					_output(x, cy, &v);
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

					_pixel_shader(&v);
					_output(x, cy, &v);
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

					_pixel_shader(&v);
					_output(x, cy, &v);
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
	}

	void SoftRenderer::_pixel_shader(RasterizerVertex * vio)
	{
	}

	void SoftRenderer::_output(int x, int y, const RasterizerVertex * vi)
	{
		int index = y * mWidth + x;

		mColorBuffer[index].r = vi->color.r;
		mColorBuffer[index].g = vi->color.g;
		mColorBuffer[index].b = vi->color.b;
	}

	void SoftRenderer::Begin()
	{
	}

	void SoftRenderer::End()
	{
	}

	void SoftRenderer::Present()
	{
		mContext->Present(mColorBuffer);
	}

}

