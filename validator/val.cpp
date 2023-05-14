#include <bits/stdc++.h>
#include <cassert>
#include "testlib.h"

#define MAXN 100000
#define MAXP 1000000000

using namespace std;

signed main (signed argc, char** argv) {
  registerValidation(argc, argv);

  int n = inf.readInt(1, MAXN);
  inf.readSpace();
  int m = inf.readInt(1, MAXN);
  inf.readEoln();

  for (int i = 0; i < n; ++i) {
    int a = inf.readInt(1, MAXP);
    inf.readSpace();
    int b = inf.readInt(1, MAXP);
    inf.readEoln();
  }

  for (int i = 0; i < m - 1; ++i) {
    int t = inf.readInt(1, n);
    inf.readSpace();
    int a = inf.readInt(0, MAXP);
    inf.readSpace();
    int b = inf.readInt(0, MAXP);
    inf.readSpace();
    int c = inf.readInt(0, MAXP);
    inf.readSpace();
    int d = inf.readInt(0, MAXP);
    inf.readEoln();
  }
  inf.readEof();

  return 0;
}

