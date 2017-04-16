/*
*	rml
*
*	Author: SiZhong.Wang(M-001)
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include <string>
#include "MRmlAllocator.h"

#ifndef RML_STRING
typedef std::string rml_string;
#endif

#ifndef RML_ALLOC
#define rml_alloc malloc
#define rml_free free
#endif

namespace Rad {

	class rml_document;

	class rml_node
	{
	public:
		rml_node(rml_document * pdoc, char * n, char * v, int p);
		~rml_node();

		const char * 
			name() const { return _name; }
		const char *
			value() const { return _value; }

		void
			set_value(const char * value);

		rml_node * 
			first_node() const;
		rml_node * 
			first_node(const char * name) const;
		rml_node * 
			next_sibling() const;
		rml_node * 
			next_sibling(const char * name) const;
		
		rml_node * 
			append(const char * name, const char * value);
		void 
			clear();

		void 
			print(rml_string & o_str, const rml_string & prefix) const;

	protected:
		char * 
			_parse_child(char * str);
		rml_node *
			_append_impl(char * name, char * value);
		
	protected:
		rml_document * _document;
		int _flag;
		char * _name;
		char * _value;

		rml_node * _next;
		rml_node * _linker_beg;
		rml_node * _linker_end;
	};

	//
	class rml_document : public rml_node
	{
		friend class rml_node;

	public:
		rml_document();
		~rml_document();

		bool 
			open(const char * filename);
		bool 
			parse(char * data, bool _alloc = false);
		void 
			clear();

		void
			print(rml_string & str);
		bool 
			save(const rml_string & filename);
		bool 
			save(FILE * fp);

	protected:
		char * i_data;
		rml_allocator<rml_node, 128> i_allocator;
	};

}
