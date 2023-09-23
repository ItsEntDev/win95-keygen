// ItsEntDev @ 22/09/23
#include <string>
#include <vector>

#ifndef LEARNCPP_CL_ARGS_HPP
#define LEARNCPP_CL_ARGS_HPP

namespace CLArgs {
    using ArgListType = std::vector<std::string>;

    struct Parameter {
        std::string longName;
        std::string shortName;
    };

    struct Argument {
        const Parameter parameter;
        bool present;
        bool empty;
        std::string value;
    };

    struct ArgPair {
        std::string name;
        std::string value;
    };

    class Parser {
    private:
        // this is how you get the vector argv
        static CLArgs::ArgListType vectorizeArgv(int argc, char **argv);
        static std::string stripDashes(std::string arg);
        CLArgs::ArgListType args;
        std::vector<CLArgs::ArgPair> validArgs;

    public:
        // construct Parser with the 'args' attribute filled
        Parser(int argc, char **argv);
        // why is argv a vector<string>? because it's easier.
        // there's only a single function because it has all the needed data
        // due to the Parameter included.
        /*
         * Returns an Argument struct which contains data about
         * the argument such as whether it is there or not and
         * what its value is. If present is false, value is empty.
        */
        CLArgs::Argument argument(CLArgs::Parameter parameter);
        CLArgs::Argument argument(std::string longName, std::string shortName);
        CLArgs::Argument argument(std::string longName);

        // compressed string of arguments
        std::string argsString;

    };
}

#endif //LEARNCPP_CL_ARGS_HPP
