/*
 * Note that each string will reduce to a string with a certain 1-bit value that is
 * determined completely by the string.
 * Now let f(n, m, c) be the numbers of strings consisting of n 0's, m 1's, and reduce to c (c is 0 or 1)
 * If c = 0:
 *      Not that if the first bit is a 1, then we will get a 0 no matter what the other bits are, so we add to our
 *      answer (n+m-1)Choose(m-1), otherwise the first bit is a 0, and since we want the string to reduce to 0,
 *      we must have the first 2 bits be 01 (because 00 reduces to 1). This is equivalent to finding the number
 *      of strings with n-1 zeros, m 1's and reduce to 1
 * Similar analysis is used for the case where c = 1.
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

const int MOD = 1000000007, MX = 2e5 + 5;

int fact[MX], factInv[MX];

ll inv(ll a, ll b) {
	return 1 < a ? b - inv(b % a, a) * b / a : 1;
}

ll add(ll x, ll y) {
	x += y;
	while(x >= MOD)
		x -= MOD;
	while(x < 0)
		x += MOD;
	return x;
}

ll mul(ll x, ll y) {
	return (x * y) % MOD;
}

ll nCk(ll n, ll k) {
	if (k > n)
		return 0;
	ll ret = fact[n];
	return mul(ret, mul(factInv[k], factInv[n - k]));
}

int f(int n, int m, int c) {
	if (n + m == 1)
		return m == c;
	int ret = 0;
	if (!c) {
		if (m)
			ret = add(ret, nCk(n + m - 1, m - 1));
		if (n)
			ret = add(ret, f(n - 1, m, 1));
	} else {
		if (n)
			ret = add(ret, f(n - 1, m, 0));
	}
	return ret;
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

	fact[0] = 1;
	factInv[0] = inv(fact[0], MOD);

	for (int i = 1; i < MX; ++i) {
		fact[i] = mul(i, fact[i - 1]);
		factInv[i] = inv(fact[i], MOD);
	}
	int n, m, g;
	cin >> n >> m >> g;
	cout << f(n, m, g) << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
