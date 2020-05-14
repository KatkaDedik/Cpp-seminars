/* A regular grammar has rules of the form ⟦A → xB⟧ or ⟦A → x⟧ where
 * ⟦A⟧ and ⟦B⟧ are non-terminals and ⟦x⟧ is a terminal. */

#include <cassert>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm> 

 /* Implement a class ‹grammar›, which is default-constructible and
  * has 2 methods:
  *
  *  • ‹add_rule›, which takes 2 or 3 arguments: a single ‹char› for
  *    the left-hand non-terminal (a capital alphabetic letter), a
  *    terminal and optionally another non-terminal,
  *  • ‹generate›, a ‹const› method which takes 2 arguments: the
  *    starting non-terminal and an integer which gives the maximum
  *    length of a word, and returns an ‹std::vector› of ‹std::string›
  *    with all the «words» the grammar can generate (within the given
  *    size bound), sorted lexicographically. */




class grammar
{

    std::map<char, std::vector<std::pair<char, char>>> rules;

public:

    grammar() = default;

    void add_rule(char non_term, char term, char optional = 0) 
    {
        if (rules.find(non_term) == rules.end()) {
            rules.insert({ non_term, std::vector<std::pair<char, char>>() });
        }
        rules.at(non_term).push_back(std::make_pair(term, optional));
    }



    std::vector<std::string> generate(char starting_non_terminal, int max_length, std::string start_of_word = "") const 
    {
        std::vector<std::string> ret;

        if (max_length == 0) {
            return ret;
        }

        auto found = rules.find(starting_non_terminal);
        if (found == rules.end()) {
            return ret;
        }
        else {
            for (auto r : rules.at(starting_non_terminal)) {
                std::string word = start_of_word + r.first;

                if (r.second != NULL) {
                    std::vector<std::string> tmp = generate(r.second, max_length - 1, word);
                    for (auto t : tmp) {
                        ret.push_back(t);
                    }
                }
                else {
                    ret.push_back(word);
                }
            }
        }
        std::sort(ret.begin(), ret.end());
        return ret;
    }
};

int main()
{
    auto w = [](auto... words) /* helper function */
    {
        return std::vector< std::string >{ words... };
    };

    grammar g;
    const grammar& cg = g;

    assert(cg.generate('A', 10) == w());
    g.add_rule('A', 'a');
    assert(cg.generate('A', 10) == w("a"));

    g.add_rule('A', 'b', 'A');
    assert(cg.generate('A', 3) == w("a", "ba", "bba"));

    g.add_rule('A', 'x', 'X');
    g.add_rule('X', 'x');
    assert(cg.generate('A', 3) == w("a", "ba", "bba", "bxx", "xx"));
    g.add_rule('X', 'x', 'X');
    assert(cg.generate('X', 3) == w("x", "xx", "xxx"));
}
