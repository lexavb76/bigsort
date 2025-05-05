#ifndef ITERATOR_SWITCH_H
#define ITERATOR_SWITCH_H

#include <memory>

template<typename T, typename U>
class IterSwitch
{
    std::unique_ptr<T> it_first_p = nullptr;
    std::unique_ptr<U> it_sec_p   = nullptr;

public:
    explicit IterSwitch(T it) { it_first_p = make_unique<T>(it); }
    explicit IterSwitch(U it) { it_sec_p   = make_unique<U>(it); }
    decltype(auto) operator*()  const { return it_first_p ? *(*it_first_p) : *(*it_sec_p); }
    decltype(auto) operator->() const { return it_first_p ?  (*it_first_p) :  (*it_sec_p); }
    IterSwitch &operator++()
    {
        if (it_first_p)
            ++(*it_first_p);
        else
            ++(*it_sec_p);
        return *this;
    }
    bool operator!=(const IterSwitch &other) const { return  it_first_p ?  (*it_first_p != *other.it_first_p) :  (*it_sec_p != *other.it_sec_p);  }
};

#endif // ITERATOR_SWITCH_H
