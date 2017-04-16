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

#define EPSILON_E4 (float)(1E-4)
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

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

}