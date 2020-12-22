/*
 * O(k*n*log(n)^2) solution (which gets TLE) using
 * D&C dp optimization + persistent segment tree
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


struct Node {
	int L, R, sum;
	Node(int L = -1, int R = -1, int sum = 0) : L(L), R(R), sum(sum) {}

	friend ostream &operator<<(ostream &os, const Node &node) {
		os << "L: " << node.L << " R: " << node.R << " sum: " << node.sum;
		return os;
	}
};

struct SegmentTree {
	vi A;
	V<Node> t;
	// root[i] is the root of the tree after doing the i'th update, root[0] is initial tree
	vi roots;
	int n;

	// combine all values without messing with pointers to L, R
	void combine(Node& val, Node& valL, Node& valR) {
		val.sum = valL.sum + valR.sum;
	}
	// must use build, use the equivalent of zeros if no values are added yet
	int build(int tl, int tr) {
		int v = sz(t);
		if (tl == tr) { // default leaf node
			t.pb(Node());
		} else {
			int tm = (tl + tr) >> 1;
			// ignore non-ptr values, will add them through combine
			int L = build(tl, tm), R = build(tm + 1, tr);
			v = sz(t);
			t.pb(Node(L, R, 0));
			combine(t[v], t[t[v].L], t[t[v].R]);
		}
		return v;
	}
	Node query(int v, int tl, int tr, int l, int r) {
		if (l > r)
			return Node();	// empty node, should be ignored
		if (l == tl && r == tr)
			return t[v];
		int tm = (tl + tr) >> 1;
		Node valL = query(t[v].L, tl, tm, l, min(r, tm));
		Node valR = query(t[v].R, tm + 1, tr, max(l, tm + 1), r);
		Node ret;
		combine(ret, valL, valR);
		return ret;
	}
	int update(int v, int tl, int tr, int idx, int newVal) {
		int vUpd = sz(t);
		t.pb(t[v]);
		if (tl == tr) {
			t[vUpd].sum += newVal;
		} else {
			int tm = (tl + tr) >> 1;
			if (idx <= tm) {
				int LUpd = update(t[v].L, tl, tm, idx, newVal);
				t[vUpd].L = LUpd;
			} else {
				int RUpd = update(t[v].R, tm + 1, tr, idx, newVal);
				t[vUpd].R = RUpd;
			}
			combine(t[vUpd], t[t[vUpd].L], t[t[vUpd].R]);
		}
		return vUpd;
	}
	void init(int _n) {
		n = _n;
		roots.pb(build(0, n - 1));
	}
	SegmentTree(int _n) {
		init(_n);
	}
	SegmentTree() {}
	SegmentTree(const vi& _A) : A(_A) {
		init(sz(A));
	}
	// add extra parameters to check which roots to traverse
	Node query(int rootID, int i, int j) {
		return query(rootID, 0, n - 1, i, j);
	}
	void update(int idx, int newVal) {
		roots.pb(update(roots.back(), 0, n - 1, idx, newVal));
	}
};

const int MX_N = 35005, MX_K = 55;
int n, k, a[MX_N], root[MX_N], dp[MX_N][MX_K], last[MX_N], d;
SegmentTree ST;

void computeRow(int row, int l, int r, int optL, int optR) {
	if (l > r)
		return;
	int mid = (l + r) / 2;
	db(row, l, r, optL, optR, mid);
	int optMid;
	dp[mid][row] = -OO;
	for (int i = optL; i <= min(optR, mid); ++i) {
		int cand = dp[i - 1][row - 1] + ST.query(root[mid], i, mid).sum;
		if (cand > dp[mid][row])
			dp[mid][row] = cand, optMid = i;
	}
	db(optMid, dp[mid][row]);
	computeRow(row, l, mid - 1, optL, optMid);
	computeRow(row, mid + 1, r, optMid, optR);
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

	ST = SegmentTree(n + 1);

	for (int i = 1; i <= n; ++i) {
		cin >> a[i];
		last[a[i]] = -1;
	}

	for (int i = 1; i <= n; ++i) {
		if (last[a[i]] != -1)
			ST.update(last[a[i]], -1);
		ST.update(i, 1);
		root[i] = ST.roots.back();
		last[a[i]] = i;
	}

	for (int i = 1; i < MX_N; ++i) {
		dp[i][0] = -OO;
	}

	for (int j = 1; j <= k; ++j) {
		computeRow(j, 1, n, 1, n);
	}

//	for (int j = 1; j <= k; ++j) {
//		for (int i = 1; i <= n; ++i) {
//			cout << dp[i][j] << ' ';
//		}ndl;
//	}

	cout << dp[n][k] << '\n';


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
