/*
 * The given graph is a bamboo tree, a tree with one central node
 * where we have branches coming out of it, and those branches don't
 * have anymore branching. So we can solve the problem using a vector of
 * BITs, for each branch we maintain which edges are white and which are black,
 * and for 2 nodes we find if there's any white edges in the path between them.
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

struct FenwickTree {
    vector<int> bit;
    int n, LOGN;
    void init(int _n) {
    	n = _n;
		LOGN = log2(n) + 1;
		bit.assign(n + 1, 0);
    }
    FenwickTree(int _n) {
        init(_n);
    }
    FenwickTree(vector<int> a) {
    	init(sz(a));
    	for (int i = 0; i < n; ++i)
            add(i + 1, 0);
    }
    int sum(int idx) {
        int ret = 0;
        for (; idx > 0; idx -= idx & -idx)
            ret += bit[idx];
        return ret;
    }
    int sum(int l, int r) {
        return sum(r) - sum(l - 1);
    }
    void add(int idx, int delta) {
        for (; idx <= n; idx += idx & -idx)
            bit[idx] += delta;
    }
};

int n;
vvi adj, b;
vii pos;
vii edges;

void dfs(int u, int p) {
	pos[u] = mp(sz(b) - 1, sz(b.back()));
	b.back().pb(u);

	for (auto& v : adj[u]) {
		if (v != p)
			dfs(v, u);
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

	cin >> n;

	adj.assign(n + 1, vi());
	edges.assign(n, pii());

	for (int i = 1; i <= n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
		edges[i] = mp(u, v);
	}

	int root = 1;

	for (int u = 1; u <= n; ++u) {
		if (sz(adj[u]) > 2)
			root = u;
	}

	pos.assign(n + 1, mp(-1, -1));
	V<FenwickTree> fts;

	for (auto& v : adj[root]) {
		b.pb(vi());
		dfs(v, root);
		fts.pb(FenwickTree(b.back()));
	}

	int m;

	cin >> m;

	for (int i = 1; i <= m; ++i) {
		int t;
		cin >> t;
		if (t <= 2) {
			int id;
			cin >> id;
			int a = edges[id].fs, b = edges[id].sc;
			if (b == root)
				swap(a, b);

			int delta = (t == 1 ? -1 : 1);


			if (a == root)
				fts[pos[b].fs].add(pos[b].sc + 1, delta);
			else
				fts[pos[b].fs].add(max(pos[a].sc, pos[b].sc) + 1, delta);
		} else {
			int a, b;
			cin >> a >> b;

			if (a == b) {
				cout << "0\n";
				continue;
			}

			if (b == root)
				swap(a, b);

			if (a == root) {
				if (fts[pos[b].fs].sum(1, pos[b].sc + 1))
					cout << "-1\n";
				else
					cout << pos[b].sc + 1 << '\n';
			} else {
				int branchA = pos[a].fs, branchB = pos[b].fs;
				int idxA = pos[a].sc, idxB = pos[b].sc;
				if (branchA == branchB) {
					if (idxA > idxB)
						swap(idxA, idxB);
					if (fts[branchA].sum(idxA + 1 + 1, idxB + 1))
						cout << "-1\n";
					else
						cout << idxB - idxA << '\n';
				} else {
					if (fts[branchA].sum(1, idxA + 1) + fts[branchB].sum(1, idxB + 1))
						cout << "-1\n";
					else
						cout << idxB + 1 + idxA + 1 << '\n';
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
