// ItsEntDev @ 19/08/23
#include <iostream>
#include <string>
#include <random>
#include <chrono>
#ifdef __linux__
#include <pwd.h>
#include <unistd.h>
#endif
#include "shared_utils.hpp"

// Implementation of the function from shared_utils.h
void clearTerminalAscii() {
    std::cout << "\033[2J\033[1;1H";
}

void debug(const std::string& msg) {
    std::cout << "DEBUG: " << msg << std::endl;
}

void debug(std::string& msg) {
    std::cout << "DEBUG: " << msg << std::endl;
}

std::string asLowercase(const std::string& str) {
    std::string lowercaseStr;
    std::locale unsetLocale;
    for (char letter : str) {
        lowercaseStr.push_back(std::tolower(letter, unsetLocale));
    }

    return lowercaseStr;
}

std::string asUppercase(const std::string& str) {
    std::string uppercaseStr;
    std::locale unsetLocale;
    for (char letter : str) {
        uppercaseStr.push_back(std::toupper(letter, unsetLocale));
    }

    return uppercaseStr;
}

std::vector<std::string> split_string(const std::string& string, const std::string& delimiter) {
    // define local variables
    // working start of the string,
    // working end of the string,
    // length of the delimiter
    uint64_t start = 0, end, len = delimiter.length();
    // working token and finished result
    std::string token;
    std::vector<std::string> result;

    // loop until end reached
    // find next delimiter position
    while ((end = string.find(delimiter, start)) != std::string::npos) {
        // split the string based on the found positions
        token = string.substr(start, end - start);
        // adjust start for next loop
        start = end + len;
        // add token to result
        result.push_back(token);
    } result.push_back(string.substr(start)); // add final token to result
    // return result to caller
    return result;
}

template <typename T, size_t rows, size_t cols>
T copy2dArray(T array[rows][cols]) {
    T copy[rows][cols] = {0};
    for (int r = 0; r < rows; r++) {
        T nestedCopy[cols] = {0};
        for (int c = 0; c < cols; c++) {
            nestedCopy = array[r][c];
        } copy[r] = nestedCopy;
    } return copy;
}

template <typename T, size_t rows, size_t cols>
T copy2dArray(const T array[rows][cols]) {
    T copy[rows][cols] = {0};
    for (int r = 0; r < rows; r++) {
        T nestedCopy[cols] = {0};
        for (int c = 0; c < cols; c++) {
            nestedCopy[c] = array[r][c];
        } copy[r] = nestedCopy;
    } return copy;
}

uint64_t psuedoSecureULong(uint64_t min, uint64_t max) {

    std::random_device dev;
    std::mt19937::result_type seed = dev() ^ (
            (std::mt19937::result_type)
                    std::chrono::duration_cast<std::chrono::seconds>(
                            std::chrono::system_clock::now().time_since_epoch()
                    ).count() +
            (std::mt19937::result_type)
                    std::chrono::duration_cast<std::chrono::microseconds>(
                            std::chrono::high_resolution_clock::now().time_since_epoch()
                    ).count() +
            (std::mt19937::result_type)
                    std::chrono::duration_cast<std::chrono::nanoseconds>(
                            std::chrono::high_resolution_clock::now().time_since_epoch())
                    .count());
    std::mt19937 rng(seed);
    std::uniform_int_distribution<uint64_t> rnd(min, max); // distribution in range [<, >]

    return rnd(rng);
}

std::vector<Pair<uint64_t, uint64_t>> paddedRangeTable(std::vector<Pair<uint64_t, uint64_t>> &vec, uint64_t pad) {
    std::vector<Pair<uint64_t, uint64_t>> paddedVec = vec;
    for (uint64_t i = 0; i < pad; ++i) { paddedVec.push_back(vec.back()); }
    return paddedVec;
}

std::vector<uint64_t> randomLenNums(uint64_t length, std::vector<Pair<uint64_t, uint64_t>> &rangeTable) {
    std::vector<uint64_t> result;
    std::vector<Pair<uint64_t, uint64_t>> localTable;
    if (length > rangeTable.size()) { localTable = paddedRangeTable(rangeTable, length - rangeTable.size()); }
    else { localTable = rangeTable; }
    for (uint64_t i = 0; i < length; ++i) {
        result.push_back(psuedoSecureULong(localTable.at(i).a, localTable.at(i).b));
    } return result;
}

std::vector<uint64_t> randomLenNums(uint64_t length, uint64_t rangeMin, uint64_t rangeMax) {
    std::vector<uint64_t> result;
    for (uint64_t i = 0; i < length; ++i) {
        result.push_back(psuedoSecureULong(rangeMin, rangeMax));
    } return result;
}

std::vector<uint64_t> randomLenNums(uint64_t length) {
    std::vector<uint64_t> result;
    for (uint64_t i = 0; i < length; ++i) {
        result.push_back(psuedoSecureULong(0, 9));
    } return result;
}

std::vector<Pair<uint64_t, uint64_t>> genericRangeTable(uint64_t listLength, uint64_t rangeMin, uint64_t rangeMax) {
    std::vector<Pair<uint64_t, uint64_t>> rangeList;
    for (uint64_t i = 0; i < listLength; ++i) { rangeList.push_back({rangeMin, rangeMax}); }
    return rangeList;
}

std::string compressToS(std::vector<int>& vec) {
    std::string working;
    for (int num : vec) { working += std::to_string(num); }
    return working;
}

std::string compressToS(std::vector<uint64_t>& vec) {
    std::string working;
    for (uint64_t num : vec) { working += std::to_string(num); }
    return working;
}

uint64_t compressToUl(std::vector<uint64_t>& vec) {
    std::string inProgress = compressToS(vec);
    return std::stoul(inProgress);
}

std::vector<Pair<uint64_t, uint64_t>> grtWithEnd(uint64_t llen, uint64_t lmin, uint64_t lmax, uint64_t emin, uint64_t emax) {
    if (llen <= 0) { return std::vector<Pair<uint64_t, uint64_t>>{}; };
    std::vector<Pair<uint64_t, uint64_t>> result = genericRangeTable(llen - 1, lmin, lmax);
    result.push_back({emin, emax});
    return result;
}

#ifdef _WIN32

#endif

#ifdef __linux__
std::string PosixUtils::getUserName() {
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw)
    {
        return pw -> pw_name;
    }

    return "";
}

std::string PosixUtils::getHomeDir() {
    std::string username = PosixUtils::getUserName();
    if (username.empty()) {
        throw std::exception();
    }

    return "/home/" + username;
}
#endif
