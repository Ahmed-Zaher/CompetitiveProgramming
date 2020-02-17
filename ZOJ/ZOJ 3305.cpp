/*
 * At any given point let's represent the materials we used so far
 * by a mask where i'th bit is on if we used i'th material.
 * For the next combination we can only take unused materials, so
 * we try all the possible combinations that we can take and check
 * if we have such combination in input.
 * By enumerating the possible combinations efficiently, and using
 * dynamic programming we get O(M + 3^N) complexity
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

const int MX = 16;
int N, M, dp[1 << MX];
bool got[1 << MX];

int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	while (cin >> N >> M) {

		for (int i = 0; i < (1 << N); ++i)
			dp[i] = got[i] = 0;

		for (int i = 1; i <= M; ++i) {
			int K, mask = 0;
			cin >> K;
			bool good = true;
			for (int j = 1; j <= K; ++j) {
				int b;
				cin >> b;
				if ((1 << --b) & mask)
					good = false;
				mask |= (1 << b);
			}
			if (good)
				got[mask] = true;
		}


		for (int i = (1 << N) - 1; i >= 0; --i) {
			for (int j = (i + 1) | i; j < (1 << N); j = (j + 1) | i) {
				assert((i & j) == i);
				if (got[i ^ j])
					dp[i] = max(dp[i], 1 + dp[j]);
			}
		}

		cout << dp[0] << '\n';
	}

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
