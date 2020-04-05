/*
 * We can reduce this to a 2-SAT problem, consider the variables
 * x_1, x_2, .., x_n, where x_i is 0 if p_i is in A, 1 if it's in B
 * Consider x_i, x_j, x_j where p_j = a - p_i, p_k = b - p_i
 * We want x_i = x_j = x_j, because otherwise one of the tree numbers
 * will be in a set the number that it needs is in the other set
 * Note that i, j, k aren't necessarily distinct because p_i can be equal
 * to p_j for example if p_i = p_j = a / 2
 * But what if such j, k don't exist? If none of them exists then clearly
 * the answer is no, if only j exists then we have an extra constraint, which
 * is that x_i = 0, if only k exists then we have x_i = 1
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


int n, a, b;
mii idx;
si p;
vi pp, vis;
V<vi> adj, adjR;

struct SCC {
	vi vis, order, comp, CCID;
	vvi CC;
	int n;
	SCC(int _n) : n(_n) {
		vis.assign(n + 1, 0);
		for (int i = 0; i < n; ++i)
			if (!vis[i])
				dfs1(i);
		vis.assign(n + 1, 0);
		CCID.assign(n + 1, 0);
		reverse(order.begin(), order.end());
		for (auto& u : order)
			if (!vis[u]) {
				comp.clear();
				dfs2(u);
				for (auto& u : comp)
					CCID[u] = sz(CC);
				CC.pb(comp);
			}

	}
	void dfs1(int u) {
	    vis[u] = true;
	    for (auto& v : adj[u])
	        if (!vis[v])
	            dfs1(v);
	    order.pb(u);
	}
	void dfs2(int u) {
	    vis[u] = true;
	    comp.pb(u);
	    for (auto& v : adjR[u])
	    	if (!vis[v])
	    		dfs2(v);
	}
};

void no() {
	cout << "NO\n";
	exit(0);
}

void add(int x, int y) {
	adj[x].pb(y);
	adjR[y].pb(x);
}

void equalize(int i, int j) {
	add(2 * i, 2 * j);
	add(2 * i + 1, 2 * j + 1);
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

	pp.pb(0);

	cin >> n >> a >> b;

	for (int i = 0; i < n; ++i) {
		int pi;
		cin >> pi;
		idx[pi] = i;
		p.insert(pi);
	}

	if (a == b) {
		for (auto& pi : p) {
			if (!present(p, b - pi))
				no();
		}
		cout << "YES\n";
		for (int i = 1; i <= n; ++i)
			cout << 1 << " \n"[i == n];
		return 0;
	}

	adj.assign(2 * n, vi());
	adjR.assign(2 * n, vi());

	for (auto& pi : p) {
		int i = idx[pi];
		int b1 = present(p, a - pi);
		int b2 = present(p, b - pi);
		if (!(b1 + b2))
			no();
		if (b1 && !b2) {
			add(2 * i, 2 * i + 1);
			int j = idx[a - pi];
			equalize(i, j);
		} else if (b2 && !b1) {
			add(2 * i + 1, 2 * i);
			int j = idx[b - pi];
			equalize(i, j);
		} else {
			int j = idx[a - pi];
			equalize(i, j);
			j = idx[b - pi];
			equalize(i, j);
		}
	}


	SCC scc(2 * n);
	V<bool> assignment(n, false);
	for (int i = 0; i < n * 2; i += 2) {
		if (scc.CCID[i] == scc.CCID[i + 1])
			no();
		assignment[i / 2] = scc.CCID[i] > scc.CCID[i + 1];
	}

	cout << "YES\n";

	for (int i = 0; i < n; ++i)
		cout << assignment[i] << " \n"[i == n];


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
