/*
 * The problem basically wants the maximum number of vertex disjoint
 * paths from the given b banks to the borders of the grid, this can
 * be solved using maxflow.
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

int T, s, a;
int dr[] = {0, 0, -1, 1};
int dc[] = {1, -1, 0, 0};

void connect(int u, int v, int w) {
	adj[2 * u + 1].pb(mp(2 * v, w));
}

int f(int r, int c) {
	return r * (a + 2) + c;
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

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {

		cin >> s >> a;

		int n = (s + 2) * (a + 2) + 2;

		adj.assign(2 * n, vii());

		for (int i = 0; i < n; ++i)
			adj[2 * i].pb(mp(2 * i + 1, 1));

		int src = n - 2, snk = n - 1;

		int b;

		cin >> b;

		for (int i = 1; i <= b; ++i) {
			int r, c;

			cin >> r >> c;

			connect(src, f(r, c), 1);

		}

		for (int r = 0; r < s + 2; ++r) {
			for (int c = 0; c < a + 2; ++c) {
				if (!r || r == s + 1 || !c || c == a + 1)
					connect(f(r, c), snk, 1);
				else {
					for (int k = 0; k < 4; ++k) {
						int to_r = r + dr[k];
						int to_c = c + dc[k];
						connect(f(r, c), f(to_r, to_c), 1);
					}
				}
			}
		}
		FordFulkerson ff(2 * n);


		cout << (ff.getFlow(src * 2 + 1, snk * 2) == b ? "possible\n" : "not possible\n");
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
