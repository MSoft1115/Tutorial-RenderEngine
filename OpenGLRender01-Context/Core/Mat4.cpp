#include "Mat4.h"
#include "Float3.h"
#include "Float4.h"
#include "Quat.h"

namespace X {

	inline void MatAdd(Mat4 & m, const Mat4 & m1, const Mat4 & m2)
	{
#ifdef SIMD_MatAdd

		SIMD_MatAdd(&m, &m1, &m2);

#else
		m._11 = m1._11 + m2._11, m._12 = m1._12 + m2._12, m._13 = m1._13 + m2._13, m._14 = m1._14 + m2._14;
		m._21 = m1._21 + m2._21, m._22 = m1._22 + m2._22, m._23 = m1._23 + m2._23, m._24 = m1._24 + m2._24;
		m._31 = m1._31 + m2._31, m._32 = m1._32 + m2._32, m._33 = m1._33 + m2._33, m._34 = m1._34 + m2._34;
		m._41 = m1._41 + m2._41, m._42 = m1._42 + m2._42, m._43 = m1._43 + m2._43, m._44 = m1._44 + m2._44;
#endif
	}

	inline void MatSub(Mat4 & m, const Mat4 & m1, const Mat4 & m2)
	{
#ifdef SIMD_MatSub

		SIMD_MatSub(&m, &m1, &m2);

#else
		m._11 = m1._11 - m2._11, m._12 = m1._12 - m2._12, m._13 = m1._13 - m2._13, m._14 = m1._14 - m2._14;
		m._21 = m1._21 - m2._21, m._22 = m1._22 - m2._22, m._23 = m1._23 - m2._23, m._24 = m1._24 - m2._24;
		m._31 = m1._31 - m2._31, m._32 = m1._32 - m2._32, m._33 = m1._33 - m2._33, m._34 = m1._34 - m2._34;
		m._41 = m1._41 - m2._41, m._42 = m1._42 - m2._42, m._43 = m1._43 - m2._43, m._44 = m1._44 - m2._44;
#endif
	}

	inline void MatMultiply(Mat4 & m, const Mat4 & m1, const Mat4 & m2)
	{
#ifdef SIMD_MatMul

		SIMD_MatMul(&m, &m1, &m2);

#else
		m._11 = m1._11 * m2._11 + m1._12 * m2._21 + m1._13 * m2._31 + m1._14 * m2._41;
		m._12 = m1._11 * m2._12 + m1._12 * m2._22 + m1._13 * m2._32 + m1._14 * m2._42;
		m._13 = m1._11 * m2._13 + m1._12 * m2._23 + m1._13 * m2._33 + m1._14 * m2._43;
		m._14 = m1._11 * m2._14 + m1._12 * m2._24 + m1._13 * m2._34 + m1._14 * m2._44;

		m._21 = m1._21 * m2._11 + m1._22 * m2._21 + m1._23 * m2._31 + m1._24 * m2._41;
		m._22 = m1._21 * m2._12 + m1._22 * m2._22 + m1._23 * m2._32 + m1._24 * m2._42;
		m._23 = m1._21 * m2._13 + m1._22 * m2._23 + m1._23 * m2._33 + m1._24 * m2._43;
		m._24 = m1._21 * m2._14 + m1._22 * m2._24 + m1._23 * m2._34 + m1._24 * m2._44;

		m._31 = m1._31 * m2._11 + m1._32 * m2._21 + m1._33 * m2._31 + m1._34 * m2._41;
		m._32 = m1._31 * m2._12 + m1._32 * m2._22 + m1._33 * m2._32 + m1._34 * m2._42;
		m._33 = m1._31 * m2._13 + m1._32 * m2._23 + m1._33 * m2._33 + m1._34 * m2._43;
		m._34 = m1._31 * m2._14 + m1._32 * m2._24 + m1._33 * m2._34 + m1._34 * m2._44;

		m._41 = m1._41 * m2._11 + m1._42 * m2._21 + m1._43 * m2._31 + m1._44 * m2._41;
		m._42 = m1._41 * m2._12 + m1._42 * m2._22 + m1._43 * m2._32 + m1._44 * m2._42;
		m._43 = m1._41 * m2._13 + m1._42 * m2._23 + m1._43 * m2._33 + m1._44 * m2._43;
		m._44 = m1._41 * m2._14 + m1._42 * m2._24 + m1._43 * m2._34 + m1._44 * m2._44;
#endif
	}

	inline void MatScale(Mat4 & m, const Mat4 & m1, float v)
	{
#ifdef SIMD_MatScale

		SIMD_MatScale(&m, &m1, v);

#else
		m._11 = m1._11 * v, m._12 = m1._12 * v, m._13 = m1._13 * v, m._14 = m1._14 * v;
		m._21 = m1._21 * v, m._22 = m1._22 * v, m._23 = m1._23 * v, m._24 = m1._24 * v;
		m._31 = m1._31 * v, m._32 = m1._32 * v, m._33 = m1._33 * v, m._34 = m1._34 * v;
		m._41 = m1._41 * v, m._42 = m1._42 * v, m._43 = m1._43 * v, m._44 = m1._44 * v;
#endif
	}


	const Mat4 Mat4::Zero = Mat4(0, 0, 0, 0,
								 0, 0, 0, 0,
								 0, 0, 0, 0,
								 0, 0, 0, 0);

	const Mat4 Mat4::Identity = Mat4(1, 0, 0, 0,
									 0, 1, 0, 0,
									 0, 0, 1, 0,
									 0, 0, 0, 1);
	
	Mat4::Mat4()
	{
	}

	Mat4::~Mat4()
	{
	}

	Mat4::Mat4(float m11, float m12, float m13, float m14, 
			   float m21, float m22, float m23, float m24, 
			   float m31, float m32, float m33, float m34,
			   float m41, float m42, float m43, float m44) 
	: _11(m11), _12(m12), _13(m13), _14(m14)
	, _21(m21), _22(m22), _23(m23), _24(m24)
	, _31(m31), _32(m32), _33(m33), _34(m34)
	, _41(m41), _42(m42), _43(m43), _44(m44) 
	{
	}

	Mat4::Mat4(const float * p)
	{
		memcpy(this, p, sizeof(float) * 16);
	}

	Mat4::Mat4(const Mat4 & m)
	{
		*this = m;
	}

	Mat4& Mat4::operator =(const Mat4 & m)
	{
		_11 = m._11, _12 = m._12, _13 = m._13, _14 = m._14;
		_21 = m._21, _22 = m._22, _23 = m._23, _24 = m._24;
		_31 = m._31, _32 = m._32, _33 = m._33, _34 = m._34;
		_41 = m._41, _42 = m._42, _43 = m._43, _44 = m._44;
		return (*this);
	}

	Mat4 Mat4::operator -() const
	{
		return Mat4(-_11, -_12, -_13, -_14,
					-_21, -_22, -_23, -_24,
					-_31, -_32, -_33, -_34,
					-_41, -_42, -_43, -_44);
	}

	Mat4 Mat4::operator +(const Mat4 & mat) const
	{
		Mat4 m;
		MatAdd(m, *this, mat);
		return m;
	}

	Mat4 Mat4::operator -(const Mat4 & mat) const
	{
		Mat4 m;
		MatSub(m, *this, mat);
		return m;
	}

	Mat4 Mat4::operator *(const Mat4 & m2) const
	{
		Mat4 m;
		MatMultiply(m, *this, m2);
		return m;
	}

	Mat4 Mat4::operator *(float v) const
	{
		Mat4 m;
		MatScale(m, *this, v);
		return m;
	}

	Mat4 Mat4::operator /(float v) const
	{
		d_assert (v != 0);
		return ((*this)) * (1 / v);
	}

	Mat4& Mat4::operator +=(const Mat4 & mat)
	{
		MatAdd(*this, *this, mat);
		return (*this);
	}

	Mat4& Mat4::operator -=(const Mat4 & mat)
	{
		MatSub(*this, *this, mat);
		return (*this);
	}

	Mat4& Mat4::operator *=(const Mat4 & mat)
	{
		*this = *this * mat;
		return (*this);
	}

	Mat4& Mat4::operator *=(float v)
	{
		MatScale(*this, *this, v);
		return (*this);
	}

	Mat4& Mat4::operator /=(float v)
	{
		d_assert (v != 0);
		return ((*this)) *= (1 / v);
	}

	bool Mat4::operator ==(const Mat4 & m) const
	{
		return _11 == m._11 && _12 == m._12 && _13 == m._13 && _14 == m._14 &&
			   _21 == m._21 && _22 == m._22 && _23 == m._23 && _24 == m._24 &&
			   _31 == m._31 && _32 == m._32 && _33 == m._33 && _34 == m._34 &&
			   _41 == m._41 && _42 == m._42 && _43 == m._43 && _44 == m._44;
	}

	bool Mat4::operator !=(const Mat4 & m) const
	{
		return _11 != m._11 || _12 != m._12 || _13 != m._13 || _14 != m._14 ||
			   _21 != m._21 || _22 != m._22 || _23 != m._23 || _24 != m._24 ||
			   _31 != m._31 || _32 != m._32 || _33 != m._33 || _34 != m._34 ||
			   _41 != m._41 || _42 != m._42 || _43 != m._43 || _44 != m._44;
	}

	float* Mat4::operator [](int index)
	{
		d_assert(index < 4);
		return m[index];
	}

	const float* Mat4::operator [](int index) const
	{
		d_assert(index < 4);
		return m[index];
	}

	void Mat4::Transpose()
	{
		Mat4 m;

		m._11 = _11, m._12 = _21, m._13 = _31, m._14 = _41;
		m._21 = _12, m._22 = _22, m._23 = _32, m._24 = _42;
		m._31 = _13, m._32 = _23, m._33 = _33, m._34 = _43;
		m._41 = _14, m._42 = _24, m._43 = _34, m._44 = _44;

		(*this) = m;
	}

	void Mat4::Inverse()
	{
		float m00 = m[0][0], m10 = m[0][1], m20 = m[0][2], m30 = m[0][3];
		float m01 = m[1][0], m11 = m[1][1], m21 = m[1][2], m31 = m[1][3];
		float m02 = m[2][0], m12 = m[2][1], m22 = m[2][2], m32 = m[2][3];
		float m03 = m[3][0], m13 = m[3][1], m23 = m[3][2], m33 = m[3][3];

		float v0 = m20 * m31 - m21 * m30;
		float v1 = m20 * m32 - m22 * m30;
		float v2 = m20 * m33 - m23 * m30;
		float v3 = m21 * m32 - m22 * m31;
		float v4 = m21 * m33 - m23 * m31;
		float v5 = m22 * m33 - m23 * m32;

		float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
		float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
		float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
		float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

		float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

		float d00 = t00 * invDet;
		float d10 = t10 * invDet;
		float d20 = t20 * invDet;
		float d30 = t30 * invDet;

		float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m10 * m31 - m11 * m30;
		v1 = m10 * m32 - m12 * m30;
		v2 = m10 * m33 - m13 * m30;
		v3 = m11 * m32 - m12 * m31;
		v4 = m11 * m33 - m13 * m31;
		v5 = m12 * m33 - m13 * m32;

		float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m21 * m10 - m20 * m11;
		v1 = m22 * m10 - m20 * m12;
		v2 = m23 * m10 - m20 * m13;
		v3 = m22 * m11 - m21 * m12;
		v4 = m23 * m11 - m21 * m13;
		v5 = m23 * m12 - m22 * m13;

		float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		(*this) = Mat4(
			d00, d10, d20, d30,
			d01, d11, d21, d31,
			d02, d12, d22, d32,
			d03, d13, d23, d33);
	}

	void Mat4::MakeTransform(const Float3 & trans, const Quat & rotate, const Float3 & scale)
	{
		Mat4 mRotate = rotate.ToMatrix();

		_11 = mRotate._11 * scale.x;
		_12 = mRotate._12 * scale.y;
		_13 = mRotate._13 * scale.z;
		_14 = 0;

		_21 = mRotate._21 * scale.x;
		_22 = mRotate._22 * scale.y;
		_23 = mRotate._23 * scale.z;
		_24 = 0;

		_31 = mRotate._31 * scale.x;
		_32 = mRotate._32 * scale.y;
		_33 = mRotate._33 * scale.z;
		_34 = 0;

		_41 = trans.x; _42 = trans.y; _43 = trans.z, _44 = 1;
	}

	void Mat4::MakeTransform(const Float3 & trans, const Float3 & scale)
	{
		_11 = scale.x; _12 = 0; _13 = 0; _14 = 0;
		_21 = 0; _22 = scale.y; _23 = 0; _24 = 0;
		_31 = 0; _32 = 0; _33 = scale.z; _34 = 0;
		_41 = trans.x; _42 = trans.y; _43 = trans.z; _44 = 1;
	}

	void Mat4::MakeTranslate(float x, float y, float z)
	{
		_11 = 1, _12 = 0, _13 = 0, _14 = 0;
		_21 = 0, _22 = 1, _23 = 0, _24 = 0;
		_31 = 0, _32 = 0, _33 = 1, _34 = 0;
		_41 = x, _42 = y, _43 = z, _44 = 1;
	}

	void Mat4::MakeRotation(const Float3 & n, float rads)
	{
		float sine = sin(rads);
		float cosine = cos(rads);
		float _1_sub_cosine = 1 - cosine;

		float x_y = n.x * n.y;
		float x_z = n.x * n.z;
		float y_z = n.y * n.z;

		(*this) = Mat4::Identity;

		_11 = n.x * n.x * _1_sub_cosine + cosine;
		_12 = x_y * _1_sub_cosine + n.z * sine;
		_13 = x_z * _1_sub_cosine - n.y * sine;

		_21 = x_y * _1_sub_cosine - n.z * sine;
		_22 = n.y * n.y * _1_sub_cosine + cosine;
		_23 = y_z * _1_sub_cosine + n.x * sine;

		_31 = x_z * _1_sub_cosine + n.y * sine;
		_32 = y_z * _1_sub_cosine - n.x * sine;
		_33 = n.z * n.z * _1_sub_cosine + cosine;
	}

	void Mat4::MakeRotation(const Float3 & xaxis, const Float3 & yaxis, const Float3 & zaxis)
	{
		_11 = xaxis.x,  _12 = xaxis.y,  _13 = xaxis.z,  _14 = 0.0f;
		_21 = yaxis.x,  _22 = yaxis.y,  _23 = yaxis.z,  _24 = 0.0f;
		_31 = zaxis.x,  _32 = zaxis.y,  _33 = zaxis.z,  _34 = 0.0f;
		_41 = 0,        _42 = 0,        _43 = 0,        _44 = 1.0f;
	}

	void Mat4::MakeScale(float x, float y, float z)
	{
		_11 = x, _12 = 0, _13 = 0, _14 = 0;
		_21 = 0, _22 = y, _23 = 0, _24 = 0;
		_31 = 0, _32 = 0, _33 = z, _34 = 0;
		_41 = 0, _42 = 0, _43 = 0, _44 = 1;
	}

	void Mat4::MakeViewTM(const Float3 & vEye, const Float3 & vAt, const Float3 & vUp)
	{
		Float3 xaxis, yaxis, zaxis;

		zaxis = vAt - vEye;
		zaxis.Normalize();

		xaxis = Float3::Cross(vUp, zaxis);
		xaxis.Normalize();
		
		yaxis = Float3::Cross(zaxis, xaxis);
		yaxis.Normalize();
		
		_11 = xaxis.x, _21 = xaxis.y, _31 = xaxis.z;
		_12 = yaxis.x, _22 = yaxis.y, _32 = yaxis.z;
		_13 = zaxis.x, _23 = zaxis.y, _33 = zaxis.z;
		_41 = -xaxis.Dot(vEye);
		_42 = -yaxis.Dot(vEye);
		_43 = -zaxis.Dot(vEye);
		_14 = 0.0f;
		_24 = 0.0f;
		_34 = 0.0f;
		_44 = 1.0f;
	}

	void Mat4::MakeViewTM(const Float3 & vEye, const Quat & qOrient)
	{
		Float3 xaxis, yaxis, zaxis;

		xaxis = qOrient.GetXVector();
		yaxis = qOrient.GetYVector();
		zaxis = qOrient.GetZVector();

		_11 = xaxis.x, _21 = xaxis.y, _31 = xaxis.z;
		_12 = yaxis.x, _22 = yaxis.y, _32 = yaxis.z;
		_13 = zaxis.x, _23 = zaxis.y, _33 = zaxis.z;
		_41 = -xaxis.Dot(vEye);
		_42 = -yaxis.Dot(vEye);
		_43 = -zaxis.Dot(vEye);
		_14 = 0.0f;
		_24 = 0.0f;
		_34 = 0.0f;
		_44 = 1.0f;
	}

	void Mat4::MakeOrthoTM(float w, float h, float zn, float zf)
	{
		float xScale = 2.0f / w;
		float yScale = 2.0f / h;
		float inv = 1.0f / (zf - zn);

		_11 = xScale,	_12 = 0,		_13 = 0,			_14 = 0;
		_21 = 0,		_22 = yScale,	_23 = 0,			_24 = 0;
		_31 = 0,		_32 = 0,		_33 = inv,			_34 = 0;
		_41 = 0,		_42 = 0,		_43 = -zn * inv,	_44 = 1;
	}

	void Mat4::MakePerspectiveTM(float fovy, float aspect,float zn, float zf)
	{
		float yScale = 1 / tan(fovy / 2);
		float xScale = yScale / aspect;
		float inv = 1.0f / (zf - zn);

		_11 = xScale,	_12 = 0,		_13 = 0,				_14 = 0;
		_21 = 0,		_22 = yScale,	_23 = 0,				_24 = 0;
		_31 = 0,		_32 = 0,		_33 = zf * inv,			_34 = 1;
		_41 = 0,		_42 = 0,		_43 = -zn * zf * inv,	_44 = 0;
	}

}
