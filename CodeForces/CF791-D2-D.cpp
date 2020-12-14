/*
 * Solving the problem where k = 1 is an easy classical problem, the solution
 * to that will be extended to solve this one.
 * Let D(s, t) be the min. # of jumps to go from s, t, it's clear that
 * D(s, t) = ceil(dist(s, t) / k)
 * We note that ceil(i / k) increases by 1 only when i goes from 0 to 1 mod k, and decreases
 * by one when it goes from 1 to 0 mod k. So we will use the same idea from k = 1, but we will
 * consider the number of nodes whose distance from current node is i mod k, for all i.
 * We also consider these numbers for every subtree.
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
	for (const char& c : s) o << c; return o;
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

const int MX = 2e5 + 5;
int n, k, subTr[MX][5];
ll sumD0, ans;
vvi adj;

void dfs0(int u, int p, int d) {
	subTr[u][0]++;
	sumD0 += (d + k - 1) / k;

	for (auto &v : adj[u]) {
		if (v != p) {
			dfs0(v, u, d + 1);
			for (int i = 0; i < k; ++i) {
				subTr[u][(i + 1) % k] += subTr[v][i];
			}
		}
	}
}

void dfs(int u, int p, ll sumD, vi distMod) {
	db(u, p, sumD, distMod);
//	db(subTr[u][0], subTr[u][1], subTr[u][4]);
	ans += sumD;
	for (auto &v : adj[u]) {
		if (v != p) {
			sumD += distMod[0] - subTr[v][k - 1];
			sumD -= subTr[v][0];
			vi nextDistMod(k);
			for (int i = 0; i < k; ++i) {
				nextDistMod[(i + 1) % k] += distMod[i] - subTr[v][(i - 1 + k) % k];
				nextDistMod[i] += subTr[v][i];
			}
			db(subTr[u][0] - subTr[v][k - 1], subTr[v][0]);
			dfs(v, u, sumD, nextDistMod);
			sumD -= distMod[0] - subTr[v][k - 1];
			sumD += subTr[v][0];
		}
	}
}

int main() {
#ifdef LOCAL
	auto stTime = clock();
 	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> k;

	adj.assign(n + 1, vi());

	for (int i = 0; i < n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
	}

	dfs0(1, -1, 0);

	vi distMod;

	for (int i = 0; i < k; ++i) {
		distMod.pb(subTr[1][i]);
	}
	dfs(1, -1, sumD0, distMod);

	db(ans);

	cout << ans / 2 << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
