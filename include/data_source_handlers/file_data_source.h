#ifndef FILE_DATA_SOURCE_H
#define FILE_DATA_SOURCE_H

#include <string>
#include <string_view>
#include <fstream>
#include <ios>
#include "data_source_base.h"

class FileDataSource : public DataSourceBase<FileDataSource>
{
    template<typename T = std::string>
    void divide_to_chunks(std::ifstream &is)
    {
        std::size_t size_acc = 0;
        bool swap_chunk_to_file = false;
        dSortedContPtr<T> tmp_chunk(new dSortedContPtr<>::element_type);
        std::string line;
        while (std::getline(is, line)) {
            size_acc += line.length() + 1;
            if (size_acc > chunk_size) {
                d_chunk_vec.emplace_back(tmp_chunk, swap_chunk_to_file);
                tmp_chunk = dSortedContPtr<>(new dSortedContPtr<>::element_type);
                size_acc = 0;
            }
            tmp_chunk->insert(line);
        }
        d_chunk_vec.emplace_back(tmp_chunk, swap_chunk_to_file); // Store the last incomplete chunk
    }

public:
    explicit FileDataSource(std::string_view fname)
        : DataSourceBase<FileDataSource>{fs::file_size(fname)}
    {
        std::ifstream is(fname.data());
        cerr << fname << std::boolalpha << ' ' << is.fail() << endl;
        cerr << "size = " << size << endl;
        cerr << "chunk_size = " << chunk_size << endl;
        divide_to_chunks<>(is);
        is.close();
        cerr << "number of chunks = " << d_chunk_vec.size() << endl;
#if 1
        for (auto &&chunk : d_chunk_vec) {
            for (auto &&it : chunk) {
//            for (auto &&it = chunk.begin(), &&end = chunk.end(); it != end; ++it) {
                cout << it << endl;
            }
        }
#endif
    }
};
#endif // FILE_DATA_SOURCE_H
