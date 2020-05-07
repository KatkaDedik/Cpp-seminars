/*Write a parser for simplified JSON: in our version, object keys are
barewords (i.e. there is no escaping to deal with) and values are
integers, arrays or objects (no strings or floats). The EBNF:

    (* toplevel elements *)
    value   = blank, ( integer | array | object ), blank ;
    integer = [ '-' ], digits  | '0' ;
    array   = '[', valist, ']' | '[]' ;
    object  = '{', kvlist, '}' | '{}' ;

    (* compound data *)
    valist  = value,  { ',', value } ;
    kvlist  = kvpair, { ',', kvpair } ;
    kvpair  = blank, key, blank, ':', value ;

    (* lexemes *)
    digits  = nonzero, { digit } ;
    nonzero = '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' ;
    digit   = '0' | nonzero ;
    key     = keychar, { keychar } ;
    keychar = ? ASCII upper- or lower-case alphabetical character ? ;
    blank   = { ? ASCII space, tab or newline character ? } ;

It is okay to use ‹std::isspace› to implement the ‹blank› nonterminal. The
interface should be as follows:

    struct json_value;
    using json_ptr = std::unique_ptr< json_value >;
    using json_ref = const json_value &;
    json_ptr json_parse( const std::string &s );
    enum class json_type { integer, array, object };

    struct json_error : std::runtime_error {};

    struct json_value
    {
        virtual json_type type() const = 0;
        virtual int int_value() const = 0;
        virtual json_ref item_at( int ) const = 0;
        virtual json_ref item_at( const std::string & ) const = 0;
        virtual int length() const = 0;
        virtual ~json_value();
    };

Semantic requirements:

 • when a string that is passed to ‹json_parse› does not conform to
   the above grammar, the function should throw an exception of
   type ‹json_error›,
 • a duplicated key within an object should throw ‹json_error› too,
 • the ‹item_at› overloads should throw an instance of
   ‹std::out_of_range› when the specified element does not exist,
 • the ‹item_at› overload which accepts an integer, when called on a
   value of type ‹json_type::object›, should return elements in
   key-alphabetical order,
 • ‹length› should never throw (integers should have a length of 0).
~
*/


/* You can put any private test cases into this file. It will *not* be part of
 * the submission. */

#include "parser.hpp"
#include <cassert>
#include <iostream>


void err_test() {
	bool err = false;
	try {
		json_parse(R"mojeslovo( {  
        "employee": {  
            "name":       "sonoo",   
            "salary":      56000,   
            "married":    true  
        }  
    }   )mojeslovo");
	}
	catch (json_error)
	{
		err = true;
	}
	assert(err);
	err = false;

	try {
		json_parse("01");
	}
	catch (json_error)
	{
		err = true;
	}

	assert(err);
	err = false;

	try {
		json_parse("0 0");
	}
	catch (json_error)
	{
		err = true;
	}

	assert(err);
	err = false;
	
	json_ptr empty = json_parse("{}");
	try {
		empty->item_at(0);
	}
	catch (json_error)
	{
		err = true;
	}

	assert(err);
	err = false;
	try {
		json_parse("{ : 0 }");
	}
	catch (json_error)
	{
		err = true;
	}

	assert(err);
	

	json_ptr k = json_parse("   214748364798 ");

}

void basic_test()
{

	json_ptr j = json_parse("{ cisla : [ 69, 420 ] , Zaporne : [-469, -384], nuLA : 0}");
	assert(j->length() == 3);
	assert(j->type() == json_type::object);
	assert(j->item_at(0).type() == json_type::array);
	assert(j->item_at(0).length() == 2);
	assert(j->item_at(0).item_at(0).int_value() == -469);
	assert(j->item_at("nuLA").type() == json_type::integer);
	assert(j->item_at("nuLA").int_value() == 0);

}

void milanove() {

	bool exceptionThrown = false;

	/* Simple tests */

	assert(json_parse("22")->int_value() == 22);
	assert(json_parse("-22 ")->int_value() == -22);
	assert(json_parse("19990       ")->int_value() == 19990);
	assert(json_parse("0")->int_value() == 0);

	try {
		json_parse("-0");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("-2  a");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("22.2");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("666@    ");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("99: nine");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	assert(json_parse(" [] \n")->length() == 0);
	assert(json_parse("[2]")->item_at(0).int_value() == 2);
	assert(json_parse("[2]")->length() == 1);
	assert(json_parse(" [   265, 2   ]")->item_at(0).int_value() == 265);
	assert(json_parse("[265  , 2   ]")->item_at(1).int_value() == 2);
	assert(json_parse("[265 , 2   ]")->length() == 2);
	assert(json_parse("[19990       ]")->item_at(0).int_value() == 19990);
	assert(json_parse("[19990 \n\n\n\n]")->length() == 1);
	assert(json_parse("[0]")->item_at(0).int_value() == 0);
	assert(json_parse("[-2,   4, -5]    ")->item_at(2).int_value() == -5);

	try {
		json_parse("[  -0]");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("[2");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("[aaaa]");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("    [[]");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse(" [{}");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("[[[[]]]");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	assert(json_parse("{}  ")->length() == 0);
	assert(json_parse("{a:1}")->item_at("a").int_value() == 1);
	assert(json_parse("{a:1}")->item_at(0).int_value() == 1);
	assert(json_parse("  {  aaaa  : 2  }  ")->item_at("aaaa").int_value() == 2);
	assert(json_parse("{ a: [2], b:{}, c:-24 }")->item_at("a").item_at(0).int_value() == 2);
	assert(json_parse("{ a: [2], b:{}, c:-24 }")->item_at("b").length() == 0);
	assert(json_parse("{ a: [2], b:{}, c:-24 }")->item_at("c").int_value() == -24);
	assert(json_parse("{b:2,a:1,c:0}")->item_at(0).int_value() == 1);
	assert(json_parse("{b:2,a:1,c:0}")->item_at(1).int_value() == 2);

	try {
		json_parse("{a:[]");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{ a: -0}");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{[2}");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{a2:2}");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("  {{}");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{{{}}}}");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;


	try {
		json_parse("{a:2,b:3,c:4}")->item_at("d");
	}
	catch (std::out_of_range &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{a:2,b:3,c:4}")->item_at(3);
	}
	catch (std::out_of_range &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{   a  : 2, a : 2222    }");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{x:0,y:0,z:{x:0},u:[1\n0,-1],v:[{}],w:[1]}");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{eb: -8, fbx -8520 }");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	std::cout << "Simple tests - OK" << std::endl;

	/* More sophisticated tests */

	assert(json_parse("4000000000")->length() == 0); // just to see it accepts n > INT_MAX

	assert(json_parse(" [ [ { a : [ {} ] } ] ] \n")->length() == 1);
	assert(json_parse(" [ [ { a : [ {} ] } ] ] \n")->item_at(0).item_at(0).item_at("a").length() == 1);
	assert(json_parse("[ 22, -23, { v : 0 } ]")->item_at(0).int_value() == 22);
	assert(json_parse("[ 22, [ { a : [ -23, { v : 0 } ] } ] ]")->item_at(1).item_at(0).item_at("a").item_at(
		0).int_value() == -23);
	assert(json_parse("[ 22, [ { a : [ -23, { v : 0 } ] } ] ]")->item_at(1).item_at(0).item_at("a").item_at(
		1).item_at("v").int_value() == 0);
	assert(json_parse("[2, [], {}, -2, 1, 0, 1, 0]")->length() == 8);
	assert(json_parse(" [{abc: 2, bca: 3, bac: 4}]")->item_at(0).item_at(1).int_value() == 4);
	assert(json_parse(" [{abc: 2, bca: 3, bac: 4}]")->item_at(0).item_at(2).int_value() == 3);
	assert(json_parse("[[], [], [], {}]")->length() == 4);

	try {
		json_parse("[ ]");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("[ [ [ { a : { } ] } ] ] \n]");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse(" [ [ { a : [ { } ] } ] ] \n a");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("[ 2, [ { a : [ -23, { v : -0 } ] } ] ]");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("[ 2, [ { a : [ -23, { v : 0 } ] } ] ]")->item_at(1).item_at(-1);
	}
	catch (std::out_of_range &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("[ 2, [ { a : [ -23, { v : 0 } ] } ] ]")->item_at(1).item_at(0).item_at("a").item_at(-2);
	}
	catch (std::out_of_range &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("\n[ 22, [ { a [ -23, { v : 0 } ] } ] ]\n");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("[2, [-], {}, -2, 1, 0, 1, 0]");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("[2, ,[], {}, -2, 1, 0, 1, 0]");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	assert(json_parse("{aa:[2, {bc:0\n}]}  \n\n\n\n")->length() == 1);
	assert(json_parse("{aa:[2, {bc:0\n}]}  \n\n\n\n")->item_at("aa").item_at(0).int_value() == 2);
	assert(json_parse("{aa:[2, {bc:0\n}]}  \n\n\n\n")->item_at(0).item_at(1).item_at("bc").int_value() == 0);

	assert(json_parse("{x:{}, y:{}, z:{}}")->length() == 3);
	assert(json_parse("{x:{  a : []  }, z:{  a : []  }, y:{  a : [2]  }}")->length() == 3);
	assert(json_parse("{x:{ a : []  }, z:{ a : []  }, y:{  a : [2]  }}")->item_at("x").item_at("a").length() == 0);
	assert(json_parse("{x:{  a : []  }, z:{ a : []  }, y:{  a : [2]  }}")->item_at(1).item_at(0).item_at(0).int_value() == 2);

	assert(json_parse("  {  aaaa  : 2  , aa: 0 , aaa: [{a:[2]}]}  ")->item_at("aaaa").int_value() == 2);
	assert(json_parse("  {  aaaa  : 2  , aa: 0 , aaa: [{a:[2]}]}  ")->item_at("aa").int_value() == 0);
	assert(json_parse("  {  aaaa  : 2  , aa: 0 , aaa: [{a:[2]}]}  ")->item_at(0).int_value() == 0);
	assert(json_parse("  {  aaaa  : 2  , aa: 0 , aaa: [{a:[2]}]}  ")->item_at(2).int_value() == 2);
	assert(json_parse("  {  aaaa  : 2  , aa: 0 , aaa: [{a:[2]}]}  ")->item_at("aaa").item_at(0).item_at("a").item_at(
		0).int_value() == 2);

	try {
		json_parse("{ }");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{aa:[2, {bc:0\n}]}  \n\n\n\n a:2");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{{aa:[2, {bc:0\n}]}  \n\n\n\n");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse(" { aa:[2, {bc 0\n}]}  \n\n\n\n");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{x:{  []  }, z:{  [ ]  }, y:{  [2]  }}");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{x:{  a : []  }, z:{  a : []  }, y:{  a : [2]  }}")->item_at("t");
	}
	catch (std::out_of_range &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{x:{  a : []  }, z:{ a : []  }, y:{ a : [2]  }}")->item_at(1).item_at(1);
	}
	catch (std::out_of_range &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("{x:{  a : []  }, z:{ a : []  }, y:{ a : [2]  }}")->item_at(1).item_at(-1);
	}
	catch (std::out_of_range &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("  {  aaaa  : 2  , aa: 0 , aaa: [{a:[2]}}     ");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("  {  aaaa  : 2  , aa: 0 , aaa: [{a:[2,,]}]} ");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("  {  aa23aa  : 2  , aa: 0 , aaa: [{a:[2]}]} ");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("  {  aaaa  :: 2  , aa: 0 , aaa: [{a:[2]}]} ");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("  {  aa.aa  : 2  , aa: 0 , aaa: [{a:[2]}]} ");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	try {
		json_parse("  {  aaaa  : t  , aa: 0 , aaa: [{a:[2]}]} ");
	}
	catch (json_error &) {
		exceptionThrown = true;
	}
	assert(exceptionThrown);
	exceptionThrown = false;

	std::cout << "Sophisticated tests - OK" << std::endl;

}

int main()
{

	err_test();
	basic_test();
	assert(1);
}
