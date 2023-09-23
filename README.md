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
win95-keygen's frontend uses a header called 'cl_args.hpp'. It's very simple and basic. Usage docs: Read the code.
### Command-line arguments
(MODE INDEPENDENT):
  --version/-V
  --help/-H
  --mode/-M generate/validate
  --output-raw/-R
  
(VALIDATE MODE ONLY):
  --keys/-L key1,key2,key3
  
(GENERATE MODE ONLY):
  --key-type/-K retail/oem
  --amount/-A number
  --constant-chunk-b/-B
  --output-raw/-R

Arguments can be in any order. --help and --version override everything else.
Argument names are CAsE SeNSitIVE but values are not. (Except keys)
If arguments are not supplied, defaults will be used.

### What do the arguments mean?
mode: whether to generate keys or check them.
mode generate sub-arguments:
  - key type is if the CLI should generate Retail keys or OEM keys.
  - amount is an integer representing how many keys it should output.
  - constant chunk b is whether the n % 7 randomiser should be used or if it should use an always-valid (0077777). by default it will be random, but you can make it constant by passing this flag.
  - output raw is if it should give you some info on selected options and tell you when it's done, or just print the keys. useful for writing to a file.

mode validate sub-arguments:
  - a list of comma-seperated keys. order is what order they will be checked in.

## Will this be updated?
If there are any features I feel I really need to add... maybe.
But I probably won't. So, yeah. I'll try and get a windows build,
but MingW64 doesn't work on my machine. (headers are missing)
