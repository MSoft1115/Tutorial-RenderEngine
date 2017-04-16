#include "Float2.h"

namespace X {

	const Float2 Float2::Zero = Float2(0, 0);

	Float2::Float2(float _x, float _y)
		: x(_x), y(_y)
	{
	}

	Float2::Float2()
		: x(0), y(0)
	{
	}

	Float2::~Float2()
	{
	}

	Float2::Float2(const Float2 & rk)
	{
		x = rk.x; y = rk.y;
	}

	Float2 & Float2::operator =(const Float2 & rk)
	{
		x = rk.x; y = rk.y;
		return *this;
	}

	bool Float2::operator ==(const Float2 & rk) const
	{
		return x == rk.x && y == rk.y;
	}

	bool Float2::operator !=(const Float2 & rk) const
	{
		return x != rk.x || y != rk.y;
	}

	bool Float2::operator > (const Float2 & rk) const
	{
		return x > rk.x && y > rk.y;
	}

	bool Float2::operator >=(const Float2 & rk) const
	{
		return x >= rk.x && y >= rk.y;
	}

	bool Float2::operator < (const Float2 & rk) const
	{
		return x < rk.x && y < rk.y;
	}

	bool Float2::operator <=(const Float2 & rk) const
	{
		return x <= rk.x && y <= rk.y;
	}

	Float2 Float2::operator -() const
	{
		return Float2(-x, -y);
	}

	Float2 Float2::operator + (const Float2 & rk) const
	{
		return Float2(x + rk.x, y + rk.y);
	}

	Float2 Float2::operator - (const Float2 & rk) const
	{
		return Float2(x - rk.x, y - rk.y);
	}

	Float2 Float2::operator * (const Float2 & rk) const
	{
		return Float2(x * rk.x, y * rk.y);
	}

	Float2 Float2::operator / (const Float2 & rk) const
	{
		return Float2(x / rk.x, y / rk.y);
	}

	Float2 Float2::operator * (float rk) const
	{
		return Float2(x * rk, y * rk);
	}

	Float2 Float2::operator / (float rk) const
	{
		d_assert (rk != 0);
		return (*this) * (1 / rk);
	}

	Float2& Float2::operator += (const Float2 & rk)
	{
		x += rk.x; y += rk.y;
		return *this;
	}

	Float2& Float2::operator -= (const Float2 & rk)
	{
		x -= rk.x; y -= rk.y;
		return *this;
	}

	Float2& Float2::operator *= (float rk)
	{
		x *= rk; y *= rk;
		return *this;
	}

	Float2& Float2::operator /= (float rk)
	{
		d_assert (rk != 0);
		return (*this) *= (1 / rk);
	}

	Float2& Float2::operator *= (const Float2 & rk)
	{
		x *= rk.x; y *= rk.y;
		return *this;
	}

	Float2& Float2::operator /= (const Float2 & rk)
	{
		x /= rk.x; y /= rk.y;
		return *this;
	}

	float & Float2::operator[](int index)
	{
		d_assert (index < 2);
		return m[index];
	}

	float Float2::operator[](int index) const
	{
		d_assert (index < 2);
		return m[index];
	}

	float Float2::Length() const
	{
		return ::sqrt(LengthSq());
	}

	float Float2::LengthSq() const
	{
		return x * x + y * y;
	}

	float Float2::Normalize()
	{
		float len = Length();

		if (len > EPSILON_E4)
		{
			float inv_len = 1 / len;
			x *= inv_len;
			y *= inv_len;
		}
		else
		{
			x = y = 0;
		}

		return len;
	}

	float Float2::Distance(const Float2 & rk) const
	{
		return (*this - rk).Length();
	}

	float Float2::DistanceSq(const Float2 & rk) const
	{
		return (*this - rk).LengthSq();
	}

	float Float2::Dot(const Float2 & rk) const
	{
		return x * rk.x + y * rk.y;
	}

	void Float2::Saturate()
	{
		x = clamp<float>(x, 0, 1);
		y = clamp<float>(y, 0, 1);
	}

}