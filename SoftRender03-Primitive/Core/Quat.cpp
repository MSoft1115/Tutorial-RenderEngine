#include "Quat.h"

namespace X {

	inline void QuatMultiply(Quat & q, const Quat & q1, const Quat & q2)
	{
#ifdef SIMD_QuatMul
		SIMD_QuatMul(&q, &q1, &q2);
#else
		q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
		q.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
		q.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
		q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
#endif
	}


	const Quat Quat::Zero = Quat(0, 0, 0, 0);
	const Quat Quat::Identity = Quat(0, 0, 0, 1);

	Quat::Quat(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{
	}

	Quat::Quat(const Quat &q) : x(q.x), y(q.y), z(q.z), w(q.w)
	{
	}

	Quat::Quat(const Float3 & axis, float rads)
	{
		FromAxis(axis, rads);
	}

	Quat::Quat()
		: x(0), y(0), z(0), w(1)
	{
	}

	Quat::~Quat()
	{
	}

	Quat & Quat::operator =(const Quat & q)
	{
		x = q.x, y = q.y, z = q.z, w = q.w;
		return *this;
	}

	Quat Quat::operator -() const
	{
		return Quat(-x, -y, -z, w);
	}

	Quat Quat::operator +(const Quat & q) const
	{
		return Quat(x + q.x, y + q.y, z + q.z, w + q.w);
	}

	Quat Quat::operator -(const Quat & q) const
	{
		return Quat(x - q.x, y - q.y, z - q.z, w - q.w);
	}

	Quat Quat::operator *(const Quat & q2) const
	{
		Quat q;
		QuatMultiply(q, *this, q2);
		return q;
	}

	Quat Quat::operator *(float v) const
	{
		return Quat(x * v, y * v, z * v, w * v);
	}

	Quat & Quat::operator +=(const Quat & q)
	{
		x += q.x, y += q.y, z += q.z, w += q.w;
		return *this;
	}

	Quat & Quat::operator -=(const Quat & q)
	{
		x -= q.x, y -= q.y, z -= q.z, w -= q.w;
		return *this;
	}

	Quat & Quat::operator *=(const Quat & q2)
	{
		*this = *this * q2;
		return *this;
	}

	bool Quat::operator ==(const Quat & q) const
	{
		return x == q.x && y == q.y && z == q.z && w == q.w;
	}

	bool Quat::operator !=(const Quat & q) const
	{
		return x != q.x || y != q.y || z != q.z || w != q.w;
	}

	float Quat::Dot(const Quat & rk) const
	{
		return x * rk.x + y * rk.y + z * rk.z + w * rk.w;
	}

	void Quat::Inverse()
	{
		float len2 = x * x + y * y + z * z + w * w;

		d_assert (len2 > 0);

		len2 = 1.0f / len2;

		x = -x * len2;
		y = -y * len2;
		z = -z * len2;
		w = w * len2;
	}

	void Quat::InverseUnit()
	{
		Conjugate();
	}

	void Quat::Normalize()
	{
		float len = sqrt(x * x + y * y + z * z + w * w);

		d_assert (len != 0);

		if (len > 0)
		{
			float inv_len = 1.0f / len;
			x *= inv_len;
			y *= inv_len;
			z *= inv_len;
			w *= inv_len;
		}
	}

	void Quat::Conjugate()
	{
		x = -x; y = -y; z = -z;
	}

	void Quat::FromAxis(const Float3 & axis, float rads)
	{
		float half_rads = rads / 2.0f;
		float cosine = cos(half_rads);
		float sine = sin(half_rads);

		x = axis.x * sine;
		y = axis.y * sine;
		z = axis.z * sine;
		w = cosine;
	}

	void Quat::FromMatrix(const Mat4 & m)
	{
		Quat q;
		float s = 0.0f;
		float t = 0.0f;

		// check the diagonal
		if(m._11 + m._22 + m._33 > 0.0)
		{
			s = 0.5f / sqrt(1.0f + m._11 + m._22 + m._33);

			q.w = 0.25f / s;
			q.x = (m._23 - m._32) * s;
			q.y = (m._31 - m._13) * s;
			q.z = (m._12 - m._21) * s;
		}

		// column 1
		else if(m._11 > m._22 && m._11 > m._33)
		{
			s = 2.0f * sqrt(1.0f + m._11 - m._22 - m._33);

			q.x = 0.25f * s;
			q.y = (m._12 + m._21) / s;
			q.z = (m._13 + m._31) / s;
			q.w = (m._23 - m._32) / s;
		}

		// column 2
		else if( m._22 > m._33)
		{
			s = 2.0f * sqrt(1.0f + m._22 - m._11 - m._33);

			q.y = 0.25f * s;
			q.x = (m._21 + m._12) / s;
			q.z = (m._32 + m._23) / s;
			q.w = (m._31 - m._13) / s;
		}

		// column 3
		else
		{
			s = 2.0f * sqrt(1.0f + m._33 - m._11 - m._22);

			q.z = 0.25f * s;
			q.x = (m._31 + m._13) / s;
			q.y = (m._32 + m._23) / s;
			q.w = (m._12 - m._21) / s;
		}

		*this = q;
	}

	Quat Quat::Slerp(const Quat & a, const Quat & b, float t)
	{
		const Quat & q1 = a; const Quat & q2 = b;
		Quat q2_t;

		float cosine = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
		if (cosine < 0.0f)
		{
			cosine = -cosine;
			q2_t.x = -q2.x;
			q2_t.y = -q2.y;
			q2_t.z = -q2.z;
			q2_t.w = -q2.w;
		}
		else
		{
			q2_t = q2;
		}

		float t0, t1;
		if (1.0f - cosine > EPSILON_E4)
		{
			float theta = acos(cosine);
			float inv_sine = 1.0f / sin(theta);
			t0 = sin((1.0f - t) * theta) * inv_sine;
			t1 = sin(t * theta) * inv_sine;
		}
		else
		{
			t0 = 1.0f - t;
			t1 = t;
		}

		return q1 * t0 + q2_t * t1;
	}

	void Quat::ToAxis(Float3 & axis, float & rads) const
	{
		rads = acos(w);

		float sin_theta_inv = 1.0f / sin(rads);

		axis.x = x * sin_theta_inv;
		axis.y = y * sin_theta_inv;
		axis.z = z * sin_theta_inv;

		rads *= 2;
	}

	Mat4 Quat::ToMatrix() const
	{
		Mat4 m;

		m._11 = 1.0f - 2.0f * (y * y + z * z);
		m._12 = 2.0f * (x * y + w * z);
		m._13 = 2.0f * (x * z - w * y);
		m._14 = 0.0f;

		m._21 = 2.0f * (x * y - w * z); 
		m._22 = 1.0f - 2.0f * (x * x + z * z);
		m._23 = 2.0f * (y * z + w * x);
		m._24 = 0.0f;

		m._31 = 2.0f * (w * y + x * z);
		m._32 = 2.0f * (y * z - w * x);
		m._33 = 1.0f - 2.0f * (x * x + y * y);
		m._34 = 0.0f;

		m._41 = 0.0f;
		m._42 = 0.0f;
		m._43 = 0.0f;
		m._44 = 1.0f;

		return m;
	}

	Float3 Quat::GetXVector() const
	{
		Float3 xAxis;

		xAxis.x = 1.0f - 2.0f * (y * y + z * z);
		xAxis.y = 2.0f * (x * y + w * z);
		xAxis.z = 2.0f * (x * z - w * y);

		return xAxis;
	}

	Float3 Quat::GetYVector() const
	{
		Float3 yAxis;

		yAxis.x = 2.0f * (x * y - w * z); 
		yAxis.y = 1.0f - 2.0f * (x * x + z * z);
		yAxis.z = 2.0f * (y * z + w * x);

		return yAxis;
	}

	Float3 Quat::GetZVector() const
	{
		Float3 zAxis;

		zAxis.x = 2.0f * (w * y + x * z);
		zAxis.y = 2.0f * (y * z - w * x);
		zAxis.z = 1.0f - 2.0f * (x * x + y * y);

		return zAxis;
	}

}