/*
 * The distance between two fixed nodes is a linear function in the number of days.
 * Finding the diameter at day i is equivalent to drawing a line for each path, which
 * goes through the point (0, sum of C_e for all edges e on that path)
 * and has slope = sum of A_e for all edges e on that path, then finding the highest
 * intersection point of these lines with the vertical line x = i.
 * (this value can be computed by using the standard algorithm for finding tree diameter)
 * This maximum is a convex function in the number of days, hence we can use ternary search
 * to find at which day it becomes minimum
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

int N, D;
VV<pair<int, pii>> adj;

pair<ll, ll> dfs(int u, int par, int d) {
	ll mxH[] = {0, 0, 0}, diam = 0;

	for (auto& prr : adj[u]) {
		int v = prr.fs;
		ll w = prr.sc.fs + prr.sc.sc * 1LL * d;
		if (v != par) {
			pair<ll, ll> pr = dfs(v, u, d);
			diam = max(diam, pr.fs);
			mxH[0] = pr.sc + w;
			sort(mxH, mxH + 3);
		}
	}

	diam = max(diam, mxH[1] + mxH[2]);

	return mp(diam, mxH[2]);
}

ll f(int d) {
	return dfs(1, -1, d).fs;
}

pair<int, ll> ts() {
	int l = 0, r = D;
	while (r - l > 3) {
		int diff = r - l;
		int candL = l + diff / 3, candR = r - diff / 3;
		if (f(candL) <= f(candR))
			r = candR;
		else
			l = candL;
	}
	int idx = l;
	ll ans = f(l);
	for (int i = l; i <= r; ++i) {
		ll cand = f(i);
		if(ans > cand)
			ans = cand, idx = i;
	}
	return mp(idx, ans);
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

	cin >> N >> D;

	adj.assign(N + 1, V<pair<int, pii>>());

	for (int i = 1; i <= N - 1; ++i) {
		int u, v, a, c;
		cin >> u >> v >> c >> a;
		adj[u].pb(mp(v, mp(c, a)));
		adj[v].pb(mp(u, mp(c, a)));
	}

	pair<ll, ll> ans = ts();


	cout << ans.fs << '\n' << ans.sc << '\n';


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
