#include "Float4.h"
#include "Float3.h"
#include "Mat4.h"

namespace X {

	const Float4 Float4::Zero = Float4(0, 0, 0, 0);

	Float4::Float4(float _x, float _y, float _z, float _w)
		: x(_x), y(_y), z(_z), w(_w)
	{
	}

	Float4::Float4()
		: x(0), y(0), z(0), w(1)
	{
	}

	Float4::~Float4()
	{
	}

	Float4::Float4(const Float3 & rk, float _w)
	{
		x = rk.x; y = rk.y; z = rk.z; w = _w;
	}

	Float4::Float4(const Float4 & rk)
	{
		x = rk.x; y = rk.y; z = rk.z; w = rk.w;
	}

	Float4 & Float4::operator =(const Float4 & rk)
	{
		x = rk.x; y = rk.y; z = rk.z; w = rk.w;
		return *this;
	}

	bool Float4::operator ==(const Float4 & rk) const
	{
		return x == rk.x && y == rk.y && z == rk.z && w == rk.w;
	}

	bool Float4::operator !=(const Float4 & rk) const
	{
		return x != rk.x || y != rk.y || z != rk.z || w != rk.w;
	}

	bool Float4::operator > (const Float4 & rk) const
	{
		return x > rk.x && y > rk.y && z > rk.z && w > rk.w;
	}

	bool Float4::operator >=(const Float4 & rk) const
	{
		return x >= rk.x && y >= rk.y && z >= rk.z && w >= rk.w;
	}

	bool Float4::operator < (const Float4 & rk) const
	{
		return x < rk.x && y < rk.y && z < rk.z && w < rk.w;
	}

	bool Float4::operator <=(const Float4 & rk) const
	{
		return x <= rk.x && y <= rk.y && z <= rk.z && w <= rk.w;
	}

	Float4 Float4::operator -() const
	{
		return Float4(-x, -y, -z, -w);
	}

	Float4 Float4::operator + (const Float4 & rk) const
	{
		return Float4(x + rk.x, y + rk.y, z + rk.z, w + rk.w);
	}

	Float4 Float4::operator - (const Float4 & rk) const
	{
		return Float4(x - rk.x, y - rk.y, z - rk.z, w - rk.w);
	}

	Float4 Float4::operator * (const Float4 & rk) const
	{
		return Float4(x * rk.x, y * rk.y, z * rk.z, w * rk.w);
	}

	Float4 Float4::operator / (const Float4 & rk) const
	{
		return Float4(x / rk.x, y / rk.y, z / rk.z, w / rk.w);
	}

	Float4 Float4::operator * (float rk) const
	{
		return Float4(x * rk, y * rk, z * rk, w * rk);
	}

	Float4 Float4::operator / (float rk) const
	{
		d_assert (rk != 0);
		return (*this) * (1 / rk);
	}

	Float4& Float4::operator += (const Float4 & rk)
	{
		x += rk.x; y += rk.y; z += rk.z; w += rk.w;
		return *this;
	}

	Float4& Float4::operator -= (const Float4 & rk)
	{
		x -= rk.x; y -= rk.y; z -= rk.z; w -= rk.w;
		return *this;
	}

	Float4& Float4::operator *= (float rk)
	{
		x *= rk; y *= rk; z *= rk; w *= rk;
		return *this;
	}

	Float4& Float4::operator /= (float rk)
	{
		d_assert (rk != 0);
		return (*this) *= (1 / rk);
	}

	Float4& Float4::operator *= (const Float4 & rk)
	{
		x *= rk.x; y *= rk.y; z *= rk.z; w *= rk.w;
		return *this;
	}

	Float4& Float4::operator /= (const Float4 & rk)
	{
		x /= rk.x; y /= rk.y; z /= rk.z; w /= rk.w;
		return *this;
	}

	Float4 Float4::operator * (const Mat4 & rk) const
	{
		Float4 o = *this;

		o.Transform(rk);

		return o;
	}

	Float4& Float4::operator *= (const Mat4 & rk)
	{
		Transform(rk);

		return *this;
	}

	float & Float4::operator[](int index)
	{
		return m[index];
	}

	float Float4::operator[](int index) const
	{
		return m[index];
	}

	float Float4::Length() const
	{
		return sqrt(LengthSq());
	}

	float Float4::LengthSq() const
	{
		return x * x + y * y + z * z;
	}

	float Float4::Normalize()
	{
		float len;

		len = Length();

		if (len > 0)
		{
			float inv_len = 1 / len;
			x *= inv_len;
			y *= inv_len;
			z *= inv_len;
			w = 1;
		}
		else
		{
			x = y = z = 0;
			w = 1;
		}

		return len;
	}

	float Float4::Distance(const Float4 & rk) const
	{
		return (*this - rk).Length();
	}

	float Float4::DistanceSq(const Float4 & rk) const
	{
		return (*this - rk).LengthSq();
	}

	float Float4::Dot(const Float4 & rk) const
	{
		return x * rk.x + y * rk.y + z * rk.z;
	}

	void Float4::Transform(const Mat4 & m)
	{
#ifdef SIMD_Transform4

		SIMD_Transform4(this, this, &m);

#else
		Float4 vTmp;

		vTmp.x = x * m._11 + y * m._21 + z * m._31 + w * m._41;
		vTmp.y = x * m._12 + y * m._22 + z * m._32 + w * m._42;
		vTmp.z = x * m._13 + y * m._23 + z * m._33 + w * m._43;
		vTmp.w = x * m._14 + y * m._24 + z * m._34 + w * m._44;

		*this = vTmp;
#endif

	}

	void Float4::TransformA(const Mat4 & m)
	{
#ifdef SIMD_Transform4

		SIMD_Transform4(this, this, &m);
		w = 1;

#else
		Float4 vTmp;

		vTmp.x = x * m._11 + y * m._21 + z * m._31 + m._41;
		vTmp.y = x * m._12 + y * m._22 + z * m._32 + m._42;
		vTmp.z = x * m._13 + y * m._23 + z * m._33 + m._43;
		vTmp.w = 1;

		*this = vTmp;
#endif
	}

	void Float4::TransformN(const Mat4 & m)
	{
#ifdef SIMD_TransformN4

		SIMD_TransformN4(this, this, &m);

#else
		Float4 vTmp;

		vTmp.x = x * m._11 + y * m._21 + z * m._31;
		vTmp.y = x * m._12 + y * m._22 + z * m._32;
		vTmp.z = x * m._13 + y * m._23 + z * m._33;
		vTmp.w = 1;

		*this = vTmp;
#endif
	}

	void Float4::Saturate()
	{
		r = clamp<float>(r, 0, 1);
		g = clamp<float>(g, 0, 1);
		b = clamp<float>(b, 0, 1);
		a = clamp<float>(a, 0, 1);
	}

	Float4 Float4::Cross(const Float4 & lk, const Float4 & rk)
	{
		Float4 v;

#ifdef SIMD_Cross4

		SIMD_Cross4(&v, &lk, &rk);

#else
		v.x = lk.y * rk.z - lk.z * rk.y;
		v.y = lk.z * rk.x - lk.x * rk.z;
		v.z = lk.x * rk.y - lk.y * rk.x;
		v.w = 1;
#endif

		return v;
	}

}