#include "stdio.h"
#include "unistd.h"
#include "assert.h"

#define int long long

#define MAXN 100000
#define BLACK 0
#define RED 1
#define FST 0 // first hand
#define SEC 1 // second hand
#define LEFT_CHILD 0 // left child
#define RIGHT_CHILD 1 // right child
#define MOD 1000000007

// r-b tree function
#define GET_NODE(x) (pool[(x)]) // transform id into node
#define PARENT(x) (pool[(x)].pid) // return parent id
#define GRAND_PARENT(x) (PARENT(PARENT((x)))) // return grand parent id
#define CHILD(x, lr) (GET_NODE((x)).chd[(lr)])
#define IS_RIGHT_CHILD(x) (GET_NODE(PARENT((x))).chd[RIGHT_CHILD] == (x)) // check if the node is right child
#define NO_LEFT_CHILD(x) (GET_NODE((x)).chd[LEFT_CHILD] == 0) // check if a node has left child
#define UNCLE(x) (GET_NODE(GRAND_PARENT((x))).chd[!IS_RIGHT_CHILD(PARENT((x)))]) // return uncle id
#define SIBLING(x) (GET_NODE(PARENT(x)).chd[!IS_RIGHT_CHILD((x))]) // return sibling id
#define KEY(x) (GET_NODE((x)).price[RIGHT_CHILD] - GET_NODE((x)).price[LEFT_CHILD]) // get key

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
  print_rb_tree(GET_NODE(root).chd[LEFT_CHILD], layer + 1);
  for (int i = 0; i < layer; ++i) {
    printf(" ");
  }
  printf("%lld, %lld, %s, %lld, %lld, %lld, %lld\n",
      tid2id[root], KEY(root), 
      GET_NODE(root).color ? "RED" : "BLACK",
      GET_NODE(root).sum[0], GET_NODE(root).sum[1],
      GET_NODE(root).price[0], GET_NODE(root).price[1]);
  print_rb_tree(GET_NODE(root).chd[RIGHT_CHILD], layer + 1);

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
  pool[id].chd[LEFT_CHILD] = pool[id].chd[RIGHT_CHILD] = 0;
  pool[id].color = RED;
  pool[id].sz = 1;
  pool[id].id = id;
  pool[id].pid = pid;
  return id;
}

void node_swap (int a_nid, int b_nid) {
  Node tmp = GET_NODE(a_nid);
  GET_NODE(a_nid).price[FST] = GET_NODE(b_nid).price[FST];
  GET_NODE(a_nid).price[SEC] = GET_NODE(b_nid).price[SEC];

  GET_NODE(b_nid).price[FST] = tmp.price[FST];
  GET_NODE(b_nid).price[SEC] = tmp.price[SEC];

  int tmpid = tid2id[a_nid];
  tid2id[a_nid] = tid2id[b_nid];
  tid2id[b_nid] = tmpid;

  id2tid[tid2id[a_nid]] = a_nid;
  id2tid[tid2id[b_nid]] = b_nid;
}

void push(int nid) {
  int lc = CHILD(nid, LEFT_CHILD), rc = CHILD(nid, RIGHT_CHILD);

  GET_NODE(nid).sz = GET_NODE(lc).sz + GET_NODE(rc).sz + 1;
  GET_NODE(nid).sum[FST] = GET_NODE(lc).sum[FST] + GET_NODE(rc).sum[!(GET_NODE(lc).sz & 1)] + GET_NODE(nid).price[GET_NODE(lc).sz & 1];
  GET_NODE(nid).sum[SEC] = GET_NODE(lc).sum[SEC] + GET_NODE(rc).sum[GET_NODE(lc).sz & 1] + GET_NODE(nid).price[!(GET_NODE(lc).sz & 1)];

  return;
}

void push_up(int nid) {
  push(nid);
  if (PARENT(nid)) {
    push_up(PARENT(nid));
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
  if (!GET_NODE(id).chd[LEFT_CHILD] || !GET_NODE(id).chd[RIGHT_CHILD])
    return id;
  else {
    int tar = GET_NODE(id).chd[LEFT_CHILD];
    while (GET_NODE(tar).chd[RIGHT_CHILD]) {
      tar = GET_NODE(tar).chd[RIGHT_CHILD];
    }
    
    node_swap(id, tar);
    return tar;
  }
}

// rotate, nid is the origin root, and dir is the index of child which will be the new root.
void rotate(int nid, int dir) {
  int pid = PARENT(nid), cid = GET_NODE(nid).chd[dir];

  // link cid to pid
  if (pid)
    GET_NODE(pid).chd[IS_RIGHT_CHILD(nid)] = cid;
  else
    rb_root = cid;
  GET_NODE(cid).pid = pid;

  // link the child of cid to nid
  GET_NODE(nid).chd[dir] = GET_NODE(cid).chd[!dir];
  GET_NODE(GET_NODE(cid).chd[!dir]).pid = nid;

  // link cid to nid
  GET_NODE(cid).chd[!dir] = nid;
  GET_NODE(nid).pid = cid;

  // update data
  push(nid);
  push(cid);

  return;
}

// the complex flow controll of rb insertion
void casely_insert(int nid) {
  if (PARENT(nid) == 0)
    GET_NODE(nid).color = BLACK;
  else if (GET_NODE(PARENT(nid)).color == BLACK)
    ;
  else if (UNCLE(nid) && GET_NODE(UNCLE(nid)).color == RED) {
    GET_NODE(PARENT(nid)).color = GET_NODE(UNCLE(nid)).color = BLACK;
    GET_NODE(GRAND_PARENT(nid)).color = RED;
    casely_insert(GRAND_PARENT(nid));
  }
  else {
    if (IS_RIGHT_CHILD(nid) != IS_RIGHT_CHILD(PARENT(nid))) {
      rotate(PARENT(nid), IS_RIGHT_CHILD(nid));
      nid = GET_NODE(nid).chd[IS_RIGHT_CHILD(nid)];
    }

    GET_NODE(GRAND_PARENT(nid)).color = RED;
    GET_NODE(PARENT(nid)).color = BLACK;
    rotate(GRAND_PARENT(nid), IS_RIGHT_CHILD(PARENT(nid)));
  }
  return;
}

void casely_delete(int nid) {
  if (PARENT(nid)) {
    int sibling = SIBLING(nid);
    if (GET_NODE(SIBLING(nid)).color == RED) {
      GET_NODE(PARENT(nid)).color = RED;
      GET_NODE(sibling).color = BLACK;

      rotate(PARENT(nid), IS_RIGHT_CHILD(sibling));
      casely_delete(nid);
    }
    else if (GET_NODE(PARENT(nid)).color == BLACK &&
        GET_NODE(CHILD(sibling, LEFT_CHILD)).color == BLACK &&
        GET_NODE(CHILD(sibling, RIGHT_CHILD)).color == BLACK) {
      GET_NODE(sibling).color = RED;
      casely_delete(PARENT(nid));
    }
    else if (GET_NODE(CHILD(sibling, LEFT_CHILD)).color == BLACK &&
        GET_NODE(CHILD(sibling, RIGHT_CHILD)).color == BLACK) {
      GET_NODE(sibling).color = RED;
      GET_NODE(PARENT(nid)).color = BLACK;
      return;
    }
    else {
      if (GET_NODE(CHILD(sibling, IS_RIGHT_CHILD(sibling))).color != RED) {
        rotate(sibling, !IS_RIGHT_CHILD(sibling));
        GET_NODE(sibling).color = RED;
        GET_NODE(PARENT(sibling)).color = BLACK;
        sibling = PARENT(sibling);
      }

      GET_NODE(sibling).color = GET_NODE(PARENT(sibling)).color;
      GET_NODE(PARENT(sibling)).color = BLACK;
      GET_NODE(CHILD(sibling, IS_RIGHT_CHILD(sibling))).color = BLACK;
      rotate(PARENT(sibling), IS_RIGHT_CHILD(sibling));
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

  int chd = CHILD(nid, NO_LEFT_CHILD(nid));
  if (!PARENT(nid)) {
    rb_root = chd;
    GET_NODE(chd).pid = 0;
    GET_NODE(chd).color = BLACK;
  }
  else {
    GET_NODE(PARENT(nid)).chd[IS_RIGHT_CHILD(nid)] = chd;
    GET_NODE(chd).pid = PARENT(nid);


    if (GET_NODE(nid).color == BLACK) {
      if (GET_NODE(chd).color == RED)
        GET_NODE(chd).color = BLACK;
      else
       casely_delete(chd);
    }
    push_up(PARENT(nid));
  }


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

  for (int i = 1; i <= n; ++i) {
    int big, small;
    scanf("%lld%lld", &big, &small);
    if (big < small)
      big ^= small ^= big ^= small;

    id2tid[i] = rb_insert(rb_root_ptr, big, small);
    tid2id[id2tid[i]] = i;
  }

  printf("%lld\n", GET_NODE(rb_root).sum[FST]);
  int prev = GET_NODE(rb_root).sum[FST] % MOD;

  for (int i = 0; i < m - 1; ++i) {
    int id, c, d, e, f;
    scanf("%lld%lld%lld%lld%lld", &id, &c, &d, &e, &f);
    int big = (c * prev + d) % MOD;
    int small = (e * prev + f) % MOD;
    if (big < small)
      big ^= small ^= big ^= small;

    rb_delete(id2tid[id]);
    id2tid[id] = rb_insert(rb_root_ptr, big, small);
    tid2id[id2tid[id]] = id;
    printf("%lld\n", GET_NODE(rb_root).sum[FST]);
    prev = GET_NODE(rb_root).sum[FST] % MOD;
  }

  return 0;
}
