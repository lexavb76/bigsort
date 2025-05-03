#ifndef DATA_SOURCE_BASE_H
#define DATA_SOURCE_BASE_H

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>
namespace fs = std::filesystem;


using std::cout;
using std::cerr;
using std::endl;
using isIter = std::istream_iterator<std::string>;

template<typename T = std::string, typename Compare = std::less<T>>
using dSortedContPtr = std::shared_ptr<std::multiset<T, Compare>>; // Multiset keeps its data sorted

constexpr std::size_t CHUNK_SIZE_MAX = 1024 * 1024 * 100;
constexpr std::size_t CHUNK_SIZE_OPT = 16 * 16 * 4 * 3;

template<typename T = std::string>
class DataChunk
{
    constexpr static int FNAME_LEN = 18;
    constexpr static int FEXT_LEN = 8;
    bool swap_to_file;
    char swap_f_name[FNAME_LEN] = ".bigsort.tmp.xxxx";
    std::unique_ptr<std::ifstream> is_p;
    std::unique_ptr<std::ofstream> os_p;
    dSortedContPtr<T> data_sorted_p;

public:
    explicit DataChunk(dSortedContPtr<T> data, bool swap_to_file = false)
        : data_sorted_p(data)
    {
        if (swap_to_file && data_sorted_p->size()) {
            std::random_device seed;
            std::mt19937 gen(seed());
            std::uniform_int_distribution<char> uf_dis('A', 'Z');
            for (int i = FNAME_LEN - FEXT_LEN - 1; i < FNAME_LEN - 1; ++i) {
                swap_f_name[i] = uf_dis(gen);
            }
            os_p = std::make_unique<std::ofstream>(
                swap_f_name); // Move tmp object to osPtr as unique_ptr does not have copy Ctor
            std::for_each(begin(), end(),
                          [&os=os_p](const T &str) { (*os) << str << endl; }
                          );
            os_p->close();
            if (!fs::exists(swap_f_name))
                throw(fs::filesystem_error((std::stringstream{} << "File '" << swap_f_name << "' was not created.").str(),
                                           std::make_error_code(std::errc::io_error)));
        }
    }

#if 0
    ~DataChunk() { cerr << "DataChunk Dtor. data size = " << data_vec_uptr->size() << endl;
    }
#endif

    auto begin() { return data_sorted_p->begin(); }
    auto end() { return data_sorted_p->end(); }
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
    DataSourceBase() {} // Protected Ctor to avoid unexpected base class instantiation
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
