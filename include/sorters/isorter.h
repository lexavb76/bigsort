#ifndef ISORTER_H
#define ISORTER_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <vector>
#include <fstream>
#include "imerger.h"
#include "types.h"
#include "iterator_switch.h"

#define iSorterType ISorter<QuickSorter<T>>

/**
 * @brief The ISorter class.
 * Interface for Sort algorithms. Uses CRTP pattern instead of virtual functions.
 */
template<typename Derived>
class ISorter
{
protected:
    ~ISorter() {} // Protected Dtor to avoid unexpected deletion through base class pointer
public:
    enum mode_e { MODE_MERGE_ONLY = 0 };
};

/**
 * @brief The QuickSorter class.
 * Implements Quick Sort algorithm.
 */
template<class T> // Use template to get any data source type object
    requires requires(T::containerType x) { x; }
class QuickSorter : public iSorterType
{
    T::containerType const &chunks_;
    iSorterType::mode_e mode_;
    std::shared_ptr<IMerger<T>> mrg_p_{new NaiveMerger<T>(chunks_)};

public:
    QuickSorter() { /* std::clog << "QSorter default ctor" << std::endl; */ }
    QuickSorter(const QuickSorter &)            = default;
    QuickSorter(QuickSorter &&)                 = delete;
    QuickSorter &operator=(const QuickSorter &) = delete;
    QuickSorter &operator=(QuickSorter &&)      = delete;

    QuickSorter(const T::containerType &chunks, iSorterType::mode_e mode)
        : chunks_(chunks)
        , mode_(mode)
    {}

    QuickSorter(const T::containerType &chunks, iSorterType::mode_e mode, std::shared_ptr<IMerger<T>> mrg_p)
        : chunks_(chunks)
        , mode_(mode)
        , mrg_p_(mrg_p)
    {}

    void sorted_print()
    {
        auto cont_tmp = mrg_p_->merge();
        for (auto &&val : cont_tmp)
            cout << val << endl;
    }
};


#endif // ISORTER_H
