#include "file_data_source.h"
#include <memory>

#ifndef MANAGER_H
#define MANAGER_H

template<typename Sorter,
         typename Data = FileDataSource>
class Manager
{
    const char *src_name_;
    std::unique_ptr<Sorter> srt_p_;
    Data d_obj_;

public:
    Manager(const char *src)
        : src_name_(src),
          srt_p_(std::make_unique<Sorter>()),
          d_obj_(src_name_)

    {
        std::cerr << "Manager default ctor" << std::endl;
    }

    int run()
    {
        auto & chunk_vec = d_obj_.get_d_chunk_vec();
#if 1
        for (auto &&chunk : chunk_vec) {
            for (auto &&it : chunk) {
                cout << it << endl;
            }
        }
#endif
        return 0;
    }
};

#endif // MANAGER_H
