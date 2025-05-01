#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <string_view>
#include <vector>

using std::cout;
using std::endl;
namespace fs = std::filesystem;

class DataChunk
{};

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
    std::vector<DataChunk> d_chunk_vec;

protected:
    std::size_t calculate_chunk_size()
    {
        std::size_t chsize = (size > 1)? size / 2 + 1 : size;
        return chsize;
    }

public:
    explicit DataSourceBase(std::size_t dsize)
        : size(dsize)
    {
        chunk_size = calculate_chunk_size();
    }
};

class FileDataSource : public DataSourceBase<FileDataSource>
{
public:
    explicit FileDataSource(std::string_view fname)
        : DataSourceBase<FileDataSource>{fs::file_size(fname)}
    {
        std::ifstream is(fname.data());
        std::string str;
        is >> str;
        cout << fname << std::boolalpha << ' ' << is.fail() << endl;
        cout << static_cast<int>(str.c_str()[1]);
        std::cout << is.rdbuf();
        cout << "size = " << size << endl;
        cout << "chunk_size = " << chunk_size << endl;
    }
};
#endif // DATA_SOURCE_H
