#include "Float3.h"
#include "Mat4.h"
#include "Quat.h"

namespace X {

	const Float3 Float3::Zero = Float3(0, 0, 0);

	Float3::Float3(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z)
	{
	}

	Float3::Float3()
		: x(0), y(0), z(0)
	{
	}

	Float3::~Float3()
	{
	}

	Float3::Float3(const Float3 & rk)
	{
		x = rk.x; y = rk.y; z = rk.z;
	}

	Float3 & Float3::operator =(const Float3 & rk)
	{
		x = rk.x; y = rk.y; z = rk.z;
		return *this;
	}

	bool Float3::operator ==(const Float3 & rk) const
	{
		return x == rk.x && y == rk.y && z == rk.z;
	}

	bool Float3::operator !=(const Float3 & rk) const
	{
		return x != rk.x || y != rk.y || z != rk.z;
	}

	bool Float3::operator > (const Float3 & rk) const
	{
		return x > rk.x && y > rk.y && z > rk.z;
	}

	bool Float3::operator >=(const Float3 & rk) const
	{
		return x >= rk.x && y >= rk.y && z >= rk.z;
	}

	bool Float3::operator < (const Float3 & rk) const
	{
		return x < rk.x && y < rk.y && z < rk.z;
	}

	bool Float3::operator <=(const Float3 & rk) const
	{
		return x <= rk.x && y <= rk.y && z <= rk.z;
	}

	Float3 Float3::operator -() const
	{
		return Float3(-x, -y, -z);
	}

	Float3 Float3::operator + (const Float3 & rk) const
	{
		return Float3(x + rk.x, y + rk.y, z + rk.z);
	}

	Float3 Float3::operator - (const Float3 & rk) const
	{
		return Float3(x - rk.x, y - rk.y, z - rk.z);
	}

	Float3 Float3::operator * (const Float3 & rk) const
	{
		return Float3(x * rk.x, y * rk.y, z * rk.z);
	}

	Float3 Float3::operator / (const Float3 & rk) const
	{
		return Float3(x / rk.x, y / rk.y, z / rk.z);
	}

	Float3 Float3::operator * (float rk) const
	{
		return Float3(x * rk, y * rk, z * rk);
	}

	Float3 Float3::operator / (float rk) const
	{
		d_assert (rk != 0);
		return (*this) * (1 / rk);
	}

	Float3& Float3::operator += (const Float3 & rk)
	{
		x += rk.x; y += rk.y; z += rk.z;
		return *this;
	}

	Float3& Float3::operator -= (const Float3 & rk)
	{
		x -= rk.x; y -= rk.y; z -= rk.z;
		return *this;
	}

	Float3& Float3::operator *= (float rk)
	{
		x *= rk; y *= rk; z *= rk;
		return *this;
	}

	Float3& Float3::operator /= (float rk)
	{
		d_assert (rk != 0);
		return (*this) *= (1 / rk);
	}

	Float3& Float3::operator *= (const Float3 & rk)
	{
		x *= rk.x; y *= rk.y; z *= rk.z;
		return *this;
	}

	Float3& Float3::operator /= (const Float3 & rk)
	{
		x /= rk.x; y /= rk.y; z /= rk.z;
		return *this;
	}

	Float3 Float3::operator * (const Mat4 & rk) const
	{
		Float3 o = *this;

		o.Transform(rk);

		return o;
	}

	Float3& Float3::operator *= (const Mat4 & rk)
	{
		Transform(rk);

		return *this;
	}

	Float3 Float3::operator * (const Quat & rk) const
	{
		Float3 v = *this;

		v.TransformQ(rk);

		return v;
	}

	Float3 & Float3::operator *=(const Quat & rk)
	{
		TransformQ(rk);

		return *this;
	}

	float & Float3::operator[](int index)
	{
		d_assert (index < 3);
		return m[index];
	}

	float Float3::operator[](int index) const
	{
		d_assert (index < 3);
		return m[index];
	}

	float Float3::Length() const
	{
		return sqrt(LengthSq());
	}

	float Float3::LengthSq() const
	{
		return x * x + y * y + z * z;
	}

	float Float3::Normalize()
	{
		float len;

#if 0

		len = SIMD_Normalize3(*this, *this);
		len = 1 / len;

#else
		len = Length();

		if (len > 0)
		{
			float inv_len = 1 / len;
			x *= inv_len;
			y *= inv_len;
			z *= inv_len;
		}
		else
		{
			x = y = z = 0;
		}
#endif

		return len;

	}

	float Float3::Distance(const Float3 & rk) const
	{
		return (*this - rk).Length();
	}

	float Float3::DistanceSq(const Float3 & rk) const
	{
		return (*this - rk).LengthSq();
	}

	float Float3::Dot(const Float3 & rk) const
	{
		return x * rk.x + y * rk.y + z * rk.z;
	}

	void Float3::Transform(const Mat4 & m)
	{
#ifdef SIMD_Transform3

		SIMD_Transform3(this, this, &m);

#else
		Float3 vTmp;
		float vTmp_w;

		vTmp.x = x * m._11 + y * m._21 + z * m._31 + m._41;
		vTmp.y = x * m._12 + y * m._22 + z * m._32 + m._42;
		vTmp.z = x * m._13 + y * m._23 + z * m._33 + m._43;
		vTmp_w = x * m._14 + y * m._24 + z * m._34 + m._44;
		vTmp /= vTmp_w;

		*this = vTmp;
#endif
	}

	void Float3::TransformA(const Mat4 & m)
	{
#ifdef SIMD_TransformA3
		
		SIMD_TransformA3(this, this, &m);

#else
		Float3 vTmp;

		vTmp.x = x * m._11 + y * m._21 + z * m._31 + m._41;
		vTmp.y = x * m._12 + y * m._22 + z * m._32 + m._42;
		vTmp.z = x * m._13 + y * m._23 + z * m._33 + m._43;

		*this = vTmp;
#endif
	}


	void Float3::TransformN(const Mat4 & m)
	{
#ifdef SIMD_TransformN3

		SIMD_TransformN3(this, this, &m);

#else
		Float3 vTmp;

		vTmp.x = x * m._11 + y * m._21 + z * m._31;
		vTmp.y = x * m._12 + y * m._22 + z * m._32;
		vTmp.z = x * m._13 + y * m._23 + z * m._33;

		*this = vTmp;
#endif
	}

	void Float3::TransformQ(const Quat & q)
	{
		Float3 qv(q.x, q.y, q.z);

		Float3 uv = Float3::Cross(qv, *this);
		Float3 uuv = Float3::Cross(qv, uv);

		uv *= (2.0f * q.w);
		uuv *= 2.0f;

		*this = *this + uv + uuv;
	}

	void Float3::Saturate()
	{
		r = clamp<float>(r, 0, 1);
		g = clamp<float>(g, 0, 1);
		b = clamp<float>(b, 0, 1);
	}

	Float3 Float3::Cross(const Float3 & lk, const Float3 & rk)
	{
		Float3 v;

#ifdef SIMD_Cross3

		SIMD_Cross3(&v, &lk, &rk);

#else
		v.x = lk.y * rk.z - lk.z * rk.y;
		v.y = lk.z * rk.x - lk.x * rk.z;
		v.z = lk.x * rk.y - lk.y * rk.x;
#endif

		return v;
	}

}