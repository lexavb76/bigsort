#ifndef DATA_SOURCE_BASE_H
#define DATA_SOURCE_BASE_H

#include <cstddef>
#include <vector>
#include "data_chunk.h"

class ISortedCont
{
public:
    virtual ~ISortedCont() {}
};

constexpr std::size_t CHUNK_SIZE_MAX = 1024 * 1024 * 100;
constexpr std::size_t CHUNK_SIZE_OPT = 16 * 16 * 4 * 3;

/**
 * @brief The IDataSource class.
 * Interface for raw input data to be sorted (from file, stdin, etc.)
 */
template<typename Derived>
class DataSourceBase
{
protected:
    std::size_t size;
    std::size_t chunk_size;
    std::vector<DataChunk<>> d_chunk_vec;

protected:
    DataSourceBase()  {} // Protected Ctor to avoid unexpected base class instantiation
    ~DataSourceBase() {} // Protected Dtor to avoid unexpected deletion through base class pointer
    std::size_t calculate_chunk_size()
    {
        std::size_t chsize = (size > CHUNK_SIZE_MAX)? CHUNK_SIZE_MAX :
                                 (size > CHUNK_SIZE_OPT)? CHUNK_SIZE_OPT : size;
        return chsize;
    }

public:
    explicit DataSourceBase(std::size_t dsize)
        : size(dsize)
    {
        chunk_size = calculate_chunk_size();
        d_chunk_vec.reserve(size / chunk_size + 1);
    }
};

#endif // DATA_SOURCE_BASE_H
