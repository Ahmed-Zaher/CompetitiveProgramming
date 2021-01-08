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

struct Node {
	vi vals, resL, resR;
};

struct SegmentTree {
	vi A;
	V<Node> t;
	int n;
	void fillRes(vi& a, vi& b, vi& ans) {
		int cur = 0;
		int n = sz(a), m = sz(b);
		ans.assign(n, 0);
		for (int i = 0; i < n; ++i) {
			while (cur < m && b[cur] < a[i])
				++cur;
			ans[i] = cur;
		}
	}
	void build(int v, int tl, int tr) {
		if (tl == tr)
			t[v].vals.pb(A[tl]);
		else {
			int tm = (tl + tr) >> 1;
			build(v << 1, tl, tm);
			build(v << 1 | 1, tm + 1, tr);
			Node& valL = t[v << 1], valR = t[v << 1 | 1];
			t[v].vals.assign(sz(valL.vals) + sz(valR.vals), 0);
			merge(valL.vals.begin(), valL.vals.end(), valR.vals.begin(), valR.vals.end(), t[v].vals.begin());
			fillRes(t[v].vals, valL.vals, t[v].resL);
			fillRes(t[v].vals, valR.vals, t[v].resR);
		}
	}
	int query(int v, int tl, int tr, int l, int r, int x) {
		db(tl, tr, l, r, x);
		db(t[v].vals, t[v].resL, t[v].resR);
		if (l > r)
			return 0;
		if (x > tr - tl)
			return 0;
		if (l == tl && r == tr)
			return r - l + 1 - x;
		int tm = (tl + tr) >> 1;
		return query(v << 1, tl, tm, l, min(r, tm), t[v].resL[x])
			+ query(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r, t[v].resR[x]);
	}
	void init(int _n) {
		n = _n;
		t.assign(n << 2, Node());
		build(1, 0, n - 1);
	}
	SegmentTree(const vi& _A) : A(_A) {
		init(sz(A));
	}
	int query(int i, int j, int x) {
		db(i, j, x, t[1].vals);
		return query(1, 0, n - 1, i, j,
			   lower_bound(t[1].vals.begin(), t[1].vals.end(), x) - t[1].vals.begin());
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

	int n, k;

	cin >> n >> k;

	vi a(n);
	vi b(n);
	vvi occ(100005);

	for (int i = 0; i < n; ++i) {
		cin >> a[i];
		if (sz(occ[a[i]]) < k)
			b[i] = -OO;
		else
			b[i] = occ[a[i]][sz(occ[a[i]]) - k];
		occ[a[i]].pb(i);
	}

	db(b);

	SegmentTree ST(b);

	int last = 0;

	int q;

	cin >> q;

	for (int i = 0; i < q; ++i) {
		int x, y;
		cin >> x >> y;
		int l = (x + last) % n, r = (y + last) % n;
		if (l > r)
			swap(l, r);
		db(l, r);
		last = r - l + 1 - ST.query(l, r, l);
		cout << last << '\n';
	}


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
