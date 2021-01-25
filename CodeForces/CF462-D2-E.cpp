/*
 * When answering queries about the total paper length, we only care about how much paper we have at each interval
 * of unit length from [x, x+1], the order or 'shape' of paper in that range is irrelevant.
 * We will represent our paper with a segment tree of length = sheet width, where i'th element holds the total length
 * of paper at the interval [i, i+1] in the sheet.
 * The most important observation is that with every query of type 1, the sheet width decreases by
 * min(p, curr length of sheet - p). We will always fold the smaller side, but that means that we now use
 * indexing that is the opposite direction from what the queries assume, so we note that fact and do the appropriate
 * transformations.
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

struct SegmentTree {
	vi t, A;
	int n;
	int query(int v, int tl, int tr, int l, int r) {
		if (l > r)
			return 0;
		if (l == tl && r == tr)
			return t[v];
		int tm = (tl + tr) >> 1;
		int valL = query(v << 1, tl, tm, l, min(r, tm));
		int valR = query(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r);
		return valL + valR;
	}
	void update(int v, int tl, int tr, int idx, int newVal) {
		if (tl == tr)
			t[v] += newVal;
		else {
			int tm = (tl + tr) >> 1;
			if (idx <= tm)
				update(v << 1, tl, tm, idx, newVal);
			else
				update(v << 1 | 1, tm + 1, tr, idx, newVal);
			int valL = t[v << 1], valR = t[v << 1 | 1];
			t[v] = valL + valR;
		}
	}
	void init(int _n) {
		n = _n;
		t.assign(n << 2, 0);
	}
	SegmentTree(int _n) {
		init(_n);
	}
	int query(int i, int j) {
		return query(1, 0, n - 1, i, j);
	}
	void update(int idx, int newVal) {
		update(1, 0, n - 1, idx, newVal);
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

	int n, q;

	cin >> n >> q;

	SegmentTree ST(n);

	for (int i = 0; i < n; ++i) {
		ST.update(i, 1);
	}

	int L = 0, R = n, LR = 1;

	for (int qq = 1; qq <= q; ++qq) {
		int t;
		cin >> t;
		if (t == 1) {
			int p;
			cin >> p;
			int a, b;
			if (LR)
				a = p, b = R - L - p;
			else
				a = R - L - p, b = p;
//			db(a, b);
			if (a <= b) {
				int x = L, y = L + 2 * a - 1;
				while (x < L + a)
					ST.update(y, ST.query(x, x)), ++x, --y;
				L += a;
				if (!LR)
					LR = !LR;
			} else {
				if (LR)
					LR = !LR;
				int x = R - b, y = R - b - 1;

				while (x < R)
					ST.update(y, ST.query(x, x)), ++x, --y;
				R -= b;
			}
			assert(L < R);
		} else {
			int l, r;
			cin >> l >> r;
			if (LR)
				l += L, r += L;
			else
				l = R - l, r = R - r, swap(l, r);
//			db(L, R, l, r);
			cout << ST.query(l, r - 1) << '\n';
		}
	}

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
