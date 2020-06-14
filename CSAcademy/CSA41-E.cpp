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


struct FenwickTree {
    vector<int> bit;
    int n, LOGN;
    void init(int _n) {
    	n = _n;
		LOGN = log2(n) + 1;
		bit.assign(n + 1, 0);
    }
    FenwickTree() {}
    FenwickTree(vector<int> a) {
    	init(sz(a) + 1);
    	for (int i = 0; i < n; ++i)
            rangeAdd(i + 1, i + 1, a[i]);
    }
    int sum(int idx) {
        int ret = 0;
        for (; idx > 0; idx -= idx & -idx)
            ret += bit[idx];
        return ret;
    }
    int sum(int l, int r) {
        return sum(r) - sum(l - 1);
    }
    void add(int idx, int delta) {
        for (; idx <= n; idx += idx & -idx)
            bit[idx] += delta;
    }
    // for range updates and (only) point queries
    void rangeAdd(int l, int r, int val) {
        add(l, val);
        add(r + 1, -val);
    }
    int pointQuery(int idx) {
        int ret = 0;
        for (; idx > 0; idx -= idx & -idx)
            ret += bit[idx];
        return ret;
    }
};

int N, M;
vi h, c;
FenwickTree ft;

int bs(int v) {
	int lo = 1, hi = N + 1;

	while (lo < hi) {
		int mid = (lo + hi) >> 1;

		if (ft.pointQuery(mid) <= v)
			hi = mid;
		else
			lo = mid + 1;
	}

	return lo;

}

int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> N >> M;

	h.assign(N, 0);
	c.assign(M, 0);

	for (int i = 0; i < N; ++i)
		cin >> h[i];

	for (int i = 0; i < M; ++i)
		cin >> c[i];

	sort(h.rbegin(), h.rend());

	h.pb(0);

	ft = FenwickTree(h);


	for (int i = 0; i < M; ++i) {

		if (c[i] > N || ft.pointQuery(c[i]) == 0) {
			cout << i << '\n';
			return 0;
		}

		int v = ft.pointQuery(c[i]), w = ft.pointQuery(c[i] + 1);
		if (v == w) {
			int p1 = bs(v), p2 = bs(v - 1) - 1;

			if (p2 > c[i]) {
				ft.rangeAdd(c[i] + 1, p2, -1);
				ft.rangeAdd(p1, p1 + (p2 - c[i]) - 1, 1);
			}
		}

		ft.rangeAdd(1, c[i], -1);

	}

	cout << M << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
