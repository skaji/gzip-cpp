#ifndef COMPRESSION_CPP_GZIP_HPP
#define COMPRESSION_CPP_GZIP_HPP

#include <string>

namespace compression {
namespace cpp {

class Gzip {
 public:
  Gzip();
  ~Gzip();
  bool compress(const std::string& in, std::string& out);
  bool decompress(const std::string& in, std::string& out);
  bool is_gzip(const std::string& in);
};

}
}

#endif
