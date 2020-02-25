/*
 * Let's iterate over l, for a fixed l, max a[l...r] is monotonically increasing w.r.t r
 * And min b[l...r] is monotonically decreasing w.r.t. r, we are interested in r's s.t.
 * max a[l...r] - min b[l...r] = 0, and we see that max a[l...r] - min b[l...r] is monotonically
 * increasing so we can do binary search on r.
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

int n;
vi a, b;

struct SegmentTree {
	vii t;
	int n;
	pii combine(pii valL, pii valR) {
		return mp(max(valL.fs, valR.fs), min(valL.sc, valR.sc));
	}
	void build(int v, int tl, int tr) {
		if (tl == tr)
			t[v] = mp(a[tl], b[tl]);
		else {
			int tm = (tl + tr) / 2;
			build(v * 2, tl, tm);
			build(v * 2 + 1, tm + 1, tr);
			pii valL = t[v * 2], valR = t[v * 2 + 1];
			t[v] = combine(valL, valR);
		}
	}
	pii query(int v, int tl, int tr, int l, int r) {
		if (l > r)
			return mp(-2e9, 2e9);
		if (l == tl && r == tr)
			return t[v];
		int tm = (tl + tr) / 2;
		pii valL = query(2 * v, tl, tm, l, min(r, tm));
		pii valR = query(2 * v + 1, tm + 1, tr, max(l, tm + 1), r);
		return combine(valL, valR);
	}
	void init(int _n) {
		n = _n;
		t.assign(4 * n, pii());
		build(1, 0, n - 1);
	}
	SegmentTree() {}
	SegmentTree(int _n) {
		init(_n);
	}
	pii query(int i, int j) {
		return query(1, 0, n - 1, i, j);
	}
};

SegmentTree ST;

int bs(int l, int lim) {
	int lo = l, hi = n;
	while (lo < hi) {
		int mid = (lo + hi) / 2;
		pii p = ST.query(l, mid);
		if (p.fs - p.sc > lim)
			hi = mid;
		else
			lo = mid + 1;
	}
	return lo;
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

	a.assign(n + 1, 2e9);
	b.assign(n + 1, -2e9);

	for (int i = 0; i < n; ++i) {
		cin >> a[i];
	}
	for (int i = 0; i < n; ++i) {
		cin >> b[i];
	}

	ST = SegmentTree(n + 1);

	ll ans = 0;

	for (int l = 0; l < n; ++l) {
		ans += bs(l, 0) - bs(l, -1);
	}

	cout << ans << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
