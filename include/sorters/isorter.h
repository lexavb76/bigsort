#ifndef ISORTER_H
#define ISORTER_H

#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include <fstream>
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

public:
    inline QuickSorter() { /* std::clog << "QSorter default ctor" << std::endl; */ }
    QuickSorter(const QuickSorter &)            = default;
    QuickSorter(QuickSorter &&)                 = delete;
    QuickSorter &operator=(const QuickSorter &) = delete;
    QuickSorter &operator=(QuickSorter &&)      = delete;

    QuickSorter(const T::containerType &chunks, iSorterType::mode_e mode)
        : chunks_(chunks)
        , mode_(mode)
    {}

    void print()
    {
        dSortedCont<> cont_tmp;
        int chunks_num = chunks_.size();
        std::vector<IterSwitch<std::ifstream, decltype(cont_tmp.begin())>> iters_beg;
        std::vector<IterSwitch<std::ifstream, decltype(cont_tmp.end())>>   iters_end;
        iters_beg.reserve(chunks_num);
        iters_end.reserve(chunks_num);
        if (mode_ == iSorterType::MODE_MERGE_ONLY) {
            for (auto &&chunk : chunks_) { // Get iterators for all chunks
                // clog << "==========================================================================" << endl;
                iters_beg.push_back(chunk.begin());
                iters_end.push_back(chunk.end());
            }
            bool touch = true;
            while (touch) { // Go merge chunks!
                touch = false;
                for (int i = 0; i < chunks_num; ++i) {
                    if (iters_beg[i] != iters_end[i]) {
                        cont_tmp.insert(*iters_beg[i]);
                        ++iters_beg[i];
                        touch = true;
                    }
                }
                for (auto &&val : cont_tmp) {
                    cout << val << endl;
                }
                cont_tmp.clear();
            }
        }
    }
};


#endif // ISORTER_H
