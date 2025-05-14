#include "file_data_source.h"
#include "manager.h"
#include "isorter.h"
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <random>

constexpr std::size_t INPUT_LINES_NUM = 16 * 16;
//constexpr char input_src[] = "1.txt"; // Also valid
constexpr char const *input_src = "1.txt";
using dataType = unsigned long long;

int main()
{
#if 0
    std::random_device seed;
    std::mt19937 gen(seed());
    //    std::uniform_int_distribution<dataType> uf_dis(0, static_cast<dataType>(gen.max()));
    auto data_max = std::numeric_limits<dataType>::max();
    std::uniform_int_distribution<dataType> uf_dis(0, data_max);
    std::ofstream of(input_src);
    cerr << uf_dis.min() << " - " << uf_dis.max() << endl;

    for (std::size_t i = 1; i < INPUT_LINES_NUM + 1; ++i) {
//        of << std::noskipws << std::hex << std::setw(1) << std::setfill('0') << uf_dis(gen) << endl;
        of << std::noskipws << std::hex << std::setw(1) << std::setfill('0') << uf_dis(gen)<< "    Alex!!!" << endl;
    }
    of.close();
    std::ifstream is(input_src);
#else
    auto &is = std::cin;
#endif
    Manager<QuickSorter<FileDataSource>, FileDataSource> mng{is}; //Choose sorting algorithm here
    return mng.run();
}
