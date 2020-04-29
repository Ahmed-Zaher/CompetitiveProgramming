/*
 * Let's answer the queries offline, for every node we store all the queries
 * that queries it. And maintain a segment tree that stores all the leaves
 * and initially has the distances from the root to them. Then do a DFS, and when
 * traversing an edge we update the segment tree so that the values in it are
 * the values of distances from the current node we're traversing
 */

#include <bits/stdc++.h>

using namespace std;

const int OO = 1e9;
const double EPS = 1e-9;

#define ndl cout << '\n'
#define sz(v) int(v.size())
#define pb push_back
#define mp make_pair
#define fs first
#define sc second
#define present(a, x) (a.find(x) != a.end())
#ifdef LOCAL
#define db(...) ({cout << "> Line " << __LINE__  \
		<< ": "; _db(#__VA_ARGS__, __VA_ARGS__);})
#define RNG() rng()
#else
#define db(...) true
#define RNG() true
#endif

template<class T>
void _db(const char *dbStr, T e) {
	cout << dbStr << " = " << e << endl;
}
template<class T, class... L>
void _db(const char *dbStr, T e, L... r) {
  while(*dbStr != ',') cout << *dbStr++;
  cout << " = " << e << ','; _db(dbStr + 1, r...);
}
template<class S, class T>
ostream& operator<<(ostream& o, const map<S, T>& v) {
	o << "["; int i = 0;
	for (const pair<S, T>& pr : v) o << (!i++ ? "" : ", ") << "{"
	<< pr.fs << " : " << pr.sc << "}"; return o << "]";
}
template<template <class, class...> class S, class T, class... L>
ostream& operator<<(ostream& o, const S<T, L...>& v) {
	o << "["; int i = 0;
	for (const auto& e : v) o << (!i++ ? "" : ", ") << e;
	return o << "]";
}
template<class S, class T>
ostream& operator<<(ostream& o, const pair<S, T>& pr) {
	return o << "(" << pr.fs << ", " << pr.sc << ")";
}
ostream& operator<<(ostream& o, const string& s) {
	for (const char& c : s) o << c;
	return o;
}

template<class T> using V = vector<T>;
template<class T> using VV = V<V<T>>;
template<class T> using VVV = VV<V<T>>;
using ll = long long;
using pii = pair<int, int>;
using vi = V<int>;
using vii = V<pii>;
using vvi = VV<int>;
using mii = map<int, int>;
using umii = unordered_map<int, int>;
using si = set<int>;
using usi = unordered_set<int>;


struct SegmentTree {
	V<ll> t, lazy;
	int n;
	void push(int v) {
		t[v << 1] += lazy[v];
		t[v << 1 | 1] += lazy[v];
		lazy[v << 1] += lazy[v];
		lazy[v << 1 | 1] += lazy[v];
		lazy[v] = 0;
	}
	ll query(int v, int tl, int tr, int l, int r) {
		if (l > r)
			return 1e18;
		if (l == tl && r == tr)
			return t[v];
		push(v);
		int tm = (tl + tr) >> 1;
		ll valL = query(v << 1, tl, tm, l, min(r, tm));
		ll valR = query(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r);
		return min(valL, valR);
	}
	void lazyUpdate(int v, int tl, int tr, int l, int r, ll newVal) {
		if (l > r)
			return;
		if (l == tl && tr == r) {
			t[v] += newVal;
			lazy[v] += newVal;
		} else {
			push(v);
			int tm = (tl + tr) >> 1;
			lazyUpdate(v << 1, tl, tm, l, min(r, tm), newVal);
			lazyUpdate(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r, newVal);
			ll valL = t[v << 1], valR = t[v << 1 | 1];
			t[v] = min(valL, valR);
		}
	}
	void init(int _n) {
		n = _n;
		t.assign(n << 2, 0);
		lazy.assign(n << 2, 0);
	}
	SegmentTree(int _n) {
		init(_n);
	}
	SegmentTree() {}
	ll query(int i, int j) {
		return query(1, 0, n - 1, i, j);
	}
	void lazyUpdate(int l, int r, ll newVal) {
		lazyUpdate(1, 0, n - 1, l, r, newVal);
	}
};

struct Query {
	int v, l, r;
};

int n, q;
VV<pii> adj;
V<pair<int, ll>> leaves;
vii range;
V<Query> queries;
vi Next, Prev;
V<ll> ans;
vvi queriesOnNode;
SegmentTree ST;


void dfs(int u, int p = -1, ll totW = 0) {
	for (auto& pr : adj[u]) {
		int v = pr.fs, w = pr.sc;
		if (v != p) {
			dfs(v, u, totW + w);
			range[u].fs = min(range[u].fs, range[v].fs);
			range[u].sc = max(range[u].sc, range[v].sc);
		}
	}

	if (sz(adj[u]) == 1) {
		range[u] = mp(sz(leaves), sz(leaves));
		Next[u] = Prev[u] = sz(leaves);
		leaves.pb(mp(u, totW));
	}
}

void dfs2(int u, int p = -1) {
	for (auto& i : queriesOnNode[u]) {
		int l = Next[queries[i].l], r = Prev[queries[i].r];
		ans[i] = ST.query(l, r);
	}

	for (auto& pr : adj[u]) {
		int v = pr.fs, w = pr.sc;
		if (v != p) {

			ST.lazyUpdate(0, sz(leaves) - 1, w);
			ST.lazyUpdate(range[v].fs, range[v].sc, -2 * w);

			dfs2(v, u);

			ST.lazyUpdate(0, sz(leaves) - 1, -w);
			ST.lazyUpdate(range[v].fs, range[v].sc, 2 * w);
		}
	}
}

int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> q;

	adj.assign(n + 1, vii());

	for (int i = 2; i <= n; ++i) {
		int pi, wi;
		cin >> pi >> wi;
		adj[i].pb(mp(pi, wi));
		adj[pi].pb(mp(i, wi));
	}

	for (int i = 1; i <= n; ++i)
		sort(adj[i].begin(), adj[i].end());

	db(adj);

	Next.assign(n + 1, -1);
	Prev.assign(n + 1, -1);
	range.assign(n + 1, mp(OO, -1));

	dfs(1);

	for (int i = n - 1; i >= 1; --i) {
		if (Next[i] == -1)
			Next[i] = Next[i + 1];
	}

	for (int i = 2; i <= n; ++i) {
		if (Prev[i] == -1)
			Prev[i] = Prev[i - 1];
	}

	ST = SegmentTree(sz(leaves));

	db(leaves, range);
	db(Next, Prev);

	for (int i = 0; i < sz(leaves); ++i)
		ST.lazyUpdate(i, i, leaves[i].sc);

	queries.assign(q, Query());
	queriesOnNode.assign(n + 1, vi());


	for (int i = 0; i < q; ++i) {
		cin >> queries[i].v >> queries[i].l >> queries[i].r;
		queriesOnNode[queries[i].v].pb(i);
	}

	ans.assign(q, 0);

	dfs2(1);

	for (auto& elem : ans)
		cout << elem << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
