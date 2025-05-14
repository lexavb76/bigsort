#ifndef DATA_SOURCE_BASE_H
#define DATA_SOURCE_BASE_H

#include <cstddef>
#include <vector>
#include "data_chunk.h"
#include "types.h"

class ISortedCont
{
public:
    virtual ~ISortedCont() {}
};

/**
 * @brief The IDataSource class.
 * Interface for raw input data to be sorted (from file, stdin, etc.)
 */
template<typename Derived>
class DataSourceBase
{
public:
    using containerType = std::vector<DataChunk<>>;

protected:
    std::size_t data_size_ = CHUNK_SIZE_MAX;
    std::size_t chunk_size_ = CHUNK_SIZE_OPT;
    containerType d_chunk_vec_;

protected:
    DataSourceBase() // Protected Ctor to avoid unexpected base class instantiation
    {}

    ~DataSourceBase() {} // Protected Dtor to avoid unexpected deletion through base class pointer
    std::size_t calculate_chunk_size()
    {
        std::size_t chsize = (data_size_ > CHUNK_SIZE_MAX)? CHUNK_SIZE_MAX :
                                 (data_size_ > CHUNK_SIZE_OPT)? CHUNK_SIZE_OPT : data_size_;
        return chsize;
    }

public:
    explicit DataSourceBase(std::size_t dsize)
        : data_size_(dsize)
    {
        chunk_size_ = calculate_chunk_size();
        d_chunk_vec_.reserve(data_size_ / chunk_size_ + 1);
    }

    [[ nodiscard("Return value discarded") ]]
    containerType const & get_d_chunk_vec() const noexcept { return Derived::get_d_chunk_vec(); }
};

#endif // DATA_SOURCE_BASE_H
