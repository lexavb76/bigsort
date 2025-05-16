#ifndef DATA_CHUNK_H
#define DATA_CHUNK_H
#include <cstring>
#include <random>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <ios>
#include <system_error>
#include "iterator_switch.h"
#include "types.h"

namespace fs = std::filesystem;

template<typename T = std::string>
class DataChunk
{
public:
    constexpr static int FNAME_LEN = 18;
    constexpr static int FEXT_LEN = 12;
    bool swap_to_file_;
    char swap_f_name_[FNAME_LEN] = ".bigsort.tmp.xxxx";
    std::unique_ptr<std::ifstream> is_p_ = nullptr;
    dSortedContPtr<T> data_sorted_p_;

    using value_type = T;
    using iterator_type = IterSwitch <std::ifstream, decltype(data_sorted_p_->begin())>;

public:
    DataChunk &operator=(const DataChunk &) = delete;
    DataChunk &operator=(DataChunk &&)      = delete;
    DataChunk() {}

    explicit DataChunk(dSortedContPtr<T> data, bool swap_to_file = true)
        : swap_to_file_(swap_to_file)
        , data_sorted_p_(data)
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
            std::unique_ptr<std::ofstream> os_p
                = // Move tmp object as unique_ptr does not have copy Ctor
                std::make_unique<std::ofstream>(swap_f_name_);
            std::for_each(data_sorted_p_->begin(),
                          data_sorted_p_->end(),
                          [&os = os_p](const T &str) { (*os) << std::noskipws << str << endl; });
            os_p->close();
            if (!fs::exists(swap_f_name_))
                throw(fs::filesystem_error((std::stringstream{} << "File '" << swap_f_name_
                                                                << "' was not created.").str(),
                                           std::make_error_code(std::errc::io_error)));
            data_sorted_p_->clear();
            is_p_ = std::make_unique<std::ifstream>(swap_f_name_);
        }
    }

    DataChunk(const DataChunk &other) noexcept(noexcept(std::memcpy))
        : swap_to_file_(other.swap_to_file_)
        , is_p_(other.is_p_)
        , data_sorted_p_(other.data_sorted_p_)
    {
        // clog << "DataChunk copy Ctor. data size = " << data_sorted_p_->size() << endl;
        std::memcpy(&swap_f_name_, &other.swap_f_name_, FNAME_LEN);
    }

    DataChunk(DataChunk &&other) noexcept(noexcept(std::memcpy))
        : swap_to_file_(other.swap_to_file_)
        , is_p_(std::move(other.is_p_))
        , data_sorted_p_(std::move(other.data_sorted_p_))
    {
        // clog << "DataChunk move Ctor. data size = " << data_sorted_p_->size() << endl;
        std::memcpy(&swap_f_name_, &other.swap_f_name_, FNAME_LEN);
    }

    ~DataChunk()
    {
        // clog << "DataChunk Dtor. data size = " << data_sorted_p_->size() << endl;
        if (is_p_) is_p_->close();
    }

    auto begin() const
    {
        // clog << "begin(). swap = " << std::boolalpha << swap_to_file_ << endl;
        return swap_to_file_ ? iterator_type
                   (*is_p_)
                             : iterator_type
                   (data_sorted_p_->begin());
    }
    auto end() const
    {
        // clog << "end(). swap = " << std::boolalpha << swap_to_file_ << endl;
        return swap_to_file_ ? iterator_type
                   (nullptr)
                             : iterator_type
                   (data_sorted_p_->end());
    }
};

#endif // DATA_CHUNK_H
