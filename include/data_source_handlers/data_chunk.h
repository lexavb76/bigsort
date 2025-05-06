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
#include <string>
#include "iterator_switch.h"

using std::cout;
using std::cerr;
using std::endl;

namespace fs = std::filesystem;

template<typename T = std::string, typename Compare = std::less<T>>
using dSortedContPtr = std::shared_ptr<std::multiset<T, Compare>>; // Multiset keeps its data sorted

//template<typename T = std::string>
//using isIter = std::istream_iterator<T>;

template<typename T = std::string>
class DataChunk
{
    constexpr static int FNAME_LEN = 18;
    constexpr static int FEXT_LEN = 8;
    bool swap_to_file_;
    char swap_f_name_[FNAME_LEN] = ".bigsort.tmp.xxxx";
    std::unique_ptr<std::ifstream> is_p_ = nullptr;
    dSortedContPtr<> data_sorted_p_;

public:
    explicit DataChunk(dSortedContPtr<> data, bool swap_to_file = true)
        : swap_to_file_(swap_to_file), data_sorted_p_(data)
    {
        if (swap_to_file && data_sorted_p_->size()) {
            // Generate unique tmp file name:
            std::random_device seed;
            std::mt19937 gen(seed());
            std::uniform_int_distribution<char> uf_dis('A', 'Z');
            for (int i = FNAME_LEN - FEXT_LEN - 1; i < FNAME_LEN - 1; ++i) {
                swap_f_name_[i] = uf_dis(gen);
            }
            // ***
            std::unique_ptr<std::ofstream> os_p = std::make_unique<std::ofstream>(
                swap_f_name_); // Move tmp object as unique_ptr does not have copy Ctor
            std::for_each(data_sorted_p_->begin(), data_sorted_p_->end(),
                          [&os=os_p](const T &str) { (*os) << std::noskipws << str << endl; }
                          );
            os_p->close();
            if (!fs::exists(swap_f_name_))
                throw(fs::filesystem_error((std::stringstream{} << "File '" << swap_f_name_ << "' was not created.").str(),
                                           std::make_error_code(std::errc::io_error)));
            data_sorted_p_->clear();
            is_p_ = std::make_unique<std::ifstream>(
                swap_f_name_); // Move tmp object as unique_ptr does not have copy Ctor
#if 0
            for (auto &&it = begin(), &&end = this->end(); it != end; ++it) {
//            for (auto &&it = isIter<T>(*is_p_), &&end = isIter<T>(); it != end; ++it) {
                cout << "Alex " << *it << endl;
            }
#endif
        }
    }
#if 0
    ~DataChunk()
    {
        if (is_p_) {
            cerr << "DataChunk Dtor. data size = " << data_sorted_p_->size() << endl;
            is_p_->close();
        }
    }
#endif
    auto begin()
    {
        cerr << "begin(). swap = " << std::boolalpha << swap_to_file_ << endl;
        return swap_to_file_ ? IterSwitch<isIter<T>, decltype(data_sorted_p_->begin())>
                   (isIter<T>(*is_p_))
                             : IterSwitch<isIter<T>, decltype(data_sorted_p_->begin())>
                   (data_sorted_p_->begin());
    }
    auto end()
    {
        cerr << "end(). swap = " << std::boolalpha << swap_to_file_ << endl;
        return swap_to_file_ ? IterSwitch<isIter<T>, decltype(data_sorted_p_->end())>
                   (isIter<T>())
                             : IterSwitch<isIter<T>, decltype(data_sorted_p_->end())>
                   (data_sorted_p_->end());
    }
};

#endif // DATA_CHUNK_H
