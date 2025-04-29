#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

#ifndef MANAGER_H
#define MANAGER_H

/**
 * @brief The ISorter class.
 * Interface for Sort algorithms. Uses CRTP pattern instead of virtual functions.
 */
template<typename Derived>
class ISorter
{};

/**
 * @brief The QSorter class.
 * Implements Quick Sort algorithm.
 */
class QSorter : public ISorter<QSorter>
{
public:
    QSorter() { std::cout << "QSorter default ctor" << std::endl; }
};

class DataChunk
{};

/**
 * @brief The DataObject class.
 * Resposibility: handle raw input data to be sorted (from file, stdin, etc.)
 */
class DataObject
{
    std::size_t size;
    std::vector<DataChunk> d_chunk_vec;
};


template<typename Sorter>
class Manager
{
    std::unique_ptr<ISorter<Sorter>> sorter_uptr;
    DataObject d_obj;

public:
    Manager()
        : sorter_uptr(std::make_unique<Sorter>())
    {
        std::cout << "Manager default ctor" << std::endl;
    }
};

#endif // MANAGER_H
