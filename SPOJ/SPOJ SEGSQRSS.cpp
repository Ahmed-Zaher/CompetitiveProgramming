/*
 * We can solve this using a segment tree with lazy propagation,
 * assume that our range has elements a[l..r], and we increment this
 * segment, the sum of squares would be: S = sum of (a[i] + c)^2 for all i in [l, r]
 * Note that S = (sum of a[i]^2) + (r-l+1)*c^2 + 2c*(sum of a[i]), so we need to
 * maintain for each node its sum of a[i] as well and compute the new values in O(1).
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


struct Node {
	int l, r, set;
	ll sum, sumSq;
	Node() {
		sum = sumSq = 0;
		set = -1;
		l = r = -1;
	}
};

struct SegmentTree {
	vi A;
	V<Node> t;
	int n;
	void push(int v) {
		if (t[v].set == -1)
			return;
		t[v * 2].set = t[v * 2 + 1].set = t[v].set;
		t[v * 2].sum = (t[v * 2].r - t[v * 2].l + 1) * 1LL * t[v].set;
		t[v * 2 + 1].sum = (t[v * 2 + 1].r - t[v * 2 + 1].l + 1) * 1LL * t[v].set;

		t[v * 2].sumSq = (t[v * 2].r - t[v * 2].l + 1) * 1LL * t[v].set * t[v].set;
		t[v * 2 + 1].sumSq = (t[v * 2 + 1].r - t[v * 2 + 1].l + 1) * 1LL * t[v].set * t[v].set;
		t[v].set = -1;
	}
	Node combine(Node& valL, Node& valR) {
		if (valL.l == -1)
			return valR;
		if (valR.l == -1)
			return valL;

		Node ret;
		ret.l = valL.l;
		ret.r = valR.r;
		ret.sum = valL.sum + valR.sum;
		ret.sumSq = valL.sumSq + valR.sumSq;

		return ret;
	}

	void build(int v, int tl, int tr) {
		if (tl == tr) {
			t[v].l = tl;
			t[v].r = tr;
			t[v].sum = A[tl];
			t[v].sumSq = A[tl] * 1LL * A[tl];
		} else {
			int tm = (tl + tr) / 2;
			build(v * 2, tl, tm);
			build(v * 2 + 1, tm + 1, tr);
			Node valL = t[v * 2], valR = t[v * 2 + 1];
			t[v] = combine(valL, valR);
		}
	}

	Node query(int v, int tl, int tr, int l, int r) {
		if (l > r)
			return Node();
		if (l == tl && r == tr)
			return t[v];
		push(v);
		int tm = (tl + tr) / 2;
		Node valL = query(2 * v, tl, tm, l, min(r, tm));
		Node valR = query(2 * v + 1, tm + 1, tr, max(l, tm + 1), r);
		return combine(valL, valR);
	}
	void lazyUpdate(int v, int tl, int tr, int l, int r, bool type, int newVal) {
		if (l > r)
			return;
		if (l == tl && tr == r) {
			int len = t[v].r - t[v].l + 1;
			if (!type) {
				t[v].set = newVal;
				t[v].sum = len * 1LL * newVal;
				t[v].sumSq = len * 1LL * newVal * newVal;
			} else {
				t[v].set = max(newVal, t[v].set + newVal);
				t[v].sumSq += len * 1LL * newVal * newVal + 2LL * newVal * t[v].sum;
				t[v].sum = len * 1LL * newVal;
			}

		} else {
			push(v);
			int tm = (tl + tr) / 2;
			lazyUpdate(v * 2, tl, tm, l, min(r, tm), type, newVal);
			lazyUpdate(v * 2 + 1, tm+1, tr, max(l, tm + 1), r, type, newVal);
			Node valL = t[v * 2], valR = t[v * 2 + 1];
			t[v] = combine(valL, valR);
		}
	}
	void init(int _n) {
		n = _n;
		t.assign(4 * n, Node());
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
	void lazyUpdate(int l, int r, bool type, int newVal) {
		lazyUpdate(1, 0, n - 1, l, r, type, newVal);
	}
};


int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int T;

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		int N, Q;

		cin >> N >> Q;

		vi A(N);

		for (int i = 0; i < N; ++i) {
			cin >> A[i];
		}

		SegmentTree ST(A);

		cout << "Case " << tc << ":\n";

		for (int i = 0; i < Q; ++i) {
			int t, st, nd, x;
			cin >> t >> st >> nd;
			if (t == 2)
				cout << ST.query(--st, --nd).sumSq << '\n';
			else {
				cin >> x;
				ST.lazyUpdate(--st, --nd, t, x);
			}
		}
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
