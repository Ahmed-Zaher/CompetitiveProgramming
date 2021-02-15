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

const int MX = 12;
int n, m, N;
int best[MX][1 << MX], mem[MX][1 << MX];

int dp(int i, int mask) {
//	db(i, mask);
	if (i == N)
		return !mask ? 0 : -OO;
	int& ret = mem[i][mask];
	if (ret != -1)
		return ret;
	ret = 0;
	for (int subMask = mask;; subMask = (subMask - 1) & mask) {
		ret = max(ret, best[i][subMask] + dp(i + 1, mask ^ subMask));
		if (!subMask)
			break;
	}
	return ret;
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

	int T;

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		cin >> n >> m;
		vvi grid(m, vi(n));
		vi mx(m);
		for (int j = 0; j < n; ++j) {
			for (int i = 0; i < m; ++i) {
				cin >> grid[i][j];
				mx[i] = max(mx[i], grid[i][j]);
			}
		}
		db(grid);
		vi p(m);
		for (int i = 0; i < m; ++i) {
			p[i] = i;
		}
		sort(p.begin(), p.end(), [&](int i, int j) { return mx[i] > mx[j]; });
		auto gridTmp = grid;
		for (int i = 0; i < m; ++i) {
			grid[i] = gridTmp[p[i]];
		}

		N = min(n, m);

		db(grid);
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < (1 << n); ++j) {
				mem[i][j] = -1;
				best[i][j] = 0;
				for (int k = 0; k < n; ++k) {
					if (j & (1 << k))
						best[i][j] += grid[i][k];
				}
			}
		}
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < (1 << n); ++j) {
				for (int k = 0; k < n; ++k) {
					best[i][j] = max(best[i][j], best[i][((j >> k) | (j << (n - k))) & ((1 << n) - 1)]);
				}
			}
		}

		db(best[0][(1 << n) - 1]);


		cout << dp(0, (1 << n) - 1) << '\n';
	}

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
