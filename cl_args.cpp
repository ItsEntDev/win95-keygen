// ItsEntDev @ 22/09/23
#include "cl_args.hpp"

CLArgs::Parser::Parser(int argc, char **argv) {
    this->args = CLArgs::Parser::vectorizeArgv(argc, argv);
    std::string currentArg;
    std::string nextArg;
    bool skip = false;
    for (uint64_t i = 0; i < this->args.size(); ++i) {
        currentArg = this->args.at(i);
        if (i + 1 < this->args.size()) { nextArg = this->args.at(i + 1); } else { nextArg = "-"; }
        // continue filling argsString
        this->argsString += currentArg + " ";
        // check to make sure argument doesn't already exist in validArgs
        skip = false;
        for (const CLArgs::ArgPair& pair : this->validArgs) { if (currentArg == pair.name) { skip = true; } }
        // check if currentArg is an argument name
        if (currentArg.starts_with("-") && !skip) {
            if (nextArg.starts_with("-")) {
                this->validArgs.push_back({currentArg, ""});
            } else {
                this->validArgs.push_back({currentArg, nextArg});
            }
        }
    }
}

std::string CLArgs::Parser::stripDashes(std::string arg) {
    unsigned short counter = 0;
    while (arg.size() > 1 && counter <= 2 && arg.starts_with("-")) {
        arg.erase(arg.begin());
    } return arg;
}

CLArgs::ArgListType CLArgs::Parser::vectorizeArgv(int argc, char **argv) {
    CLArgs::ArgListType result;
    for (int i = 1; i < argc; ++i) {
        result.emplace_back(argv[i]);
    } return result;
}

CLArgs::Argument CLArgs::Parser::argument(const CLArgs::Parameter parameter) {
    CLArgs::Argument argument{parameter, false, true, ""};
    std::string strippedName;
    bool found = false;
    for (const CLArgs::ArgPair& pair : this->validArgs) {
        strippedName = CLArgs::Parser::stripDashes(pair.name);
        if (strippedName == parameter.longName || strippedName == parameter.shortName) {
            argument.present = true;
            found = true;
        } if (!pair.value.empty() && found) {
            argument.empty = false; argument.value = pair.value;
        } if (found) { break; }
    } return argument;
}

CLArgs::Argument CLArgs::Parser::argument(std::string longName, std::string shortName) {
    CLArgs::Argument argument{{longName, shortName}, false, true, ""};
    std::string strippedName;
    bool found = false;
    for (const CLArgs::ArgPair& pair : this->validArgs) {
        strippedName = CLArgs::Parser::stripDashes(pair.name);
        if (strippedName == longName || strippedName == shortName) {
            argument.present = true;
            found = true;
        } if (!pair.value.empty() && found) {
            argument.empty = false; argument.value = pair.value;
        } if (found) { break; }
    } return argument;
}

CLArgs::Argument CLArgs::Parser::argument(std::string longName) {
    CLArgs::Argument argument{{longName, ""}, false, true, ""};
    std::string strippedName;
    bool found = false;
    for (const CLArgs::ArgPair& pair : this->validArgs) {
        strippedName = CLArgs::Parser::stripDashes(pair.name);
        if (strippedName == longName) {
            argument.present = true;
            found = true;
        } if (!pair.value.empty() && found) {
            argument.empty = false; argument.value = pair.value;
        } if (found) { break; }
    } return argument;
}
