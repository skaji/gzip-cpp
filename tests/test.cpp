#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "picotest.h"
#include "../gzip.hpp"

using namespace std;
using namespace compression;

static bool slurp(const char* filename, string& content) {
  std::ostringstream out;
  ifstream in;
  in.open(filename, ifstream::in);
  if (in.fail()) {
    return false;
  }
  out << in.rdbuf();
  in.close();
  content.assign(out.str());
  return true;
}

void basic_test() {
  ok(!gzip::is_gzip(""));
  ok(!gzip::is_gzip("a"));
  ok(!gzip::is_gzip("\x1f"));
  ok(gzip::is_gzip("\x1f\x8b"));

  string empty_gzip1("\x1F\x8B\x08\x00\x00\x00\x00\x00\x00\x13\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00", 20); // mac
  string empty_gzip2("\x1F\x8B\x08\x00\x00\x00\x00\x00\x00\x03\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00", 20); // linux
  ok(gzip::is_gzip(empty_gzip1));
  ok(gzip::is_gzip(empty_gzip2));
  string out1;
  string out2;
  ok(gzip::decompress(empty_gzip1, out1));
  ok(gzip::decompress(empty_gzip2, out2));
  ok(out1 == "");
  ok(out2 == "");
  string gzipped;
  ok(gzip::compress("", gzipped));
  ok(gzipped == empty_gzip1 || gzipped == empty_gzip2);

  string a_string("\x1F\x8B\x08\x00\x28\x23\x24\x5A\x00\x03\x4B\x04\x00\x43\xBE\xB7\xE8\x01\x00\x00\x00", 21);
  ok(gzip::is_gzip(a_string));
  string out;
  ok(gzip::decompress(a_string, out));
  ok(out == "a");

  string plain("abc\x00" "efg", 7);
  gzipped.clear();
  ok(gzip::compress(plain, gzipped));
  out.clear();
  ok(gzip::decompress(gzipped, out));
  ok(out.size() == 7);
  ok(out == plain);
}

void loop_test() {
  string c;
  slurp("../libcompression_gzip.a", c);
  string plain;
  for (int i = 0; i < 10000; ++i) {
    plain += c;
    if (plain.size() > 256*1024) {
      break;
    }
  }
  note("plain string size: %ld", plain.size());

  const int times = 5000;
  note("loop start: times %d, pid %d", times, getpid());
  fflush(stdout);

  for (int i = 0; i < times; ++i) {
    string gzipped;
    gzip::compress(plain, gzipped);
    gzip::is_gzip(gzipped);
    string back;
    gzip::decompress(gzipped, back);
    if (i % (times/10) == 0) {
      ok(plain == back);
      fflush(stdout);
    }
  }
}

int main() {
  subtest("basic", basic_test);
  subtest("loop", loop_test);
  done_testing();
  return 0;
}
