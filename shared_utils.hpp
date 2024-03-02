// ItsEntDev @ 19/08/23
#ifndef LEARNCPP_SHARED_UTILS_H
#define LEARNCPP_SHARED_UTILS_H
#include <cstdint>

template <typename T1, typename T2>
struct Pair {
    T1 a;
    T2 b;
};

// Clears the terminal using an Ascii escape sequence
void clearTerminalAscii();
void debug(const std::string& msg);

// Returns a string of the lowercase version of the passed string reference
std::string asLowercase(const std::string& str);
// Returns a string of the uppercase version of the passed string reference
std::string asUppercase(const std::string& str);
// Returns a vector of split strings with delimiters removed
std::vector<std::string> split_string(const std::string& str, const std::string& delimiter);

//
template <typename T, size_t rows, size_t cols>
T copy2dArray(T array[rows][cols]);
// Const version
template <typename T, size_t rows, size_t cols>
T copy2dArray(const T array[rows][cols]);

uint64_t psuedoSecureULong(uint64_t min, uint64_t max);

std::vector<Pair<uint64_t, uint64_t>> paddedRangeTable(std::vector<Pair<uint64_t, uint64_t>> &vec, uint64_t pad);
std::vector<uint64_t> randomLenNums(uint64_t length, std::vector<Pair<uint64_t, uint64_t>> &rangeTable);
std::vector<uint64_t> randomLenNums(uint64_t length, uint64_t rangeMin, uint64_t rangeMax);
std::vector<uint64_t> randomLenNums(uint64_t length);
std::vector<Pair<uint64_t, uint64_t>> genericRangeTable(uint64_t listLength, uint64_t rangeMin, uint64_t rangeMax);
std::vector<Pair<uint64_t, uint64_t>> grtWithEnd(uint64_t llen, uint64_t lmin, uint64_t lmax, uint64_t emin, uint64_t emax);

std::string compressToS(std::vector<int>& vec);
std::string compressToS(std::vector<uint64_t>& vec);
uint64_t compressToUl(std::vector<uint64_t>& vec);

#ifdef _WIN32
// windows specific functionality
// nothing here right now
namespace WinUtils {}
#endif

#ifdef __linux__
// linux specific functionality
namespace PosixUtils {
    std::string getUserName();

    std::string getHomeDir();
}
#endif

#ifndef SHARED_UTILS_DISABLE_ALIASES
// to disable alias names you can do
// #define SHARED_UTILS_DISABLE_ALIASES
// before #include-ing
using RangeTable = std::vector<Pair<uint64_t, uint64_t>>;
#endif

#endif  //LEARNCPP_SHARED_UTILS_H
