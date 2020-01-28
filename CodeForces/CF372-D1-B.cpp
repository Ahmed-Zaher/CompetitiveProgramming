/*
 * The constraints allow for up to O(n^5) solution
 * Let f[i][j][b][c] be the number of rectangles lying within
 * rows b, i and columns i, c, and have top right corner at (i, j)
 * we can compute f in O(n^4)
 *
 * Let dp[a][b][c][d] be the answer to a query with the
 * corresponding parameters
 *
 * We note that dp[a][b][c][d] has exactly all the rectangles
 * in dp[a][b][c][d - 1] plus the rectangles lying in the query range
 * and have top right corner at (i, d) for all i s.t. a <= i <= c
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


const int MX = 41;
ll dp[MX][MX][MX][MX], f[MX][MX][MX][MX], sumR[MX][MX][MX];
char grid[MX][MX];


int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int n, m, q;

	cin >> n >> m >> q;

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			cin >> grid[i][j];
		}
	}

	for (int i = 1; i <= n; ++i) {
		for (int l = 1; l <= m; ++l) {
			for (int r = l; r <= m; ++r) {
				for (int j = l; j <= r; ++j) {
					sumR[i][l][r] += (grid[i][j] - '0');
				}
			}
		}
	}

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			for (int b = j; b >= 1; --b) {
				int sum = 0;
				for (int c = i; c <= n; ++c) {
					sum += sumR[c][b][j];
					if (b + 1 <= j)
						f[i][j][b][c] += f[i][j][b + 1][c];
					if (c - 1 >= i)
						f[i][j][b][c] += f[i][j][b][c - 1];
					if (b + 1 <= j && c - 1 >= i)
						f[i][j][b][c] -= f[i][j][b + 1][c - 1];
					f[i][j][b][c] += (sum == 0);
				}
			}
		}
	}

	for (int a = 1; a <= n; ++a) {
		for (int c = a; c <= n; ++c) {
			for (int b = 1; b <= m; ++b) {
				for (int d = b; d <= m; ++d) {
					for (int i = a; i <= c; ++i) {
						dp[a][b][c][d] += f[i][d][b][c];
					}
					if (d > b)
						dp[a][b][c][d] += dp[a][b][c][d - 1];
				}
			}
		}
	}

	for (int i = 1; i <= q; ++i) {
		int a, b, c, d;

		cin >> a >> b >> c >> d;

		cout << dp[a][b][c][d] << '\n';
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
