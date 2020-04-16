/*
 * Let dp[i] be the max profit we can get if we consider billboards
 * with indices i, i+1, .., n-1 (0-based).
 * If we have the last i billboards, then we either skip i'th billboard
 * or we take j billboards (j <= k) and then we add to that dp[i+j+1]
 * the +1 is to skip an extra position so that we don't have two adjacent
 * groups of billboards.
 *
 * So, dp[i] = max(dp[i+1], max(dp[i+j+1]+sum[i..j-1]) for j in [1,k])
 *
 * Let g[i] = dp[i+1]+sum[0..i-1], and substitute in the above recurrence
 *
 * We get dp[i] = max(dp[i+1], max(g[i+j] - sum[0..i-1]))
 * 				= max(dp[i+1], [max(g[j]) for j in [i+1,i+k]] - sum[0..i-1])
 * So the only variable is [max(g[j]) for j in [i+1,i+k]], we can use sets that
 * store both g[j] and j, and query the maximum of that set, if its index is appropriate
 * we take it, if not we remove it and find the next maximum
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

struct cmp {
	bool operator()(pair<ll, int> p1, pair<ll, int> p2) {
		return mp(p1.fs, p1.sc) > mp(p2.fs, p2.sc);
	}
};

int n, k;
set<pair<ll , int>, cmp> g;
vi c;
V<ll> sum, dp;


int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> k;

	sum.assign(n, 0);
	c.assign(n, 0);
	dp.assign(n + 1, 0);

	for (int i = 0; i < n; ++i) {
		cin >> c[i];
		sum[i] = c[i];
		if (i)
			sum[i] += sum[i - 1];
	}

	g.insert(mp(sum[n - 1], n));


	for (int i = n - 1; i >= 0; --i) {
		while (g.begin()->sc > i + k)
			g.erase(g.begin());

		dp[i] = max(dp[i + 1], g.begin()->fs - (i ? sum[i - 1] : 0));

		g.insert(mp(dp[i + 1] + (i ? sum[i - 1] : 0), i));
	}

	cout << dp[0] << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
