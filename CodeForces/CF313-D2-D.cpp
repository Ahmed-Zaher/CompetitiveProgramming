/*
 * Let's first for every segment [i, j] (1 <= i <= j <= n)
 * find the minimum cost to cover it and store that in dp[i][j]
 * we can do that in O(n^2) if we fill the table in order
 * of decreasing j - i
 * Then we do another dp to compute the answer, let dp_[i][j]
 * denote the minimal cost to get #holes >= k knowing that
 * we already have j holes and can only pick holes in [i, n]
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


const int MX_N = 302, MX_M = 1e5 + 2;
int n, m, k, nxt[MX_N];
ll dp[MX_N][MX_N], c[MX_N][MX_N], dp_[MX_N][MX_N];
vi ls[MX_N];
pair<pii, int> segs[MX_M];


int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);


	cin >> n >> m >> k;


	for (int i = 0; i < MX_N; ++i) {
		for (int j = 0; j < MX_N; ++j) {
			c[i][j] = dp[i][j] = 1e18;
		}
	}


	for (int i = 0; i < m; ++i) {
		int l, r, cc;
		cin >> l >> r >> cc;
		c[l][r] = min(c[l][r], cc * 1LL);
	}

	for (int len = n; len >= 1; --len) {
		for (int i = 1; i <= n - len + 1; ++i) {
			dp[i][i + len - 1] = c[i][i + len - 1];
			dp[i][i + len - 1] = min(dp[i][i + len - 1], dp[i - 1][i + len - 1]);
			dp[i][i + len - 1] = min(dp[i][i + len - 1], dp[i][i + len]);
		}
	}

	for (int i = 0; i < k; ++i) {
		dp_[n + 1][i] = 1e18;
	}

	for (int i = k; i <= n; ++i) {
		dp_[n + 1][i] = 0;
	}

	for (int pos = n; pos >= 1; --pos) {
		for (int got = 0; got < pos; ++got) {
			ll& ret = dp_[pos][got];
			ret = dp_[pos + 1][got];
			for (int nextPos = pos; nextPos <= n; ++nextPos) {
				assert(got + nextPos - pos + 1 <= nextPos);
				ret = min(ret, dp[pos][nextPos] + dp_[nextPos + 1][got + nextPos - pos + 1]);
			}
		}
	}

	if (dp_[1][0] > 1e15)
		cout << "-1\n";
	else
		cout << dp_[1][0] << '\n';



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
