TODO:
* ~~Fix include guards to be full path, not just filename~~
* Change all occurrences of size_t to std::size_t
* ~~Use std::unreachable() any place with known unreachable?~~
* ~~Remove underscores from template parameters because they're causing warnings.~~
* Add concept checks to autos
* Add random access container wrapper that pads out with dummy (or truncates) to a certain size
* Add random access container wrapper that mmap
* Add random access container wrapper that decompresses as you read
* Add divide-and-conquer aligner interface methods and algorithm