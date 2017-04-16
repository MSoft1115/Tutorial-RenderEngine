#pragma once

namespace Rad {

	template <class T, int BLOCK_SIZE>
	class rml_allocator
	{
	public:
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;

		struct element
		{
			element * next;
			unsigned char mem[sizeof(T)];
		};

		struct block
		{
			block * next;
			element elements[BLOCK_SIZE];
		};

	public:
		rml_allocator()
		{
			i_blocks = NULL;
			i_free = NULL;
			i_active = 0;
		}

		~rml_allocator()
		{
			clear();
		}

		bool empty() const
		{
			return i_active == 0;
		}

		void clear()
		{
			while (i_blocks)
			{
				block * bl = i_blocks;
				i_blocks = i_blocks->next;
				delete bl;
			}

			i_blocks = NULL;
			i_free = NULL;
			i_active = 0;
		}

		pointer allocate()
		{
			if (!i_free)
			{
				block * bl = new block();
				bl->next = i_blocks;
				i_blocks = bl;

				for (int i = 0; i < BLOCK_SIZE; ++i)
				{
					bl->elements[i].next = i_free;
					i_free = &bl->elements[i];
				}
			}

			++i_active;

			element * e = i_free;
			i_free = i_free->next;
			e->next = NULL;

			return (T *)e->mem;
		}

		void deallocate(pointer p)
		{
			element * e = (element*)(((char*)p) - sizeof(element*));
			e->next = i_free;
			i_free = e;

			--i_active;
		}

		size_t max_size() const
		{
			return (size_t)(-1) / sizeof(T);
		}

		void construct(pointer p)
		{
			::new((void*)p) value_type();
		}

		void construct(pointer p, const value_type& x)
		{
			::new((void*)p) value_type(x);
		}

		void deconstruct(pointer p)
		{
			p->~value_type();
		}

	private:
		rml_allocator(const rml_allocator &)
		{
		}

		rml_allocator & operator=(const rml_allocator &)
		{
			return *this;
		}

	protected:
		int i_active;
		element * i_free;
		block * i_blocks;
	};

}