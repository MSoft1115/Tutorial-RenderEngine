#include "MRml.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

namespace Rad {

#define RML_FLAG_POOL_NAME 1
#define RML_FLAG_POOL_VALUE 2
#define RML_FLAG_POOL_ALL 3

	rml_node::rml_node(rml_document * pdoc, char * n, char * v, int p)
		: _document(pdoc)
		, _name(n)
		, _value(v)
		, _flag(p)
		, _next(NULL)
		, _linker_beg(NULL)
		, _linker_end(NULL)
	{
	}

	rml_node::~rml_node()
	{
		clear();

		if (_flag & RML_FLAG_POOL_NAME)
		{
			rml_free(_name);
		}

		if (_flag & RML_FLAG_POOL_VALUE)
		{
			rml_free(_value);
		}
	}

	void rml_node::set_value(const char * value)
	{
		if (_flag & RML_FLAG_POOL_VALUE)
		{
			rml_free(_value);
		}
		else
		{
			_flag |= RML_FLAG_POOL_VALUE;
		}

		_value = (char *)rml_alloc(strlen(value) + 1);
		strcpy(_value, value);
	}

	rml_node * rml_node::first_node() const
	{
		return _linker_beg;
	}

	rml_node * rml_node::first_node(const char * name) const
	{
		rml_node * node = first_node();

		while (node != NULL)
		{
			if (strcmp(node->_name, name) == 0)
				return node;

			node = node->next_sibling();
		}

		return NULL;
	}

	rml_node * rml_node::next_sibling() const
	{
		return _next;
	}

	rml_node * rml_node::next_sibling(const char * name) const
	{
		rml_node * node = next_sibling();

		while (node != NULL)
		{
			if (strcmp(node->_name, name) == 0)
				return node;

			node = node->next_sibling();
		}

		return NULL;
	}

	rml_node * rml_node::append(const char * name, const char * value)
	{
		char * n = (char *)rml_alloc(strlen(name) + 1);
		char * v = (char *)rml_alloc(strlen(value) + 1);

		strcpy(n, name);
		strcpy(v, value);

		rml_node * node = _document->i_allocator.allocate();
		::new((void*)node) rml_node(_document, n, v, RML_FLAG_POOL_ALL);

		if (_linker_beg != NULL)
		{
			_linker_end->_next = node;
			_linker_end = node;
		}
		else
		{
			_linker_beg = _linker_end = node;
		}

		return node;
	}

	rml_node * rml_node::_append_impl(char * name, char * value)
	{
		rml_node * node = _document->i_allocator.allocate();
		::new((void*)node) rml_node(_document, name, value, 0);

		if (_linker_beg != NULL)
		{
			_linker_end->_next = node;
			_linker_end = node;
		}
		else
		{
			_linker_beg = _linker_end = node;
		}

		return node;
	}

	void rml_node::clear()
	{
		rml_node * node = _linker_beg;
		while (node != NULL)
		{
			rml_node * next = node->_next;

			node->~rml_node();
			_document->i_allocator.deallocate(node);

			node = next;
		}

		_linker_beg = _linker_end = NULL;
	}

	void rml_node::print(rml_string & o_str, const rml_string & prefix) const
	{
		rml_string line_str;

		if (strchr(_value, '\n') == NULL)
		{
			line_str = prefix + _name + " " + _value + "\n";
		}
		else
		{
			if (_value[strlen(_value) - 1] == '\n')
				line_str = prefix + _name + " {" + _value + prefix + "}\n";
			else
				line_str = prefix + _name + " {" + _value + "\n" + prefix + "}\n";
		}

		o_str += line_str;

		rml_node * node = first_node();

		if (node != NULL)
		{
			o_str += prefix;
			o_str += "{\n";

			while (node != NULL)
			{
				node->print(o_str, prefix + "\t");

				node = node->next_sibling();
			}

			o_str += prefix;
			o_str += "}\n";
		}

	}

	char * _rml_skin_line(char * str)
	{
		while (*str)
		{
			if (*str++ == '\n')
				break;
		}

		return str;
	}

	bool _rml_is_white(char ch)
	{
		return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
	}

	char * _rml_skip_white(char * str)
	{
		while (_rml_is_white(*str))
		{
			++str;
		}

		return str;
	}

	char * rml_node::_parse_child(char * str)
	{
		rml_node * node = NULL;

		char * name = NULL;
		char * value = NULL;
		int nameLen = 0;
		int valueLen = 0;

		while (*str != 0)
		{
			name = NULL;
			value = NULL;
			nameLen = 0;
			valueLen = 0;

			// trim
			str = _rml_skip_white(str);

			// single-comment
			if (*str == '/' && *(str + 1) == '/')
			{
				str = _rml_skin_line(str);
			}

			// multi-comment
			if (*str == '/' && *(str + 1) == '*')
			{
				while (*str)
				{
					str = _rml_skin_line(str);
					str = _rml_skip_white(str);

					if (*str == '*' && *(str + 1) == '/')
					{
						str += 2;
						break;
					}
				}
			}

			// trim
			str = _rml_skip_white(str);

			// parse name
			name = str;
			while (*str)
			{
				if (_rml_is_white(*str))
					break;

				++nameLen;
				++str;
			}

			// trim
			while (*str)
			{
				if (*str != ' ' && *str != '\t')
					break;

				++str;
			}

			// parse value
			if (*str && *str == '{')
			{
				++str;

				value = str;

				int depth = 0;
				while (*str)
				{
					if (*str == '{')
					{
						++depth;
					}
					else if (*str == '}')
					{
						if (depth-- <= 0)
							break;
					}

					++valueLen;
					++str;
				}
			}
			else
			{
				value = str;
				while (*str)
				{
					if (*str == '\r' || *str == '\n')
						break;

					++valueLen;
					++str;
				}
			}

			str = _rml_skin_line(str);

			// trim value
			while (valueLen > 0)
			{
				if (value[valueLen - 1] != ' ' && value[valueLen - 1] != '\t')
					break;

				--valueLen;
			}

			name[nameLen] = 0;
			value[valueLen] = 0;

			if (*name == '{' && node != NULL)
			{
				str = node->_parse_child(str);
			}
			else if (*name == '}')
			{
				break;
			}
			else if (*name != '\0')
			{
				node = _append_impl(name, value);
			}
		}

		return str;
	}


	//
	rml_document::rml_document()
		: rml_node(this, NULL, NULL, 0)
		, i_data(NULL)
	{
	}

	rml_document::~rml_document()
	{
		clear();
	}

	bool rml_document::open(const char * filename)
	{
		clear();

		FILE * fp = fopen(filename, "rb");
		if (fp != NULL)
		{
			int length = 0;
			int cur = ftell(fp);

			fseek(fp, 0, SEEK_END);
			length = ftell(fp);
			fseek(fp, cur, SEEK_SET);

			i_data = (char *)rml_alloc(length + 1);
			fread(i_data, length, 1, fp);
			i_data[length] = 0;

			_parse_child(i_data);
		}

		return first_node() != NULL;
	}

	bool rml_document::parse(char * data, bool _alloc)
	{
		clear();

		if (_alloc)
		{
			int length = strlen(data);
			i_data = (char *)rml_alloc(length + 1);
			memcpy(i_data, data, length);
			i_data[length] = 0;
			
			data = i_data;
		}

		_parse_child(data);

		return first_node() != NULL;
	}

	void rml_document::clear()
	{
		rml_node::clear();

		if (i_data)
		{
			rml_free(i_data);
			i_data = NULL;
		}

		i_allocator.clear();
	}

	void rml_document::print(rml_string & str)
	{
		rml_node * n = first_node();
		while (n != NULL)
		{
			n->print(str, "");

			n = n->next_sibling();
		}
	}

	bool rml_document::save(const rml_string & filename)
	{
		FILE * fp = fopen(filename.c_str(), "wb");
		if (fp)
		{
			save(fp);
			fclose(fp);

			return true;
		}

		return false;
	}

	bool rml_document::save(FILE * fp)
	{
		rml_string str;

		print(str);

		fwrite(str.c_str(), 1, str.length(), fp);

		return true;
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif