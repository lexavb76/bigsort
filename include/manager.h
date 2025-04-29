#include <memory>
#include "isorter.h"

#ifndef MANAGER_H
#define MANAGER_H

template<typename Sorter, typename Receiver>
class Manager
{
    std::unique_ptr<ISorter<Sorter>> sorter_uptr;
    IDataSource<Receiver> d_obj;

public:
    Manager()
        : sorter_uptr(std::make_unique<Sorter>())
    {
        std::cout << "Manager default ctor" << std::endl;
    }
};

#endif // MANAGER_H
