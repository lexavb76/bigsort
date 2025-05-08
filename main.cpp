#include "manager.h"
#include "isorter.h"
#include <cstddef>
#include <filesystem>
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
#if 0
    std::ifstream is(input_src);
    auto is_it_end = std::istream_iterator<std::string>();
    auto is_it_beg = is_it_end;
    std::string str;
    for (int i = 0; i < 10; ++i) {
        std::getline(is, str);
        cerr << str << " ---> getline 1" << endl;
        is_it_beg = std::istream_iterator<std::string>(is);
        cerr << *is_it_beg << " ---> iterator" << endl;
        auto _ = *++is_it_beg;
        std::getline(is, str);
        cerr << str << " ---> getline 2" << endl;
    }
#endif
    try {
        Manager<QuickSorter, FileDataSource> mng(input_src); //Choose sorting algorithm here
        return mng.run();
    } catch (fs::filesystem_error &e) {
        cerr << e.what() << endl;
        return 1;
    }
}
