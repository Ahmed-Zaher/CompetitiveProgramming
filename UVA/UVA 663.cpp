/*
 * Consider a bipartite graph where L = {1, 2, 3, ..}, R = {A, B, C, ..}
 * And |L| = |R| = n. We add an edge to i in L to j in R if the number i
 * lies within borders of slide j. The problem tells us that there is a matching of size n
 * in that graph. Let's find one such matching, and for each i find which j it was
 * paired with. We want to know if the edge (i, j) exists not only in this perfect matching
 * but all possible perfect matching and hence we can uniquely determine that number
 * i can only belong to slide j. We can check for that by simply removing that edge from
 * the graph and try to find a perfect matching again, if we do find one then that implies
 * that the number i can belong to another slide, and hence can't be uniquely determined.
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

V<vii> adj;

struct FordFulkerson {
	int n, src, snk, visCnt;
	vi vis, cap;
	V<vii> adjAug;
	pii badEdge;
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
		for (int u = 0; u < n; ++u)
			for (auto& pr : adj[u]) {
				int v = pr.fs, c = pr.sc;
				if (mp(u, v) != badEdge)
					addAugEdge(u, v, c);
			}
	}
	FordFulkerson(int _n, pii _badEdge = mp(-1, -1)) : n(_n), badEdge(_badEdge)  {}
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

struct Rect {
	int xmin, xmax, ymin, ymax;

	bool inside(int x, int y) {
		return x > xmin && x < xmax && y > ymin && y < ymax;
	}

};

int tc, n;


int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	while (cin >> n && n) {
		adj.assign(2 * n + 2, vii());

		V<Rect> rects(n + 1);

		for (int i = 1; i <= n; ++i)
			cin >> rects[i].xmin >> rects[i].xmax
				>> rects[i].ymin >> rects[i].ymax;

		int src = 0, snk = 2 * n + 1;

		for (int i = 1; i <= n; ++i) {
			adj[src].pb(mp(i, 1));
			adj[n + i].pb(mp(snk, 1));
			int x, y;
			cin >> x >> y;
			for (int j = 1; j <= n; ++j)
				if (rects[j].inside(x, y))
					adj[i].pb(mp(n + j, 1));
		}

		FordFulkerson ff(2 * n + 2);

		cout << "Heap " << ++tc << '\n';

		ff.getFlow(src, snk);

		vii edges;
		for (int u = 1; u <= n; ++u) {
			for (auto& pr : ff.adjAug[u]) {
				int v = pr.fs, edge = pr.sc, edgeCap = ff.cap[edge];

				if (v >= n + 1 && v <= 2 * n && !edgeCap)
					edges.pb(mp(u, v));

			}
		}

		vii goodEdges;

		for (auto& edge : edges) {
			FordFulkerson fff(2 * n + 2, edge);
			if (fff.getFlow(src, snk) == n - 1)
				goodEdges.pb(edge);
		}


		if (goodEdges.empty())
			cout << "none\n";
		else {
			for (auto& edge : goodEdges)
				edge = mp(edge.sc, edge.fs);

			sort(goodEdges.begin(), goodEdges.end());

			for (int i = 0; i < sz(goodEdges); ++i) {
				pii edge = goodEdges[i];

				cout << '(' << char(edge.fs - (n + 1) + 'A') << ','
						<< edge.sc << ')';

				if (i + 1 < sz(goodEdges))
					cout << ' ';
			}

			ndl;
		}

		ndl;

	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
