/*
 * Instead of finding a path 1->2->3 that doesn't visit a vertex twice,
 * let's find if we can find 2 vertex-disjoint paths 2->1, 2->3
 * Now that's a more popular problem that can be solved using max flow.
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


int t, n, m;
V<vii> adj;

struct FordFulkerson {
	int n, src, snk, visCnt;
	vi vis, cap;
	V<vii> adjAug;
	void addAugEdge(int u, int v, int c, int cRev = 0) {
		adjAug[u].pb(mp(v, sz(cap)));
		cap.pb(c);
		adjAug[v].pb(mp(u, sz(cap)));
		cap.pb(cRev);
	}
	void init() {
		visCnt = 0;
		vis.assign(n + 1, 0);
		cap.clear();
		adjAug.assign(n + 1, vii());
		for (int u = 1; u <= n; ++u)
			for (auto& pr : adj[u]) {
				int v = pr.fs, c = pr.sc;
				addAugEdge(u, v, c);
			}
	}
	FordFulkerson(int _n) : n(_n)  {}
	int getFlow(int _src, int _snk) {
		init();
		src = _src, snk = _snk;
		int flow = 0, fAdd;
		do {
			++visCnt;
			fAdd = dfs(src, OO);
			flow += fAdd;
		} while (fAdd);
		return flow;
	}
	int dfs(int u, int mn) {
		vis[u] = visCnt;
		if (u == snk)
			return mn;
		for (auto& pr : adjAug[u]) {
			int v = pr.fs, edge = pr.sc, edgeCap = cap[edge];
			if (vis[v] != visCnt && edgeCap) {
				int f = dfs(v, min(mn, edgeCap));
				if (f) {
					cap[edge] -= f;
					cap[edge ^ 1] += f;
					return f;
				}
			}
		}
		return 0;
	}
};


int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> t;

	for (int tc = 1; tc <= t; ++tc) {
		cin >> n >> m;

		vii edges;

		for (int i = 1; i <= m; ++i) {
			int u, v;
			cin >> u >> v;
			edges.pb(mp(u, v));
			n = max(n, u);
			n = max(n, v);
		}

		if (n <= 2) {
			cout << "NO\n";
			continue;
		}

		adj.assign(2 * n + 2, vii());

		for (int i = 1; i <= n; ++i) {
			adj[2 * i].pb(mp(2 * i - 1, 1));
		}

		for (int i = 0; i < m; ++i) {
			int u, v;
			u = edges[i].fs;
			v = edges[i].sc;
			adj[2 * u - 1].pb(mp(2 * v, 1));
			adj[2 * v - 1].pb(mp(2 * u, 1));
		}


		adj[1].pb(mp(2 * n + 1, 1));
		adj[5].pb(mp(2 * n + 1, 1));

		FordFulkerson ff(2 * n + 1);

		cout << (ff.getFlow(3, 2 * n + 1) == 2 ? "YES\n" : "NO\n");
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
