#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "gzip.hpp"
using namespace std;

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

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: example FILENAME" << endl;
    exit(1);
  }

  const char* filename = argv[1];
  string plain;
  if (!slurp(filename, plain)) {
    cerr << "Cannot open " << filename << endl;
    exit(1);
  }

  compression::cpp::Gzip gzip;

  string gzipped;
  if (!gzip.compress(plain, gzipped)) {
    cerr << "Failed to gzip data" << endl;
    exit(1);
  }

  string back;
  if (!gzip.decompress(gzipped, back)) {
    cerr << "Failed to gunzip data" << endl;
    exit(1);
  }

  cerr << filename << endl;
  cerr << " plain   size = " << plain.size()   << endl;
  cerr << " gzipped size = " << gzipped.size() << endl;
  cerr << endl;

  if (!gzip.is_gzip(gzipped)) {
    cerr << "SOMETHING WRONG!" << endl;
    exit(1);
  }
  if (plain != back) {
    cerr << "SOMETHING WRONG!" << endl;
    exit(1);
  }

  cout << gzipped;

  return 0;
}
