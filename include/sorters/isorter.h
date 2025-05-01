#ifndef ISORTER_H
#define ISORTER_H

#include <cstdio>
#include <iostream>

using std::cout;
using std::endl;
/**
 * @brief The ISorter class.
 * Interface for Sort algorithms. Uses CRTP pattern instead of virtual functions.
 */
template<typename Derived>
class ISorter
{};

/**
 * @brief The QuickSorter class.
 * Implements Quick Sort algorithm.
 */
class QuickSorter : public ISorter<QuickSorter>
{
public:
    QuickSorter() { std::cout << "QSorter default ctor" << std::endl; }
};


#endif // ISORTER_H
