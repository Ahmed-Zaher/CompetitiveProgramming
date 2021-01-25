/*
 * The answer is computed using D&C dp optimization in O(n*k*logn), another value that is computer is f[L][R]
 * which is the min cost to move all the gold in mines L..R into exactly one of the mines in that range.
 * Here it is computed in O(n^2) using Knuth dp optimization.
 * This solution gets TLE due to the tight time constraints on the problem, but runs in < 5sec on maximal cases.
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

const int MX = 5005;
int n, k, w[MX], x[MX];
ll f[MX][MX], optFIdx[MX][MX], sumDist[MX][MX], dp[MX][MX];

void computeRow(int row, int L, int R, int optL, int optR) {
	if (L > R)
		return;
	int mid = (L + R) / 2;
	int midOpt;
	ll& ret = dp[row][mid];
	ret = 1e18;
	for (int i = optL; i <= min(optR, mid - 1); ++i) {
//		db(i, f[i + 1][mid] + dp[row - 1][i]);
		if (f[i + 1][mid] + dp[row - 1][i] < ret) {
			ret = f[i + 1][mid] + dp[row - 1][i];
			midOpt = i;
		}
	}
//	db(row, L, R, optL, optR, mid, midOpt);
//	db(ret, midOpt);
	computeRow(row, L, mid - 1, optL, midOpt);
	computeRow(row, mid + 1, R, midOpt, optR);
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

	cin >> n >> k;

	for (int i = 1; i <= n; ++i)
		cin >> x[i] >> w[i];

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			sumDist[i][j] = abs(x[i] - x[j]) * 1LL * w[j] + sumDist[i][j - 1];
//			db(i, j, dist[i][j]);
		}
	}



	for (int i = 1; i <= n; ++i)
		f[i][i] = 0, optFIdx[i][i] = i;

	for (int len = 2; len <= n; ++len)
		for (int st = 1; st <= n - len + 1; ++st) {
			int en = st + len - 1;
			ll&ret = f[st][en];
			ret = 1e18;
			int bestLIdx = optFIdx[st][en - 1];
			int bestRIdx = optFIdx[st + 1][en];
			for (int mid = bestLIdx; mid <= bestRIdx; ++mid)
				if (sumDist[mid][en] - sumDist[mid][st - 1] < ret)
					ret = sumDist[mid][en] - sumDist[mid][st - 1], optFIdx[st][en] = mid;
		}


	for (int i = 0; i <= n; ++i) {
		for (int j = 0; j <= n; ++j) {
			if (j < i || (!i && j))
				dp[i][j] = 1e18;
		}
	}

//	for (int i = 1; i <= n; ++i) {
//		for (int j = 1; j <= n; ++j) {
//			cout << dp[i][j] << ' ';
//		}ndl;
//	}ndl;
	for (int row = 1; row <= n - 1; ++row) {
		computeRow(row, row + 1, n, 0, n - 1);
	}


//	for (int i = 0; i <= n; ++i) {
//		for (int j = 0; j <= n; ++j) {
//			cout << dp[i][j] << ' ';
//		}ndl;
//	}ndl;

	cout << dp[k][n] << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
