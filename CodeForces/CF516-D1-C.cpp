/*
 * We can make 2n trees, first n and last n are the same as the input's n trees,
 * now a query over a range [l, r] is just asking about the pair of trees x, y (x < y)
 * such that 2*h_x + 2*h_y + sum of d_i over i in [x, y) is maximized
 * This can be done by a segment tree, suppose we are merging two consecutive segments,
 * the best pair in the combined segment is either the best pair in one of the two smaller segments,
 * or a pair where x is in the first segment and y is in the second. In the latter case, we need
 * to find for each segment [l, r], which x has the maximum 2h_x + sum of d_i for i in [x, r],
 * and which x has maximum 2h_x + sum of d_i for i in [l, x)
 * This can be easily done if we compute the sum of d_i in each segment
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

int n, m;
vi h, d;

struct Node {
	int L, R;
	ll best, mxPref, mxSuff, sumD;

	Node(int l, int r, ll best, ll mxPref, ll mxSuff, ll sumD) : L(l), R(r), best(best), mxPref(mxPref), mxSuff(mxSuff),
	                                                             sumD(sumD) {}
};

struct SegmentTree {
	vi A;
	V<Node> t;
	int n;
	Node combine(Node& valL, Node& valR) {
		 if (valL.L == -1)
		 	return valR;
		 if (valR.L == -1)
		 	return valL;
		ll best = max(valL.best, valR.best);
		best = max(best, valL.mxSuff + valR.mxPref);
		ll mxPref = max(valL.mxPref, valL.sumD + valR.mxPref);
		ll mxSuff = max(valR.mxSuff, valR.sumD + valL.mxSuff);
		return Node(valL.L, valR.R, best, mxPref, mxSuff, valL.sumD + valR.sumD);
	}
	void build(int v, int tl, int tr) {
		if (tl == tr)
			t[v] = Node(tl, tr, 0, 2 * h[tl], 2LL * h[tl] + d[tl], d[tl]);
		else {
			int tm = (tl + tr) >> 1;
			build(v << 1, tl, tm);
			build(v << 1 | 1, tm + 1, tr);
			Node valL = t[v << 1], valR = t[v << 1 | 1];
			t[v] = combine(valL, valR);
		}
	}
	Node query(int v, int tl, int tr, int l, int r) {
		if (l > r)
			return Node(-1, -1, -1, -1, -1, -1);
		if (l == tl && r == tr)
			return t[v];
		int tm = (tl + tr) >> 1;
		Node valL = query(v << 1, tl, tm, l, min(r, tm));
		Node valR = query(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r);
		return combine(valL, valR);
	}
	void init(int _n) {
		n = _n;
		t.assign(n << 2, Node(-1, -1, -1, -1, -1, -1));
		build(1, 0, n - 1);
	}
	SegmentTree(int _n) {
		init(_n);
	}
	SegmentTree(const vi& _A) : A(_A) {
		init(sz(A));
	}
	Node query(int i, int j) {
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

	h.assign(2 * n, 0);
	d.assign(2 * n, 0);

	for (int i = 0; i < n; ++i) {
		cin >> d[i];
		d[i + n] = d[i];
	}
	for (int i = 0; i < n; ++i) {
		cin >> h[i];
		h[i + n] = h[i];
	}

	SegmentTree ST(2 * n);

	for (int i = 0; i < m; ++i) {
		int a, b;
		cin >> a >> b;
		--a, --b;
		if (a <= b)
			cout << ST.query(b + 1, a - 1 + n).best << '\n';
		else
			cout << ST.query(b + 1, a - 1).best << '\n';
	}

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
