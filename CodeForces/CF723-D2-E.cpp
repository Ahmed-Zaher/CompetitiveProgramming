/*
 * Consider a connected component of the graph, clearly all nodes with odd
 * degree can never be included in the answer, we can show that all nodes
 * with even degree can be included in the answer, this can be done by finding
 * a cycle, removing it's edges, then repeat until there's no cycles, the operation
 * of cycle removal maintains the parity of the degree of the vertices. The remaining
 * graph will be a forest, we can for each tree take a leaf-to-leaf path, and remove its edges,
 * and then repeat. This will maintain degree parity except for the path end points, but those have odd
 * degree and are useless anyways. At the end all even degree nodes will be isolated, and clearly
 * can be included in our answer
 *
 * As for how to construct the answer, we can add a dummy node and connect it to all odd degree nodes
 * and find Euler tour/cycle of that new graph, that's because Euler tour/cycle can be decomposed into
 * edge disjoint cycles, and some paths, whose end points will always be nodes with odd degree.
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


int T, n, m;
V<vi> adj, adj2;
vii ans;
V<bool> vis;
vi CC;
map<pii, int> edgeCnt;
list<int> cyc;

void euler(list<int>::iterator it, int u) {
	for (auto& v : adj2[u])
		if (edgeCnt[{u, v}]) {
			--edgeCnt[{u, v}], --edgeCnt[{v, u}];
			euler(cyc.insert(it, u), v);
		}
}

void dfs(int u) {
	vis[u] = true;
	CC.pb(u);

	for (auto& v : adj[u]) {
		adj2[u].pb(v);
		++edgeCnt[mp(u, v)];
		if (!vis[v]) {
			dfs(v);
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


	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		cin >> n >> m;
		adj.assign(n + 1, vi());
		adj2.assign(n + 1, vi());
		ans.clear();

		for (int i = 1; i <= m; ++i) {
			int u, v;
			cin >> u >> v;
			adj[u].pb(v);
			adj[v].pb(u);
		}

		int even = 0;

		vis.assign(n + 1, 0);

		for (int i = 1; i <= n; ++i) {
			if (!vis[i]) {
				edgeCnt.clear();
				CC.clear();
				dfs(i);

				for (auto& u : CC) {
					if (sz(adj2[u]) % 2 == 0)
						++even;
					else
						adj2[0].pb(u), adj2[u].pb(0), ++edgeCnt[mp(0, u)], ++edgeCnt[mp(u, 0)];
				}

				int src = adj2[0].empty() ? CC[0] : 0;

				cyc.clear();
				euler(cyc.begin(), src);

				vi U;

				U.pb(src);

				for (auto& elem : cyc)
					U.pb(elem);


				reverse(U.begin(), U.end());


				for (int i = 1; i < sz(U); ++i)
					if (U[i] && U[i - 1])
						ans.pb(mp(U[i - 1], U[i]));


				for (auto& u : CC)
					adj2[u].clear();
				adj2[0].clear();
			}
		}

		cout << even << '\n';


		for (auto& pr : ans)
			cout << pr.fs << ' ' << pr.sc << '\n';
	}


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
