// ItsEntDev @ 20/09/23
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "key95.h"
#include "shared_utils.h"

#if !defined _WIN32 && !defined __linux__
#error "Platform is not supported."
#endif

enum Mode {
    GENERATE,
    VALIDATE
};

struct Paramaters {
    Mode mode;
    std::vector<std::string> toValidate;
    KeyType keyType;
    bool useChunkB;
    int amount;
    bool raw;
};

void displayHelp();
Paramaters getParams(int argc, char *argv[]);
void validateFrontendPrint(const std::string& key);

// Globals
Paramaters params{};

// Product Key Generator CLI for Windows and Linux
// by ItsEntDev @ 20th Sep 2023
//
// Key formats:
// OEM key format: xxxyy-OEM-00NNNNN-zzz
// Retail key format: xxx-NNNNNNN
int main(int argc, char *argv[]) {
    // get paramaters from command-line args
    params = getParams(argc, argv);
    /*
     * a little note on params.raw
     * params.raw is for when you want
     * to write the keys to a file.
     * it removes the key info and done messages
     * so you only capture the win95 keys.
     */

    if (params.mode == VALIDATE) {
        for (const std::string& key : params.toValidate) { validateFrontendPrint(key); }
        exit(0);
    }

    // switch statement for the paramaters
    switch (params.keyType) {
        // generate retail key
        case RETAIL: {
            if (!params.raw) {
                // display info about chosen paramaters
                std::cout
                        << "Key info: (keyType=retail,useChunkB="
                        << params.useChunkB
                        << ",amount="
                        << params.amount
                        << ")"
                << std::endl;
            } /* just using this to make the space look better */
            // use the RetailKey::generate() method overload, which is just a wrapper around having to use a bunch
            // of chunk methods. it wasn't hard to write so I did it. you can also just call generateKey from
            // key95's global namespace which has three different overloads for ease of use
            for (int i = 0; i < params.amount; ++i) { std::cout << RetailKey::generate(params.useChunkB) << std::endl; }
            if (!params.raw) {
                // inform that keygen is finished
                std::cout << "Key generation finished." << std::endl;
            } // and exit
            exit(0);
        }
        // generate oem key
        case OEM: {
            // same thing as retail but it says OEM
            if (!params.raw) {
                std::cout
                        << "Key info: (keyType=retail,useChunkB="
                        << params.useChunkB
                        << ",amount="
                        << params.amount
                        << ")"
                        << std::endl;
            }
            // read retail's comments
            for (int i = 0; i < params.amount; ++i) { std::cout << OEMKey::generate(params.useChunkB) << std::endl; }
            // params.raw check
            if (!params.raw) {
                // inform that keygen is finished
                std::cout << "Key generation finished." << std::endl;
            } // and exit
            exit(0);
        }
        // statement should be impossible to reach, but just in case
        default: {
            if (!params.raw) {
                // display problem
                std::cout << "params.keyType not matched" << std::endl;
            } // and exit WITH CODE 1
            exit(1);
        }
    }
}

// Displays help for command-line arguments
void displayHelp() {
    // it's just std::cout
    std::cout << "Command line arguments available:\n" << std::endl;
    std::cout <<
        "<mode (generate/validate)>\n\n" \
        "generate mode args: "
        "<key type (oem/retail)> " \
        "<randomize chunk B (true/false)> " \
        "<amount of keys (int)>" \
        "<output raw>\n\n" \
        "validate mode args: [raw] <keychain1(<key1>,<key2>)> <keychain2(<key1>,<key2>)>\n" \
        "SPACES ARE NOT PERMITTED INSIDE A KEYCHAIN\n"
    << std::endl;
    std::cout << "If arguments are not supplied, defaults will be used." << std::endl;
    std::cout << "Arguments MUST be in the correct order, or will be ignored." << std::endl;
    exit(0);
}

// gets paramaters to choose right option series
Paramaters getParams(int argc, char *argv[]) {
    Paramaters paramaters{
        Mode::GENERATE,
        {},
        KeyType::RETAIL,
        true,
        1,
        false
    }; // paramater create
    if (argc > 1) {
        // display help menu
        if (std::strcmp(argv[1], "help") == 0) { displayHelp(); }
        // check mode
        if (std::strcmp(argv[1], "generate") == 0) { paramaters.mode = Mode::GENERATE; }
        if (std::strcmp(argv[1], "validate") == 0) { paramaters.mode = Mode::VALIDATE; }
        // go into very badly designed argument checks for generate
        if (argc > 2 && paramaters.mode == GENERATE) {
            if (std::strcmp(argv[2], "retail") == 0) { paramaters.keyType = KeyType::RETAIL; }
            else if (std::strcmp(argv[2], "oem") == 0) { paramaters.keyType = KeyType::OEM; }
            if (argc > 3) {
                // nested ifs cause its easy
                // it checks for it being 'false' or '0'
                // because I personally kept forgetting
                // you had to write false and not 0
                if (std::strcmp(argv[3], "false") == 0 ||
                    std::strcmp(argv[3], "0") == 0) { paramaters.useChunkB = false; }
                if (argc > 4) {
                    paramaters.amount = std::stoi(argv[4]);
                    if (argc > 5) {
                        // param raw is for writing to files
                        if (std::strcmp(argv[5], "true") == 0 || std::strcmp(argv[5], "1") == 0) {
                            paramaters.raw = true;
                        }
                    }
                }
            }
        } else if (argc > 2 && paramaters.mode == VALIDATE) {
            // parse validation keys
            // passing key format
            // keychain: multiple keys, any type, no spaces, seperated by commas <key1>,<key2>,<key3>
            // args: multiple chains, seperated by spaces <keychain1> <keychain 2> <keychain 3>
            int offset = 2;
            if (std::strcmp(argv[2], "raw") == 0) { paramaters.raw = true; offset = 3; }
            for (int i = 0; i < argc - offset; ++i) {
                std::vector<std::string> keyChain = split_string(argv[i + offset], ",");
                for (const std::string& key : keyChain) { paramaters.toValidate.push_back(key); }
            }
        } else {
            displayHelp();
        }
    } return paramaters;
}

void validateFrontendPrint(const std::string& key) {
    KeyType keyType = getKeyType(key);
    std::string type;
    std::string valid = "Failed";
    switch (keyType) {
        case KeyType::RETAIL: {
            type = "Retail";
            if (validateKey(KeyType::RETAIL, key)) { valid = "Passed"; }
            break;
        } case KeyType::OEM: {
            type = "OEM";
            if (validateKey(KeyType::OEM, key)) { valid = "Passed"; }
            break;
        } default: { type = "Unknown"; valid = "Failed"; break; }

    } if (params.raw) {
        std::cout << asLowercase(valid) << std::endl;
    } else { std::cout << "Key `" << key << "` (" << type << "): " << valid << std::endl; }

}
