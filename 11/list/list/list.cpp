/* In this exercise, we will define a ‹list› class that behaves like
 * the lists in functional programming: the values and structure
 * will be immutable, but it'll be possible to fairly cheaply create
 * new lists by prepending values to existing lists. */

#include <cassert>
#include <memory>

 /* Define a class template ‹list› with a singe type parameter ‹T›,
  * with the following interface (all methods are ‹const›):
  *
  *  • ‹head› returns the value of the current list
  *  • ‹tail› returns the remainder of the list as another ‹list›
  *    instance, without copying any values
  *  • ‹empty› which returns ‹true› if the list is empty
  *  • a default constructor which creates an empty list (i.e. the
  *    ‹[]› constructor you know from Haskell)
  *  • a 2-parameter constructor which takes the value and the tail
  *    of the list (i.e. the ‹(:)› constructor).
  *
  * Hint: It is preferable to store the values «inline» in the nodes.
  * You should also use 2 data types, one for the list itself and
  * another for nodes: this will make it easier to implement empty
  * lists and in general make the implementation nicer. */

template< typename T >
struct node
{
    const T& value;
    std::shared_ptr< node<T> > next;
    node(const T& v, const std::shared_ptr<node<T>>& n) : value(v), next(n) {}

    std::shared_ptr<node<T>>& get_next() const { return next; }

    T& get_value() const { return value; }

};



template< typename T >
class list
{

    std::shared_ptr<node<T>> head_node;

public:

    list<T>() : head_node(nullptr) {}

    list<T>(const T& value, const list<T>& next) : head_node(std::make_shared<node<T>>(value, next.head_node())) {}

    list<T>(const std::shared_ptr<node<T>>& new_head) : head_node(new_head) {}

    T head() const {
        if (!empty()) {
            return head_node->get_value();
        }
        return NULL;
    }

    node<T>& get_head() const { return head_node; }

    list<T> tail()const
    {
        if (!empty()) {
            return list<T>(head_node->get_next());
        }
        return *this;
    }

    bool empty() const {
        return head_node == nullptr;
    }
}; /* ref: 24 lines */

int main()
{
    using ilist = list< int >;
    using ulist = list< std::unique_ptr< int > >;
    auto mku = [](int v) { return std::make_unique< int >(v); };

    ilist i(1, ilist(2, ilist(3, ilist())));
    ulist u(mku(1), ulist(mku(2), ulist()));

    assert(i.head() == 1);
    assert(i.tail().head() == 2);
    assert(i.tail().tail().head() == 3);
    assert(i.tail().tail().tail().empty());

    auto j = i.tail();
    assert(i.tail().head() == j.head());

    assert(*u.head() == 1);
    assert(*u.tail().head() == 2);
    assert(u.tail().tail().empty());

    auto v = u.tail();
    assert(*v.head() == 2);
}
