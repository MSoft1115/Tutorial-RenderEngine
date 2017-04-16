#pragma once

#include "XStdafx.h"

namespace X {

	struct Int3
	{
		union {
			struct {
				int x, y, z;
			};

			struct {
				int u, v, w;
			};

			int m[3];
		};

		Int3() : x(0), y(0), z(0) {}
		Int3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

		Int3 operator -() const
		{
			return Int3(-x, -y, -z);
		}

		Int3 operator +(const Int3 & rk) const
		{
			return Int3(x + rk.x, y + rk.y, z + rk.z);
		}

		Int3 operator -(const Int3 & rk) const
		{
			return Int3(x - rk.x, y - rk.y, z - rk.z);
		}

		void operator +=(const Int3 & rk)
		{
			x += rk.x;
			y += rk.y;
			z += rk.z;
		}

		void operator -=(const Int3 & rk)
		{
			x -= rk.x;
			y -= rk.y;
			z -= rk.z;
		}

		Int3 operator *(const Int3 & rk) const
		{
			return Int3(x * rk.x, y * rk.y, z * rk.z);
		}

		Int3 operator /(const Int3 & rk) const
		{
			return Int3(x / rk.x, y / rk.y, z / rk.z);
		}

		void operator *=(const Int3 & rk)
		{
			x *= rk.x;
			y *= rk.y;
			z *= rk.z;
		}

		void operator /=(const Int3 & rk)
		{
			x /= rk.x;
			y /= rk.y;
			z /= rk.z;
		}

		int operator[](int i) const
		{
			d_assert(i < 3);

			return m[i];
		}

		int & operator[](int i)
		{
			d_assert(i < 3);

			return m[i];
		}

		bool operator ==(const Int3 & rk) const
		{
			return x == rk.x && y == rk.y && z == rk.z;
		}

		bool operator !=(const Int3 & rk) const
		{
			return x != rk.x || y != rk.y || z != rk.z;
		}

		bool operator > (const Int3 & rk) const
		{
			return x > rk.x && y > rk.y && z > rk.z;
		}

		bool operator >=(const Int3 & rk) const
		{
			return x >= rk.x && y >= rk.y && z >= rk.z;
		}

		bool operator < (const Int3 & rk) const
		{
			return x < rk.x && y < rk.y && z < rk.z;
		}

		bool operator <=(const Int3 & rk) const
		{
			return x <= rk.x && y <= rk.y && z <= rk.z;
		}

		Int3 & operator =(const Int3 & rk)
		{
			x = rk.x, y = rk.y, z = rk.z;
			return *this;
		}
	};

}