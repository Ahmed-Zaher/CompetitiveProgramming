/*
 * Let dp(u, d) be the maximal subset in subtree of u, knowing
 * that any vertex we take must be at least d edges away from u.
 * If we root the tree at 1, our answer will be dp(1, 0)
 * If d = 0, that implies that we can include node u, and hence
 * we can get dp(u, d) = a[u] + sum of dp(v, k), over all children
 * v of u. Otherwise, there will be one subtree of children of u that
 * will have minimum depth, we try all children and for each children
 * we try each depth, and give the other children d's such that all
 * constraints are satisfied
 * i.e.,
 * dp(u, d) = max(dp(v, max(dep-1, d-1)) + sum of dp(w, max(k-dep, d-1)) over all children w != v)
 * over all children v of u and valid depths from u
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

const int MX = 205;
int n, k, mem[MX][MX], a[MX], p[MX];
vvi adj;

int dp(int u, int d) {
	int& ret = mem[u][d];
	if (ret != -1)
		return ret;
	ret = 0;
	if (!d) {
		int cand = a[u];
		for (auto& v : adj[u]) {
			if (v != p[u])
				cand += dp(v, k);
		}
		ret = cand;
	}

	int cand = 0;

	ret = max(ret, cand);

	for (auto& v : adj[u]) if (v != p[u]) {
		for (int dd = 1; dd <= (k + 1) / 2; ++dd) {
			int ddd = k + 1 - dd;
			cand = dp(v, max(d - 1, dd - 1));
			for (auto& w : adj[u]) if (w != v && w != p[u]) {
				cand += dp(w, max(d - 1, ddd - 1));
			}
			ret = max(ret, cand);
		}
	}
	return ret;
}

void dfs(int u, int par = -1) {
	p[u] = par;
	for (auto& v : adj[u]) {
		if (v != par)
			dfs(v, u);
	}
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

	cin >> n >> k;

	for (int i = 1; i <= n; ++i) {
		cin >> a[i];
	}

	adj.assign(n + 1, vi());

	for (int i = 1; i <= n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
	}

	dfs(1);
	db(adj);
	for (int i = 1; i <= n; ++i) {
		db(i, p[i]);
	}

	memset(mem, -1, sizeof(mem));

	cout << dp(1, 0) << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
