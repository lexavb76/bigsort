#include "data_source.h"
#include <memory>

#ifndef MANAGER_H
#define MANAGER_H

template<typename Sorter,
         typename Data = FileDataSource>
class Manager
{
    const char *source_name;
    std::unique_ptr<Sorter> sorter_uptr;
    Data d_obj;

public:
    Manager(const char *src)
        : source_name(src),
          sorter_uptr(std::make_unique<Sorter>()),
          d_obj(source_name)

    {
        std::cerr << "Manager default ctor" << std::endl;
    }
};

#endif // MANAGER_H
