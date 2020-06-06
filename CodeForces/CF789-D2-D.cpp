/*
 * Let's take each undirected edge (u, v) and replace it with
 * two directed edges u->v, and v->u, call the resulting graph G`
 * Let e1 = (u1, v1), e2 = (u2, v2) be the edges that are visited once
 * in a path. Then path is good only if, after removing u1->v1 or v1->u1
 * and removing u2->v2 or v2->u2 from G`, we can get an Eulerial tour/cycle
 * in G`. Which happens only if all nodes in G` have in-degree=out-degree
 * or all nodes except the start node having out-deg=in-deg+1 and end node
 * having in-deg=out-deg
 * By analyzing the difference cases of e1 and e2 we note that e1 and e2 can
 * makes a good path only if one of the following is satisfied:
 * 1. e1 and e2 are not self loops and they are incident
 * 2. e1 and e2 are self loops
 * 3. one of e1 and e2 is a self loop and the other is not
 * Along with the condition that the graph (ignoring isolated nodes) is connected.
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


int n, m, l, cnt;
vvi adj;
V<bool> vis, isolated;

void dfs(int u) {
	vis[u] = 1;
	++cnt;
	for (auto& v : adj[u]) {
		if (!vis[v])
			dfs(v);
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

	cin >> n >> m;

	adj.assign(n + 1, vi());
	vis.assign(n + 1, 0);
	isolated.assign(n + 1, 1);

	for (int i = 1; i <= m; ++i) {
		int u, v;
		cin >> u >> v;
		isolated[u] = isolated[v] = 0;
		if (u == v)
			++l;
		else {
			adj[u].pb(v);
			adj[v].pb(u);
		}
	}

	int cc = 0;
	for (int i = 1; i <= n; ++i) {
		if (isolated[i])
			continue;
		if (!vis[i]) {
			dfs(i);
			++cc;
		}
	}

	if (cc > 1) {
		cout << "0\n";
	} else {
		ll ans = l * 1LL * (m - l) + (l * 1LL * (l - 1)) / 2;
		for (int i = 1; i <= n; ++i)
			ans += (sz(adj[i]) * 1LL * (sz(adj[i]) - 1)) / 2;
		cout << ans << '\n';
	}

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
