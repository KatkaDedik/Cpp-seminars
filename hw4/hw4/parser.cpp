#include "parser.hpp"

json_ptr json_parse_recursive(const std::string &s, size_t &index);

void skip_space(const std::string &s, size_t &index)
{
	for (; index < s.size(); index++) {
		if (!isspace(s[index]))
		{
			break;
		}
	}
}

json_integer integer_parse(const std::string &s, size_t &index)
{
	bool negative = false;
	const std::string & number = "0123456789";
	if (s[index] == '-') {
		index++;
		if (s[index] == '0' || index == s.size()) {
			throw json_error("invalid number");
		}
		negative = true;
	}

	int integer = std::atoi(s.data() + index);

	while (number.find(s[index]) != -1) {
		index++;
	}
	
	if (negative) {
		return json_integer{ - integer };
	}
	return json_integer{ integer };

}

json_array array_parse(const std::string &s, size_t &index)
{
	index++;
	if (index == s.size()) {
		throw json_error("invalid array");
	}
	if (s[index] == ']') {
		index++;
		return json_array{};
	}
	
	json_array a;

	index--;

	do{
		index++;
		if (index == s.size()) {
			throw json_error("invalid array");
		}
		a.array.emplace_back(json_parse_recursive(s, index));

		if (index == s.size()) {
			throw json_error("invalid array");
		}
	} while (s[index] == ',');

	if (s[index] == ']') {
		index++;
		return a;
	}
	throw json_error("invalid array");
}

json_object object_parse(const std::string &s, size_t &index)
{
	index++;
	if (index == s.size()) {
		throw json_error("invalid object");
	}
	if (s[index] == '}') {
		index++;
		return json_object{};
	}

	json_object obj;

	index--;

	do {
		index++;
		if (index == s.size()) {
			throw json_error("invalid object");
		}

		skip_space(s, index);

		size_t begin = index;
		while ((s[index] >= 'A' && s[index] <= 'Z') || (s[index] >= 'a' && s[index] <= 'z')) {
			index++;
		}
		if (index == begin) {
			throw json_error("key cannot be empty!");
		}
		std::string key = s.substr(begin, index - begin);

		skip_space(s, index);


		if (s[index] != ':') {
			throw json_error("invalid object");
		}
		index++;

		obj.object.emplace(std::make_pair(key, json_parse_recursive(s, index)));
		
	} while (s[index] == ',');

	if (s[index] == '}') {
		index++;
		return obj;
	}
	throw json_error("invalid object");

}

json_ptr json_parse_recursive(const std::string &s, size_t &index)
{
	const std::string & nonzero = "-123456789";
	json_ptr ret;

	skip_space(s, index);
	
	if (nonzero.find(s[index]) != -1)
	{
		ret = std::make_unique<json_integer>(integer_parse(s, index));
	}
	else 
	{
		switch (s[index])
		{
		case '[':
			ret = std::make_unique<json_array>(array_parse(s, index));
			break;
		case '{':
			ret = std::make_unique<json_object>(object_parse(s, index));
			break;
		case '0':
			if (nonzero.find(s[index + 1]) != -1) {
				throw json_error("invalid input");
			}
			ret = std::make_unique<json_integer>(json_integer{ 0 });
			index++;
			break;
		default:
			throw json_error("invalid input");
		}
	}
	skip_space(s, index);

	return ret;
}

json_ptr json_parse(const std::string &s) 
{
	size_t index = 0;
	json_ptr ret = json_parse_recursive(s, index);

	if (index != s.size()) {
		throw json_error("invalid input");
	}

	return ret;
}
