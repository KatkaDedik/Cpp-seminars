#pragma once
#include "parser.hpp"

#include <functional>
#include <algorithm>

class query_iterator;
class map_iterator;

class path_segment
{
	int int_direction = -1;
	std::string str_direction = "";

public:

	path_segment(int n) : int_direction(n) {}
	path_segment(const std::string &str) : str_direction(str) {}

	bool get_type() const { return str_direction.size() > 0; }
	int get_int() const { return int_direction; }
	const std::string& get_str() const { return str_direction; }
	bool operator!=(const path_segment& segment) const { return (segment.get_int() == int_direction) && (segment.get_str() == str_direction); }
	bool operator==(const path_segment& segment) const { return (segment.get_int() != int_direction) || (segment.get_str() != str_direction); }
};

class path
{
	std::vector<path_segment> segments;
public:
	path() = default;
	path(const std::vector<path_segment> & seg) : segments(seg) {}

	path extend(int input);
	path extend(const std::string& input);
	int front_i() const;
	int back_i() const;
	const std::string& front_s() const;
	const std::string& back_s() const;
	path tail() const;
	bool empty() const;
	path_segment pop();
	path_segment& get(int index);
	size_t size() const;
	bool operator!=(const path& p)const;
	const std::vector<path_segment>& get() const;
};

template <typename Tfrom, typename Tto>
class template_query
{

	std::function<Tto(path, Tfrom)> fun;

public:

	template_query(const std::function<Tto(path, Tfrom)>& f) : fun(f) {}

	map_iterator begin() {}
	map_iterator end() {}
	S operator[](path& segments) const {}
	
	template <typename T>
	template_query<Tto,T> map(std::function<T(path, Tto)>) {}
	bool filter(std::function<bool(path, Tto)>) {}
};

template <typename Tfrom, typename Tto>
class map_iterator
{
	path segments;
	const template_query<Tfrom, Tto>& json;

public:
	map_iterator(path & p, const json_query& j) : segments(p), json(j) {}
	map_iterator& operator++();
	const json_value &operator*();
	bool operator!=(const map_iterator & iterator)const;
};

class json_query
{
	json_ptr json;

public:

	json_query(json_ptr&& p) : json(std::move(p)) {}

	query_iterator begin();
	query_iterator end();
	json_ref operator[](path& segments) const;

	template <typename S>
	template_query<json_ref, S> map(std::function<S(path, const json_value &)> );
	bool filter(std::function<bool( path, const json_value &)>);
};

class query_iterator
{
	path segments;
	const json_query& json;
	
public:
	query_iterator(path & p, const json_query& j) : segments(p), json(j) {}
	query_iterator& operator++();
	const json_value &operator*();
	bool operator!=(const query_iterator & iterator)const;
};

inline json_query query(std::string &input){ return json_query(json_parse(input)); }