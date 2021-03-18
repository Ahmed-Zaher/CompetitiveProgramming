#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
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

template<class T, class S> using pb_map = tree<T, S,
		less<T>, rb_tree_tag, tree_order_statistics_node_update>;

const int LG = 31;

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

	vvi idxB(LG);

	si s;
	vi a(n);

	for (int i = 0; i < n; ++i) {
		cin >> a[i];
		for (int j = 0; j < LG; ++j) {
			if (a[i] & (1 << j))
				idxB[j].pb(i);
		}
		s.insert(a[i]);
	}

	for (int i = 0; i < LG; ++i) {
		idxB[i].pb(n);
	}

	vi ptrB(LG, 0);

	vii LR = monoStFunc(a);

	ll ans = 0;

	for (int i = 0; i < n; ++i) {

		for (int j = 0; j < LG; ++j) {
			if (a[i] & (1 << j))
				++ptrB[j];
		}

		int L = LR[i].fs + 1, R = LR[i].sc - 1;

		int LL = -1, RR = n;

		for (int j = 0; j < LG; ++j) {
			if (!(a[i] & (1 << j))) {
				int hi = idxB[j][ptrB[j]];
				int lo = -1;
				if (ptrB[j])
					lo = idxB[j][ptrB[j] - 1];
				LL = max(LL, lo);
				RR = min(RR, hi);
			}
		}

		db(i, a[i], L, R, LL, RR);

		if (LL >= L)
			ans += (R - i + 1) * 1LL * (LL - L + 1);

		if (RR <= R)
			ans += (R - RR + 1) * 1LL * (i - L + 1);

		if (LL >= L && RR <= R)
			ans -= (R - RR + 1) * 1LL * (LL - L + 1);

	}

	cout << ans << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
