/*
 * Let ans(m) be the max. # of cubes we can get if we were to chose
 * an X <= m. Clearly ans(m) = 1 + ans(max i - f(i)^3) for i in [1, m]
 * Where f(i) = max j s.t. j^3 <= i.
 * But note that max i - f(i)^3 = max(m - f(m)^3, (m - 1) - (f(m) - 1)^3)
 * That is, we either initially take a side of length = f(m) or f(m) - 1.
 * The case of X = m - 1 is better than any lower X, since (n+1)^3 - n^3
 * is strictly increasing. The case of X = m is better than any X >= m.
 * Now we can compute ans(m) efficiently
 * Now as to find the particular max X, if we took a side of length f(m)
 * that would contribute to a bigger X, and that contribution alone is bigger
 * than contribution of picking a smaller side plus the contribution of all subsequent
 * (smaller) sides. But this might make the #blocks lower because in that case:
 * m - f(m)^3 < (m - 1) - (f(m) - 1)^3.
 *
 * We know that given m, in the optimal solution we'd pick an X and return
 * 1 + ans(m`), in fact we can replace that m` with any Y as long as
 * ans(m`) = ans(m) - 1. If there exists such m` where m` <= m - f(m)^3,
 * we can greedily make the choice of taking a side of length f(m) and add f(m)^3
 * to our X, otherwise we take f(m)-1, and add (f(m)-1)^3.
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

ll bs(ll x) {
	ll lo = 1, hi = 1e6;

	while (lo < hi) {
		ll mid = (lo + hi + 1) >> 1;
		if (mid * mid * mid <= x)
			lo = mid;
		else
			hi = mid - 1;
	}
	return lo;
}

int f(ll m) {
	int ret = 0;
	while (m) {
		ll p = bs(m);
		ll y = p * p * p, yy = (p - 1) * (p - 1) * (p - 1);
		m = max(y - 1 - yy, m - y);
		++ret;
	}
	return ret;
}

ll getMin(int A) {
	ll lo = 0, hi = 1e16;

	while (lo < hi) {
		ll mid = (lo + hi) >> 1;

		if (f(mid) >= A)
			hi = mid;
		else
			lo = mid + 1;
	}
	return lo;
}


int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	ll m, tot = 0, cnt = 0;

	cin >> m;

	while (m) {
		int ans = f(m);
		ll nextM = getMin(ans - 1);
		ll p = bs(m);
		ll y = p * p * p, yy = (p - 1) * (p - 1) * (p - 1);
		if (nextM <= m - y)
			tot += y, m = m - y;
		else
			tot += yy, m = y - 1 - yy;
		++cnt;
	}

	cout << cnt << ' ' << tot << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
