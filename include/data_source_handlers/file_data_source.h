#ifndef FILE_DATA_SOURCE_H
#define FILE_DATA_SOURCE_H

#include <string>
#include <string_view>
#include <fstream>
#include "data_source_base.h"

class FileDataSource : public DataSourceBase<FileDataSource>
{
    template<typename T = std::string>
    void divide_to_chunks(std::istream &is)
    {
        std::size_t size_acc = 0;
        bool swap_chunk_to_file = false;
        dSortedContPtr<T> tmp_chunk_p(new dSortedContPtr<>::element_type);
        std::string line;
        while (std::getline(is, line)) {
            size_acc += line.length() + 1;
            if (size_acc > chunk_size_) {
                d_chunk_vec_.emplace_back(tmp_chunk_p, swap_chunk_to_file);
                tmp_chunk_p = dSortedContPtr<T>(new dSortedContPtr<T>::element_type);
                size_acc = 0;
            }
            tmp_chunk_p->insert(line);
        }
        d_chunk_vec_.emplace_back(tmp_chunk_p, swap_chunk_to_file); // Store the last incomplete chunk
    }

public:
    explicit inline FileDataSource(std::string_view fname)
        : DataSourceBase<FileDataSource>{fs::file_size(fname)}
    {
        std::ifstream is(fname.data());
        // clog << "size = " << data_size_ << endl;
        // clog << "chunk_size = " << chunk_size_ << endl;
        divide_to_chunks<>(is);
        is.close();
        // clog << "number of chunks = " << d_chunk_vec_.size() << endl;
    }

    explicit inline FileDataSource(std::istream &is)
        : DataSourceBase<FileDataSource>{}
    {
        clog << "size = " << data_size_ << endl;
        clog << "chunk_size = " << chunk_size_ << endl;
        divide_to_chunks<>(is);
        clog << "number of chunks = " << d_chunk_vec_.size() << endl;
    }

    [[ nodiscard("Return value discarded") ]]
    containerType const & get_d_chunk_vec() const noexcept { return d_chunk_vec_; }
};
#endif // FILE_DATA_SOURCE_H
