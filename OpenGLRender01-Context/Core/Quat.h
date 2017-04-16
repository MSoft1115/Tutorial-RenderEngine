#pragma once

#include "XStdafx.h"
#include "Float3.h"
#include "Mat4.h"

namespace X {

	class Quat
	{
	public:
		static const Quat Zero;
		static const Quat Identity;

	public:
		union
		{
			struct {
				float x, y, z, w;
			};
			float m[4];
		};

	public:
		Quat(float x, float y, float z, float w);
		Quat(const Float3 & axis, float rads);
		Quat(const Quat & q);
		Quat();
		~Quat();

		Quat & 
			operator =(const Quat & q);
		Quat 
			operator -() const;

		Quat 
			operator +(const Quat & q) const;
		Quat 
			operator -(const Quat & q) const;
		Quat 
			operator *(const Quat & q) const;
		Quat 
			operator *(float v) const;

		Quat & 
			operator +=(const Quat & q);
		Quat & 
			operator -=(const Quat & q);
		Quat & 
			operator *=(const Quat & q);

		bool 
			operator ==(const Quat & q) const;
		bool 
			operator !=(const Quat & q) const;

	public:
		float 
			Dot(const Quat & rk) const;
		void 
			Inverse();
		void 
			InverseUnit();
		void 
			Normalize();
		void 
			Conjugate();

		void 
			FromAxis(const Float3 & axis, float rads);
		void 
			FromMatrix(const Mat4 & m);
		void 
			ToAxis(Float3 & axis, float & rads) const;
		Mat4 
			ToMatrix() const;

		Float3 
			GetXVector() const;
		Float3 
			GetYVector() const;
		Float3 
			GetZVector() const;
		
		static Quat 
			Slerp(const Quat & a, const Quat & b, float t);
	};

}
