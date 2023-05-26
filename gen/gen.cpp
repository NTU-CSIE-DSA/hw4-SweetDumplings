#include "testlib.h"
#include <bits/stdc++.h>

#define MAXN 100000
#define MAXP 1000000000
using namespace std ;

int main(int argc, char *argv[]) {
	registerGen(argc, argv, 1);

  int n = atoi(argv[1]);
  int m = atoi(argv[2]);

  println(n, m);
  for (int i = 0; i < n; ++i) {
    int a = rnd.next(1, MAXP), b = rnd.next(1, MAXP);
    println(a, b);
  }

  for (int i = 0; i < m - 1; ++i) {
    int t = rnd.next(1, n), a = rnd.next(0, MAXP), b = rnd.next(0, MAXP), c = rnd.next(0, MAXP), d = rnd.next(0, MAXP);
    println(t, a, b, c, d);
  }
}
