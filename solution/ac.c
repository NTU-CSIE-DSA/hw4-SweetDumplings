#include <stdio.h>
#include <assert.h>

#define ll long long

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
  ll price[2], sum[2], chd[2];
  ll color, sz, id, pid;
} Node;

Node pool[MAXN + 1];
ll queue[MAXN + 1];
ll fnt = 0, bck = MAXN;
ll rb_root = 0;
ll *rb_root_ptr = &rb_root;
ll id2tid[MAXN + 1];
ll tid2id[MAXN + 1];

void print_rb_tree(ll root, ll layer) {
  assert(layer < 10);
  if (layer == 0) {
    printf("f====================\n");
    printf("id, key, color, sum[0], sum[1], val[0], val[1]\n");
  }
  if (root == 0)
    return;
  print_rb_tree(GET_NODE(root).chd[LEFT_CHILD], layer + 1);
  for (ll i = 0; i < layer; ++i) {
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
ll new_node (ll big, ll small, ll pid) {
  ll id = queue[fnt];
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

void node_swap (ll a_nid, ll b_nid) {
  Node tmp = GET_NODE(a_nid);
  GET_NODE(a_nid).price[FST] = GET_NODE(b_nid).price[FST];
  GET_NODE(a_nid).price[SEC] = GET_NODE(b_nid).price[SEC];

  GET_NODE(b_nid).price[FST] = tmp.price[FST];
  GET_NODE(b_nid).price[SEC] = tmp.price[SEC];

  ll tmpid = tid2id[a_nid];
  tid2id[a_nid] = tid2id[b_nid];
  tid2id[b_nid] = tmpid;

  id2tid[tid2id[a_nid]] = a_nid;
  id2tid[tid2id[b_nid]] = b_nid;
}

void push(ll nid) {
  ll lc = CHILD(nid, LEFT_CHILD), rc = CHILD(nid, RIGHT_CHILD);

  GET_NODE(nid).sz = GET_NODE(lc).sz + GET_NODE(rc).sz + 1;
  GET_NODE(nid).sum[FST] = GET_NODE(lc).sum[FST] + GET_NODE(rc).sum[!(GET_NODE(lc).sz & 1)] + GET_NODE(nid).price[GET_NODE(lc).sz & 1];
  GET_NODE(nid).sum[SEC] = GET_NODE(lc).sum[SEC] + GET_NODE(rc).sum[GET_NODE(lc).sz & 1] + GET_NODE(nid).price[!(GET_NODE(lc).sz & 1)];

  return;
}

void push_up(ll nid) {
  push(nid);
  if (PARENT(nid)) {
    push_up(PARENT(nid));
  }
}

// put the deleted node back to the queue
void del_node (ll tid) {
  queue[bck] = tid;
  INC(bck);
  return;
}

// the insert of basic BST
ll insert (ll *root, ll big, ll small, ll pid) {
  ll curr_big = pool[*root].price[SEC], curr_small = pool[*root].price[FST];
  if (*root == 0) {
    *root = new_node(big, small, pid);
    return *root;
  }
  else {
    ll chd = (big - small < curr_big - curr_small);
    return insert(&(pool[*root].chd[chd]), big, small, *root);
  }
}

ll delete_nd (ll id) {
  if (!GET_NODE(id).chd[LEFT_CHILD] || !GET_NODE(id).chd[RIGHT_CHILD])
    return id;
  else {
    ll tar = GET_NODE(id).chd[LEFT_CHILD];
    while (GET_NODE(tar).chd[RIGHT_CHILD]) {
      tar = GET_NODE(tar).chd[RIGHT_CHILD];
    }
    
    node_swap(id, tar);
    return tar;
  }
}

// rotate, nid is the origin root, and dir is the index of child which will be the new root.
void rotate(ll nid, ll dir) {
  ll pid = PARENT(nid), cid = GET_NODE(nid).chd[dir];

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
void casely_insert(ll nid) {
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

void casely_delete(ll nid) {
  if (PARENT(nid)) {
    ll sibling = SIBLING(nid);
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
ll rb_insert(ll *root, ll big, ll small) {
  ll nid = insert(root, big, small, 0);
  casely_insert(nid);

  push_up(nid);

  return nid;
}

void rb_delete(ll id) {
  ll nid = delete_nd(id);

  ll chd = CHILD(nid, NO_LEFT_CHILD(nid));
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

int main () {
  ll n, m;
  scanf("%lld%lld", &n, &m);

  pool[rb_root].color = BLACK;

  for (ll i = 1; i <= MAXN; ++i) {
    queue[i - 1] = i;
  }

  for (ll i = 1; i <= n; ++i) {
    ll big, small;
    scanf("%lld%lld", &big, &small);
    if (big < small)
      big ^= small ^= big ^= small;

    id2tid[i] = rb_insert(rb_root_ptr, big, small);
    tid2id[id2tid[i]] = i;
  }

  printf("%lld\n", GET_NODE(rb_root).sum[FST]);
  ll prev = GET_NODE(rb_root).sum[FST] % MOD;

  for (ll i = 0; i < m - 1; ++i) {
    ll id, c, d, e, f;
    scanf("%lld%lld%lld%lld%lld", &id, &c, &d, &e, &f);
    ll big = (c * prev + d) % MOD;
    ll small = (e * prev + f) % MOD;
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
