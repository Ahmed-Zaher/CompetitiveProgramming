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
	vi t;
	int n;
	int query(int v, int tl, int tr, int l, int r) {
		if (l > r)
			return 0;
		if (l == tl && r == tr)
			return t[v];
		int tm = (tl + tr) >> 1;
		int valL = query(v << 1, tl, tm, l, min(r, tm));
		int valR = query(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r);
		return max(valL, valR);
	}
	void update(int v, int tl, int tr, int idx, int newVal) {
		if (tl == tr)
			t[v] = newVal;
		else {
			int tm = (tl + tr) >> 1;
			if (idx <= tm)
				update(v << 1, tl, tm, idx, newVal);
			else
				update(v << 1 | 1, tm + 1, tr, idx, newVal);
			int valL = t[v << 1], valR = t[v << 1 | 1];
			t[v] = max(valL, valR);
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

	int n;

	cin >> n;

	vi p(n + 1), l(n + 1);

	p[n] = 2e9 + 5;

	for (int i = 0; i < n; ++i) {
		cin >> p[i] >> l[i];
	}

	db(p, l);

	vi mxIdx(n + 1);

	for (int i = n; i >= 0; --i) {
		mxIdx[i] = upper_bound(p.begin(), p.end(), p[i] + l[i]) - p.begin() - 1;
	}

	vi mxX(n + 1);

	SegmentTree ST(n + 1);

	for (int i = n; i >= 0; --i) {
		mxX[i] = p[i] + l[i];
		if (i + 1 <= mxIdx[i])
			mxX[i] = max(mxX[i], ST.query(i + 1, mxIdx[i]));
		ST.update(i, mxX[i]);
	}

	db(mxIdx, mxX);


	const int LG = 20;

	vvi par(n + 1, vi(LG, -1));
	vvi val(n + 1, vi(LG, 0));


	for (int i = 0; i < n; ++i) {
		par[i][0] = upper_bound(p.begin(), p.end(), mxX[i]) - p.begin();
		val[i][0] = p[par[i][0]] - mxX[i];
		db(i, par[i][0], val[i][0]);
	}



	for (int lg = 1; lg < LG; ++lg) {
		for (int i = 0; i < n; ++i) {
			if (par[i][lg - 1] != -1) {
				par[i][lg] = par[par[i][lg - 1]][lg - 1];
				val[i][lg] = val[i][lg - 1] + val[par[i][lg - 1]][lg - 1];
			}
		}
	}

	int q;

	cin >> q;

	for (int i = 1; i <= q; ++i) {
		int x, y;
		cin >> x >> y;
		--x, --y;
		if (mxX[x] >= p[y])
			cout << "0\n";
		else {
			int cur = x;
			int ans = 0;
			for (int lg = LG - 1; lg >= 0; --lg) {
				if (par[cur][lg] != -1 && mxX[par[cur][lg]] < p[y])
					ans += val[cur][lg], cur = par[cur][lg];
			}
			ans += val[cur][0];
			cout << ans << '\n';
		}
	}


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
