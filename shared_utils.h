// ItsEntDev @ 19/08/23
#ifndef LEARNCPP_SHARED_UTILS_H
#define LEARNCPP_SHARED_UTILS_H

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
std::vector<std::string> split_string(std::string str, std::string delimiter);

//
template <typename T, size_t rows, size_t cols>
T copy2dArray(T array[rows][cols]);
// Const version
template <typename T, size_t rows, size_t cols>
T copy2dArray(const T array[rows][cols]);

ulong psuedoSecureULong(ulong min, ulong max);

std::vector<Pair<ulong, ulong>> paddedRangeTable(std::vector<Pair<ulong, ulong>> &vec, ulong pad);
std::vector<ulong> randomLenNums(ulong length, std::vector<Pair<ulong, ulong>> &rangeTable);
std::vector<ulong> randomLenNums(ulong length, ulong rangeMin, ulong rangeMax);
std::vector<ulong> randomLenNums(ulong length);
std::vector<Pair<ulong, ulong>> genericRangeTable(ulong listLength, ulong rangeMin, ulong rangeMax);
std::vector<Pair<ulong, ulong>> grtWithEnd(ulong llen, ulong lmin, ulong lmax, ulong emin, ulong emax);

std::string compressToS(std::vector<int>& vec);
std::string compressToS(std::vector<ulong>& vec);
ulong compressToUl(std::vector<ulong>& vec);

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
using RangeTable = std::vector<Pair<ulong, ulong>>;
#endif

#endif  //LEARNCPP_SHARED_UTILS_H
