/*
 * Let totElemCnt[i] denote the count of every element for every time
 * it occurs in a subsequence with gcd = i, i.e. it's the sum of the lengths
 * of all subsequences with gcd = i.
 * Now let's say that we have an array of length 2 having all 2's, we can compute totElemCnt[i]
 * by the following recurrence:
 * 	totElemCnt[n] = g(n), g(n) = 2^(n-1) + 2 * g(n - 1), g(0) = 0
 * And we can use dp to compute g. Then we record for every divisor how many times it occurs in a
 * and compute totElemCnt[i] in decreasing order of i.
 * We first assume that all subsequences of elements divisible by i have gcd = i, then we exclude
 * the cases if i wasn't the gcd but iterating over all multiples of i and subtracting those cases
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


const int MOD = 1000000007;

ll power(ll num, ll exp) {
	ll ret = 1;
	while (exp) {
		if (exp & 1)
			ret = (ret * num) % MOD;
		num = (num * num) % MOD;
		exp >>= 1;
	}
	return ret;
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

const int MX = 1000001;
vvi divs(MX);
vi freq(MX), totElemCnt(MX), g(MX);



int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);


	for (int i = 1; i < MX; ++i)
		for (int j = i; j < MX; j += i)
			divs[j].pb(i);

	g[1] = 1;

	for (int i = 2; i < MX; ++i)
		g[i] = add(power(2, i - 1), mul(2, g[i - 1]));

	int n;

	cin >> n;


	for (int i = 0; i < n; ++i) {
		int ai;
		cin >> ai;

		for (auto& d : divs[ai])
			++freq[d];
	}

	for (int i = MX - 1; i >= 2; --i) {
		int f = freq[i];
		totElemCnt[i] = g[f];

		for (int j = 2 * i; j < MX; j += i)
			totElemCnt[i] = add(totElemCnt[i] , -totElemCnt[j]);
	}


	int ans = 0;

	for (int i = 2; i < MX; ++i)
		ans = add(ans, mul(totElemCnt[i], i));

	cout << ans << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
