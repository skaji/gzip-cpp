# gzip in C++

Simple zlib wrapper for C++

```cpp
#include <string>
#include <gzip.hpp>
using std::string;

int main() {
  compression::cpp::Gzip gzip;

  string plain = "This is a plain text.";
  string gzipped;
  bool ok1 = gzip.compress(plain, gzipped);
  bool ok2 = gzip.is_gzip(gzipped);

  string back;
  bool ok3 = gzip.decompress(gzipped, back);
  assert(plain == back);
}
```

# Based on

https://oku.edu.mie-u.ac.jp/~okumura/compression/zlib.html

# Author

Shoichi Kaji

# License

MIT
