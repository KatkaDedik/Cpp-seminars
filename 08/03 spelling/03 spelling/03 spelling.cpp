/* The file ‹/usr/share/dict/words› contains one English word per
 * line. Write a class, ‹spell›, the constructor of which takes the
 * path to a file of this type (one word per line) and with a single
 * method, ‹check›, which takes an ‹std::string› which contains a
 * single word and returns ‹true› if the word is in the provided
 * list. */

#include <cassert>
#include <string>
#include <set>
#include <fstream>

class spell
{
	
	std::set<std::string> words;

public:
	spell(std::string path)
	{
		std::ifstream f("../.." + path);
		std::string line;
		while (!std::getline(f, line)) 
		{
			words.insert(line);
		}
		f.close();
	}

	bool check(std::string word) const
	{
		return (words.find(word) != words.end());
	}
};

int main()
{
	const spell sp("/usr/share/dict/words");
	assert(sp.check("carpet"));
	assert(sp.check("pudding"));
	assert(!sp.check("teppich"));
	assert(sp.check("coconut"));
	assert(!sp.check("coconot"));
	assert(!sp.check("onut"));
	assert(!sp.check("gea"));
	assert(sp.check("gear"));
}
