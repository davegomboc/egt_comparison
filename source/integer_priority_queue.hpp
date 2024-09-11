#ifndef EGT_COMPARISON_INTEGER_PRIORITY_QUEUE_HPP
#define EGT_COMPARISON_INTEGER_PRIORITY_QUEUE_HPP


#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wsign-compare"


// A case of a max-heap priority queue for integers from 0 to N, where the
// elements can be reprioritized.
//
// Why only integers?  So that we can easily build a mapping to know where one
// element is in the queue to be able to reprioritize it -- certainly could be
// generalized.
//
// CRS would have liked to use std::*_heap, but it does have "callbacks" when
// things get moved, preventing keeping track of which element is where (for
// removal or reprioritizing).

// priority_type is the type of the priorities
// value_type is the type of integer being stored in the priority queue
template <typename priority_type, typename value_type = std::size_t>
class integer_priority_queue_type {

    // Needs to support special "-1" as "not in queue" -- or, we'd need
    // to have the value+1 stored, but that's more of a pain.
    //
    // This causes all kinds of "sign-conversion" warnings that we suppress.
    using node_type = long int;

private:
    class heap_element_type {
    public:
        heap_element_type() {}
        heap_element_type(value_type v, priority_type p)
          : the_value(std::move(v)), the_priority(std::move(p)) {}

        priority_type const & priority() const {
            return the_priority;
        }
        priority_type & priority() {
            return the_priority;
        }

        value_type value() const {
            return the_value;
        }

    private:
        value_type the_value;
        priority_type the_priority;
    };

    // Mapping from value_type to its index (node_type) within the heap elements
    std::vector<node_type> the_indices;
    // Mapping from index (node_type) to the corresponding heap element
    std::vector<heap_element_type> the_elements;

    static node_type parent(node_type const & i) {
        assert(0 < i);
        return (i - 1) / 2;
    }

    static node_type left_child(node_type const & i) {
        assert(-1 != i);
        return 2 * i + 1;
    }

    static node_type right_child(node_type const & i) {
        assert(-1 != i);
        return 2 * i + 2;
    }

    bool valid(node_type const & i) const {
        return (0 <= i) and (i < the_elements.size());
    }

    // the "toi" location had better not be worth saving
    void move(node_type const & fromi, node_type const & toi) {
        the_elements[toi] = std::move(the_elements[fromi]);
        the_indices[the_elements[toi].value()] = toi;
    }

    void swap(node_type const & i1, node_type const & i2) {
        using std::swap;
        swap(the_elements[i1], the_elements[i2]);
        the_indices[the_elements[i1].value()] = i1;
        the_indices[the_elements[i2].value()] = i2;
    }

    void set(node_type const & i, heap_element_type e) {
        the_elements[i] = std::move(e);
        the_indices[the_elements[i].value()] = i;
    }

    heap_element_type rem(node_type const & i) {
        the_indices[the_elements[i].value()] = -1;
        return std::move(the_elements[i]);
    }

public:
    // n = number of elements
    integer_priority_queue_type() {}
    integer_priority_queue_type(
        std::vector<priority_type> const & initpriorities
    ) {
        for (value_type i{}; i < initpriorities.size(); ++i) {
            the_elements.emplace_back(i, initpriorities[i]);
            the_indices.emplace_back(i);
        }
        makeheap();
    }

    bool empty() const {
        return the_elements.empty();
    }

    std::size_t size() const {
        return the_elements.size();
    }

    value_type top() const {
        assert(not empty());
        return the_elements[0].value();
    }

    void pop() {
        removeat(0);
    }

    void insert(value_type val, priority_type priority) {
        if (val >= the_indices.size()) the_indices.resize(val + 1);
        else {
            if (the_indices[val] != -1) { // already in queue
                newpriority(std::move(val), std::move(priority));
                return;
            }
        }
        the_elements.resize(the_elements.size() + 1);

        node_type i = the_elements.size() - 1;
        while (i > 0 and priority > the_elements[parent(i)].priority()) {
            move(parent(i), i);
            i = parent(i);
        }
        set(i, heap_element_type(val, priority));
    }

    void newpriority(value_type val, priority_type priority) {
        if (val >= the_indices.size()) return; // not already in queue
        node_type i = the_indices[val];
        if (-1 == i) return; // not already in queue
        if (priority >= the_elements[i].priority()) {
            // Increasing the priority is not a problem.
            the_elements[i].priority() = std::move(priority);
            while (0 < i and the_elements[i].priority()
             > the_elements[parent(i)].priority()) {
                swap(parent(i), i);
                i = parent(i);
            }
        } else {
            // Decreasing the priority can be harder.
            if ((not valid(left_child(i))
             or priority >= the_elements[left_child(i)].priority())
             and (not valid(right_child(i))
             or priority >= the_elements[right_child(i)].priority())) {
                // It turns out that it can stay put.
                the_elements[i].priority() = std::move(priority);
            } else {
                removeat(i);
                insert(std::move(val), std::move(priority));
            }
        }
    }

    void remove(value_type val) {
        node_type i = the_indices[val];
        if (-1 == i) return; // not in queue already
        removeat(i);
    }

private:
    void removeat(node_type i) {
        rem(i);
        if (1 < the_elements.size()) {
            for (; 0 < i; i = parent(i)) {
                move(parent(i), i);
            }
            heap_element_type toins = rem(the_elements.size() - 1);
            the_elements.resize(the_elements.size() - 1);
            insertatfreetop(std::move(toins));
        } else {
            the_elements.resize(0);
        }
    }

    void insertatfreetop(heap_element_type toins) {
        node_type i = 0;
        while (i < the_elements.size()) {
            if (valid(left_child(i))
             and toins.priority() < the_elements[left_child(i)].priority()) {
                if (valid(right_child(i))
                 and the_elements[left_child(i)].priority()
                 < the_elements[right_child(i)].priority()) {
                    move(right_child(i), i);
                    i = right_child(i);
                } else {
                    move(left_child(i), i);
                    i = left_child(i);
                }
            } else if (valid(right_child(i))
             and toins.priority() < the_elements[right_child(i)].priority()) {
                move(right_child(i), i);
                i = right_child(i);
            } else {
                break;
            }
        }
        set(i, std::move(toins));
    }

    void makeheap() {
        for (node_type i = the_elements.size() / 2; i >= 0; i--)
        heapify(i);
    }

    void heapify(node_type i) {
        while (valid(left_child(i))) {
            if (the_elements[i].priority()
             < the_elements[left_child(i)].priority()) {
                if (valid(right_child(i))
                 and the_elements[left_child(i)].priority()
                 < the_elements[right_child(i)].priority()) {
                    swap(i, right_child(i));
                    i = right_child(i);
                } else {
                    swap(i, left_child(i));
                    i = left_child(i);
                }
            } else if (valid(right_child(i)) and the_elements[i].priority()
             < the_elements[right_child(i)].priority()) {
                swap(i, right_child(i));
                i = right_child(i);
            } else break;
        }
    }

public:
    // Intended for debugging purposes only.
    void print(std::ostream & os) const {
        os << "heap:";
        for (auto const & e : the_elements) {
            os << " (" << e.val << ',' << e.pri << ')';
        }
        os << std::endl;
        os << "loc:";
        for (auto const & l : the_indices) {
            os << ' ' << l;
        }
        os << std::endl;
        printtree(os);
    }

    // Intended for debugging purposes only.
    void printtree(std::ostream & os, node_type i = 0, int lvl = 0) const {
        if (valid(left_child(i))) {
            printtree(os, left_child(i), lvl + 7);
        }
        for (int j{}; j < lvl; ++j) os << " ";
        os << "(" << the_elements[i].val << ',' << the_elements[i].pri << ')'
         << std::endl;
        if (valid(right_child(i))) {
            printtree(os, right_child(i), lvl + 7);
        }
    }
};


#pragma GCC diagnostic pop


#endif // EGT_COMPARISON_INTEGER_PRIORITY_QUEUE_HPP
