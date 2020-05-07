#pragma once
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct json_value;

using json_ptr = std::unique_ptr<json_value>;

using json_ref = const json_value &;

json_ptr json_parse(const std::string &s);

enum class json_type { integer, array, object };

struct json_error : std::runtime_error {
	using std::runtime_error::runtime_error;
};

struct json_value {
	virtual json_type type() const = 0;
	virtual int int_value() const = 0;
	virtual json_ref item_at(int) const = 0;
	virtual json_ref item_at(const std::string &) const = 0;
	virtual int length() const = 0;
	virtual ~json_value() = default;
};

struct json_integer : public json_value {
	int val;

	json_integer(int v) : val(v) {}

	json_type type() const override { return json_type::integer; }
	int int_value() const override { return val; };
	json_ref item_at(int) const override {
		throw std::out_of_range("not collection");
	};
	json_ref item_at(const std::string &) const override {
		throw std::out_of_range("not object");
	};
	int length() const override { return 0; };
};

struct json_array : public json_value {

	std::vector<json_ptr> array;

	json_type type() const override { return json_type::array; }

	int int_value() const override { throw json_error("array in not int!"); }

	json_ref item_at(int index) const override {
		if (index >= static_cast<int>(array.size()) || index < 0) {
			throw std::out_of_range("not object");;
		}
		return *array.at(index).get();
	}
	json_ref item_at(const std::string &) const override {
		throw std::out_of_range("not an object");;
	}

	int length() const override { return static_cast<int>(array.size()); }
};

struct json_object : public json_value {

	std::map<std::string, json_ptr> object;

	json_type type() const override { return json_type::object; }

	int int_value() const override { throw json_error("not int!"); };

	json_ref item_at(int index) const override
	{
		if (index >= static_cast<int>(object.size()) || index < 0) {
			throw std::out_of_range("index is out of range");
		}
		return *std::next(object.begin(), index)->second;
	}

	json_ref item_at(const std::string &str) const override {
		return *object.at(str).get();
	}
	int length() const override { return object.size(); }
};
