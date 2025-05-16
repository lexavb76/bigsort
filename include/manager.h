#include "file_data_source.h"
#include <memory>

#ifndef MANAGER_H
#define MANAGER_H

template<class Sorter,
         class Data = FileDataSource>
class Manager
{
    const char *src_name_ = nullptr;
    Data d_obj_;
    std::unique_ptr<Sorter> sorter_p_;

public:
    explicit Manager(std::istream &is)
      : d_obj_(is)
    {}

    explicit Manager(const char *src)
      : src_name_(src),
        d_obj_(src_name_)
    {}

    int run()
    {
        sorter_p_ = std::make_unique<Sorter>(d_obj_.get_d_chunk_vec(), Sorter::MODE_MERGE_ONLY);

#if 0
        auto & chunk_vec = d_obj_.get_d_chunk_vec();
        for (auto &&chunk : chunk_vec) {
            clog << "Print chunk" << endl;
            for (auto &&str : chunk) {
            cout << str << endl;
        }
    }
#endif
#if 0
        for (auto &&chunk : chunk_vec) {
            clog << "Print chunk" << endl;
            for (auto &&i = chunk.begin(), end = chunk.end(); i != end; ++i)
            cout << *i << endl;
    }
#endif
        sorter_p_->sorted_print();
        return 0;
    }
};

#endif // MANAGER_H
