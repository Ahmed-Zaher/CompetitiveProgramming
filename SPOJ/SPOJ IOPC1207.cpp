/*
 * Consider an arbitrary cuboid and look at it from the top, so we're seeing a square.
 * Every cell of that square has effects from queries in 3 directions, the 2 directions
 * we see (assume they're x and y) and on that is perpendicular to our eyes (assume it's z).
 * The x-queries affect some vertical (or horizontal, depending on orientation of axis, but
 * that doesn't matter) strips of the square, while the y-queries affect some horizontal strips
 * so overall, ignoring z-queries, we have 4 groups of cells in the square:
 * 1. cells whose x coordinates have been affected by odd number of only x-queries
 * 2. cells whose y coordinates have been affected by odd number of only y-queries
 * 3. cells whose y coordinates have been affected by odd number of both x and y-queries
 * 4. cells whose y coordinates have been affected by odd number of neither x nor y-queries
 * Type 1 and 2 will be red, while 3 and 4 are green, we can find the size of every group using
 * segment trees with lazy propagation.
 * Now this square is repeated (z2 - z1 + 1) times along the z axis, but some of those repetitions
 * have inverted color while the other is exactly the same.
 * The number of squares having inverted color is the number of y-coordinates who have been affected
 * by y-queries an odd number of times.
 *
 * And so we can compute the number of green cells.
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

#define y1 aaaaaaaa
#define y2 aaaaaaaaa


int T, N[3], Q;


struct Node {
	int l, r, sum;
	bool b;
	Node(int _l = -1, int _r = -1) : l(_l), r(_r), sum(0), b(0) {}
};

struct SegmentTree {
	vi A;
	V<Node> t;
	int n;

	void push(int v) {
		if (!t[v].b)
			return;
		t[v].b = 0;
		t[v * 2].b = !t[v * 2].b;
		t[v * 2 + 1].b = !t[v * 2 + 1].b;
		t[v * 2].sum = (t[v * 2].r - t[v * 2].l + 1) - t[v * 2].sum;
		t[v * 2 + 1].sum = (t[v * 2 + 1].r - t[v * 2 + 1].l + 1) - t[v * 2 + 1].sum;
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
		return ret;
	}
	void build(int v, int tl, int tr) {
		if (tl == tr)
			t[v] = Node(tl, tr);
		else {
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
	void lazyUpdate(int v, int tl, int tr, int l, int r) {
		if (l > r)
			return;
		if (l == tl && tr == r) {
			t[v].sum = (t[v].r - t[v].l + 1) - t[v].sum;
			t[v].b = !t[v].b;
		} else {
			push(v);
			int tm = (tl + tr) / 2;
			lazyUpdate(v * 2, tl, tm, l, min(r, tm));
			lazyUpdate(v * 2 + 1, tm+1, tr, max(l, tm + 1), r);
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
	Node query(int i, int j) {
		return query(1, 0, n - 1, i, j);
	}
	void lazyUpdate(int l, int r) {
		lazyUpdate(1, 0, n - 1, l, r);
	}
};

int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);


	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {

		V<SegmentTree> ST;

		for (int i = 0; i < 3; ++i) {
			cin >> N[i];
			ST.pb(SegmentTree(N[i]));
		}

		cin >> Q;

		for (int q = 1; q <= Q; ++q) {
			int t;
			cin >> t;
			if (t != 3) {
				int l, r;
				cin >> l >> r;
				ST[t].lazyUpdate(l, r);
			} else {

				int c[2][3];

				for (int i = 0; i < 2; ++i)
					for (int j = 0; j < 3; ++j)
						cin >> c[i][j];

				int l[3];

				for (int i = 0; i < 3; ++i)
					l[i] = c[1][i] - c[0][i] + 1;

				int s[3];

				for (int i = 0; i < 3; ++i)
					s[i] = ST[i].query(c[0][i], c[1][i]).sum;

				ll A = s[0] * 1LL * (l[1] - s[1]) + s[1] * 1LL * (l[0] - s[0]);
				ll B = l[0] * 1LL * l[1] - A;

				cout << (l[2] - s[2]) * A + s[2] * B << '\n';
			}
		}

	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
