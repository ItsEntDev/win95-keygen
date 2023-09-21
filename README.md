# win95-keygen - A Windows 95 key generation/validation CLI
win95-keygen is a CLI tool for generating and checking validity of Windows 95 product keys, both OEM and Retail.
Well, really, it's a frontend to key95.h.

## IFAQ

### Why Windows 95?
Because I made this project while I was 2 weeks into learning C++, and the algorithm
was *relatively* easy to implement. It also taught me about regex and vectors.

### Is it the best one?
Almost certainly no. I'm not a great dev, and there is loads of bloat in shared_utils.h.
Most of the functions in there are redundant and unneeded.

### Can I contribute?
Sure. If you addition 1) is useful 2) builds on both platforms and functions correctly
I'll accept a PR.

### Can I make issues?
Yes, but I am quite unlikely to fix them. I'm just a hobby dev interested in retro stuff.

### Can I use key95?
Yes. It's open-source for a reason.
It isn't much, just a library for all the Win95 product-key related stuff.

## Usage Docs
### win95-keygen <mode (generate/validate)>

### arguments for generate mode:
win95-keygen generate <key type (retail/oem)> <amount (int)> <use chunk b (true/false 1/0)> <output raw (true/false 1/0)>

### arguments for validate mode:
win95-keygen validate <OPTIONAL <output raw (true/false 1/0)>> <keychain1 (key1,key2,key3)> <keychain2 (key1,key2,key3)> etc...

### what do the arguments mean?
mode: whether to generate keys or check them.
mode generate sub-arguments:
  - key type is if the CLI should generate Retail keys or OEM keys.
  - amount is an integer representing how many keys it should output.
  - use chunk b is whether the n % 7 randomiser should be used or if it should use an always-valid (0077777). setting false/0 may make it generate faster.
  - output raw is if it should give you some info on selected options and tell you when it's done, or just print the keys. useful for writing to a file

mode validate sub-arguments:
  - OPTIONAL output raw is the same thing, but optional. if not given it is set to false.
  - a list of space-delimited keychains, which are each a list of comma-seperated keys. order is what order they will be checked in.

Arguments MUST be passed in order, but you do not have to pass all of them.
IE: `win95-keygen generate retail 10` (win95-keygen mode key-type amount use-chunk-b=default output-raw=default) is ok but `win95-keygen generate 10 true retail` (win95-keygen mode amount use-chunk-b key-type) is not.
