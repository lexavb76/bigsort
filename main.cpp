#include "manager.h"
#include "isorter.h"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <random>

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

    for (int i = 1; i < 257; ++i) {
//        of << std::noskipws << std::hex << std::setw(1) << std::setfill('0') << uf_dis(gen) << endl;
        of << std::noskipws << std::hex << std::setw(1) << std::setfill('0') << uf_dis(gen)<< "    Alex!!!" << endl;
//        if (i % 16 == 0) {
//            of << std::hex << '\n';
//        }
    }
    of.close();
    try {
        Manager<QuickSorter, FileDataSource> mng(input_src); //Choose sorting algorithm here

    } catch (fs::filesystem_error &e) {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}
