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

vvi adj, adjR;
int n;

struct SCC {
	vi vis, order, comp, CCID;
	vvi CC;
	int n;
	SCC(int _n) : n(_n) {
		vis.assign(n + 1, 0);
		for (int i = 1; i <= n; ++i)
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
	adjR.assign(n + 1, vi());

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			int aij;
			cin >> aij;
			if (aij) {
				adj[i].pb(j);
				adjR[j].pb(i);
			}
		}
	}

	SCC scc(n);

	if (sz(scc.CC) == 1)
		cout << "YES\n";
	else
		cout << "NO\n";

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
