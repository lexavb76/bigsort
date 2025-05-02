#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using isIter = std::istream_iterator<std::string>;

template<typename T = std::string>
using dVecUptr = std::shared_ptr<std::vector<T>>;

namespace fs = std::filesystem;

constexpr std::size_t CHUNK_SIZE_MAX = 1024 * 1024 * 100;
constexpr std::size_t CHUNK_SIZE_OPT = 16 * 16 * 4;

template<typename T = std::string>
class DataChunk
{
    dVecUptr<T> data_vec_uptr;

public:
#if 0
    template<typename U>
    explicit DataChunk(U &&data)
#endif
    explicit DataChunk(dVecUptr<T> data)
        : data_vec_uptr(data)
    {}

    ~DataChunk() { cerr << "DataChunk Dtor. data size = " << data_vec_uptr->size() << endl;
    }

    auto begin() { return data_vec_uptr->begin(); }

    auto end() { return data_vec_uptr->end(); }
};

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

class FileDataSource final : public DataSourceBase<FileDataSource>
{
protected:
    template<typename InIter>
    void divide_to_chunks(const InIter &first, const InIter &last)
    {
        std::size_t size_acc = 0;
        dVecUptr<> tmp_vec(new dVecUptr<>::element_type);
        for (auto it = first; it != last; ++it) {
            size_acc += it->length() + 1;
            if (size_acc > chunk_size) {
                d_chunk_vec.emplace_back(tmp_vec);
#if 0
                for (auto &&it : d_chunk_vec) {
                    for (auto i = it.begin(), end = it.end(); i != end; ++i)
                        cerr << *i << endl;
                }
                cerr << "----> " << size_acc << endl;
#endif
                tmp_vec = dVecUptr<>(new dVecUptr<>::element_type);
                size_acc = 0;
            }
            tmp_vec->push_back(*it);
        }
        d_chunk_vec.emplace_back(tmp_vec); // Store the last incomplete chunk
    }

public:
    explicit FileDataSource(std::string_view fname)
        : DataSourceBase<FileDataSource>{fs::file_size(fname)}
    {
        std::ifstream is(fname.data());
        std::string str;
        auto is_begin = isIter(is);
        auto is_end = isIter();
        cerr << fname << std::boolalpha << ' ' << is.fail() << endl;
        cerr << "size = " << size << endl;
        cerr << "chunk_size = " << chunk_size << endl;
        divide_to_chunks(is_begin, is_end);
        cerr << "number of chunks = " << d_chunk_vec.size() << endl;
#if 1
        for (auto &&chunk : d_chunk_vec) {
            for (auto &&it : chunk) {
                cout << it << endl;
            }
        }
#endif
    }
};
#endif // DATA_SOURCE_H
