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

#ifdef LOCAL
const int MX = 1e4 + 5;
#else
const int MX = 1e8 + 5;
#endif

bool prime[MX];
int primeCnt[MX];

int MOD;

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

ll inv(ll a, ll b) {
	return 1 < a ? b - inv(b % a, a) * b / a : 1;
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

	for (int i = 2; i < MX; ++i) {
		prime[i] = 1;
	}

	for (int i = 2; i < MX; ++i) {
		if (prime[i])
			for (int j = i * 2; j < MX; j += i)
				prime[j] = 0;
	}

	for (int i = 1; i < MX; ++i) {
		primeCnt[i] = prime[i] + primeCnt[i - 1];
	}

	int T;
	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		int N;
		cin >> N >> MOD;
		if (N <= 2) {
			cout << "0\n";
			continue;
		}
		int i;
		int ans = 1;
		for (i = 1; i * i <= N; ++i) {
			if (prime[i]) {
				ll x = i;
				int alpha = 0;
				while (x <= N)
					alpha += N / x, x *= i;
				db(i, alpha);
				ans = (ans * 1LL * (1 + alpha)) % MOD;
			}
		}
		int alphaTwo = 0;
		{
			ll x = 2;
			while (x <= N)
				alphaTwo += N / x, x *= 2;
			db(alphaTwo);
			ans = (ans * 1LL * inv(alphaTwo + 1, MOD)) % MOD;
		}
		int Sqrt = i - 1;
		db(ans, Sqrt);
		for (int i = 1; i <= Sqrt; ++i) {
			// pw = #primes p > Sqrt s.t. floor(N / p) = i
			// = #primes p > Sqrt s.t. floor(N/p) <= i - #primes p > Sqrt s.t. floor(N/p) <= i - 1
			int mnX = N / (i + 1) + 1, mnXNext = N / i + 1;
			db(mnX, mnXNext);
			db(primeCnt[mnXNext - 1], primeCnt[mnX - 1]);
			assert(mnXNext >= mnX);
			if (mnXNext - 1 > Sqrt) {
				int pw = primeCnt[mnXNext - 1] - primeCnt[max(mnX - 1, Sqrt)];
				db(i, pw, power(1 + i, pw));
				ans = (ans * power(1 + i, pw)) % MOD;
			}
			db(ans);
		}
		db(ans);
		cout << (ans - 1 + MOD) % MOD << endl;
	}

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
