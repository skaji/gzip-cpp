#include "gzip.hpp"

#include <zlib.h>
#include <sstream>

const size_t BUFFER_SIZE = 65536;

using std::ostringstream;
using std::string;

namespace {

typedef int (*FUNC)(z_stream* z, int z_const);

bool call_(z_stream& z, FUNC func, int compress_const, const string& in, string& out) {
  char outbuf[BUFFER_SIZE];
  z.next_in = (Bytef*)in.c_str();
  z.avail_in = in.size();
  z.next_out = (Bytef*)outbuf;
  z.avail_out = BUFFER_SIZE;

  ostringstream out_ss;
  for (;;) {
    int status = func(&z, compress_const);
    if (status == Z_STREAM_END) {
      break;
    } else if (status != Z_OK) {
      return false;
    }

    if (z.avail_out == 0) {
      out_ss.write(outbuf, BUFFER_SIZE);
      z.next_out = (Bytef*)outbuf;
      z.avail_out = BUFFER_SIZE;
    }
  }
  int rest = BUFFER_SIZE - z.avail_out;
  if (rest > 0) {
    out_ss.write(outbuf, rest);
  }
  out.assign(out_ss.str());
  return true;
}

}

namespace compression {
namespace cpp {

Gzip::Gzip() {}
Gzip::~Gzip() {}

bool Gzip::is_gzip(const string& in) {
  return in.size() > 1 && in[0] == '\x1F' && in[1] == '\x8B';
}

bool Gzip::compress(const string& in, string& out) {
  z_stream z;
  z.zalloc = Z_NULL;
  z.zfree = Z_NULL;
  z.opaque = Z_NULL;
  if (deflateInit2(&z, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
    return false;
  }
  bool ok = call_(z, deflate, Z_FINISH, in, out);
  return (deflateEnd(&z) == Z_OK) && ok;
}

bool Gzip::decompress(const string& in, string& out) {
  z_stream z;
  z.zalloc = Z_NULL;
  z.zfree = Z_NULL;
  z.opaque = Z_NULL;
  if (inflateInit2(&z, MAX_WBITS + 16) != Z_OK) {
    return false;
  }
  bool ok = call_(z, inflate, Z_NO_FLUSH, in, out);
  return (inflateEnd(&z) == Z_OK) && ok;
}

}
}
