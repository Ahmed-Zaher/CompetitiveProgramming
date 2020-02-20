/*
 * We want to know if the min cut is unique: https://stackoverflow.com/a/7675034
 * There is one implementation note, when trying to add to the capacity
 * of an edge in the cut, instead of starting the flow from scratch, we continue
 * with what we got in the augmented graph so far, and before considering another
 * edge, we reverse the changes we did to the first edge.
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

struct Dinic {
	int n, src, snk, visCnt;
	vi vis, cap, d;
	vii changes;
	V<vii> adjAug;
	void addAugEdge(int u, int v, int c) {
		adjAug[u].pb(mp(v, sz(cap)));
		cap.pb(c);
		adjAug[v].pb(mp(u, sz(cap)));
		cap.pb(c);
	}
	void init() {
		visCnt = 0;
		vis.assign(n + 1, 0);
		d.assign(n + 1, OO);
		cap.clear();
		adjAug.assign(n + 1, vii());
		for (int u = 1; u <= n; ++u) // make sure u, n are appropriate values
			for (auto& pr : adj[u]) {
				int v = pr.fs, c = pr.sc;
				addAugEdge(u, v, c);
			}
	}
	Dinic(int _n) : n(_n)  {}
	int getFlow(int _src, int _snk) {
		init();
		src = _src, snk = _snk;
		int flow = 0, fAdd;
		while (bfs()) {
			do {
				++visCnt;
				fAdd = dfs(src, OO);
				flow += fAdd;
			} while (fAdd);
		}
		return flow;
	}
	int getExtraFlow(int _src, int _snk) {
		src = _src, snk = _snk;
		int flow = 0, fAdd;
		int l = sz(changes);
		while (bfs()) {
			do {
				++visCnt;
				fAdd = dfs(src, OO);
				flow += fAdd;
			} while (fAdd);
		}
		for (int r = sz(changes); l < r; ++l) {
			int edge = changes.back().fs;
			int f = changes.back().sc;
			cap[edge] += f;
			cap[edge ^ 1] -= f;
			changes.pop_back();
		}
		return flow;
	}
	int dfs(int u, int mn) {
		vis[u] = visCnt;
		if (u == snk)
			return mn;
		for (auto& pr : adjAug[u]) {
			int v = pr.fs, edge = pr.sc;
			int edgeCap = cap[edge];
			if (vis[v] != visCnt && edgeCap && d[v] == d[u] + 1) {
				int f = dfs(v, min(mn, edgeCap));
				if (f) {
					changes.pb(mp(edge, f));
					cap[edge] -= f;
					cap[edge ^ 1] += f;
					return f;
				}
			}
		}
		return 0;
	}
	bool bfs() {
		vis[src] = ++visCnt;
		queue<int> q;
		q.push(src);
		d[src] = 0;
		while (!q.empty()) {
			int u = q.front();
			if (u == snk)
				return 1;
			q.pop();
			for (auto& pr : adjAug[u]) {
				int v = pr.fs, edge = pr.sc, edgeCap = cap[edge];
				if (edgeCap)
					if (vis[v] != visCnt)
						q.push(v), d[v] = d[u] + 1, vis[v] = visCnt;
			}
		}
		return 0;
	}
};

V<bool> vis;
Dinic flow(0);
si X;

void dfs1(int u) {
	X.insert(u);
	vis[u] = true;
	for (auto& pr : flow.adjAug[u]) {
		int v = pr.fs, eID = pr.sc;
		db(u, v, eID);
		if (!vis[v]) {
			if (flow.cap[eID])
				dfs1(v);
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

	freopen("attack.in", "r", stdin);
	freopen("attack.out", "w", stdout);

	int n, m, s, t;

	while (cin >> n >> m >> s >> t && n) {

		adj.assign(n + 1, vii());

		for (int i = 1; i <= m; ++i) {
			int u, v, w;
			cin >> u >> v >> w;
			adj[u].pb(mp(v, w));
		}

		flow = Dinic(n);

		flow.getFlow(s, t);

		vis.assign(n + 1, 0);
		X.clear();
		dfs1(s);

		bool no = false;

		for (int u = 1; u <= n; ++u) {
			bool done = false;
			for (auto& pr : flow.adjAug[u]) {
				int v = pr.fs;
				if (present(X, u) && !present(X, v)) {
					flow.addAugEdge(u, v, 1);
					int ff = flow.getExtraFlow(s, t);
					db(u, v, ff);
					if (!ff) {
						done = true;
						no = true;
						break;
					}
					flow.adjAug[u].pop_back();
					flow.adjAug[v].pop_back();
					flow.cap.pop_back();
					flow.cap.pop_back();
				}
			}
			if (done)
				break;
		}

		if (!no)
			cout << "UNIQUE\n";
		else
			cout << "AMBIGUOUS\n";

	}


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
