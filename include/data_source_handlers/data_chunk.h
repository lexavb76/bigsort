#ifndef DATA_CHUNK_H
#define DATA_CHUNK_H
#include <random>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <ios>
#include <system_error>
#include <memory>
#include <set>
#include <iostream>
#include <iterator>
#include <string>
#include "iterator_switch.h"

using std::cout;
using std::cerr;
using std::endl;

namespace fs = std::filesystem;

template<typename T = std::string, typename Compare = std::less<T>>
using dSortedContPtr = std::shared_ptr<std::multiset<T, Compare>>; // Multiset keeps its data sorted

template<typename T = std::string>
class DataChunk
{
    constexpr static int FNAME_LEN = 18;
    constexpr static int FEXT_LEN = 8;
    bool swap_to_file_;
    char swap_f_name_[FNAME_LEN] = ".bigsort.tmp.xxxx";
    std::unique_ptr<std::ifstream> is_p_;
    dSortedContPtr<> data_sorted_p_;

public:
    explicit DataChunk(dSortedContPtr<> data, bool swap_to_file = true)
        : data_sorted_p_(data)
    {
        std::unique_ptr<std::ofstream> os_p;
        if (swap_to_file && data_sorted_p_->size()) {
            // Generate unique tmp file name:
            std::random_device seed;
            std::mt19937 gen(seed());
            std::uniform_int_distribution<char> uf_dis('A', 'Z');
            for (int i = FNAME_LEN - FEXT_LEN - 1; i < FNAME_LEN - 1; ++i) {
                swap_f_name_[i] = uf_dis(gen);
            }
            // ***
            os_p = std::make_unique<std::ofstream>(
                swap_f_name_); // Move tmp object as unique_ptr does not have copy Ctor
            std::for_each(data_sorted_p_->begin(), data_sorted_p_->end(),
                          [&os=os_p](const T &str) { (*os) << std::noskipws << str << endl; }
                          );
            os_p->close();
            if (!fs::exists(swap_f_name_))
                throw(fs::filesystem_error((std::stringstream{} << "File '" << swap_f_name_ << "' was not created.").str(),
                                           std::make_error_code(std::errc::io_error)));
            data_sorted_p_->clear();
        }
    }

#if 0
    ~DataChunk() { cerr << "DataChunk Dtor. data size = " << data_vec_uptr->size() << endl;
    }
#endif

    auto begin()
    {
        return swap_to_file_ ? IterSwitch<std::istream_iterator<T>, decltype(data_sorted_p_->begin())>
                   (std::istream_iterator<T>(*is_p_))
                             : IterSwitch<std::istream_iterator<T>, decltype(data_sorted_p_->begin())>
                   (data_sorted_p_->begin());
    }
    auto end()
    {
        return swap_to_file_ ? IterSwitch<std::istream_iterator<T>, decltype(data_sorted_p_->end())>
                   (std::istream_iterator<T>())
                             : IterSwitch<std::istream_iterator<T>, decltype(data_sorted_p_->end())>
                   (data_sorted_p_->end());
    }
};

#endif // DATA_CHUNK_H
