#ifndef ITERATOR_SWITCH_H
#define ITERATOR_SWITCH_H

#include <iostream>
#include <istream>
#include <iterator>
#include <memory>

using std::cout;
using std::cerr;
using std::endl;

template<typename T = std::string>
using isIter = std::istream_iterator<T>;

template<typename T,
         typename U,
         typename CharT = char,
         typename Traits = std::char_traits<CharT>,
         typename Distance = std::ptrdiff_t >
class IterSwitch
{
    std::unique_ptr<T> it_first_p = nullptr;
    std::unique_ptr<U> it_sec_p   = nullptr;

public:
    using iterator_category = std::input_iterator_tag;
//    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T::value_type;
    using difference_type = Distance;
    using pointer   = const value_type *;
    using reference = const value_type &;
    using char_type = CharT;
    using traits_type = Traits;
    using istream_type = std::basic_istream<CharT, Traits>;

    explicit IterSwitch(T it)
    {
        cerr << "IterSwich Ctor. ifstream. " << endl;
        it_first_p = std::make_unique<T>(it);
#if 0
//        for (auto &&it = *it_first_p, &&end = isIter<>(); it != end; ++it) {
        for (auto &&end = isIter<>(); it != end; ++it) {
            cerr << "Alex " << *it << endl;
        }
#endif
    }
    explicit IterSwitch(std::unique_ptr<T> &&it) { it_first_p = std::move(it); }
    explicit IterSwitch(U it)
    {
        cerr << "IterSwich Ctor. multimap. " << endl;
        it_sec_p   = std::make_unique<U>(it);
    }
    explicit IterSwitch(std::unique_ptr<U> &&it) { it_sec_p   = std::move(it); }
    decltype(auto) operator*()  const { return it_first_p ? *(*it_first_p) : *(*it_sec_p); }
    decltype(auto) operator->() const { return it_first_p ?  (*it_first_p) :  (*it_sec_p); }
    bool operator==(const IterSwitch &other) const { return it_first_p ? (*it_first_p == *other.it_first_p) : (*it_sec_p == *other.it_sec_p); }
    bool operator!=(const IterSwitch &other) const { return ! (*this == other); }

    IterSwitch &operator++()
    {
        if (it_first_p)
            ++(*it_first_p);
        else
            ++(*it_sec_p);
        return *this;
    }

    IterSwitch operator++(int)
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }
#if 0
    IterSwitch &operator--()
    {
        if (it_first_p)
            --(*it_first_p);
        else
            --(*it_sec_p);
        return *this;
    }

    IterSwitch operator--(int)
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }
#endif
};
#endif // ITERATOR_SWITCH_H
