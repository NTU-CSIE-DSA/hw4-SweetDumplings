#include <algorithm>
#include <bits/stdc++.h>
#include <cstdlib>
#define int long long
#define pii pair<int,int>
#define pb push_back
#define MAXN 100000

using namespace std;

struct Treap {
  Treap *l, *r;
  int pri;
  pii key;
  int price[2], sum[2];
  int sz;
  Treap(int big, int small, pii _key)
    : price{small, big}, sum{small, big}, key(_key), l(nullptr), r(nullptr), pri(rand()), sz(1) {}
};

Treap *treap = nullptr;

void print_treap(Treap *a, int start) {
  if (!a) return;
  if (start == 0) {
    cout << "==========================\n";
    cout << "id, key, price[0], price[1], sum[0], sum[1]\n";
  }
  print_treap(a->l, start + 1);
  for (int i = 0; i < start; ++i) {
    cout << ' ';
  }
  cout << a->key.second << ' ' << a->key.first << ' ' << a->price[0] << ' ' << a->price[1] << ' ' << a->sum[0] << ' ' << a->sum[1] << '\n';
  print_treap(a->r, start + 1);
  if (start == 0) {
    cout << "==========================\n";
  }
}

void push(Treap *a) {
  int l_sz = a->l ? a->l->sz : 0;
  a->sz = 1 + l_sz;
  a->sz += a->r ? a->r->sz : 0;

  a->sum[0] = l_sz ? a->l->sum[0] : 0;
  a->sum[1] = l_sz ? a->l->sum[1] : 0;
  a->sum[0] += a->price[l_sz & 1];
  a->sum[1] += a->price[!(l_sz & 1)];
  a->sum[0] += a->r ? a->r->sum[!(l_sz & 1)] : 0;
  a->sum[1] += a->r ? a->r->sum[(l_sz & 1)] : 0;
  return;
}

Treap *merge(Treap *a, Treap *b) {
  if (!a || !b) return a ? a : b;
  if (a->pri > b->pri) {
    a->r = merge(a->r, b);
    push(a);
    return a;
  }
  else {
    b->l = merge(a, b->l);
    push(b);
    return b;
  }
}

void split(Treap *t, pii k, Treap *&a, Treap *&b) {
  if (!t) a = b = nullptr;
  else if (t->key >= k) {
    a = t;
    split(t->r, k, a->r, b);
    push(a);
  }
  else {
    b = t;
    split(t->l, k, a, b->l);
    push(b);
  }
}

void insert(int id, int big, int small) {
  pii key = {big - small, id};
  Treap *new_treap = new Treap(big, small, key);
  Treap *lt, *rt;
  split(treap, key, lt, rt);
  treap = merge(lt, new_treap);
  treap = merge(treap, rt);
}

void erase(pii key) {
  Treap *lt, *rt, *trash;
  split(treap, key, lt, rt);
  split(lt, {key.first, key.second + 1}, lt, trash);
  treap = merge(lt, rt);
  delete trash;
}

int id2key[MAXN + 1] = {};

signed main () {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int n, m;
  cin >> n >> m;

  for (int i = 1; i <= n; ++i) {
    int big, small;
    cin >> big >> small;

    if (big < small)
      swap(big, small);

    id2key[i] = big - small;
    insert(i, big, small);
  }

  // print_treap(treap, 0);
  cout << treap->sum[0] << '\n';
  
  for (int i = 1; i < m; ++i) {
    int id, big, small;
    cin >> id >> big >> small;

    if (big < small)
      swap(big, small);

    erase({id2key[id], id});
    insert(id, big, small);
    id2key[id] = big - small;
    // print_treap(treap, 0);
    cout << treap->sum[0] << '\n';
  }

  return 0;
}
