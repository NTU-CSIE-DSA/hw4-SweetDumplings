#include "stdio.h"
#include "unistd.h"
#include "assert.h"

#define int long long

#define MAXN 100000
#define BLACK 0
#define RED 1
#define FST 0 // first hand
#define SEC 1 // second hand
#define LC 0 // left child
#define RC 1 // right child

// r-b tree function
#define ND(x) (pool[(x)]) // transform id into node
#define PAR(x) (pool[(x)].pid) // return parent id
#define GPAR(x) (PAR(PAR((x)))) // return grand parent id
#define CHD(x, lr) (ND((x)).chd[(lr)])
#define IS_RC(x) (ND(PAR((x))).chd[RC] == (x)) // check if the node is right child
#define NO_LC(x) (ND((x)).chd[LC] == 0) // check if a node has left child
#define UNCLE(x) (ND(GPAR((x))).chd[!IS_RC(PAR((x)))]) // return uncle id
#define SIBLING(x) (ND(PAR(x)).chd[!IS_RC((x))]) // return sibling id
#define KEY(x) (ND((x)).price[RC] - ND((x)).price[LC]) // get key

// queue function
#define INC(x) ((x) = ((x) == MAXN ? 0 : (x) + 1))


typedef struct Node {
  int price[2], sum[2], chd[2];
  int color, sz, id, pid;
} Node;

Node pool[MAXN + 1];
int queue[MAXN + 1];
int fnt = 0, bck = MAXN;
int rb_root = 0;
int *rb_root_ptr = &rb_root;
int id2tid[MAXN + 1];
int tid2id[MAXN + 1];

void print_rb_tree(int root, int layer) {
  assert(layer < 10);
  if (layer == 0) {
    printf("f====================\n");
    printf("id, key, color, sum[0], sum[1], val[0], val[1]\n");
  }
  if (root == 0)
    return;
  print_rb_tree(ND(root).chd[LC], layer + 1);
  for (int i = 0; i < layer; ++i) {
    printf(" ");
  }
  printf("%lld, %lld, %lld, %lld, %lld, %lld, %lld\n",
      tid2id[root], KEY(root), ND(root).color,
      ND(root).sum[0], ND(root).sum[1],
      ND(root).price[0], ND(root).price[1]);
  print_rb_tree(ND(root).chd[RC], layer + 1);

  if (layer == 0) {
    printf("e====================\n");
  }
  return;
}

// gen and init a new node
int new_node (int big, int small, int pid) {
  int id = queue[fnt];
  INC(fnt);
  pool[id].price[FST] = small;
  pool[id].price[SEC] = big;
  pool[id].sum[FST] = small;
  pool[id].sum[SEC] = big;
  pool[id].chd[LC] = pool[id].chd[RC] = 0;
  pool[id].color = RED;
  pool[id].sz = 1;
  pool[id].id = id;
  pool[id].pid = pid;
  return id;
}

void node_swap (int a_nid, int b_nid) {
  Node tmp = ND(a_nid);
  ND(a_nid).price[FST] = ND(b_nid).price[FST];
  ND(a_nid).price[SEC] = ND(b_nid).price[SEC];

  ND(b_nid).price[FST] = tmp.price[FST];
  ND(b_nid).price[SEC] = tmp.price[SEC];

  int tmpid = tid2id[a_nid];
  tid2id[a_nid] = tid2id[b_nid];
  tid2id[b_nid] = tmpid;

  id2tid[tid2id[a_nid]] = a_nid;
  id2tid[tid2id[b_nid]] = b_nid;
}

void push(int nid) {
  int lc = CHD(nid, LC), rc = CHD(nid, RC);

  ND(nid).sz = ND(lc).sz + ND(rc).sz + 1;
  ND(nid).sum[FST] = ND(lc).sum[FST] + ND(rc).sum[!(ND(lc).sz & 1)] + ND(nid).price[ND(lc).sz & 1];
  ND(nid).sum[SEC] = ND(lc).sum[SEC] + ND(rc).sum[ND(lc).sz & 1] + ND(nid).price[!(ND(lc).sz & 1)];

  return;
}

void push_up(int nid) {
  push(nid);
  if (PAR(nid)) {
    push_up(PAR(nid));
  }
}

// put the deleted node back to the queue
void del_node (int tid) {
  queue[bck] = tid;
  INC(bck);
  return;
}

// the insert of basic BST
int insert (int *root, int big, int small, int pid) {
  int curr_big = pool[*root].price[SEC], curr_small = pool[*root].price[FST];
  if (*root == 0) {
    *root = new_node(big, small, pid);
    return *root;
  }
  else {
    int chd = (big - small < curr_big - curr_small);
    return insert(&(pool[*root].chd[chd]), big, small, *root);
  }
}

int delete_nd (int id) {
  if (!ND(id).chd[LC] || !ND(id).chd[RC])
    return id;
  else {
    int tar = ND(id).chd[LC];
    while (ND(tar).chd[RC]) {
      tar = ND(tar).chd[RC];
    }
    
    node_swap(id, tar);
    return tar;
  }
}

// rotate, nid is the origin root, and dir is the index of child which will be the new root.
void rotate(int nid, int dir) {
  int pid = PAR(nid), cid = ND(nid).chd[dir];

  // link cid to pid
  if (pid)
    ND(pid).chd[IS_RC(nid)] = cid;
  else
    rb_root = cid;
  ND(cid).pid = pid;

  // link the child of cid to nid
  ND(nid).chd[dir] = ND(cid).chd[!dir];
  ND(ND(cid).chd[!dir]).pid = nid;

  // link cid to nid
  ND(cid).chd[!dir] = nid;
  ND(nid).pid = cid;

  // update data
  push(nid);
  push(cid);

  return;
}

// the complex flow controll of rb insertion
void casely_insert(int nid) {
  if (PAR(nid) == 0)
    ND(nid).color = BLACK;
  else if (ND(PAR(nid)).color == BLACK)
    ;
  else if (UNCLE(nid) && ND(UNCLE(nid)).color == RED) {
    ND(PAR(nid)).color = ND(UNCLE(nid)).color = BLACK;
    ND(GPAR(nid)).color = RED;
    casely_insert(GPAR(nid));
  }
  else {
    if (IS_RC(nid) != IS_RC(PAR(nid))) {
      rotate(PAR(nid), IS_RC(nid));
      nid = ND(nid).chd[IS_RC(nid)];
      // push(nid);
      // push(PAR(nid));
    }

    ND(GPAR(nid)).color = RED;
    ND(PAR(nid)).color = BLACK;
    rotate(GPAR(nid), IS_RC(PAR(nid)));
  }
  return;
}

void casely_delete(int nid) {
  if (PAR(nid)) {
    int sibling = SIBLING(nid);
    if (ND(SIBLING(nid)).color == RED) {
      ND(PAR(nid)).color = RED;
      ND(sibling).color = BLACK;

      rotate(PAR(nid), IS_RC(sibling));
      casely_delete(nid);
    }
    else if (ND(PAR(nid)).color == BLACK &&
        ND(CHD(sibling, LC)).color == BLACK &&
        ND(CHD(sibling, RC)).color == BLACK) {
      ND(sibling).color = RED;
      casely_delete(PAR(nid));
    }
    else if (ND(CHD(sibling, LC)).color == BLACK &&
        ND(CHD(sibling, RC)).color == BLACK) {
      ND(sibling).color = RED;
      ND(PAR(nid)).color = BLACK;
      return;
    }
    else {
      if (ND(CHD(sibling, IS_RC(sibling))).color != RED) {
        rotate(sibling, !IS_RC(sibling));
        ND(sibling).color = RED;
        ND(PAR(sibling)).color = BLACK;
        sibling = PAR(sibling);
      }

      ND(sibling).color = ND(PAR(sibling)).color;
      ND(PAR(sibling)).color = BLACK;
      ND(CHD(sibling, IS_RC(sibling))).color = BLACK;
      rotate(PAR(sibling), IS_RC(sibling));
    }
  }
}

// the insert of r-b tree
int rb_insert(int *root, int big, int small) {
  int nid = insert(root, big, small, 0);

  casely_insert(nid);

  push_up(nid);

  return nid;
}

void rb_delete(int id) {
  int nid = delete_nd(id);
  // print_rb_tree(rb_root, 0);

  int chd = CHD(nid, NO_LC(nid));
  if (!PAR(nid)) {
    // printf("wrong situation\n");
    rb_root = chd;
    ND(chd).pid = 0;
    ND(chd).color = BLACK;
  }
  else {
    // printf("correct situation\n");
    ND(PAR(nid)).chd[IS_RC(nid)] = chd;
    ND(chd).pid = PAR(nid);

    if (ND(nid).color == BLACK) {
      // printf("wrong situation\n");
      if (ND(chd).color == RED)
        ND(chd).color = BLACK;
      else
       casely_delete(chd);
    }

    // if (!chd) ND(chd).pid = 0;
  }
  
  push_up(PAR(nid));

  // printf("in the delete\n");
  // print_rb_tree(rb_root, 0);

  del_node(nid);
  return;
}

signed main () {
  int n, m;
  scanf("%lld%lld", &n, &m);

  pool[rb_root].color = BLACK;

  for (int i = 1; i <= MAXN; ++i) {
    queue[i - 1] = i;
  }

  for (int i = 0; i < n; ++i) {
    int big, small;
    scanf("%lld%lld", &big, &small);
    if (big < small)
      big ^= small ^= big ^= small;

    id2tid[i] = rb_insert(rb_root_ptr, big, small);
    tid2id[id2tid[i]] = i;
  }

  // print_rb_tree(rb_root, 0);
  printf("%lld, %lld\n", ND(rb_root).sum[FST], ND(rb_root).sum[SEC]);

  for (int i = 0; i < m - 1; ++i) {
    int id, big, small;
    scanf("%lld%lld%lld", &id, &big, &small);
    if (big < small)
      big ^= small ^= big ^= small;

    rb_delete(id2tid[id - 1]);
    id2tid[id - 1] = rb_insert(rb_root_ptr, big, small);
    // print_rb_tree(rb_root, 0);
    printf("%lld %lld\n", ND(rb_root).sum[FST], ND(rb_root).sum[SEC]);
  }

  return 0;
}
