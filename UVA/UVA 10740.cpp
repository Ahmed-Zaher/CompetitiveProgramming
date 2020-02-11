/*
 * When I say path here I actually mean a walk, where nodes can repeat.
 *
 * Think of a tree of possibilities for the path, where every node
 * determines a path, the root is the path that contains only the source
 * And if we extend a given path by one edge, we add an edge in the tree
 * from the original path to the new path.
 * Now if we expand that tree enough we'll eventually find the k'th shortest path
 * We can do that efficiently if we first expand nodes that represent the first
 * shortest path then 2nd shortest path until we get the k'th shortest path.
 * And we do that by first expanding nodes with min cost, where the cost of a node
 * is the sum of its path's edges plus the shortest path distance to go from the
 * end of that path to the destination. That can be implemented using a priority queue.
 * Because i'th shortest path from s to t (if exists) has at most i * n nodes,
 * and we'll eagerly discover the nodes of those shortest path, the number of nodes
 * pushed to the priority queue will be at most 10 * n * n which is good enough.
 *
 * This is like using A* tree search but with an exact heuristic, so we expand
 * exactly the shortest paths in order of increasing length.
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


const int MX_N = 105, MX = 1e6 + 100;
int n, m, s, t, k, adj[MX_N][MX_N], dist[MX_N][MX_N], idToNode[MX];


int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	while (cin >> n >> m && (n + m)) {

		cin >> s >> t >> k;

		for (int i = 1; i <= n; ++i)
			for (int j = 1; j <= n; ++j)
				adj[i][j] = OO;

		for (int i = 0; i < m; ++i) {
			int u, v, w;
			cin >> u >> v >> w;
			adj[u][v] = min(adj[u][v], w);
		}

		for (int i = 1; i <= n; ++i)
			for (int j = 1; j <= n; ++j)
				dist[i][j] = adj[i][j];

		for (int i = 1; i <= n; ++i) {
			dist[i][i] = 0;
		}


		for (int w = 1; w <= n; ++w)
			for (int u = 1; u <= n; ++u)
				for (int v = 1; v <= n; ++v)
					if (dist[u][w] + dist[w][v] < dist[u][v])
						dist[u][v] = dist[u][w] + dist[w][v];


		priority_queue<pii, vii, greater<pii>> pq;
		pq.push(mp(dist[s][t], 0));
		idToNode[0] = s;
		int id = 1;

		vi ans;

		while (!pq.empty() && sz(ans) < 10) {
			pii front = pq.top();
			pq.pop();
			int d = front.fs, u = idToNode[front.sc];
			if (u == t)
				ans.pb(d);

			for (int v = 1; v <= n; ++v) {
				if (adj[u][v] == OO || dist[v][t] == OO)
					continue;
				pq.push(mp(d - dist[u][t] + adj[u][v] + dist[v][t], id));
				idToNode[id++] = v;
			}
		}


		assert(id < MX);

		if (sz(ans) < k)
			cout << -1 << '\n';
		else
			cout << ans[k - 1] << '\n';

	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
