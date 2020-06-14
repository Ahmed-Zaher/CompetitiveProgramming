/*
 * Let's express the solution as a sequence of operations add/sub/dup
 * An example of a solution is: add dup dup sub add dup dup add
 * Consider all the operations between the first and last dup operations,
 * (the add/sub on the sides will be treated as special cases)
 * we note that we can't have two consecutive add's, since we can replace
 * those two add's by one add, find the nearest dup from the left,
 * and swap them the add with the dup. We got the same result with lower cost
 * Similarly, we can't have two consecutive sub's
 * Knowing this, we can use dynamic programming, because we know that apart
 * from the sub's, the sequence only takes us to higher values, but also with
 * the sub's, we know that we will do at most one then do a dup operation
 * and if our number is > 2, then we can be sure that there's no cycles in
 * the dp recurrence
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

const int MX = 2e7;
ll n, x, y, mem[MX][2];



ll dp(int m, bool dbl) {
	if (m >= MX)
		return 1e18;
	ll& ret = mem[m][dbl];
	if (ret != -1)
		return ret;
	ret = abs(m - n) * x;
	if (dbl)
		ret = min(ret, dp(m << 1, 0) + min(m * x, y));
	else {
		ret = min(ret, dp(m, 1));
		if (m > 2) {
			ret = min(ret, dp(m + 1, 1) + x);
			ret = min(ret, dp(m - 1, 1) + x);
		}
	}

	return ret;
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

	for (int i = 0; i < MX; ++i)
		mem[i][0] = mem[i][1] = -1;


	cin >> n >> x >> y;

	ll ans = 1e18;

	for (int i = 1; i < MX; ++i)
		ans = min(ans, x * i + dp(i, 0));


	cout << ans << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
