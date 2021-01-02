/*
 * Let AB be the set of elements liked by both Arkady and Masha,
 * A be the set liked by only Arkady, B liked only by Masha
 * Let Phi be the set of remaining elements
 * It is clear that if we take x elements from any of these sets, we will take
 * the x smallest ones.
 * At any solution, we will take i elements from AB, we will iterate over
 * all i's and minimize the answer.
 * If i >= k, then we pick min i elements from AB, and min m - i elements from A union B union Phi
 * If i < k, then we take min k - i elements from A, min k - i elements from B,
 * min m - (i + 2 * (k - i)) elements from A union B union Phi - {all elements we took from A and B}
 * This last computation can be done using a segment tree that holds value and can tells us what is
 * the sum of the smallest x values. We can try i's in decreasing order, at first the tree has all elements
 * in A, B, Phi, then when i decreases, we consider elements from A, B to be a mandatory part of our answer
 * and remove them from the tree so that it represents A union B union Phi - {all elements we took from A and B}
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



struct SegmentTree {
	V<pair<int, ll>> t;
	int n;
	vi cmpToVal;
	pair<int, ll> combine(pair<int, ll> valL, pair<int, ll> valR) {
		return mp(valL.fs + valR.fs, valL.sc + valR.sc);
	}
	void update(int v, int tl, int tr, int idx, int newVal) {
		if (tl == tr)
			t[v].fs += newVal, t[v].sc += cmpToVal[tl] * newVal;
		else {
			int tm = (tl + tr) >> 1;
			if (idx <= tm)
				update(v << 1, tl, tm, idx, newVal);
			else
				update(v << 1 | 1, tm + 1, tr, idx, newVal);
			pair<int, ll> valL = t[v << 1], valR = t[v << 1 | 1];
			t[v] = combine(valL, valR);
		}
	}
	ll findKth(int v, int tl, int tr, int k) {
		db(tl, tr, k, cmpToVal[tl], t[v], t[v << 1]);
		if (tl == tr) {
			assert(k <= t[v].fs);
			return cmpToVal[tl] * 1LL * k;
		}
		int tm = (tl + tr) >> 1;
		pair<int, ll> x = t[v << 1];
		if (x.fs >= k)
			return findKth(v << 1, tl, tm, k);
		else
			return  t[v << 1].sc + findKth(v << 1 | 1, tm + 1, tr, k - x.fs);
	}
	void init(int _n) {
		n = _n;
		t.assign(n << 2, mp(0, 0));
	}
	SegmentTree(const vi& cmpToVal) : cmpToVal(cmpToVal) {
		init(sz(cmpToVal));
	}
	void update(int idx, int newVal) {
		update(1, 0, n - 1, idx, newVal);
	}
	ll findKth(int k) {
		db(t);
		return findKth(1, 0, n - 1, k);
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

	int n, m, k;
	cin >> n >> m >> k;
	vi c(n);

	si s;

	for (int i = 0; i < n; ++i) {
		cin >> c[i];
		s.insert(c[i]);
	}

	mii valToCmp;
	vi cmpToVal;

	for (auto &elem : s) {
		valToCmp[elem] = sz(cmpToVal);
		cmpToVal.pb(elem);
	}

	V<bool> markA(n), markB(n);

	int a;
	cin >> a;
	for (int i = 0; i < a; ++i) {
		int x;
		cin >> x;
		markA[--x] = 1;
	}

	int b;
	cin >> b;
	for (int i = 0; i < b; ++i) {
		int x;
		cin >> x;
		markB[--x] = 1;
	}

	if (k > m) {
		cout << "-1\n";
		return 0;
	}

	vi A, B, AB, ABPhi;
	V<ll> sumAB, sumABPhi;

	for (int i = 0; i < n; ++i) {
		if (markA[i] && markB[i])
			AB.pb(c[i]);
		else {
			ABPhi.pb(c[i]);
			if (markA[i])
				A.pb(c[i]);
			if (markB[i])
				B.pb(c[i]);
		}
	}

	sort(AB.begin(), AB.end());
	sumAB.assign(sz(AB), 0);
	for (int i = 0; i < sz(AB); ++i) {
		if (!i)
			sumAB[i] = AB[i];
		else
			sumAB[i] = AB[i] + sumAB[i - 1];
	}

	sort(ABPhi.begin(), ABPhi.end());
	sumABPhi.assign(sz(ABPhi), 0);
	for (int i = 0; i < sz(ABPhi); ++i) {
		if (!i)
			sumABPhi[i] = ABPhi[i];
		else
			sumABPhi[i] = ABPhi[i] + sumABPhi[i - 1];
	}

	sort(A.begin(), A.end());
	sort(B.begin(), B.end());

	db(A, B, AB, ABPhi, sumAB, sumABPhi);

	ll ans = 1e18;

	for (int i = k; i <= min(sz(AB), m); ++i) {
		if (m - i <= n - sz(AB)) {
			ans = min(ans, sumAB[i - 1] + ((m - i) ? sumABPhi[m - i - 1] : 0));
		}
	}

	SegmentTree ST(cmpToVal);

	for (auto &elem : ABPhi) {
		ST.update(valToCmp[elem], 1);
	}

	db(ans);

	db(ST.findKth(0));

	int p = 0;
	ll acc = 0;
	for (int i = min(m, min(sz(AB), k - 1)); i >= 0; --i) {
		if (k - i > min(sz(A), sz(B)))
			break;
		if (m - i > sz(ABPhi))
			break;
		if ((i + 2 * (k - i)) > m)
			break;
		while (p < k - i)
			acc += A[p] + B[p], ST.update(valToCmp[A[p]], -1), ST.update(valToCmp[B[p]], -1), ++p;
		db(i, acc, sumAB[i - 1], m - (i + 2 * (k - i)));

		ans = min(ans, acc + (i ? sumAB[i - 1] : 0) + ST.findKth(m - (i + 2 * (k - i))));
	}

	db(cmpToVal, valToCmp);

	if (ans > 1e17)
		cout << "-1\n";
	else
		cout << ans << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
