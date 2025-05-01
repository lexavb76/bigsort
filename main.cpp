#include "manager.h"
#include "isorter.h"
#include "data_source.h"
#include <filesystem>

const char *input_src = "1.txt";

int main()
{
    try {
        Manager<QuickSorter, FileDataSource> mng(input_src); //Choose sorting algorithm here

    } catch (fs::filesystem_error &e) {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}
