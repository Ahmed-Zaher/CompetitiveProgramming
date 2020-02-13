/*
 * We are looking for a tour in the tree s.t. we visit all of the m given
 * nodes and use the minimum number of edges. There are some edges which must
 * be traversed because they lie on the path between two of the attacked nodes,
 * and all other edges aren't needed, let's refer to these edges as 'good' edges.
 *
 * Consider the case when we start our tour at s, and end at t: First note that
 * both s and t must be attacked cities, otherwise we can get a shorter solution.
 * Also note that all good edges that do not lie on s-t path must be visited at least twice.
 *
 * And generally, given s and t, the optimal tour is one that visits all good edges
 * twice except for the edges on s-t path, visiting them only once.
 *
 * So to minimize the cost of the tour, we want to maximize the s-t path length, and we can do
 * that by finding the diameter of the tree where good edges have cost 1, and others have cost 0.
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


int n, m, lca, edgeCnt;
vvi adj;
vi c;
V<bool> att;


void dfs1(int u, int p = -1) {

	c[u] = att[u];

	for (auto& v : adj[u]) {
		if (v != p) {
			dfs1(v, u);
			c[u] += c[v];
		}
	}

	if (!lca && c[u] == m)
		lca = u;

	for (auto& v : adj[u]) {
		if (v != p) {
			if (c[v] && (c[u] < m || u == lca))
				++edgeCnt;
		}
	}

}

pii ans = mp(-1, -1e9);

pii dfs(int u, int par) {

	pii mxH[] = {mp(0, -u), mp(0, -u), mp(0, -u)};

	for (auto& v : adj[u])
		if (v != par && c[v] != m && c[v]) {
			pii pr = dfs(v, u);
			mxH[0] = mp(pr.fs + 1, pr.sc);
			sort(mxH, mxH + 3);
		}

	ans = max(ans, mp(mxH[1].fs + mxH[2].fs, max(mxH[1].sc, mxH[2].sc)));

	return mxH[2];
}

int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> m;

	adj.assign(n + 1, vi());

	for (int i = 1; i <= n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
	}

	att.assign(n + 1, 0);

	for (int i = 1; i <= m; ++i) {
		int u;
		cin >> u;
		att[u] = 1;
	}

	c.assign(n + 1, 0);

	dfs1(1);

	dfs(lca, -1);

	cout << -ans.sc << ' ' << edgeCnt * 2 - ans.fs << '\n';



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
