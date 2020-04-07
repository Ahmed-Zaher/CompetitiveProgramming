/*
 * Let's for all i store next(i), prev(i)
 * where next(i) is the min j s.t. a_j > a_i, prev(i) is max j s.t. a_j < a_i
 * Now given a query (i, j), clearly if a_i = a_j then the answer is j - i + 1
 * If not, that means we can move a_i to next(i) and j to prev(j), while considering
 * that the answer could be next(i) - a_i or j - prev(j)
 * Now [i, j] cover all occurrences of all the numbers they include, so we just want
 * max(frequency[a[i]..a[j]]), which can be computed efficiently using a segment tree
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
		return max(valL, valR);
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


SegmentTree ST(200005);



int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int n, q;

	while (cin >> n && n) {
		cin >> q;
		vi a(n);

		for (int i = 0; i < n; ++i) {
			cin >> a[i];
			a[i] += 100000;
			ST.update(a[i], 1);
		}

		vi next(n, -1), prev(n, -1);

		{
			int last = a[0], lastIdx = 0;

			for (int i = 0; i < n; ++i) {
				if (a[i] > last) {
					for (int j = lastIdx; j < i; ++j)
						next[j] = i;
					last = a[i];
					lastIdx = i;
				}
			}
		}

		{
			int last = a[n - 1], lastIdx = n - 1;

			for (int i = n - 1; i >= 0; --i) {
				if (a[i] < last) {
					for (int j = i + 1; j <= lastIdx; ++j)
						prev[j] = i;
					last = a[i];
					lastIdx = i;
				}
			}
		}

		for (int qq = 1; qq <= q; ++qq) {
			int i, j;

			cin >> i >> j;
			--i, --j;

			int ans = 0;

			if (a[i] == a[j]) {
				ans = j - i + 1;
			} else {
				ans = next[i] - i;
				i = next[i];
				if (a[i] == a[j])
					ans = max(ans, j - i + 1);
				else {
					ans = max(ans, j - prev[j]);
					j = prev[j];
					ans = max(ans, ST.query(a[i], a[j]));
				}
			}

			cout << ans << '\n';
		}


		for (int i = 0; i < n; ++i)
			ST.update(a[i], -1);

	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
