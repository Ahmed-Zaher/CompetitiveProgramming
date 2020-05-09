/*
 * We can solve the problem if we can answer efficiently a sum(x, y)
 * which returns the sum of the grid bound by (1, 1) and (x, y)
 * Let's first precompute sum(x, y) for all x <= 2n, y <= 2m.
 * Note that after i'th step of building the grid we get a new grid of
 * size (2^i n) x (2^i m), for the given x, y we can find the smallest grid
 * which includes that cell. And now divide that grid into its 4 parts, clearly
 * (x, y) can't be in the top left part, we will compute the sum of the top left
 * part by noticing that the sum of any row at a grid after the 1st step equals
 * half the length of that row, and same for columns.
 * Then we solve the problem with (x`, y`) which are the coordinates of (x, y)
 * with respect to the part (x, y) lies in, not the top left part.
 * If (x, y) is in the bottom right part then we add the answer to that new subproblem,
 * if not, then that means we counted the ones of a flipped grid, meaning that we need
 * to take the difference between that and the grid area to get the count of ones in
 * the original grid.
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


int n, m, q;
vvi naive;
V<pair<ll, ll>> sizes;

ll solve(int x, int y) {
	if (x <= 2 * n && y <= 2 * m)
		return naive[x][y];
	int lo = 0, hi = sz(sizes) - 1;

	while (lo < hi) {
		int mid = (lo + hi) >> 1;
		if (x <= sizes[mid].fs && y <= sizes[mid].sc)
			hi = mid;
		else
			lo = mid + 1;
	}
	int N = sizes[lo].fs / 2, M = sizes[lo].sc / 2;


	ll ret = (x > N ? y * 1LL * N / 2 : 0) + (y > M ? x * 1LL * M / 2 : 0) - (x > N && y > M ? N * 1LL * M / 2 : 0);
	int to_x = x - (x > N) * N, to_y = y - (y > M) * M;
	ll add = solve(to_x, to_y);
	if (x > N && y > M)
		ret += add;
	else
		ret += to_x * 1LL * to_y - add;
	return ret;
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

	cin >> n >> m >> q;

	naive.assign(2 * n + 1, vi(2 * m + 1, 0));

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			char c;
			cin >> c;
			naive[i][j] = c - '0';
			naive[i + n][j + m] = naive[i][j];
			naive[i][j + m] = naive[i + n][j] = !naive[i][j];
		}
	}

	for (int i = 1; i <= 2 * n; ++i) {
		db(naive[i]);
	}

	for (int i = 1; i <= 2 * n; ++i)
		for (int j = 1; j <= 2 * m; ++j)
			naive[i][j] += naive[i][j - 1] + naive[i - 1][j] - naive[i - 1][j - 1];

	sizes.pb(mp(n, m));

	while (min(sizes.back().fs, sizes.back().sc) < 1000000000)
		sizes.pb(mp(sizes.back().fs * 2, sizes.back().sc * 2));


	for (int i = 1; i <= q; ++i) {
		int a, b, c, d;
		cin >> a >> b >> c >> d;
		cout << solve(c, d) - solve(c, b - 1) - solve(a - 1, d) + solve(a - 1, b - 1) << '\n';
	}


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
