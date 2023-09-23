// ItsEntDev @ 20/09/23
#include <iostream>
#include <string>
#include <vector>
#include "cl_args.hpp"
#include "key95.hpp"
#include "shared_utils.hpp"

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
    bool constantChunkB;
    int amount;
    bool raw;
};

void displayHelp();
Paramaters getParams(CLArgs::Parser *parser);
void validateFrontendPrint(const std::string& key);

// Globals
Paramaters params{};
const std::string version = "2.0.1";

// Product Key Generator CLI for Windows and Linux
// by ItsEntDev @ 20th Sep 2023
//
// Key formats:
// OEM key format: xxxyy-OEM-00NNNNN-zzz
// Retail key format: xxx-NNNNNNN
int main(int argc, char *argv[]) {
    // get parameters from command-line args
    CLArgs::Parser parser(argc, argv);
    params = getParams(&parser);

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
    } std::cout << std::boolalpha;
    // switch statement for the paramaters
    switch (params.keyType) {
        // generate retail key
        case RETAIL: {

            if (!params.raw) {
                // display info about chosen paramaters
                std::cout
                        << "Key info: (key type: retail, amount: "
                        << params.amount
                        << ", constantChunkB: "
                        << params.constantChunkB
                        << ")"
                << std::endl;
            } /* just using this to make the space look better */
            // use the RetailKey::generate() method overload, which is just a wrapper around having to use a bunch
            // of chunk methods. it wasn't hard to write so I did it. you can also just call generateKey from
            // key95's global namespace which has three different overloads for ease of use
            for (int i = 0; i < params.amount; ++i) {
                std::cout << RetailKey::generate(params.constantChunkB) << std::endl;
            } // params.raw check
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
                        << "Key info: (key type: retail, amount: "
                        << params.amount
                        << ", constantChunkB: "
                        << params.constantChunkB
                        << ")"
                        << std::endl;
            }
            // read retail's comments
            for (int i = 0; i < params.amount; ++i) {
                std::cout << OEMKey::generate(params.constantChunkB) << std::endl;
            } // params.raw check
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
        "--version/-V\n" <<
        "--help/-H\n" <<
        "--mode/-M generate/validate\n" <<
        "--output-raw/-R\n" <<
        "(VALIDATE MODE ONLY):\n" <<
        "   --keys/-L key1,key2,key3\n" <<
        "(GENERATE MODE ONLY):\n" <<
        "   --key-type/-K retail/oem\n" <<
        "   --amount/-A number\n" <<
        "   --constant-chunk-b/-B\n" <<
        "   --output-raw/-R\n"
    << std::endl;
    std::cout << "Arguments can be in any order. --help and --version override everything else." << std::endl;
    std::cout << "Argument names are CAsE SeNSitIVE but values are not. (Except keys)" << std::endl;
    std::cout << "If arguments are not supplied, defaults will be used." << std::endl;
    exit(0);
}

// gets paramaters to choose right option series
Paramaters getParams(CLArgs::Parser *parser) {
    Paramaters paramaters{
        Mode::GENERATE,
        {},
        KeyType::RETAIL,
        false,
        1,
        false
    }; // paramater create
    // completely removed for revamp using cl_args
    CLArgs::Argument helpArg = parser->argument("help", "H");
    CLArgs::Argument versionArg = parser->argument("version", "V");
    CLArgs::Argument modeArg = parser->argument("mode", "M");
    CLArgs::Argument outputRawArg = parser->argument("output-raw", "R");
    CLArgs::Argument gKeyTypeArg = parser->argument("key-type", "K");
    CLArgs::Argument gAmountArg = parser->argument("amount", "A");
    CLArgs::Argument gChunkBArg = parser->argument("constant-chunk-b", "B");
    CLArgs::Argument vKeylistArg = parser->argument("keys", "L");

    if (helpArg.present) { displayHelp(); }
    if (versionArg.present) {
        std::cout << "ent's windows 95 key generator - version " << version << std::endl; exit(0);
    } if (modeArg.present && !modeArg.empty) {
        if (asLowercase(modeArg.value) == "validate") { paramaters.mode = Mode::VALIDATE; }
    } if (outputRawArg.present) { paramaters.raw = true; }
    if (gKeyTypeArg.present && !gKeyTypeArg.empty) {
        if (asLowercase(gKeyTypeArg.value) == "oem") { paramaters.keyType = KeyType::OEM; }
    } if (gAmountArg.present && !gAmountArg.empty) { paramaters.amount = std::stoi(gAmountArg.value); }
    if (gChunkBArg.present) { paramaters.constantChunkB = true; }
    if (vKeylistArg.present && !vKeylistArg.empty) {
        std::vector<std::string> keylist = split_string(vKeylistArg.value, ",");
        for (const std::string& key : keylist) { paramaters.toValidate.push_back(key); }
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
