#ifndef ISORTER_H
#define ISORTER_H

#include <iostream>
#include <cstddef>
#include <vector>

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

class DataChunk
{};

/**
 * @brief The IDataSource class.
 * Interface for raw input data to be sorted (from file, stdin, etc.)
 */
template<typename Derived>
class IDataSource
{
    std::size_t size;
    std::vector<DataChunk> d_chunk_vec;
};

class FileDataSource : public IDataSource<FileDataSource>
{
};

#endif // ISORTER_H
