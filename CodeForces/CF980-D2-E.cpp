/*
 * Note that 2^i > sum of 2^j for j in [0, i), hence we pick nodes
 * greedily, we initially have node n, then we see if we can include
 * node n - 1 and so on.. At any point we want to know if for the node u
 * that is currently under consideration, what is the distance until
 * it reaches the tree of the nodes we have so far. And based on whether
 * we can cover all that distance we decide to take u or not. To answer
 * such queries we use LCA with binary lifting, we find LCA of the tree
 * we have so far and u. Then we get 3 cases whether that LCA is u or
 * the root of our tree or neither. Each of them can be answered in O(logn)
 * using binary lifting.
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

int n, k, rem;
vvi adj;
vi taken;

struct LCA {
	int n, LOGN;
	vi lvl, par;
	vvi P;
	LCA(int _n, int root) {
		n = _n;
		LOGN = log2(n) + 1;
		lvl.assign(n + 1, 0);
		par.assign(n + 1, -1);
		P.assign(n + 1, vi(LOGN, -1));
		dfs(root, 0, -1);

		for (int i = 1; i <= n; ++i)
			P[i][0] = par[i];

		for (int j = 1; (1 << j) <= n; ++j)
			for (int i = 1; i <= n; ++i)
				if (P[i][j - 1] != -1)
					P[i][j] = P[P[i][j - 1]][j - 1];
	}
	void dfs(int u, int level, int parent) {
		lvl[u] = level, par[u] = parent;
		for (auto& v : adj[u])
			if (v != parent)
				dfs(v, level + 1, u);
	}
	int query(int u, int v) {
		if (lvl[u] < lvl[v])
			swap(u, v);
		int log = 1;
		while ((1 << log) <= lvl[u])
			++log;
		--log;
		for (int i = log; i >= 0; --i)
			if (lvl[u] - (1 << i) >= lvl[v])
				u = P[u][i];
		if (u == v)
			return u;
		for (int i = log; i >= 0; --i)
			if (P[u][i] != -1 && P[u][i] != P[v][i])
				u = P[u][i], v = P[v][i];
		return par[u];
	}
	void color(int u, int v) {
		// color u, p[u], p[p[u]], .., v
		int curr = u;
		while (true) {
			if (!taken[curr])
				taken[curr] = 1, --rem;
			if (curr == v)
				return;
			curr = par[curr];
		}
	}
	int upd(int i, int tRoot) {
		if (taken[i])
			return tRoot;
		int u = query(tRoot, i);
		if (u == i) {
			int d = lvl[tRoot] - lvl[i];
			if (d > rem)
				return tRoot;
			color(tRoot, i);
			return i;
		}
		if (u != tRoot) {
			int d = lvl[i] + lvl[tRoot] - 2 * lvl[u];
			if (d > rem)
				return tRoot;
			color(i, u);
			color(tRoot, u);
			return u;
		}

		int log = 1;
		u = i;
		while ((1 << log) <= lvl[u])
			++log;
		--log;
		for (int j = log; j >= 0; --j)
			if (P[u][j] != -1 && !taken[P[u][j]] && lvl[P[u][j]] > lvl[tRoot])
				u = P[u][j];


		u = par[u];
		int d = lvl[i] - lvl[u];

		if (d <= rem)
			color(i, u);

		return tRoot;
	}
};



int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> k;

	adj.assign(n + 1, vi());

	for (int i = 1; i <= n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
	}

	taken.assign(n + 1, 0);

	LCA lca(n, 1);
	taken[n] = 1;
	int root = n;
	rem = n - k - 1;

	for (int i = n - 1; i >= 1; --i)
		root = lca.upd(i, root);

	for (int i = 1; i <= n; ++i)
		if (!taken[i])
			cout << i << ' ';

	ndl;

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
