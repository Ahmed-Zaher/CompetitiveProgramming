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


const int MX = 8;
int n, m, board[MX + 1], mem[MX + 1][1 << MX][1 << MX], ones[1 << MX], f[1 << MX], allOnes;

int dp(int i, int curRow, int prevRow) {
	if (i == n)
		return prevRow == allOnes ? 0 : 1000;
	int& ret = mem[i][curRow][prevRow];
	if (ret != -1)
		return ret;
	ret = OO;
	for (int mask = 0; mask < (1 << m); ++mask) {
		if (!i || (prevRow ^ f[mask]) == allOnes)
			ret = min(ret, ones[mask] + dp(i + 1, board[i + 1] ^ f[mask], curRow ^ f[mask]));
	}
	return ret;
}

struct LightedPanels {
	int minTouch(V<string> in) {
		n = sz(in);
		m = sz(in[0]);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (in[i][j] == '*')
					board[i] |= 1 << j;
			}
		}
		allOnes = (1 << m) - 1;
		for (int i = 0; i < (1 << m); ++i) {
			ones[i] = __builtin_popcount(i);
			for (int j = 0; j < m; ++j) {
				if (i & (1 << j)) {
					f[i] ^= (1 << j);
					if (j < m - 1)
						f[i] ^= (1 << (j + 1));
					if (j)
						f[i] ^= (1 << (j - 1));
				}
			}
		}
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < (1 << m); ++j) {
				for (int k = 0; k < (1 << m); ++k) {
					mem[i][j][k] = -1;
				}
			}
		}
		int ret = dp(0, board[0], 0);
		if (ret > 100)
			ret = -1;
		return ret;
	}
};

int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
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
