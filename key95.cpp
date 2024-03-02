// ItsEntDev @ 20/09/23
// wow there's a lot of stuff here
#include <iostream>
#include <regex>
#include "key95.hpp"
// shared utilities library made by me :)
// it's very specific, please only use the parts you need,
// just copy+paste, no credit needed.
// the internet is made on sharing
#include "shared_utils.hpp"

// details are pretty simple, loops through the string
// and casts the chars to numbers then adds them to the total.
// at the end, it checks to see if the total sum is divisible by 7
bool Common95::sumDivsBy7(const std::string& chunk) {
    // default result is false
    bool result = false;
    // declare sum
    uint64_t sum = 0;
    // loop through, converting char to uint64_t. i love single-line statements
    for (char ele : chunk) { sum += static_cast<uint64_t>(ele - '0'); }
    // if n mod d is zero, number n is divisible by d
    // source: maths lessons
    if (sum % 7 == 0) { result = true; }
    return result;
}

std::string Common95::genAlgorithmChunk(uint64_t length) noexcept {
    // Check if length is zero
    if (length == 0) { return ""; }
    // grtWithEnd creates a filled RangeTable but the final entry will be emin and emax.
    RangeTable rangeTable = grtWithEnd(length, 0, 9, 1, 7);
    // declare vector result
    std::vector<uint64_t> result;
    //int loopCounter = 0;
    // Generate random numbers until one is divisible by 7
    while (true/*loopCounter < 65535*/) {
        // loopCounter was used when I was testing performance and debugging.
        //++loopCounter;
        // uses the above defined range table to generate a vector of uint64_ts.
        result = randomLenNums(length, rangeTable);
        // check if the vector qualifies by compressing to string and if it is valid
        // call compressToS (compress to string) and return that new string.
        if (sumDivsBy7(compressToS(result))) { return compressToS(result); }
    } //return "0077777"; was used when loopCounter exceeded its max. '0077777' is an always-valid chunk.
}

/*
 * the first chunk is the xxxyy section.
 * xxx is a number between 000 and 366
 * which represents a day of the year.
 * yy is the last two digits of a
 * year between the periods 1995-2003.
 * these are combined to create the chunk.
 */
std::string OEMKey::chunkA() {
    // create an empty vector and a result string
    std::vector<uint64_t> data = {};
    std::string result;
    // define a custom range table for the day randomiser
    // the valid range of days is 000-366
    // the 366th day would be because of a leap year.
    // however the actual possibility of the day/year
    // is not checked, so it just uses a range table.
    RangeTable rangeTable = {
            {0, 3},
            {0, 6},
            {0, 6}
    }; std::vector<uint64_t> vec = randomLenNums(3, rangeTable); // use the table to fill a temporary vector
    // compress the temporary vector to a string
    result += compressToS(vec);
    // create a temporary string and have it at 9 by default
    std::string toAdd = "9"; // 9 represents the 199Xs
    // create a single Pair that represents a range
    Pair<uint64_t, uint64_t> range{5, 9}; // year range between 5 and 9 (1995-1999)
    // do a true-false random to change the millenium of the year
    if (psuedoSecureULong(0, 1) == 1) {
        // set decade to 0 (200X)
        toAdd = "0";
        // year range between 0 and 3 (2000-2003)
        range.a = 0;
        range.b = 3;
        // concatenate the decade with the random year
    } toAdd += std::to_string(psuedoSecureULong(range.a, range.b));
    // finally add the decade and year on the end of the result and return it
    result += toAdd;
    return result;
}

/*
 * this is a pretty simple chunk.
 * it puts '00' at the start, as all
 * microsoft OEM codes have that.
 * however, only the first zero is required.
 * then, it generates a 5-digit algo chunk
 * and appends it on then returns.
 */
std::string OEMKey::chunkB() {
   // create result with base
   std::string result = "00";
   // get algo chunk of length 5
   std::string chunk = Common95::genAlgorithmChunk(5);
   // concatenate and return
   result += chunk;
   return result;
}

/*
 * the third and final chunk
 * is just 5 random digits.
 * very simple, gets random nums,
 * compresses them and returns them.
 */
std::string OEMKey::chunkC() {
    // get random numbers, compress them, and return
    std::vector<uint64_t> vec = randomLenNums(5);
    std::string result = compressToS(vec);
    return result;
}

// this combines the chunks into
// a final, functioning OEM key.
std::string OEMKey::generate() {
    // create a result string
    std::string result;
    // concatenate chunks in order
    // using '-OEM-' and a dash to
    // make it a full OEM key.
    result += chunkA();
    result += "-OEM-";
    result += chunkB();
    result += "-";
    result += chunkC();
    // return the finished key
    return result;
}

/*
 * does the same thing as above but allows
 * you to choose if you want to randomize
 * the second chunk or not. if you don't,
 * an always-valid chunk is used instead.
 */
std::string OEMKey::generate(bool constantChunkB) {
    std::string result;
    if (constantChunkB) {
        // same process but instead
        // of chunk B it uses '0077777'
        // which always works.
        result += chunkA();
        result += "-OEM-0077777-";
        result += chunkC();
    } else {
        // same process as above
        result += chunkA();
        result += "-OEM-";
        result += chunkB();
        result += "-";
        result += chunkC();
    } return result;
}

// check if an OEM key is valid
bool OEMKey::valid(const std::string& key) {
    // first, check length
    if (key.length() != 23) { return false; }
    // next, split into the key into chunks and make
    // sure the right amount of chunks are present
    std::vector<std::string> chunks = split_string(key, "-");
    if (chunks.size() != 4) { return false; }

    // perform chunk A checks
    if (chunks.at(0).length() != 5) { return false; }
    std::regex chunkARegex("^(0[0-9][0-9]|1[0-9][0-9]|2[0-9][0-9]|3[0-6][0-6])(9[5-9]|0[0-3])$");
    if (!std::regex_match(chunks.at(0), chunkARegex)) { return false; }

    // simple check to make sure the second chunk is just OEM
    if (chunks.at(1) != "OEM") { return false; }
    // check that third chunk is the right length
    if (chunks.at(2).length() != 7) { return false; }
    // check that third chunk starts with a zero
    if (chunks.at(2).at(0) != '0') { return false; }
    // check that third chunk is divisible by 7
    if (!Common95::sumDivsBy7(chunks.at(2))) { return false; }

    // check for final section
    // using regex because it's easier
    // and I like regex. pretty simple one here.
    std::regex chunkCRegex("^[0-9]{5}$");
    if (!std::regex_match(chunks.at(3), chunkCRegex)) { return false; }

    // return true if every check has passed
    return true;
}

/*
 * a helper function that checks if the
 * first chunk of a retail key is valid
 * or not. it's used for the do-while loop
 * in RetailKey::chunkA(). it's hard-to-read.
 */
bool RetailKey::chunkAIsInvalid(std::string& chunk) {
    // if chunk is too small, it's always invalid
    if (chunk.length() > 3) { return true; }
    // if the first digit is 0, 1 or 2 it MUST be valid.
    if (chunk.at(0) == '0' || chunk.at(0) == '1' || chunk.at(0) == '2') { return false; }
    // if the first digit isn't 0, 1 or 2 make sure it isn't the same digit repeated 3 times,
    // because for some reason being 333, 444, 555, 666, 777, 888 or 999 invalidates the chunk.
    for (uint64_t i = 0; i < 3; ++i) { if (chunk.at(i) != chunk.at(0)) { return false; } }
    // return invalid if it wasn't returned as false at any other point but was the right size.
    return true;
}

/*
 * the first chunk of a retail key.
 * it's just 3 completely random
 * numbers between 000 and 999.
 */
std::string RetailKey::chunkA() {
    // create vector and string
    std::vector<uint64_t> vec;
    std::string compressed;
    // do-while the chunk is invalid
    do {
        // get random vector, compress, and check
        vec = randomLenNums(3);
        compressed = compressToS(vec);
    } while (RetailKey::chunkAIsInvalid(compressed));
    // once valid chunk found return it
    return compressed;
}

/*
 * the second and final chunk
 * in a retail key. it's a
 * 7-digit, 7ish number.
 * (a 7ish number is a number
 * where it's sum is divisible
 * by 7. it comes up a lot here.)
 */
std::string RetailKey::chunkB() {
    // very simple. get an algo chunk, then return it. easy peasy.
    std::string result = Common95::genAlgorithmChunk(7);
    return result;
}

/*
 * assemble a full, valid retail key.
 * simple joins chunk a + b with a dash,
 * then returns the value. done!
 */
std::string RetailKey::generate() {
    std::string result;
    result += RetailKey::chunkA();
    result += "-";
    result += RetailKey::chunkB();
    return result;
}

/*
 * same thing as above, but once again allows
 * you to choose if you want to use the algo
 * chunk. if you don't, it works the same way.
 */
std::string RetailKey::generate(bool constantChunkB) {
    std::string result;
    if (constantChunkB) {
        result += RetailKey::chunkA();
        result += "-0077777";
    } else {
        result += RetailKey::chunkA();
        result += "-";
        result += RetailKey::chunkB();
    } return result;
}

bool RetailKey::valid(const std::string& key) {
    // make sure key is right length for retail
    if (key.length() != 11) { return false; }
    // split the key into it's chunks and check the amount of chunks
    std::vector<std::string> chunks = split_string(key, "-");
    if (chunks.size() != 2) { return false; }

    // chunk 1 checks
    if (RetailKey::chunkAIsInvalid(chunks.at(0))) { return false; }

    // chunk 2 checks
    // check length
    if (chunks.at(1).length() != 7) { return false; }
    // check divisibility
    if (!Common95::sumDivsBy7(chunks.at(1))) { return false; }

    // if all checks have passed retail key is valid
    return true;
}

/*
 * a top-level namespace function.
 * allows you to use parameters to
 * change the output instead of namespaces.
*/
std::string generateKey(KeyType keyType, bool constantChunkB) {
    switch(keyType) {
        case RETAIL:
            return RetailKey::generate(constantChunkB);
        case OEM:
            return OEMKey::generate(constantChunkB);
        default:
            return RetailKey::generate(constantChunkB);
    }
}

// overload without constantChunkB. it will use chunk b by default.
std::string generateKey(KeyType keyType) {
    switch(keyType) {
        case RETAIL:
            return RetailKey::generate();
        case OEM:
            return OEMKey::generate();
        default:
            return RetailKey::generate();
    }
}

// overload without any paramaters.
// generates a fully random retail key by default.
std::string generateKey() {
    return RetailKey::generate();
}

KeyType getKeyType(const std::string& key) {
    KeyType keyType = KeyType::NONE;
    // check key lengths for first layer
    switch (key.length()) {
        case 11: {
            keyType = KeyType::RETAIL;
            // second layer checks
            std::regex retailFormat("^([0-9]{3})-([0-9]{7})$");
            if (!std::regex_match(key, retailFormat)) { keyType = KeyType::NONE; }
            break;
        } case 23: {
            keyType = KeyType::OEM;
            // second layer checks
            std::regex oemFormat("^(0[0-9][0-9]|1[0-9][0-9]|2[0-9][0-9]|3[0-6][0-6])(9[5-9]|0[0-3])-OEM-0([0-9]{6})-([0-9]{5})$");
            if (!std::regex_match(key, oemFormat)) { keyType = KeyType::NONE; }
            break;
        } default: { keyType = KeyType::NONE; /* unknown key type */ break; }
    } return keyType;
}

/*
 * a top-level namespace function.
 * allows you to use paramaters to
 * change the output instead of namespaces.
*/
bool validateKey(KeyType keyType, const std::string& key) {
    // use relevant validation function
    switch (keyType) {
        case RETAIL: { return RetailKey::valid(key); }
        case OEM: { return OEMKey::valid(key); }
        default: { return false; /* no key type */ }
    }
}

// key type autodetection
bool validateKey(const std::string& key) {
    // use relevant validation function
    switch (getKeyType(key)) {
        case RETAIL: { return RetailKey::valid(key); }
        case OEM: { return OEMKey::valid(key); }
        default: { return false; /* unknown key type */ }
    }
}
