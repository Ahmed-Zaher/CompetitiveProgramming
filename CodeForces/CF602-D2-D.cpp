/*
 * Represent array element i as a point (i, a[i])
 * We want to find the largest/smallest slope of line connecting
 * two points (i, a[i]), (j, a[j])
 * We can prove that this slope will always be the difference between
 * two consecutive elements within our range
 * So we can find differences between adjacent elements then for each
 * query, we just want to find the sum of max difference[i..j] for all i, j
 * in our query range - this is a classical problem that can be solved in
 * O(n) using monotonic stack
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


vii monoStFunc(vi a) {
	int n = sz(a);

	vii b(n);

	stack<pii> st;

	// find x[i]
	st.push(mp(INT_MAX, -1));

	for (int i = 0; i < n; ++i) {
		// want elements >= a[i], so remove elements < a[i]
		while (st.top().fs < a[i])	// change if needed
			st.pop();

		b[i].fs = st.top().sc;
		st.push(mp(a[i], i));
	}

	while (!st.empty())
		st.pop();

	// find y[i]
	st.push(mp(INT_MAX, n));

	for (int i = n - 1; i >= 0; --i) {
		while (st.top().fs <= a[i]) // change if needed
			st.pop();

		b[i].sc = st.top().sc;
		st.push(mp(a[i], i));
	}

	return b;
}

int n, q;
vi a, d;

int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> q;

	a.assign(n, 0);

	for (int i = 0; i < n; ++i)
		cin >> a[i];

	d.assign(n - 1, 0);

	for (int i = 0; i < n - 1; ++i)
		d[i] = abs(a[i + 1] - a[i]);

	for (int qq = 0; qq < q; ++qq) {
		int l, r;
		cin >> l >> r;
		--l, --r;
		vi v;
		for (int i = l; i < r; ++i)
			v.pb(d[i]);

		vii LR = monoStFunc(v);

		ll ans = 0;

		for (int i = 0; i < sz(v); ++i)
			ans += v[i] * 1LL * ((i - LR[i].fs) * 1LL * (LR[i].sc - i));

		cout << ans << '\n';
	}

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
