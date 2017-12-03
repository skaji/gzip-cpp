# gzip in C++ [![Build Status](https://travis-ci.org/skaji/gzip-cpp.svg?branch=master)](https://travis-ci.org/skaji/gzip-cpp)

Simple zlib wrapper for C++

```cpp
#include <string>
#include <gzip.hpp>
using std::string;

int main() {
  string plain = "This is a plain text.";
  string gzipped;
  bool ok1 = compression::gzip::compress(plain, gzipped);
  bool ok2 = compression::gzip::is_gzip(gzipped);

  string back;
  bool ok3 = compression::gzip::decompress(gzipped, back);
  assert(plain == back);
}
```

# Based on

https://oku.edu.mie-u.ac.jp/~okumura/compression/zlib.html

http://s-yata.jp/docs/zlib/

# Author

Shoichi Kaji

# License

MIT
