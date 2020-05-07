#include "query.hpp"

/***********PATH IMPLEMENTATION***********/

path path::extend(int input)
{
	segments.push_back(path_segment(input));
	return *this;
}

path path::extend(const std::string& input)
{
	segments.push_back(path_segment(input));
	return *this;
}

int path::front_i() const { return segments[0].get_int(); }

int path::back_i() const { return segments.back().get_int(); }

const std::string& path::front_s() const { return segments[0].get_str(); }

const std::string& path::back_s() const { return segments.back().get_str(); }

path path::tail() const 
{ 
	return path(std::vector<path_segment>(++segments.begin(), segments.end())); 
}

bool path::empty() const { return segments.empty(); }

path_segment path::pop()
{
	path_segment ret_segment = segments.back();
	segments.pop_back();
	return ret_segment;
}

path_segment& path::get(int index) { return segments[index]; }

size_t path::size() const { return segments.size(); }

bool path::operator!=(const path& p) const 
{
	return p.segments != segments;
}

const std::vector<path_segment>& path::get() const
{
	return segments;
}


/***********JSON_QUERY***IMPLEMENTATION***********/

query_iterator json_query::begin() 
{
	path p;
	const json_value* current = json.get();
	while (current->length() > 0) {
		current = &(current->item_at(0));
		p.extend(0);
	}
	return query_iterator(p, *this);
}

query_iterator json_query::end() 
{
	path p;
	p.extend(-1);
	return query_iterator(p, *this);
}

const json_value& json_query::operator[](path& segments) const
{
	const json_value* current = json.get();
	for (size_t i = 0; i < segments.size(); i++) {
		if (segments.get(i).get_type()) {
			current = &(current->item_at(segments.get(i).get_str()));
		}
		else {
			current = &(current->item_at(segments.get(i).get_int()));
		}
	}
	return *current;
}

bool filter(std::function<bool(path, const json_value &)>) 
{
	return true;
}


/***********QUERY_ITERATOR***IMPLEMENTATION***********/

query_iterator& query_iterator::operator++() {

	if (segments.size() == 0) {
		segments.extend(-1);
		return *this;
	}

	path_segment segment = segments.pop();
	if (json[segments].length() > segment.get_int() + 1) {
		segments.extend(segment.get_int() + 1);
	}
	return *this;
}

const json_value& query_iterator::operator*() 
{
	return json[segments];
}


bool query_iterator::operator!=(const query_iterator & iterator)const 
{
	auto res = std::mismatch(iterator.segments.get().begin(), iterator.segments.get().end(), segments.get().begin(), segments.get().end());
	return (std::get<0>(res) != iterator.segments.get().end() || std::get<1>(res) != segments.get().end());
}
