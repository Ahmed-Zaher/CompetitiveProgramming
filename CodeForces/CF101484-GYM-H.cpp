/*
 * Consider a graph of K nodes, for each p[i], p[i+1] add an edge (p[i], p[i+1])
 * with weight = g[i]. We want to color each type in one of 2 colors such that
 * the sum of weight of edges between nodes of different color is minimized. If
 * we ignore the types that must be colored with a certain color, this problem
 * becomes a common max flow problem:
 * https://www.coursera.org/lecture/advanced-algorithms-and-complexity/image-segmentation-refBK
 * To account for types that must be of a certain color, we add an edge of really big weight between
 * source (or sink, depending on how you modeled things) and that type, such that we get a huge
 * penalty added to our cut if we use a color we shouldn't use
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

int K, N;
vi belong, p, g;
V<vii> adj;
vvi adjMat;

struct FordFulkersonCapScaling {
	int n, src, snk, visCnt, mxC, delta;
	vi vis, cap;
	V<vii> adjAug;
	void addAugEdge(int u, int v, int c, int cRev = 0) {
		adjAug[u].pb(mp(v, sz(cap)));
		cap.pb(c);
		adjAug[v].pb(mp(u, sz(cap)));
		cap.pb(c);
	}
	void init() {
		visCnt = mxC = 0;
		vis.assign(n + 1, 0);
		cap.clear();
		adjAug.assign(n + 1, vii());
		for (int u = 0; u < n; ++u)
			for (auto& pr : adj[u]) {
				int v = pr.fs, c = pr.sc;
				addAugEdge(u, v, c);
				c = max(c, mxC);
			}
	}
	FordFulkersonCapScaling(int _n) : n(_n)  {}
	int getFlow(int _src, int _snk) {
		init();
		src = _src, snk = _snk;
		int flow = 0, fAdd;
		delta = 1;
		while (2 * delta <= mxC)
			delta *= 2;
		while (delta) {
			do {
				++visCnt;
				fAdd = dfs(src, OO);
				flow += fAdd;
			} while(fAdd);
			delta /= 2;
		}
		return flow;
	}
	int dfs(int u, int mn) {
		vis[u] = visCnt;
		if (u == snk)
			return mn;
		for (auto& pr : adjAug[u]) {
			int v = pr.fs, edge = pr.sc, edgeCap = cap[edge];
			if (vis[v] != visCnt && edgeCap >= delta) {
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
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int A, B;

	cin >> K >> N >> A >> B;

	belong.assign(K + 1, 0);

	for (int i = 1; i <= A; ++i) {
		int t;
		cin >> t;
		belong[t] |= 1;
	}

	for (int i = 1; i <= B; ++i) {
		int t;
		cin >> t;
		belong[t] |= 2;
	}

	adj.assign(K + 2, vii());

	int src = 0, snk = K + 1;

	for (int i = 1; i <= K; ++i) {
		if (belong[i] == 1) {
			adj[src].pb(mp(i, OO));
			adj[i].pb(mp(src, OO));
		}
		if (belong[i] == 2) {
			adj[snk].pb(mp(i, OO));
			adj[i].pb(mp(snk, OO));
		}
	}

	g.assign(N, 0);
	p.assign(N + 1, 0);

	for (int i = 1; i <= N; ++i)
		cin >> p[i];

	int ans = 0;

	for (int i = 1; i <= N - 1; ++i) {
		cin >> g[i];
		ans += g[i];
	}

	adjMat.assign(K + 1, vi(K + 1, 0));


	for (int i = 1; i <= N - 1; ++i) {
		adjMat[p[i]][p[i + 1]] += g[i];
		adjMat[p[i + 1]][p[i]] += g[i];
	}


	for (int i = 1; i <= K; ++i)
		for (int j = i + 1; j <= K; ++j)
			if (adjMat[i][j])
				adj[i].pb(mp(j, adjMat[i][j]));


	FordFulkersonCapScaling flow(K + 2);

	ans -= flow.getFlow(src, snk);

	cout << ans << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
