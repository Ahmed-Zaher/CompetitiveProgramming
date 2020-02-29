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

int distSq(int x, int y, int xx, int yy) {
	return (xx - x) * (xx - x) + (yy - y) * (yy - y);
}

struct Dinic {
	int n, src, snk, visCnt;
	vi vis, cap, d;
	V<vii> adjAug;
	void addAugEdge(int u, int v, int c, int cRev = 0) {
		adjAug[u].pb(mp(v, sz(cap)));
		cap.pb(c);
		adjAug[v].pb(mp(u, sz(cap)));
		cap.pb(cRev);	// c if graph is undirected
	}
	void init() {
		visCnt = 0;
		vis.assign(n + 1, 0);
		d.assign(n + 1, OO);
		cap.clear();
		adjAug.assign(n + 1, vii());
		for (int u = 0; u < n; ++u) // make sure u, n are appropriate values
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
	int dfs(int u, int mn) {
		vis[u] = visCnt;
		if (u == snk)
			return mn;
		for (auto& pr : adjAug[u]) {
			int v = pr.fs, edge = pr.sc, edgeCap = cap[edge];
			if (vis[v] != visCnt && edgeCap && d[v] == d[u] + 1) {
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


struct GreenWarfare {
	int minimumEnergyCost(vector<int> canonX, vector<int> canonY,
			vector<int> baseX, vector<int> baseY, vector<int> plantX,
			vector<int> plantY, int energySupplyRadius) {
		int b = sz(baseX), p = sz(plantX);
		adj.assign(2 * (b + p) + 2, vii());
		for (int i = 1; i <= b; ++i) {
			adj[0].pb(mp(2 * i - 1, OO));
			int x = baseX[i - 1], y = baseY[i - 1];

			int cost = OO;

			for (int j = 0; j < sz(canonX); ++j)
				cost = min(cost, distSq(x, y, canonX[j], canonY[j]));

			adj[2 * i - 1].pb(mp(2 * i, cost));

			for (int j = 0; j < sz(plantX); ++j)
				if (distSq(x, y, plantX[j], plantY[j]) <= energySupplyRadius * energySupplyRadius)
					adj[2 * i].pb(mp(2 * (j + 1 + b) - 1, OO));

		}

		for (int i = b + 1; i <= b + p; ++i) {
			int cost = OO;
			int x = plantX[i - (b + 1)], y = plantY[i - (b + 1)];
			for (int j = 0; j < sz(canonX); ++j)
				cost = min(cost, distSq(x, y, canonX[j], canonY[j]));

			adj[2 * i - 1].pb(mp(2 * i, cost));
			adj[2 * i].pb(mp(2 * (b + p) + 1, OO));
		}

		Dinic d(2 * (b + p) + 2);

		return d.getFlow(0, 2 * (b + p) + 1);

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





	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
