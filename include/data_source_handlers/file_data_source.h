#ifndef FILE_DATA_SOURCE_H
#define FILE_DATA_SOURCE_H

#include <string_view>
#include <fstream>
#include <ios>
#include "data_source_base.h"

class FileDataSource : public DataSourceBase<FileDataSource>
{
    template<typename InIter>
    void divide_to_chunks(const InIter &first, const InIter &last)
    {
        std::size_t size_acc = 0;
        bool need_to_swap_chunk_to_file = true;
        dSortedContPtr<> tmp_vec(new dSortedContPtr<>::element_type);
        for (auto it = first; it != last; ++it) {
            size_acc += it->length() + 1;
            if (size_acc > chunk_size) {
                d_chunk_vec.emplace_back(tmp_vec, need_to_swap_chunk_to_file);
#if 0
                for (auto &&it : d_chunk_vec) {
                    for (auto i = it.begin(), end = it.end(); i != end; ++i)
                        cerr << *i << endl;
                }
                cerr << "----> " << size_acc << endl;
#endif
                tmp_vec = dSortedContPtr<>(new dSortedContPtr<>::element_type);
                size_acc = 0;
            }
            tmp_vec->insert(*it);
        }
        d_chunk_vec.emplace_back(tmp_vec, need_to_swap_chunk_to_file); // Store the last incomplete chunk
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
#endif // FILE_DATA_SOURCE_H
