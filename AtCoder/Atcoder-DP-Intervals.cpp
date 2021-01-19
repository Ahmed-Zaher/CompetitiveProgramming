/*
 * Let's change the problem definition a little bit, we have an initial score of sum of all a_i's,
 * and if we set the whole range of i'th segment to zeros, we remove i'th segment from our answer
 * So our total answer is sum of a_i - sum of a_i's that we remove, and clearly we should minimize
 * the sum of a_i's we remove. We will use dp and construct the string. Let dp(i) be the optimal
 * answer for the string in range i..N. We can either put a 1 at i'th position and not remove any
 * segments, or we can put a bunch of zeros (from i up to some j) followed by a 1.
 * so our dp recurrence is:
 * dp(i) = min(dp(i+1), min of (dp(j+2)+sum of a's for segments that are inside [i, j]) over all j s.t. i<=j<=N)
 * We can proceed in order of decreasing i and have a segment tree s.t. when we're processing i'th element, it has
 * its j'th element be the value of dp(j+2) + score of all segments whose r <= j and l >= i. This can be done using
 * lazy propagation.
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
	ll mn, add;
	Node(ll mn, ll add) : mn(mn), add(add) {}
};

const int LIM = 1000000000;

struct SegmentTree {
	V<Node> t;
	int n;
	void push(int v) {
		t[v << 1].mn += t[v].add;
		t[v << 1].add += t[v].add;
		t[v << 1 | 1].mn += t[v].add;
		t[v << 1 | 1].add += t[v].add;
		t[v].add = 0;
	}
	Node combine(Node& valL, Node& valR) {
		return Node(min(valL.mn, valR.mn), 0);
	}
	Node query(int v, int tl, int tr, int l, int r) {
		if (l > r)
			return Node(1e18, 0);
		if (l == tl && r == tr)
			return t[v];
		push(v);
		int tm = (tl + tr) >> 1;
		Node valL = query(v << 1, tl, tm, l, min(r, tm));
		Node valR = query(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r);
		return combine(valL, valR);
	}
	void lazyUpdate(int v, int tl, int tr, int l, int r, ll newVal) {
		if (l > r)
			return;
		if (l == tl && tr == r) {
			t[v].mn += newVal, t[v].add += newVal;
		} else {
			push(v);
			int tm = (tl + tr) >> 1;
			lazyUpdate(v << 1, tl, tm, l, min(r, tm), newVal);
			lazyUpdate(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r, newVal);
			Node valL = t[v << 1], valR = t[v << 1 | 1];
			t[v] = combine(valL, valR);
		}
	}
	void init(int _n) {
		n = _n;
		t.assign(n << 2, Node(0, 0));
	}
	SegmentTree(int _n) {
		init(_n);
	}
	Node query(int i, int j) {
		return query(1, 0, n - 1, i, j);
	}
	void lazyUpdate(int l, int r, ll newVal) {
		lazyUpdate(1, 0, n - 1, l, r, newVal);
	}
};

int N, M;
vii segs[200005];


int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> N >> M;

	ll ans = 0;

	for (int i = 1; i <= M; ++i) {
		int l, r, ai;
		cin >> l >> r >> ai;
		segs[l].pb(mp(r, ai));
		ans += ai;
		assert(abs(ai) <= LIM);
	}

	V<ll> dp(N + 3);
	SegmentTree ST(N + 1);

	for (int i = N; i >= 1; --i) {
		ST.lazyUpdate(i, i, dp[i + 2]);
		for (auto &pr : segs[i])
			ST.lazyUpdate(pr.fs, N, pr.sc);

		dp[i] = min(dp[i + 1], ST.query(i, N).mn);
	}

//	db(dp);

	ans -= dp[1];

	cout << ans << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
