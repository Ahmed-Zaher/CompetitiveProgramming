/*
 * Let dp[i] be the number of 4-subsets with gcd = i
 * The answer is dp[1], since numbers that have only divisor = 1
 * also have gcd = 1.
 * And we note that:
 * dp[i] = number of 4-subsets divisible by i - dp[j * i] for j >= 2
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

const int MX = 1e4 + 5;
int divCnt[MX];
ll dp[MX];

void getDivs(int x) {
	++divCnt[1];
	if (x > 1)
		++divCnt[x];
	int i;
	for (i = 2; i * i < x; i++)
		if (x % i == 0)
			++divCnt[i], ++divCnt[x / i];
	if (i * i == x)
		++divCnt[i];
}

void rmvDivs(int x) {
	--divCnt[1];
	if (x > 1)
		--divCnt[x];
	int i;
	for (i = 2; i * i < x; i++)
		if (x % i == 0)
			--divCnt[i], --divCnt[x / i];
	if (i * i == x)
		--divCnt[i];
}

int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int N;

	while (cin >> N) {


		vi a(N);

		for (int i = 0; i < N; ++i) {
			cin >> a[i];
			getDivs(a[i]);
		}

		for (int i = MX - 1; i >= 1; --i) {
			ll x = divCnt[i];
			x = x * (x - 1) * (x - 2) * (x - 3);
			assert(x % 24 == 0);
			dp[i] = x / 24;
			for (int j = 2 * i; j < MX; j += i)
				dp[i] -= dp[j];
		}

		cout << dp[1] << '\n';

		for (int i = 0; i < N; ++i)
			rmvDivs(a[i]);

	}

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
