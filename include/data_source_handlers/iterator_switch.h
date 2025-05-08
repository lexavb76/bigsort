#ifndef ITERATOR_SWITCH_H
#define ITERATOR_SWITCH_H

#include <concepts>
#include <cstddef>
#include <iostream>
#include <istream>
#include <iterator>
#include <memory>
#include <string>

using std::cout;
using std::cerr;
using std::endl;

/**
 * @brief The IterSwitch class
 * As std::istream_iterator<std::string> treats whitespaces as a delimiter also,
 * we have to handle it ourselves.
 * Also we want to iterate in the same way through files and containers.
 */
template<std::derived_from<std::istream> T, // Use C++20 concepts here
         std::input_iterator             U,
         class CharT                   = char,
         class Traits                  = std::char_traits<CharT>,
         class Distance                = std::ptrdiff_t>
class IterSwitch
{
public:
    // Define types aliases for std::iterator_traits:
    using iterator_category = std::input_iterator_tag;
    using value_type        = std::iterator_traits<U>::value_type;
    using difference_type   = Distance;
    using pointer           = const value_type *;
    using reference         = const value_type &;
    using char_type         = CharT;
    using traits_type       = Traits;
    using istream_type      = std::basic_istream<CharT, Traits>;
    //***
    template<class V = value_type>
    using isIter = std::istream_iterator<V>;

private:
    std::shared_ptr<isIter<>> it_curr_p_ = nullptr;
    std::shared_ptr<isIter<>> it_prev_p_ = nullptr;
    std::shared_ptr<U>        it_sec_p_  = nullptr;
    istream_type             *is_p_      = nullptr;
    value_type                value_curr_;

public:
    IterSwitch()                              = delete;
    IterSwitch &operator=(const IterSwitch &) = delete;
    IterSwitch &operator=(IterSwitch &&)      = delete;

    explicit IterSwitch(T &is)
        : is_p_(&is)
    {
        // cerr << "IterSwich Ctor. ifstream. Begin" << endl;
        it_prev_p_ = it_curr_p_ = std::make_shared<isIter<>>(is);
    }

    explicit IterSwitch(std::nullptr_t) // Is end iterator
    {
        // cerr << "IterSwich Ctor. ifstream. End" << endl;
        it_prev_p_ = it_curr_p_ = std::make_shared<isIter<>>();
    }

    explicit IterSwitch(U it)
    {
        // cerr << "IterSwich Ctor. multimap. " << endl;
        it_sec_p_ = std::make_shared<U>(it);
    }

    IterSwitch(const IterSwitch &) { cerr << "IterSwich Copy Ctor." << endl; }

    IterSwitch(IterSwitch &&other)
        : IterSwitch(other)
    {
        cerr << "IterSwich Move Ctor." << endl;
    }

    ~IterSwitch() { cerr << "IterSwich Dtor." << endl; }

    decltype(auto) operator->() const { return it_curr_p_ ? (*it_curr_p_) : (*it_sec_p_); }
    bool operator==(const IterSwitch &other) const
    {
        // cerr << "Operator ==" << endl;
        return it_prev_p_ ? (*it_prev_p_ == *other.it_prev_p_) : (*it_sec_p_ == *other.it_sec_p_);
    }
    bool operator!=(const IterSwitch &other) const { return !(*this == other); }

    IterSwitch &operator++()
    {
        // cerr << "Operator ++" << "; this = " << this << endl;
        if (it_curr_p_) {
            it_prev_p_ = std::make_shared<isIter<>>(*it_curr_p_); //Deep copy of the iterator
            std::string str;
            std::getline(*is_p_, str);
            value_curr_ = **it_curr_p_ + str; // Take the first value, eaten by the iterator when initialized
            // cerr << **it_curr_p_ << " ---> iterator = " << std::hex << it_curr_p_ << endl;
            // cerr << str << " ---> getline" << endl;
            it_curr_p_ = std::make_shared<isIter<>>(*is_p_); //Points to the current position in file
        } else
            ++(*it_sec_p_);
        return *this;
    }

    IterSwitch operator++(int)
    {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    value_type operator*()
    {
        // cerr << "Operator *" << " ---> iterator = " << std::hex << it_curr_p_ << "; this = " << this << endl;
        if (it_curr_p_) {
            if (value_curr_.empty())
                ++(*this); // The first iteration. Step forward to read the first value from iterator.
            return value_curr_;
        }
        // cerr << "Operator *. Multimap" << endl;
        return static_cast<value_type>( *(*it_sec_p_));
    }
};
#endif // ITERATOR_SWITCH_H
