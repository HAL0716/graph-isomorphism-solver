#pragma once

#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>

namespace Utils {

// Returns a sorted vector from any container
template <typename C>
std::vector<int> sort(const C& c) {
    std::vector<int> r(c.begin(), c.end());
    std::sort(r.begin(), r.end());
    return r;
}

template <typename C, typename Compare>
std::vector<typename C::value_type> sort(const C& c, Compare comp) {
    std::vector<typename C::value_type> r(c.begin(), c.end());
    std::sort(r.begin(), r.end(), comp);
    return r;
}

// Returns the maximum element in a container
template <typename C>
auto max(const C& c) -> decltype(*c.begin()) {
    return *std::max_element(c.begin(), c.end());
}

// Returns true if two containers have any common element
template <typename C1, typename C2>
bool common(const C1& a, const C2& b) {
    if (a.size() > b.size())
        return common(b, a);

    std::unordered_set<typename C1::value_type> s(a.begin(), a.end());
    for (const auto& e : b)
        if (s.count(e))
            return true;
    return false;
}

} // namespace Utils