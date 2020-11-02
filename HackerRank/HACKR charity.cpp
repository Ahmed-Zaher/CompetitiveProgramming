/*
 * We will use dp with bitmasks to solve this problem
 * Suppose that we want to assign a certain group of students to a certain table, the max. profit
 * we can get if we assign the most generous guest with the most charming student that didn't yet
 * approach k guests. The same strategy applies if we assign a subset of multiple tables to a single
 * group. So for starters, this gives us a way of computing the max profit achievable if we assign
 * group i with a mask of t bits of the tables, for all pairs of (i, mask), let that value be cost(i, mask)
 * Then what is left is to use dp, let dp(i, mask) be the optimal answer if we want to cover tables
 * that are in the mask, while considering only groups 1, 2, .., i.
 * It is clear that:
 *   dp(i, mask) = max cost(i, mask`) + dp(i - 1, mask ^ mask`) for all mask` that are subsets of mask
 * This can be computed in O(m * 3^t) if we iterate over submasks efficiently using bit tricks
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

int T, n, m, t, x, k;
int charm[16][16], ones[1 << 15];
vi tableSubset[1 << 15];
ll groupCost[16][1 << 15], dp[16][1 << 15];

int main() {
#ifdef LOCAL
	auto stTime = clock();
 	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	for (int i = 0; i < (1 << 15); ++i) {
		ones[i] = __builtin_popcount(i);
	}

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		cin >> m >> n >> t;
		for (int i = 1; i <= m; ++i) {
			for (int j = 0; j < n; ++j) {
				cin >> charm[i][j];
			}
			sort(charm[i], charm[i] + n);
		}

		for (int i = 0; i < t; ++i) {
			int x;
			cin >> x;
			tableSubset[1 << i].clear();
			tableSubset[1 << i].assign(x, 0);
			for (int j = 0; j < x; ++j) {
				cin >> tableSubset[1 << i][j];
			}
			sort(tableSubset[1 << i].begin(), tableSubset[1 << i].end());
		}

		for (int i = 0; i < (1 << t); ++i) {
			if (ones[i] > 1) {
				tableSubset[i].clear();
				for (int j = 0; j < t; ++j) {
					if (i & (1 << j))
						for (auto &elem : tableSubset[1 << j]) {
							tableSubset[i].pb(elem);
						}
				}
			}
			sort(tableSubset[i].begin(), tableSubset[i].end());
		}
		cin >> k;
		int nk = n * k;
		for (int i = 1; i <= m; ++i) {
			for (int mask = 0; mask < (1 << t); ++mask) {
				if (sz(tableSubset[mask]) > nk) {
					groupCost[i][mask] = -100000000000000000LL;
					continue;
				}
				groupCost[i][mask] = 0;
				int p = sz(tableSubset[mask]) - 1, j = n - 1, deg = k;
				while (p >= 0) {
					if (!deg)
						--j, deg = k;
					groupCost[i][mask] += tableSubset[mask][p--] * 1LL * charm[i][j];
					if (!deg)
						--j, deg = k;
					--deg;
				}
			}
		}

		for (int mask = 1; mask < (1 << t); ++mask) {
			dp[0][mask] = -100000000000000000LL;
		}

		for (int i = 1; i <= m; ++i) {
			for (int mask = 1; mask < (1 << t); ++mask) {
				dp[i][mask] = -100000000000000000LL;
				for (int mask2 = mask;; mask2 = (mask2 - 1) & mask) {
					dp[i][mask] = max(dp[i][mask], groupCost[i][mask2] + dp[i - 1][mask ^ mask2]);
					if (!mask2)
						break;
				}
			}
		}
		dp[m][(1 << t) - 1] = max(dp[m][(1 << t) - 1], -1LL);
		cout << dp[m][(1 << t) - 1] << '\n';
	}


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
