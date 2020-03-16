/*
 * We note that it is necessary to have cells (r1, c2) and (r2, c2) have
 * the same parity if r1 % n = r2 % n and c1 % m = c2 % m
 * Since n and m are small, we can use dp with masks to simulate the process
 * and get the count
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

const int MX = 11, MOD = 1000000007;
int mem[11][1 << 11], cnt[MX][MX], zeros[MX][MX], ones[MX][MX], n, m;

ll add(ll x, ll y) {
    x += y;
    while(x >= MOD)
        x -= MOD;
    while(x < 0)
        x += MOD;
    return x;
}

ll mul(ll x, ll y) {
    return (x * y) % MOD;
}

ll power(ll num, ll exp) {
	ll ret = 1;
	while (exp) {
		if (exp & 1)
			ret = (ret * num) % MOD;
		num = (num * num) % MOD;
		exp >>= 1;
	}
	return ret;
}

int dp(int r, int mask) {
	if (r == n)
		return mask == (1 << m) - 1;
	int& ret = mem[r][mask];
	if (ret != -1)
		return ret;

	ret = 0;

	for (int row = 0; row < (1 << m); ++row) {
		int ways = 1, bitCnt = 0;
		for (int j = 0; j < m; ++j) {
			int bit = (row >> j) & 1;
			if ((bit && zeros[r][j]) || (!bit && ones[r][j]))
				ways = 0;
			bitCnt += bit;
			if (bit)
				ways = mul(ways, power(5, cnt[r][j] - ones[r][j]));
			else
				ways = mul(ways, power(4, cnt[r][j] - zeros[r][j]));
		}
		if (ways && (bitCnt & 1)) {
			int toAdd = mul(ways, dp(r + 1, mask ^ row));
			ret = add(ret, toAdd);
		}
	}

	return ret;
}


struct MagicalGirlLevelTwoDivOne {
	int theCount(vector<string> palette, int n, int m) {
		::n = n;
		::m = m;

		int H = sz(palette), W = sz(palette[0]);

		for (int i = 0; i < H; ++i) {
			for (int j = 0; j < W; ++j) {
				++cnt[i % n][j % m];
				if (palette[i][j] != '.') {
					int d = palette[i][j] - '0';
					if (d & 1)
						++ones[i % n][j % m];
					else
						++zeros[i % n][j % m];
				}
			}
		}

		if (n == 1 || m == 1) {
			if (n % 2 == 0 || m % 2 == 0)
				return 0;
			int totZeros = 0, totOnes = 0;
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < m; ++j) {
					totZeros += zeros[i][j];
					totOnes += ones[i][j];
				}
			}
			if (totZeros)
				return 0;
			return power(5, H * W - totOnes);
		}

		memset(mem, -1, sizeof(mem));

		return dp(0, 0);

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

	MagicalGirlLevelTwoDivOne x;

	cout << x.theCount(
	   {"844.4........",
		".8..3.8.8...5",
		".......8.....",
		"42...23....51",
		"98.2...4.....",
		"....54....72.",
		"83.....6.6.1.",
		"4266.........",
		".4.95...63...",
		"7......411.77",
		"6.934...62...",
		".8....26.562.",
		".5....2...385",
		".5.9.96....7.",
		"....2.3...2..",
		"....7....5...",
		"1.....9.9..5.",
		"8...48.8.....",
		"144...48...2."}, 7, 9);
//	cout << x.theCount({"12", "2."},
//		2,
//		2);


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
