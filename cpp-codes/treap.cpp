mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int rand(int l, int r){
    uniform_int_distribution<int> uid(l, r);
    return uid(rng);
}

struct node {
	int prior, size;
	int val, sum; // info stored for this node
	int lazy;
	node *l, *r;

	node(int _val) {
		prior = rand(1, 2e9);
		size = 1;
		sum = val = _val;
		lazy = 0;
		l = r = NULL;
	}
}; typedef node* pnode;

int sz(pnode t) {
	return t ? t->size : 0;
}

void update_size(pnode t) {
	if (t)
		t->size = sz(t->l) + 1 + sz(t->r);
}

// Lazy propagation
void push(pnode t) {
	if (!t || !t->lazy) return;

	// Calculate answer for this node
	t->val += t->lazy;
	t->sum = t->lazy * sz(t);

	if (t->l) t->l->lazy += t->lazy;
	if (t->r) t->r->lazy += t->lazy;
	t->lazy = 0;
}

// Reset value of a node to original node (except val, size, lazy)
void reset(pnode t) {
	t->sum = t->val;
}

// Merge like operations of Segment Tree
void combine(pnode &t, pnode l, pnode r) {
	if (!l || !r) return void(t = l ? l : r);
	t->sum = l->sum + r->sum;
}

// Operation of Segment Tree
void operation(pnode t) {
	if (!t) return;
	reset(t);
	push(t->l); push(t->r);
	combine(t, t->l, t); combine(t, t, t->r);
}

void split(pnode t, pnode &l, pnode &r, int val, int add=0) {
	if (!t)
		return void(l = r = NULL);
	push(t);
	int cur_val = add + sz(t->l); // Index of current element
	if (val >= t->val)
		split(t->r, t->r, r, val, cur_val + 1), l = t; // Index of right = cur_val + 1
	else
		split(t->l, l, t->l, val, add), r = t;
	update_size(t);
	operation(t);
}

void merge(pnode &t, pnode l, pnode r) {
	push(l); push(r);
	if (!l || !r) t = l ? l : r;
	else if (l->prior >= r->prior) merge(l->r, l->r, r), t = l;
	else merge(r->l, l, r->l), t = r;
	update_size(t);
	operation(t);
}

void insert(pnode &t, int val, int pos) { // pos is the key
	if (!t)
		return void(t = new node(val));
	pnode l, r;
	pnode item = new node(val);
	split(t, l, r, pos);
	merge(l, l, item);
	merge(t, l, r);
}

void erase(pnode &t, int val) {
	if (!t) return;
	if (t->val == val) merge(t, t->l, t->r);
	else erase(t->val > val ? t->l : t->r, val);
}

void unite(pnode &t, pnode l, pnode r) {
	push(l); push(r);
	if (!l || !r) return void(t = l ? l : r);
	if (l->prior < r->prior) swap(l, r);
	pnode lt, rt;
	split(r, lt, rt, l->val);
	unite(l->l, l->l, lt);
	unite(l->r, l->r, rt);
	t = l;
}

int query(pnode t, int l, int r) {
	pnode L, temp, R;
	split(t, L, temp, l); split(temp, t, R, r-l+1);
	int ans = t->sum;
	merge(temp, L, t); merge(t, temp, R);
	return ans;
}

void update(pnode t, int l, int r, int val) {
	pnode L, temp, R;
	split(t, L, temp, l); split(temp, t, R, r-l+1);
	t->lazy += val;
	merge(temp, L, t); merge(t, temp, R);
}

void printtreap(pnode t) {
	if (!t) return;
	printtreap(t->l);
	cout << t->val << " ";
	printtreap(t->r);
}

pnode treap = NULL;