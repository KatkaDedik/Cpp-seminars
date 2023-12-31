/* More data structures. A bit trie (or a bitwise trie, or a bitwise
 * radix tree) is a �binary� tree for encoding a set of binary
 * values, with quick insertion and lookup. Each edge in the tree
 * encodes a single bit (i.e. it carries a zero or a one). To make
 * our life easier, we will represent the keys using a vector of
 * booleans. */

#include <vector>
#include <cassert>
#include <memory>

 /* The key is a sequence of bits: iteration order (left to right)
  * corresponds to a path through the trie starting from the root.
  * I.e. the leftmost bit decides whether to go left or right from
  * the root, and so on. A key is present in the trie iff it
  * describes a path to a leaf node. */

using key = std::vector< bool >;

struct trie_node {
	std::unique_ptr<trie_node > left = nullptr;
	std::unique_ptr<trie_node > right = nullptr;
}; /* ref: 5 lines */

/* For simplicity, we will not have a normal �insert� method.
 * Instead, the trie will expose its root node via �root� and allow
 * explicit creation of new nodes via �make�, which accepts the
 * parent node and a boolean as arguments (the latter indicating
 * whether the newly created edge represents a 0 or a 1). Both
 * �root� and �make� should return node references. Finally, add a
 * �has� method which will check whether a given key is present in
 * the �trie�. */

class trie {

	trie_node rootNode = {};

public:
	trie() = default;

	bool has(key k) {
		trie_node* currentNode = &root();
		for (const auto & value : k) {
			if (value) {
				currentNode = currentNode->left.get();
			}
			else {
				currentNode = currentNode->right.get();
			}
			if (currentNode == nullptr) {
				return false;
			}
		}
		return true;
	}

	trie_node& root() { 
		return rootNode; 
	}

	const trie_node& root() const{
		return rootNode;
	}

	trie_node& make(trie_node& node, bool value) {

		if (value) {
			node.left = std::make_unique<trie_node>();
			return *node.left;
		}
		else {
			node.right = std::make_unique<trie_node>();
			return *node.right;
		}
	}
}; /* ref: 21 lines */

int main()
{
	trie x;

	assert(!x.has(key{ true }));
	assert(!x.has(key{ false }));

	auto &t = x.make(x.root(), true);
	assert(x.has(key{ true }));

	x.make(t, true);
	auto &tf = x.make(t, false);

	assert(x.has(key{ true }));
	assert(!x.has(key{ false }));
	assert(!x.has(key{ false, true }));
	assert(!x.has(key{ false, false }));
	assert(x.has(key{ true, true }));
	assert(x.has(key{ true, false }));
	assert(!x.has(key{ true, false, true }));

	x.make(tf, true);
	assert(x.has(key{ true, false, true }));
}
