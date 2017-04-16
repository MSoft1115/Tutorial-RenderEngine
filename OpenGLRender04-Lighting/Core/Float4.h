#pragma once

#include "XStdafx.h"

namespace X {

	class Float3;
	class Mat4;

	class Float4
	{
	public:
		static const Float4 Zero;

	public:
		union {

			struct {
				float x, y, z, w;
			};

			struct {
				float r, g, b, a;
			};

			struct {
				float m[4];
			};
		};
		

	public:
		Float4(float _x, float _y, float _z, float _w = 1);
		Float4(const Float3 & rk, float _w = 1);
		Float4(const Float4 & rk);
		Float4();
		~Float4();

		Float4 &
			operator =(const Float4 & rk);

		bool 
			operator ==(const Float4 & rk) const;
		bool 
			operator !=(const Float4 & rk) const;
		bool 
			operator > (const Float4 & rk) const;
		bool 
			operator >=(const Float4 & rk) const;
		bool 
			operator < (const Float4 & rk) const;
		bool 
			operator <=(const Float4 & rk) const;

		Float4 
			operator -() const;

		Float4 
			operator + (const Float4 & rk) const;
		Float4 
			operator - (const Float4 & rk) const;
		Float4 
			operator * (const Float4 & rk) const;
		Float4 
			operator / (const Float4 & rk) const;
		Float4 
			operator * (float f) const;
		Float4 
			operator / (float f) const;

		Float4& 
			operator += (const Float4 & rk);
		Float4& 
			operator -= (const Float4 & rk);
		Float4& 
			operator *= (float rk);
		Float4& 
			operator /= (float rk);

		Float4& 
			operator *= (const Float4 & rk);
		Float4& 
			operator /= (const Float4 & rk);

		Float4 
			operator * (const Mat4 & rk) const;
		Float4& 
			operator *= (const Mat4 & rk);

		float & 
			operator[](int index);
		float 
			operator[](int index) const;

	public:
		float 
			Dot(const Float4 & rk) const;
		float 
			Length() const;
		float 
			LengthSq() const;
		float 
			Distance(const Float4 & rk) const;
		float 
			DistanceSq(const Float4 & rk) const;
		float 
			Normalize();
		void 
			Saturate();
		
		void 
			Transform(const Mat4 & m);
		void 
			TransformA(const Mat4 & m);
		void 
			TransformN(const Mat4 & m);

		Float4
			Cross(const Float4 & rk) const { return Cross(*this, rk); }
		static Float4 
			Cross(const Float4 & lk, const Float4 & rk);
	};

	inline Float4 operator * (float lk, const Float4 & rk)
	{
		return rk * lk;
	}

	inline Float4 operator / (float lk, const Float4 & rk)
	{
		return Float4(lk / rk.x, lk / rk.y, lk / rk.z, lk / rk.w);
	}

}

