/*
 * Let's compute the answer using dynamic programming, let dp[i][j]
 * be the number of scoreboards we can get if we consider the coders
 * with index i up to N-1 (0-based indexing) knowing that the last
 * coder had a score of j. Then our answer is dp[0][VERY_BIG_SCORE]
 * At each i, the coder will get a score between [1, min(j-1, tot[i])]
 * (the case of "NNN" where player gets 0 score is an easy special case)
 * where tot[i] is the total points of the problems solved by i'th coder
 * The state transition will be:
 * dp[i][j] = sum of dp[i + 1][k] * f[description[i]][k] for k in [1, min(j-1, tot[i])]
 * Where f[i][j] is the number of ways a coder with description i can score exactly
 * j points. Computing f is a simpler problem that itself can be solved using DP
 * using tabulation with partial sums.
 * Now as for the recurrence for dp, we compute for every (i, mask, k) the following:
 * sum of dp[i][0]*f[mask][0] + dp[i][1]*f[mask][1] ... + dp[i][k]*f[mask][k]
 * this can be computed in O(1) if we again use tabulation with partial sums
 * Because we only have 3 problems and 8 different values of mask, this computation
 * is feasible. Then we also use the memory reduction trick to satisfy the memory limit
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


const int MX_CODERS = 22, MX_SCORE = 2e5 + 500, MOD = 1000000007;
int g[1 << 3][2][MX_SCORE], sumG[1 << 3][2][MX_SCORE], ones[1 << 3];
int dp[2][MX_SCORE], sumDp[1 << 3][2][MX_SCORE];


struct SRMIntermissionPhase {
	int countWays(vi points, V<string> desc) {
		for (int i = 0; i < (1 << 3); ++i)
			ones[i] = __builtin_popcount(i);

		for (int m = 0; m < (1 << 3); ++m) {
			vi scores;
			scores.pb(0);
			for (int i = 0; i < 3; ++i) {
				if (m & (1 << i))
					scores.pb(points[i]);
			}

			for (int i = 0; i < MX_SCORE; ++i) {
				g[m][0][i] = 0;
				sumG[m][0][i] = 1;
			}
			g[m][0][0] = 1;
			for (int i = 1; i <= ones[m]; ++i) {

				for (int j = 0; j < MX_SCORE; ++j)
					g[m][i & 1][j] = sumG[m][i & 1][j] = 0;

				for (int target = 1; target < MX_SCORE; ++target) {
					g[m][i & 1][target] = sumG[m][(i & 1) ^ 1][target - 1];
					if (max(0, target - scores[i]))
						g[m][i & 1][target] = (g[m][i & 1][target] - sumG[m][(i & 1) ^ 1][target - scores[i] - 1] + MOD) % MOD;
					sumG[m][i & 1][target] = (sumG[m][i & 1][target - 1] + g[m][i & 1][target]) % MOD;
				}
			}
		}

		int N = sz(desc);
		vi mask(N, 0), tot(N, 0);
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < 3; ++j)
				if (desc[i][j] == 'Y')
					mask[i] |= (1 << j), tot[i] += points[j];

		for (int i = 0; i < MX_SCORE; ++i) {
			dp[N & 1][i] = 1;
			for (int m = 0; m < (1 << 3); ++m) {
				if (i)
					sumDp[m][N & 1][i] = sumDp[m][N & 1][i - 1];
				sumDp[m][N & 1][i] = (sumDp[m][N & 1][i] + (1LL * dp[N & 1][i] * g[m][ones[m] & 1][i])) % MOD;
			}
		}


		for (int i = N - 1; i >= 0; --i) {

			for (int j = 0; j < MX_SCORE; ++j) {
				for (int m = 0; m < (1 << 3); ++m)
					sumDp[m][i & 1][j] = 0;
				dp[i & 1][j] = 0;
			}

			for (int mx = 1; mx < MX_SCORE; ++mx) {
				dp[i & 1][mx] = sumDp[mask[i]][(i & 1) ^ 1][min(mx - 1, tot[i])];
				for (int m = 0; m < (1 << 3); ++m)
					sumDp[m][i & 1][mx] = (sumDp[m][i & 1][mx - 1] + (1LL * dp[i & 1][mx] * g[m][ones[m] & 1][mx])) % MOD;
			}
		}

		return dp[0][MX_SCORE - 1];
	}
};

int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
