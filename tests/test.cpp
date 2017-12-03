#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstdlib>
#include "picotest.h"
#include "../gzip.hpp"

using namespace std;
using namespace compression;

static bool random_string(unsigned int length, string& content) {
  std::ostringstream out;
  ifstream in;
  in.open("/dev/urandom", ifstream::in);
  if (in.fail()) {
    return false;
  }
  char buf[64];
  for (;;) {
    in.read(buf, 64);
    content.append(buf, 64);
    if (content.size() > length) {
      break;
    }
  }
  in.close();
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
  string plain;
  if (!random_string(256*1024, plain)) {
    exit(1);
  }
  note("plain string size: %ld", plain.size());

  const int times = 5000;
  note("loop start: times %d, pid %d", times, getpid());
  fflush(stdout);

  bool success = true;
  for (int i = 0; i < times; ++i) {
    string gzipped;
    success &= gzip::compress(plain, gzipped);
    success &= gzip::is_gzip(gzipped);
    string back;
    success &= gzip::decompress(gzipped, back);
    success &= (plain == back);
    if (i % (times/10) == 0) {
      note("%6d / %d (size %ld -> %ld)", i, times, plain.size(), gzipped.size());
      fflush(stdout);
    }
  }
  ok(success);
}

int main() {
  subtest("basic", basic_test);
  subtest("loop", loop_test);
  return done_testing();
}
