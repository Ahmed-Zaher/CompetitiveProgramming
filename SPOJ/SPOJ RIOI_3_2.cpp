/*
 * The answer is the same as the number of sequences a_0, a_1, .., a_(N-1)
 * such that:
 * 1. a_i < a_(i+1)
 * 2. sum of a_i <= M
 * Trying to solve the problem using this definition is difficult, because
 * we need to remember the biggest number we picked and the sum of the numbers
 * we picked so far to determine which values the next number can take
 * Now consider the sequence d, where a_0 = d0, a_1 = d_0 + d_1, a_2 = d_0 + d_1 + d_2, ...
 * There is a bijection between every sequence a_i and every sequence d_i, so we can
 * just count the number of valid sequences d_i such that:
 * 1. d_i > 0
 * 2. sum of (N-i)*d_i <= M for i in [0, N)
 * We can solve that using dynamic programming, let dp[idx][tot] be the number of ways to pick
 * d_idx, d_(idx+1),.., d_(N-1) knowing that the previous d_i's had sum = tot
 * If tot > M, then dp[idx][tot] = 0
 * If idx = N, then dp[idx][tot] = 1
 * Else, dp[idx][tot] = sum of dp[idx+1][tot + (N - idx) * i] for i in [1, OO)
 * We note that for a fixed index, we want sum of elements in
 * dp[idx+1] that are at positions with difference = (N-idx), we can use partial
 * sums to do that computation in O(1), hence the overall complexity is O(NM)
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

const int MX_N = 22, MX_M = 100005, MOD = 1000000007;
int dp[MX_N][MX_M], sum[MX_N][MX_M];


int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int T;

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		int N, M;
		cin >> N >> M;
		for (int idx = N; idx >= 0; --idx) {
			for (int tot = M; tot >= 0; --tot) {
				if (idx == N)
					dp[idx][tot] = 1;
				else
					dp[idx][tot] = (tot + (N - idx) <= M ? sum[idx + 1][tot + (N - idx)] : 0);
				sum[idx][tot] = (dp[idx][tot] + (tot + (N - idx + 1) <= M ? sum[idx][tot + (N - idx + 1)] : 0)) % MOD;
			}
		}

		cout << dp[0][0] << '\n';
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
