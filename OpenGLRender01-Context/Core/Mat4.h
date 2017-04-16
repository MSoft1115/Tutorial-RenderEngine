#pragma once

#include "XStdafx.h"

namespace X {

	class Float3;
	class Float4;
	class Quat;

	class Mat4
	{
	public:
		static const Mat4 Identity;
		static const Mat4 Zero;

	public:
		union 
		{
			struct 
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
			float m16[16];
		};

	public:
		Mat4();
		~Mat4();

		Mat4(const float * p);
		Mat4(const Mat4 & m);
		Mat4(float m11, float m12, float m13, float m14, 
			 float m21, float m22, float m23, float m24, 
			 float m31, float m32, float m33, float m34,  
			 float m41, float m42, float m43, float m44);

		Mat4& 
			operator =(const Mat4 & mat);

		Mat4 
			operator -() const;
		Mat4 
			operator +(const Mat4 & mat) const; 
		Mat4 
			operator -(const Mat4 & mat) const; 
		Mat4 
			operator *(const Mat4 & mat) const; 

		Mat4&
			operator +=(const Mat4 & mat);
		Mat4& 
			operator -=(const Mat4 & mat);
		Mat4& 
			operator *=(const Mat4 & mat);

		Mat4 
			operator *(float v) const;
		Mat4 
			operator /(float v) const;

		Mat4& 
			operator *=(float v);
		Mat4& 
			operator /=(float v);

		bool 
			operator ==(const Mat4 & mat) const;
		bool 
			operator !=(const Mat4 & mat) const;

		float* 
			operator [](int index);
		const float* 
			operator [](int index) const;

	public:
		void 
			Transpose();
		void 
			Inverse();
		
		void 
			MakeTransform(const Float3 & trans, const Quat & rotate, const Float3 & scale);
		void
			MakeTransform(const Float3 & trans, const Float3 & scale);
		void 
			MakeTranslate(float x, float y, float z);
		void 
			MakeRotation(const Float3 & vAxis, float rads);
		void 
			MakeRotation(const Float3 & xAxis, const Float3 & yAxis, const Float3 & zAxis);
		void 
			MakeScale(float x, float y, float z);

		void 
			MakeViewTM(const Float3 & vEye, const Float3 & vAt, const Float3 & vUp);
		void 
			MakeViewTM(const Float3 & vEye, const Quat & qOrient);
		void 
			MakeOrthoTM(float w, float h, float zn, float zf);
		void 
			MakePerspectiveTM(float fovy, float aspect,float zn, float zf);
	};

}
