/*
 * We can model this as a max-flow situation where we have,
 * 1. Edges with capacity = 1 from source to all the clubs
 * 2. Edges with capacity = 1 from every club to each of its members
 * 3. Edges with capacity = 1 from every member to the political party he belongs to
 * 4. Edges with capacity = floor((#clubs - 1) / 2) from every party to a sink
 * We have a valid assignment if max-flow = #clubs
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


int C, P, PP;
V<vii> adj;

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


int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int T;

	string s;

	cin >> T;

	cin.ignore();

	getline(cin, s);

	for (int tc = 1; tc <= T; ++tc) {

		if (tc > 1)
			ndl;

		int C = 0, P = 0, PP = 0;

		vvi clubs(1);
		vi parties(1);

		map<string, int> nameToId, clubToId, partyToId;
		map<int, string> idToName, idToClub, idToParty;

		while (getline(cin, s) && !s.empty()) {
			stringstream ss(s);
			string t;

			ss >> t;
			nameToId[t] = ++P;
			idToName[P] = t;

			ss >> t;
			if (!present(partyToId, t)) {
				partyToId[t] = ++PP;
				idToParty[PP] = t;
			}
			parties.pb(partyToId[t]);

			clubs.pb(vi());
			while (ss >> t) {
				if (!present(clubToId, t))
					clubToId[t] = ++C, idToClub[C] = t;
				clubs.back().pb(clubToId[t]);
			}

		}

		adj.assign(C + P + PP + 2, vii());

		for (int i = 1; i <= C; ++i) {
			adj[0].pb(mp(i, 1));
		}

		for (int i = 1; i <= P; ++i) {
			for (auto& club : clubs[i]) {
				adj[club].pb(mp(C + i, 1));
			}
			adj[C + i].pb(mp(C + P + parties[i], 1));
		}

		for (int i = C + P + 1; i <= C + P + PP; ++i) {
			adj[i].pb(mp(C + P + PP + 1, (C - 1) / 2));
		}

		Dinic d(C + P + PP + 2);

		int f = d.getFlow(0, C + P + PP + 1);

		if (f != C)
			cout << "Impossible.\n";
		else {
			V<pair<string, string>> ans;
			for (int u = 1; u <= C; ++u) {
				for (auto& pr : d.adjAug[u]) {
					int v = pr.fs, eID = pr.sc;
					if (!d.cap[eID]) {
						cout << idToName[v - C] << ' ' << idToClub[u] << '\n';
					}
				}
			}
		}

	}




	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
