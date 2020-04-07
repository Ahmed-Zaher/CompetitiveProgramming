/*
 * Consider a cycle C in the cactus graph, every node on it can
 * be part of other cycles or only this cycle C, if two nodes in C
 * belong to other cycles C` and C`` respectively, then we're guaranteed
 * that C` and C`` can only reach each other with a path that goes through C,
 * because if there were some other path that would imply that some edges
 * in C belong to more than one cycle which contradicts the fact that we
 * are given a cactus graph.
 * This implies that we can only remove one edges from C, if we removed
 * none then it's impossible to get the tree, and if we removed 2 or more
 * then there'd more than 1 connected components in the resulting graph
 * Hence we can chose which edges to remove from C in |C| ways, then we
 * remove the edges of C to find the next cycle and repeat the process.
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

V<si> adj;
vi vis, cycle, path;

void dfs(int u, int p = -1) {
	if (vis[u]) {
		for (int i = 0; i < sz(path); ++i)
			if (path[i] == u)
				for (int j = i; j < sz(path); ++j)
					cycle.pb(path[j]);
		return;
	}

	path.pb(u);
	vis[u] = true;

	for (auto& v : adj[u]) {
		if (v == p)
			continue;
		dfs(v, u);
		if (!cycle.empty())
			return;
	}

	path.pop_back();
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

	int n, m, T;

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		cin >> n >> m;

		unsigned long long ans = 1;

		adj.assign(n + 1, si());

		for (int i = 1; i <= m; ++i) {
			int u, v;
			cin >> u >> v;
			adj[u].insert(v), adj[v].insert(u);
		}

		while (true) {
			cycle.clear();
			vis.assign(n + 1, 0);
			for (int i = 1; i <= n; ++i) {
				if (!vis[i]) {
					path.clear();
					dfs(i);
					if (!cycle.empty())
						break;
				}
			}

			if (cycle.empty())
				break;

			ans *= sz(cycle);
			db(cycle);
			cycle.pb(cycle[0]);
			for (int i = 0; i < sz(cycle) - 1; ++i) {
				int u = cycle[i], v = cycle[i + 1];
				adj[u].erase(v);
				adj[v].erase(u);
			}
		}

		cout << "Case " << tc << ": " << ans << '\n';
	}




	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
