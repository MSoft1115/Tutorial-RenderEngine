#pragma once

#include "XStdafx.h"

namespace X {

	class Float2
	{
	public:
		static const Float2 Zero;

	public:
		union {

			struct {
				float x, y;
			};

			struct {
				float u, v;
			};

			struct {
				float m[2];
			};
		};

	public:
		Float2(float _x, float _y);
		Float2(const Float2 & rk);
		Float2();
		~Float2();

		Float2 &
			operator =(const Float2 & rk);

		bool 
			operator ==(const Float2 & rk) const;
		bool 
			operator !=(const Float2 & rk) const;
		bool 
			operator > (const Float2 & rk) const;
		bool 
			operator >=(const Float2 & rk) const;
		bool 
			operator < (const Float2 & rk) const;
		bool 
			operator <=(const Float2 & rk) const;

		Float2 
			operator -() const;

		Float2 
			operator + (const Float2 & rk) const;
		Float2 
			operator - (const Float2 & rk) const;
		Float2 
			operator * (const Float2 & rk) const;
		Float2 
			operator / (const Float2 & rk) const;
		Float2 
			operator * (float f) const;
		Float2 
			operator / (float f) const;

		Float2 & 
			operator += (const Float2 & rk);
		Float2 & 
			operator -= (const Float2 & rk);
		Float2 & 
			operator *= (float rk);
		Float2 & 
			operator /= (float rk);

		Float2 & 
			operator *= (const Float2 & rk);
		Float2 &
			operator /= (const Float2 & rk);

		float & 
			operator[](int i);
		float
			operator[](int i) const;

	public:
		float 
			Dot(const Float2 & rk) const;
		float
			Length() const;
		float 
			LengthSq() const;
		float 
			Distance(const Float2 & rk) const;
		float 
			DistanceSq(const Float2 & rk) const;
		float 
			Normalize();
		void 
			Saturate();
	};

}

