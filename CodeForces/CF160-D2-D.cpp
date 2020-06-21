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


struct UnionFind {
	vi p, rank, setSize;
	int numSets;
	UnionFind(int n) {
		setSize.assign(n, 1);
		numSets = n;
		rank.assign(n, 0);
		p.assign(n, 0);
		for (int i = 0; i < n; ++i)
			p[i] = i;
	}
	int findSet(int i) {
		return (p[i] == i) ? i : (p[i] = findSet(p[i]));
	}
	bool isSameSet(int i, int j) {
		return findSet(i) == findSet(j);
	}
	void unionSet(int i, int j) {
		if (!isSameSet(i, j)) {
			--numSets;
			int x = findSet(i), y = findSet(j);
			if (rank[x] > rank[y]) {
				p[y] = x;
				setSize[x] += setSize[y];
			} else {
				p[x] = y;
				setSize[y] += setSize[x];
				if (rank[x] == rank[y])
					++rank[y];
			}
		}
	}
	int numDisjointSets() {
		return numSets;
	}
	int sizeOfSet(int i) {
		return setSize[findSet(i)];
	}
};


const int MX = 1e6 + 5;
int n, m;
vii edges;
vi edgez[MX];
V<string> ans;
V<si> adj;
si nodes;
map<pii, vi> M;

struct ArtPointsAndBridges {
	int n, timer;
	vi st, low;
	V<bool> vis;
	void init() {
		timer = 0;
		st.assign(n + 1, 0);
		low.assign(n + 1, 0);
		vis.assign(n + 1, 0);
	}
	ArtPointsAndBridges(int _n = 0) : n (_n) { init(); }
	void findBridges() {
		timer = 0;
		for (auto& i : nodes) {
			if (!vis[i])
				dfs(i, -1, 1);
		}

		for (auto& i : nodes)
			st[i] = low[i] = vis[i] = 0;

	}
	// type 0: art. points, type 1: bridges
	void dfs(int u, int p, int type) {
		vis[u] = true;
		st[u] = low[u] = timer++;
		int children = 0;
		for (auto& v : adj[u]) {
			if (v == p)
				continue;
			if (vis[v])	// back / cross edge
				low[u] = min(low[u], st[v]);
			else {
				dfs(v, u, type);
				low[u] = min(low[u], low[v]);
				if (type && low[v] > st[u]) {
					if (sz(M[mp(u, v)]) == 1)
						ans[M[mp(u, v)][0]] = "any";
				}
				++children;
			}
		}
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

	cin >> n >> m;

	ans.assign(m, "at least one");

	for (int i = 0; i < m; ++i) {
		int u, v, w;
		cin >> u >> v >> w;
		edges.pb(mp(u, v));
		edgez[w].pb(i);
	}

	UnionFind UF(n + 1);

	adj.assign(n + 1, si());

	ArtPointsAndBridges bridges(n);


	for (int i = 1; i < MX; ++i) {
		nodes.clear();
		set<pii> newEdges;
		M.clear();
		for (auto& j : edgez[i]) {
			int s1 = UF.findSet(edges[j].fs);
			int s2 = UF.findSet(edges[j].sc);

			if (s1 == s2)
				ans[j] = "none";
			else {
				nodes.insert(s1);
				nodes.insert(s2);
				adj[s1].insert(s2);
				adj[s2].insert(s1);
				newEdges.insert(mp(s1, s2));
				M[mp(s1, s2)].pb(j);
				M[mp(s2, s1)].pb(j);
			}
		}
		bridges.findBridges();
		for (auto& pr : newEdges) {
			UF.unionSet(pr.fs, pr.sc);
			int s3 = UF.findSet(pr.fs);
			if (s3 == pr.fs) {
				for (auto& v : adj[pr.sc])
					adj[v].erase(pr.sc);
				adj[pr.sc].clear();
			} else {
				for (auto& v : adj[pr.fs])
					adj[v].erase(pr.fs);
				adj[pr.fs].clear();
			}
		}
	}


	for (auto& elem : ans)
		cout << elem << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
