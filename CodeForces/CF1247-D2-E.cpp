/*
 * Any sequence of moves can be represented as a string consisting of R(ight) and D(own)
 * Suppose we are at (x, y), and we just finished a bunch of R moves and know that the
 * next move is D. The thing to note is that after this D move, we are dealing with the
 * subgrid (x + 1, y) , (n, m), which at this point remains unchanged regardless of previous moves
 * Same applies when changing direction from D's to R's
 *
 * So we can use dp to try all possible string, the state is dp(x, y, dir) and at each state,
 * depending on current direction, we try and append a block of R's/D's to the string
 * Note that the moves from a position to in a certain direction are restricted, but
 * it is still a consecutive range so we can use partial sums of dp values.
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

const int MX = 2005, MOD = 1000000007;
char grid[MX][MX];
int n, m;
int limLR[MX][MX], limUD[MX][MX], sumRow[MX][MX], sumCol[MX][MX];
int dp[2][MX][MX], sumDpRow[2][MX][MX], sumDpCol[2][MX][MX];

int main() {
#ifdef LOCAL
	auto stTime = clock();
 	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> m;

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			cin >> grid[i][j];
		}
	}

	if (n == 1 && m == 1) {
		cout << "1\n";
		return 0;
	}

	for (int i = 1; i <= n; ++i) {
		for (int j = m; j >= 1; --j) {
			sumRow[i][j] = (grid[i][j] == 'R') + sumRow[i][j + 1];
		}
	}

	for (int j = 1; j <= m; ++j) {
		for (int i = n; i >= 1; --i) {
			sumCol[i][j] = (grid[i][j] == 'R') + sumCol[i + 1][j];
		}
	}

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			limLR[i][j] = m - sumRow[i][j + 1];
			limUD[i][j] = n - sumCol[i + 1][j];
		}
	}

	for (int i = n; i >= 1; --i) {
		for (int j = m; j >= 1; --j) {
			for (int dir = 0; dir < 2; ++dir) {
				if (i == n && j == m) {
					dp[dir][i][j] = 1;
					sumDpRow[dir][i][j] = 1;
					sumDpCol[dir][i][j] = 1;
				} else {
					if (!dir) {
						dp[dir][i][j] = (sumDpRow[dir ^ 1][i][j + 1] - sumDpRow[dir ^ 1][i][limLR[i][j] + 1] + MOD) % MOD;
					} else {
						dp[dir][i][j] = (sumDpCol[dir ^ 1][i + 1][j] - sumDpCol[dir ^ 1][limUD[i][j] + 1][j] + MOD) % MOD;
					}
					sumDpRow[dir][i][j] = (dp[dir][i][j] + sumDpRow[dir][i][j + 1]) % MOD;
					sumDpCol[dir][i][j] = (dp[dir][i][j] + sumDpCol[dir][i + 1][j]) % MOD;
				}
			}
		}
	}

	cout << (dp[0][1][1] + dp[1][1][1]) % MOD << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
