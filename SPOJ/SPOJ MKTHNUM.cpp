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

const int MX_N = 1e5 + 5;
int n, m;
int a[MX_N];
vi t[MX_N * 4];
int X;

struct SegmentTree {
	int n;
	vi combine(vi& valL, vi& valR) {
		int i = 0, j = 0;
		vi ret;
		while (i < sz(valL) && j < sz(valR))
			valL[i] < valR[j] ? ret.pb(valL[i++]) : ret.pb(valR[j++]);

		while (i < sz(valL))
			ret.pb(valL[i++]);

		while (j < sz(valR))
			ret.pb(valR[j++]);

		return ret;
	}
	void build(int v, int tl, int tr) {
		if (tl == tr)
			t[v].pb(a[tl]);
		else {
			int tm = (tl + tr) >> 1;
			build(v << 1, tl, tm);
			build(v << 1 | 1, tm + 1, tr);
			vi& valL = t[v << 1], valR = t[v << 1 | 1];
			t[v] = combine(valL, valR);
		}
	}
	int query(int v, int tl, int tr, int l, int r) {
		if (l > r)
			return 0;
		if (l == tl && r == tr)
			return upper_bound(t[v].begin(), t[v].end(), X) - t[v].begin();
		int tm = (tl + tr) >> 1;
		return query(v << 1, tl, tm, l, min(r, tm))
			+ query(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r);
	}
	void init(int _n) {
		n = _n;
		build(1, 0, n - 1);
	}
	SegmentTree(int _n) {
		init(_n);
	}
	int query(int i, int j) {
		return query(1, 0, n - 1, i, j);
	}
};


int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> m;


	for (int i = 0; i < n; ++i) {
		cin >> a[i];
	}

	SegmentTree ST(n);

	for (int i = 0; i < m; ++i) {
		int l, r, k;
		cin >> l >> r >> k;
		--l, --r;

		ll lo = -1e9 - 100, hi = 1e9 + 100;

		while (lo < hi) {
			X = (lo + hi + 2040000000) / 2 - 1020000000;
			db(X, lo, hi);
			if (ST.query(l, r) >= k)
				hi = X;
			else
				lo = X + 1;
		}

		cout << lo << '\n';
	}

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
