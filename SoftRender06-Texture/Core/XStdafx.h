#pragma once

#if _MSC_VER
#pragma warning(disable : 4996)
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <vector>
#include <list>
#include <map>
#include <algorithm>

#define d_assert(x) assert(x)
#define safe_delete(p) { if (p) { delete (p); (p) = 0; } }
#define safe_delete_array(p) { if (p) { delete[] (p); (p) = 0; } }

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef PI
#define PI			3.141592654f
#endif

#ifndef PI2
#define PI2			(3.141592654f * 2)
#endif

#define INV_2		(1.0f / 2.0f)
#define INV_3		(1.0f / 3.0f)
#define INV_255		(1.0f / 255.0f)

#define SQRT_2		1.414213562f
#define SQRT_3		1.732050808f

#define DEG_TO_RAD	(PI / 180.0f)
#define RAD_TO_DEG	(180.0f / PI)

#define EPSILON_E4 (float)(1E-4)
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

#define MAKE_WORD(c0, c1) ((c0) << 8 | c1)
#define MAKE_DWORD(c0, c1, c2, c3) ((c0) << 24 | (c1) << 16 | (c2) << 8 | (c3))
#define MAKE_DWORD2(s0, s1) ((s0) << 16 | s1)

namespace X {
	typedef int8_t	 int8;
	typedef int16_t	 int16;
	typedef int32_t	 int32;
	typedef int64_t	 int64;

	typedef uint8_t  uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;

	typedef uint8	byte;
	typedef uint16	word;
	typedef uint32	dword;
	typedef uint64	qword;

	typedef std::string String;

	template<class T>
	inline const T & clamp(const T & x, const T & low, const T & hight)
	{
		return x < low ? low : (x > hight ? hight : x);
	}

	//
	inline bool IS_BIG_ENDIANG()
	{
		return ((*(short *)"1") > 0xFF) != 0;
	}

	inline int ENDIAN_SWAP16(int i)
	{
		return (i & 0x00FF) << 8 | (i & 0xFF00) >> 8;
	}

	inline int ENDIAN_SWAP32(int i)
	{
		return (i & 0x000000FF) << 24 | (i & 0x0000FF00) << 8 | (i & 0x00FF0000) >> 8 | (i & 0xFF000000) >> 24;
	}

	template <class T>
	inline T ENDIAN_LITTLE_TO_HOST(T i)
	{
		if (IS_BIG_ENDIANG())
		{
			assert(sizeof(T) == 2 || sizeof(T) == 4);

			return (T)(sizeof(T) == 2 ? ENDIAN_SWAP16(i) : ENDIAN_SWAP32(i));
		}

		return i;
	}

	template <class T>
	inline T ENDIAN_BIG_TO_HOST(T i)
	{
		if (!IS_BIG_ENDIANG())
		{
			d_assert(sizeof(T) == 2 || sizeof(T) == 4);

			return (T)(sizeof(T) == 2 ? ENDIAN_SWAP16(i) : ENDIAN_SWAP32(i));
		}

		return i;
	}


}