/*
 * We want to check for every node u whether the max distance to another
 * node v where v is in p is less than or equal to d.
 * We can compute that by rooting the tree at an arbitrary root then doing dfs.
 * The max distance is either in the subtree of current node or it's not.
 * Let dep[u] be the max distance from a node in p from u in the subtree of u.
 * For the other case where the max dist in not in the subtree of u, we can
 * maintain a variable in our state to denote that, by noting that this max
 * dist increases by one every time to go deeper in the tree.
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


int n, m, d, ans;
vi dep;
vvi adj;
V<bool> P;

void dfs1(int u, int p = -1) {
	dep[u] = P[u] - 1;

	for (auto& v : adj[u]) {
		if (v != p) {
			dfs1(v, u);
			if (dep[v] != -1)
				dep[u] = max(dep[u], dep[v] + 1);
		}
	}
}

void dfs2(int u, int mx, int p = -1) {
	if (mx == -1 && P[u])
		mx = 0;

	if (max(mx, dep[u]) <= d)
		++ans;
	set<pii> s;
	for (auto& v : adj[u]) {
		if (v != p) {
			s.insert(mp(-dep[v], v));
		}
	}


	for (auto& v : adj[u]) {
		if (v != p) {
			s.erase(mp(-dep[v], v));
			int mxmx = (mx == -1 ? -1 : mx + 1);
			if (!s.empty() && s.begin()->fs != 1)
				mxmx = max(mxmx, -(s.begin()->fs) + 2);
			dfs2(v, mxmx, u);
			s.insert(mp(-dep[v], v));
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

	cin >> n >> m >> d;

	P.assign(n + 1, 0);
	adj.assign(n + 1, vi());

	for (int i = 1; i <= m; ++i) {
		int x;
		cin >> x;
		P[x] = 1;
	}

	for (int i = 1; i <= n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
	}

	dep.assign(n + 1, 0);

	dfs1(1);

	dfs2(1, -1);

	cout << ans << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
