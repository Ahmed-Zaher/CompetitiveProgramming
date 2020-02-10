/*
 * Let's do BFS from the root and record the distance from it to every node
 * We  we can picture the graph as in layers, where i'th layer includes all nodes
 * with distance i from the root. We clearly get min sum of di if we take n - 1 edges
 * iff they're connect 2 nodes in different layers and keep the graph connected
 * How can we achieve that? In the pictures graph, every node (except root) has some
 * candidates that are closer to the root that can a parent to it. And if for every
 * node we pick one of those candidate parents, we'll eventually get a tree with min sum of di.
 * We can use backtracking to enumerate all those possibilities
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


int n, m, k;
vvi adj, pCand;
vii edges;
V<bool> tree;
VV<bool> ans;
bool done;

void bt(int u) {
	if (u == n + 1) {
		ans.pb(tree);
		if (sz(ans) == k)
			done = true;
		return;
	}
	for (auto& e : pCand[u]) {
		tree[e] = 1;
		bt(u + 1);
		tree[e] = 0;
		if (done)
			return;
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

	cin >> n >> m >> k;
	adj.assign(n + 1, vi());
	pCand.assign(n + 1, vi());
	edges.assign(m + 1, mp(0, 0));
	tree.assign(m + 1, 0);

	for (int i = 1; i <= m; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
		edges[i] = mp(u, v);
	}

	vi dist(n + 1, OO);
	queue<int> q;
	q.push(1);
	dist[1] = 0;
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		for (auto& v : adj[u])
			if (dist[v] == OO)
				q.push(v), dist[v] = dist[u] + 1;
	}
	for (int i = 1; i <= m; ++i) {
		int u = edges[i].fs, v = edges[i].sc;
		if (dist[u] == dist[v])
			continue;
		if (dist[u] > dist[v])
			swap(u, v);
		pCand[v].pb(i);
	}

	bt(2);

	cout << sz(ans) << '\n';

	for (int i = 0; i < sz(ans); ++i) {
		for (int j = 1; j <= m; ++j)
			cout << ans[i][j];
		ndl;
	}


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
