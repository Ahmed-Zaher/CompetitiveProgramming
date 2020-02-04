/*
 * Let's store for every edge e, the g of all the paths that go through this edge
 * It is a necessary condition that f_e >= max g, it is also sufficient, because
 * if violated, the answer won't be consistent with at least one of passengers' answers
 * Now there's no need to make f_e greater than max g, as that can only lead
 * to more inconsistencies with the passengers' answers, so we put f_e = max g
 * and check if all the passerngers' answers are consistent.
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

const int MX = 5005;
int n, m, g[MX], w[MX];
vii adj[MX];
vi path, paths[MX];

void dfs(int u, int p, int w, int i) {
	if (u == w)
		paths[i] = path;

	for (auto& pr : adj[u]) {
		int v = pr.fs, eID = pr.sc;
		if (v == p)
			continue;
		path.pb(eID);
		dfs(v, u, w, i);
		path.pop_back();
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

	for (int i = 0; i < n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(mp(v, i));
		adj[v].pb(mp(u, i));
	}

	cin >> m;

	for (int i = 0; i < m; ++i) {
		int u, v;
		cin >> u >> v >> g[i];
		dfs(u, -1, v, i);

		for (auto& e : paths[i])
			w[e] = max(w[e], g[i]);

	}

	for (int i = 0; i < m; ++i) {
		int mn = 1e7;

		for (auto& e : paths[i])
			mn = min(mn, w[e]);

		if (mn != g[i]) {
			cout << "-1\n";
			return 0;
		}
	}

	for (int i = 0; i < n - 1; ++i)
		cout << max(w[i], 1) << ' ';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
