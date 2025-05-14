#ifndef TYPES_H
#define TYPES_H

#include <memory>
#include <set>
#include <iostream>
#include <string>

constexpr std::size_t CHUNK_SIZE_MAX = 1024 * 1024 * 100;
// constexpr std::size_t CHUNK_SIZE_OPT = 16 * 16 * 4 * 3;
constexpr std::size_t CHUNK_SIZE_OPT = CHUNK_SIZE_MAX / 10000;

using std::cout;
using std::clog;
using std::endl;

template<typename T = std::string, typename Compare = std::less<T>>
using dSortedCont = std::multiset<T, Compare>; // Multiset keeps its data sorted

template<typename T = std::string, typename Compare = std::less<T>>
using dSortedContPtr = std::shared_ptr<dSortedCont<T>>; // Multiset keeps its data sorted

#endif // TYPES_H
