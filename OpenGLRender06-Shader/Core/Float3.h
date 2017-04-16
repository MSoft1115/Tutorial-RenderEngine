#pragma once

#include "XStdafx.h"

namespace X {

	class Mat4;
	class Quat;

	class Float3
	{
	public:
		static const Float3 Zero;

	public:
		union {

			struct {
				float x, y, z;
			};

			struct {
				float r, g, b;
			};

			struct {
				float u, v, w;
			};

			struct {
				float m[3];
			};
		};
		

	public:
		Float3(float _x, float _y, float _z);
		Float3(const Float3 & rk);
		Float3();
		~Float3();

		Float3 &
			operator =(const Float3 & rk);

		bool 
			operator ==(const Float3 & rk) const;
		bool 
			operator !=(const Float3 & rk) const;
		bool 
			operator > (const Float3 & rk) const;
		bool 
			operator >=(const Float3 & rk) const;
		bool 
			operator < (const Float3 & rk) const;
		bool 
			operator <=(const Float3 & rk) const;

		Float3 
			operator -() const;

		Float3 
			operator + (const Float3 & rk) const;
		Float3 
			operator - (const Float3 & rk) const;
		Float3 
			operator * (const Float3 & rk) const;
		Float3 
			operator / (const Float3 & rk) const;
		Float3 
			operator * (float f) const;
		Float3 
			operator / (float f) const;

		Float3 & 
			operator +=(const Float3 & rk);
		Float3 & 
			operator -=(const Float3 & rk);
		Float3 & 
			operator *=(float rk);
		Float3 & 
			operator /=(float rk);

		Float3 & 
			operator *=(const Float3 & rk);
		Float3 & 
			operator /=(const Float3 & rk);

		Float3 
			operator * (const Mat4 & rk) const;
		Float3 & 
			operator *=(const Mat4 & rk);

		Float3
			operator * (const Quat & rk) const;
		Float3 &
			operator *=(const Quat & rk);

		float & 
			operator[](int index);
		float 
			operator[](int index) const;

	public:
		float 
			Dot(const Float3 & rk) const;
		float 
			Length() const;
		float 
			LengthSq() const;
		float 
			Distance(const Float3 & rk) const;
		float 
			DistanceSq(const Float3 & rk) const;
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
		void 
			TransformQ(const Quat & q);

		Float3
			Cross(const Float3 & rk) const { return Cross(*this, rk); }

		static Float3 
			Cross(const Float3 & lk, const Float3 & rk);
		static Float3 
			CrossN(const Float3 & lk, const Float3 & rk);
		static Float3 
			CalcuNormal(const Float3 & a, const Float3 & b, const Float3 & c);
	};

	inline Float3 operator * (float lk, const Float3 & rk)
	{
		return rk * lk;
	}

	inline Float3 operator / (float lk, const Float3 & rk)
	{
		return Float3(lk / rk.x, lk / rk.y, lk / rk.z);
	}

	inline Float3 Float3::CrossN(const Float3 & lk, const Float3 & rk)
	{
		Float3 n = Cross(lk, rk);
		n.Normalize();

		return n;
	}

	inline Float3 Float3::CalcuNormal(const Float3 & a, const Float3 & b, const Float3 & c)
	{
		Float3 ab = b - a;
		Float3 ac = c - a;

		Float3 n = Float3::Cross(ab, ac);
		n.Normalize();

		return n;
	}

}
