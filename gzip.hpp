#ifndef COMPRESSION_GZIP_HPP
#define COMPRESSION_GZIP_HPP

#include <string>

namespace compression {
namespace gzip {

bool is_gzip(const std::string& in);
bool compress(const std::string& in, std::string& out);
bool decompress(const std::string& in, std::string& out);

}
}

#endif
