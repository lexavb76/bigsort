#ifndef DATA_SOURCE_BASE_H
#define DATA_SOURCE_BASE_H

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ios>
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

template<typename ItFirst, typename ItSec>
class IterWrapper
{
    ItFirst it_f_;
    ItSec it_s_;
    bool swap_;

public:
    IterWrapper(const ItFirst &it_f, const ItSec &it_s, bool swap)
        : it_f_(it_f)
        , it_s_(it_s)
        , swap_(swap)
    { }

    decltype(auto) operator*() const { return *it_f_; }
    decltype(auto) operator->() const { return it_f_; }
    decltype(auto) operator++() { return ++it_f_; }
    bool operator!=(const IterWrapper<ItFirst, ItSec> &other){}

};

#if 0 // Deduction guide example
template<typename ItFirst, typename ItSec>
IterWrapper(ItFirst, ItSec, bool) -> IterWrapper<ItFirst, ItSec>;
#endif

template<typename T = std::string>
class DataChunk
{
    constexpr static int FNAME_LEN = 18;
    constexpr static int FEXT_LEN = 8;
    bool swap_to_file_;
    char swap_f_name_[FNAME_LEN] = ".bigsort.tmp.xxxx";
    std::unique_ptr<std::ifstream> is_p_;
    dSortedContPtr<T> data_sorted_p_;

public:
    explicit DataChunk(dSortedContPtr<T> data, bool swap_to_file = true)
        : data_sorted_p_(data)
    {
        std::unique_ptr<std::ofstream> os_p;
        if (swap_to_file && data_sorted_p_->size()) {
            std::random_device seed;
            std::mt19937 gen(seed());
            std::uniform_int_distribution<char> uf_dis('A', 'Z');
            for (int i = FNAME_LEN - FEXT_LEN - 1; i < FNAME_LEN - 1; ++i) {
                swap_f_name_[i] = uf_dis(gen);
            }
            os_p = std::make_unique<std::ofstream>(
                swap_f_name_); // Move tmp object to osPtr as unique_ptr does not have copy Ctor
            std::for_each(begin(), end(),
                          [&os=os_p](const T &str) { (*os) << std::noskipws << str << endl; }
                          );
            os_p->close();
            if (!fs::exists(swap_f_name_))
                throw(fs::filesystem_error((std::stringstream{} << "File '" << swap_f_name_ << "' was not created.").str(),
                                           std::make_error_code(std::errc::io_error)));
        }
    }

#if 0
    ~DataChunk() { cerr << "DataChunk Dtor. data size = " << data_vec_uptr->size() << endl;
    }
#endif

    auto begin()
    {
        return IterWrapper(std::istream_iterator<T>(*is_p_), data_sorted_p_->begin(), swap_to_file_);
    }
    auto end()
    {
        return IterWrapper(std::istream_iterator<T>(), data_sorted_p_->end(), swap_to_file_);
    }
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
