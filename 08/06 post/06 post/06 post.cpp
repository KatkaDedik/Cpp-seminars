/* The goal of this exercise is simple: take an oriented graph as
 * the input and produce a list (vector) of vertices in the
 * ‘leftmost’ DFS post-order. That is, visit the successors of a
 * vertex in order, starting from leftmost (different exploration
 * order will result in different post-orders). The graph is encoded
 * as a neighbourhood list. */

#include <vector>
#include <map>
#include <cassert>
#include <set>

using graph = std::map< int, std::vector< int > >;

/* Construct the post-order of ‹g› starting from vertex ‹i›. */
std::vector< int > dfs_post(const graph &g, int i) 
{
	const unsigned size = g.size();
	std::set<int> visited;
	std::vector<int> stack;
	stack.push_back(i);
	visited.insert(i);
	std::vector<int> ret;
	ret.push_back(i);

	while (!stack.empty()) {
		int current = stack.back();
		stack.pop_back();

		for (int n : g.at(current)) {
			if (visited.find(n) == visited.end()) 
			{
				stack.push_back(n);
				ret.insert(ret.begin(), n);
				visited.insert(n);
			}
		}
		
	}
	return ret;
}

int main()
{
	graph g{ { 1, { 2, 3, 4 } },
			 { 2, { 1, 2 } },
			 { 3, { 3, 4 } },
			 { 4, {} },
			 { 5, { 3 }} };
	auto o = [](auto... v) { return std::vector< int >{ v... }; };
	assert(dfs_post(g, 5) == o(4, 3, 5));
	assert(dfs_post(g, 2) == o(4, 3, 1, 2));
	assert(dfs_post(g, 1) == o(2, 4, 3, 1));
}
