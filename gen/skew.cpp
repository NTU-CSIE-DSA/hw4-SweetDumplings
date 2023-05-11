#include "testlib.h"
#include <bits/stdc++.h>

#define pii pair<int, int>

#define MAXN 100000
using namespace std;

int main(int argc, char *argv[]) {
	registerGen(argc, argv, 1);

  int n = atoi(argv[1]);
  int m = atoi(argv[1]);

  println(n, m);
  pair<int, int> arr[MAXN] = {};
  for (int i = 0; i < n; ++i) {
    int a = rnd.next(1, MAXN), b = rnd.next(1, MAXN);
    arr[i] = {a, b};
  }

  sort(arr, arr + n, [] (pii a, pii b) {return abs(a.first - a.second) < abs(b.first - b.second);});

  for (int i = 0; i < n; ++i) {
    println(arr[i].first, arr[i].second);
  }

  for (int i = 0; i < m - 1; ++i) {
    int t = rnd.next(1, n), a = rnd.next(1, MAXN), b = rnd.next(1, MAXN);
    println(t, a, b);
  }
}
