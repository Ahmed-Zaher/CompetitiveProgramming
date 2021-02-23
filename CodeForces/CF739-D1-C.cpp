#include <bits/stdc++.h>
#include <ostream>

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
	while (*dbStr != ',') cout << *dbStr++;
	cout << " = " << e << ',';
	_db(dbStr + 1, r...);
}

template<class S, class T>
ostream &operator<<(ostream &o, const map<S, T> &v) {
	o << "[";
	int i = 0;
	for (const pair<S, T> &pr : v)
		o << (!i++ ? "" : ", ") << "{"
		  << pr.fs << " : " << pr.sc << "}";
	return o << "]";
}

template<template<class, class...> class S, class T, class... L>
ostream &operator<<(ostream &o, const S<T, L...> &v) {
	o << "[";
	int i = 0;
	for (const auto &e : v) o << (!i++ ? "" : ", ") << e;
	return o << "]";
}

template<class S, class T>
ostream &operator<<(ostream &o, const pair<S, T> &pr) {
	return o << "(" << pr.fs << ", " << pr.sc << ")";
}

ostream &operator<<(ostream &o, const string &s) {
	for (const char &c : s) o << c;
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

int n, m;
V<ll> a;

struct Node {
	int best, len, prefLen, prefSgn, suffLen, suffSgn;

	friend ostream &operator<<(ostream &os, const Node &node) {
		os << "best: " << node.best << " len: " << node.len << " prefLen: " << node.prefLen << " prefSgn: "
		   << node.prefSgn << " suffLen: " << node.suffLen << " suffSgn: " << node.suffSgn;
		return os;
	}

	Node(int x) : best(x) {}
};

struct SegmentTree {
	V<Node> t;
	int n;

	void upd(int v, ll val) {
		t[v].best = val != 0;
		t[v].len = 1;
		t[v].prefLen = t[v].suffLen = t[v].best;
		int sgn = !val ? 0 : (val > 0 ? 1 : -1);
		t[v].prefSgn = t[v].suffSgn = sgn;
	}

	Node combine(Node &valL, Node &valR) {
		if (valL.best == -1)
			return valR;
		if (valR.best == -1)
			return valL;
		Node ret(0);
		ret.len = valL.len + valR.len;
		ret.best = max(valL.best, valR.best);
		if (valL.suffSgn >= valR.prefSgn)
			ret.best = max(ret.best, valL.suffLen + valR.prefLen);
		ret.prefSgn = valL.prefSgn;
		ret.suffSgn = valR.suffSgn;
		ret.prefLen = valL.prefLen;
		if (valL.prefLen == valL.len && valL.suffSgn >= valR.prefSgn)
			ret.prefLen = valL.len + valR.prefLen;
		ret.suffLen = valR.suffLen;
		if (valR.suffLen == valR.len && valR.prefSgn <= valL.suffSgn)
			ret.suffLen = valR.len + valL.suffLen;
		return ret;
	}

	void build(int v, int tl, int tr) {
		if (tl == tr) {
			upd(v, a[tl]);
		} else {
			int tm = (tl + tr) >> 1;
			build(v << 1, tl, tm);
			build(v << 1 | 1, tm + 1, tr);
			Node valL = t[v << 1], valR = t[v << 1 | 1];
			t[v] = combine(valL, valR);
			db(tl, tr, t[v]);
		}
	}

	Node query(int v, int tl, int tr, int l, int r) {
		if (l > r)
			return Node(-1);
		if (l == tl && r == tr)
			return t[v];
		int tm = (tl + tr) >> 1;
		Node valL = query(v << 1, tl, tm, l, min(r, tm));
		Node valR = query(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r);
		return combine(valL, valR);
	}

	void update(int v, int tl, int tr, int idx, int newVal) {
		if (tl == tr) {
			a[tl] += newVal;
			upd(v, a[tl]);
		} else {
			int tm = (tl + tr) >> 1;
			if (idx <= tm)
				update(v << 1, tl, tm, idx, newVal);
			else
				update(v << 1 | 1, tm + 1, tr, idx, newVal);
			Node valL = t[v << 1], valR = t[v << 1 | 1];
			t[v] = combine(valL, valR);
		}
	}

	void init(int _n) {
		n = _n;
		t.assign(n << 2, Node(-1));
		build(1, 0, n - 1);
	}

	SegmentTree(int _n) {
		init(_n);
	}

	Node query(int i, int j) {
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

	cin >> n;

	vi a0(n);

	a.assign(n - 1, 0);

	for (int i = 0; i < n; ++i) {
		cin >> a0[i];
	}

	for (int i = 0; i < n - 1; ++i) {
		a[i] = a0[i + 1] - a0[i];
	}

	cin >> m;

	if (n == 1) {
		for (int i = 1; i <= m; ++i) {
			cout << "1\n";
		}
		return 0;
	}

	SegmentTree ST(n - 1);


	for (int i = 0; i < m; ++i) {
		int l, r, d;
		cin >> l >> r >> d;
		--l, --r;
		if (l)
			ST.update(l - 1, d);
		if (r < n - 1)
			ST.update(r, -d);
		cout << ST.t[1].best + 1 << '\n';
	}


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
