// ItsEntDev @ 20/09/23
// wow there's a lot of stuff here
#include <string>
#include <vector>

#ifndef LEARNCPP_KEY95_H
#define LEARNCPP_KEY95_H

// enum for the type of key. just a wrapper over 0 or 1
enum KeyType {
    RETAIL,
    OEM,
    NONE
};

// common functionality between both types of keys
namespace Common95 {
    // check if a number's digits sum to a number divisible by 7
    bool sumDivsBy7(const std::string& chunk);
    // generate the chunk of both keys that involves
    // numbers divisible by seven. as well as that,
    // the ending number must be (n > 0 && n < 8)
    std::string genAlgorithmChunk(ulong length) noexcept;
}

// the chunk? notation represents
// the order the segments should be
// assembled in
namespace OEMKey {
    // generates the day and year id
    std::string chunkA();
    // generates the hard bit, uses genAlgorithmChunk()
    std::string chunkB();
    // the third segment is irrelevant,
    // it's a completely random 3-digit number
    std::string chunkC();
    // as said in main.cpp these overloaded
    // functions are wrappers to assemble
    // the chunks into a full key of the type
    std::string generate();
    std::string generate(bool useChunkB);
    // check if an OEM key is valid
    bool valid(const std::string& key);
}

namespace RetailKey {
    // this is a helper function to check
    // if the chunk from chunkA() is valid
    bool chunkAIsInvalid(std::string& chunk);
    // the first segment is a 3-digit number
    // that is fully random, HOWEVER there are
    // a list of arbitrarily invalid numbers
    // and those are: 333, 444, 555, 666, 777,
    // 888 and 999. we don't know why.
    std::string chunkA();
    // the second segment is once again the
    // divisible by seven checked number
    // and uses genAlgorithmChunk()
    std::string chunkB();
    // same thing as in the OEMKey namespace
    std::string generate();
    std::string generate(bool useChunkB);
    // check if a retail key is valid
    bool valid(const std::string& key);
}

// a top-level wrapper with overloaded options to
// generate a key easily. use this if you just want
// a key from the api, especially if you're in python
std::string generateKey(KeyType keyType, bool useChunkB);
// useChunkB will default to
// (wait a min, i had to check my impl)
// true so you get a fully random key.
std::string generateKey(KeyType keyType);
// keyType will default to retail
std::string generateKey();

// gets type of key from a key
KeyType getKeyType(const std::string& key);
// validates with a passed key type
bool validateKey(KeyType keyType, const std::string& key);
// autodetects key type
bool validateKey(const std::string& key);

#endif //LEARNCPP_KEY95_H
