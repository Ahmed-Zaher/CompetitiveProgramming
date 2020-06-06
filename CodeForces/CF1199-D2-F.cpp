/*
 * The main idea is that when we are solving the problem for a rectangle
 * of size h x w, then we can take the whole rectangle with cost max(h, w)
 * If we wish to get a lower cost, then there must exist either one row that is
 * not used in any rectangle (call that 'uncovered') or one uncovered column.
 * This is simply because the answer is bounded from below by
 * max(#rows covered, #columns covered), and if it's not the case that there's
 * at least one row/column uncovered, then we get that lower bound by simply taking
 * the whole rectangle. We can use dynamic programming to solve the problem for every
 * sub-rectangle in the grid, and try all possible uncovered row/column.
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

const int MX = 55;
int mem[MX][MX][MX][MX];
bool grid[MX][MX];


int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int n;

	cin >> n;

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			char c;
			cin >> c;
			if (c == '#')
				grid[i][j] = 1;
		}
	}

	for (short l1 = 1; l1 <= n; ++l1) {
		for (short l2 = 1; l2 <= n; ++l2) {
			for (short r1 = 1; r1 <= n - l1 + 1; ++r1) {
				for (short c1 = 1; c1 <= n - l2 + 1; ++c1) {
					short r2 = r1 + l1 - 1, c2 = c1 + l2 - 1;
					if (r1 == r2 && c1 == c2)
						mem[r1][c1][r2][c2] = grid[r1][c1];
					else {
						mem[r1][c1][r2][c2] = 1 + max(r2 - r1, c2 - c1);
						for (int r3 = r1; r3 < r2; ++r3)
							mem[r1][c1][r2][c2] = min(mem[r1][c1][r2][c2], mem[r1][c1][r3][c2] + mem[r3 + 1][c1][r2][c2]);
						for (int c3 = c1; c3 < c2; ++c3)
							mem[r1][c1][r2][c2] = min(mem[r1][c1][r2][c2], mem[r1][c1][r2][c3] + mem[r1][c3 + 1][r2][c2]);
					}
				}
			}
		}
	}

	cout << mem[1][1][n][n] << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
