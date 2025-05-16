#ifndef IMERGER_H
#define IMERGER_H

#include <vector>
#include "types.h"

template<class T>
    requires requires(T::containerType x) { x; }
class IMerger
{
protected:
    T::containerType const &chunks_; // deque of DataChunks

public:
    using element_type = T::containerType::value_type; // DataChunk
    using value_type = element_type::value_type; // std::string
    virtual ~IMerger() = default;
    IMerger(T::containerType const &cont)
        : chunks_(cont)
    {}
    virtual dSortedCont<value_type> merge() const = 0; // multimap of strings
};

template<class T>
class NaiveMerger final : public IMerger<T>
{
public:
    using IMerger<T>::chunks_;
    using typename IMerger<T>::element_type;
    using typename IMerger<T>::value_type;

    NaiveMerger(T::containerType const &cont)
        : IMerger<T>(cont)
    {}

    dSortedCont<value_type> merge() const override
    {
        dSortedCont<value_type> cont_tmp; // Container of DataChunks
        int chunks_num = chunks_.size();
        std::vector<typename element_type::iterator_type> iters_beg;
        std::vector<typename element_type::iterator_type> iters_end;
        iters_beg.reserve(chunks_num);
        iters_end.reserve(chunks_num);
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
        }
        return cont_tmp;
    }
};

#endif // IMERGER_H
