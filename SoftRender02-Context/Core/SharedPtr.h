#pragma once

#include "XStdafx.h"

#define DECLARE_REF_COUNTER() \
public: \
	X::RefCounter i_reference;

namespace X {

	template <class T>
	class SharedPtr
	{
	public:
		SharedPtr() : ptr(NULL)
		{
		}

		SharedPtr(T * p) : ptr(NULL)
		{
			*this = p;
		}

		SharedPtr(const SharedPtr & p) : ptr(NULL)
		{
			*this = p;
		}

		~SharedPtr()
		{
			Release();
		}

		void Release()
		{
			if (ptr && ptr->i_reference._refdec() == 0)
			{
				ptr->Release();
				ptr = NULL;
			}
		}

		T * c_ptr() const
		{
			return ptr;
		}

		SharedPtr & operator =(T * p)
		{
			Release();

			ptr = p;
			if (ptr)
				ptr->i_reference._refinc();

			return *this;
		}

		SharedPtr & operator =(const SharedPtr & p)
		{
			Release();

			ptr = p.ptr;
			if (ptr)
				ptr->i_reference._refinc();

			return *this;
		}

		bool operator ==(const SharedPtr & p) const
		{
			return ptr == p.ptr;
		}

		bool operator !=(const SharedPtr & p) const
		{
			return ptr != p.ptr;
		}

		bool operator ==(const T * p) const
		{
			return ptr == p;
		}

		bool operator !=(const T * p) const
		{
			return ptr != p;
		}

		T * operator ->() const
		{
			return ptr;
		}

	protected:
		T * ptr;
	};

	//
	class RefCounter
	{
	public:
		RefCounter() : i_refcount(0) {}

		~RefCounter() { d_assert(i_refcount == 0); }

		int
			_refcount() { return i_refcount; }

		int
			_refinc() { return ++i_refcount; }

		int
			_refdec() { d_assert(i_refcount > 0); return --i_refcount; }

	protected:
		int i_refcount;
	};

}