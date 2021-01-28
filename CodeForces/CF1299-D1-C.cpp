#include <bits/stdc++.h>

using namespace std;

const int OO = 1e9;
const long double EPS = 1e-15;

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


int main() {
#ifdef LOCAL
	auto stTime = clock();
 	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cout << fixed;

	int n;

	cin >> n;

	vi a(n + 1);

	V<pair<ll, int>> pts;
	pts.pb(mp(0, 0));
	ll sum = 0;

	for (int i = 1; i <= n; ++i) {
		cin >> a[i];
		sum += a[i];
		pts.pb(mp(sum, i));
	}



	vi stk;
	stk.pb(0);

	for (int i = 1; i <= n; ++i) {
		while (sz(stk) >= 2) {
			auto prev = stk.back(), prevPrev = stk[sz(stk) - 2];
			long double slopePrev = ((long double) pts[prev].sc - pts[prevPrev].sc) / (pts[prev].fs - pts[prevPrev].fs);
			long double slopeCur = ((long double) pts[i].sc - pts[prev].sc) / (pts[i].fs - pts[prev].fs);
			if (slopeCur > slopePrev)
				stk.pop_back();
			else
				break;
		}
		stk.pb(i);
	}



	int cur = 1;
	pair<ll, int> sub;

	for (int i = 1; i < sz(stk); ++i) {
		for (int j = cur; j <= stk[i]; ++j) {
			cout << ((long double) pts[stk[i]].fs - sub.fs) / (pts[stk[i]].sc - sub.sc) << '\n';
		}
		cur = stk[i] + 1;
		sub = pts[stk[i]];
	}



#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
