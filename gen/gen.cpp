#include "testlib.h"
#include <bits/stdc++.h>

#define MAXN 100000
using namespace std ;

int main(int argc, char *argv[]) {
	registerGen(argc, argv, 1);

  int n = atoi(argv[1]);
  int m = atoi(argv[1]);

  println(n, m);
  for (int i = 0; i < n; ++i) {
    int a = rnd.next(1, MAXN), b = rnd.next(1, MAXN);
    println(a, b);
  }

  for (int i = 0; i < m - 1; ++i) {
    int t = rnd.next(1, n), a = rnd.next(1, MAXN), b = rnd.next(1, MAXN);
    println(t, a, b);
  }
}
