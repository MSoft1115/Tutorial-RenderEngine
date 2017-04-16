#pragma once

#include "XStdafx.h"

namespace X {

	struct Int4
	{
		union {
			struct {
				int x, y, z, w;
			};

			int m[4];
		};

		Int4() : x(0), y(0), z(0), w(0) {}
		Int4(int _x, int _y, int _z, int _w) : x(_x), y(_y), z(_z), w(_w) {}

		Int4 operator -() const
		{
			return Int4(-x, -y, -z, -w);
		}

		Int4 operator +(const Int4 & rk) const
		{
			return Int4(x + rk.x, y + rk.y, z + rk.z, z + rk.w);
		}

		Int4 operator -(const Int4 & rk) const
		{
			return Int4(x - rk.x, y - rk.y, z - rk.z, w - rk.w);
		}

		void operator +=(const Int4 & rk)
		{
			x += rk.x;
			y += rk.y;
			z += rk.z;
			w += rk.w;
		}

		void operator -=(const Int4 & rk)
		{
			x -= rk.x;
			y -= rk.y;
			z -= rk.z;
			w -= rk.w;
		}

		Int4 operator *(const Int4 & rk) const
		{
			return Int4(x * rk.x, y * rk.y, z * rk.z, w * rk.w);
		}

		Int4 operator /(const Int4 & rk) const
		{
			return Int4(x / rk.x, y / rk.y, z / rk.z, w / rk.w);
		}

		void operator *=(const Int4 & rk)
		{
			x *= rk.x;
			y *= rk.y;
			z *= rk.z;
			w *= rk.w;
		}

		void operator /=(const Int4 & rk)
		{
			x /= rk.x;
			y /= rk.y;
			z /= rk.z;
			w /= rk.w;
		}

		int operator[](int i) const
		{
			d_assert(i < 4);

			return m[i];
		}

		int & operator[](int i)
		{
			d_assert(i < 4);

			return m[i];
		}

		bool operator ==(const Int4 & rk) const
		{
			return x == rk.x && y == rk.y && z == rk.z && w == rk.w;
		}

		bool operator !=(const Int4 & rk) const
		{
			return x != rk.x || y != rk.y || z != rk.z || w != rk.w;
		}

		bool operator > (const Int4 & rk) const
		{
			return x > rk.x && y > rk.y && z > rk.z && w > rk.w;
		}

		bool operator >=(const Int4 & rk) const
		{
			return x >= rk.x && y >= rk.y && z >= rk.z && w >= rk.w;
		}

		bool operator < (const Int4 & rk) const
		{
			return x < rk.x && y < rk.y && z < rk.z && w < rk.w;
		}

		bool operator <=(const Int4 & rk) const
		{
			return x <= rk.x && y <= rk.y && z <= rk.z && w <= rk.w;
		}

		Int4 & operator =(const Int4 & rk)
		{
			x = rk.x, y = rk.y, z = rk.z, w = rk.w;
			return *this;
		}
	};
}