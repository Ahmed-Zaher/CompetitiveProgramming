/*
 * We want distinct subsequences, normally we can have a subsequence with the same values and same relative order
 * but the indices in S differ. We can enforce a constraint on the way we pick subsequences to make every subsequence
 * appear at most once. One we can do that is to only pick subsequences s.t. if the last number is x, we take
 * the last x in the array, then if the second to last number is y, we take the last y before x and so on..
 * Now we can solve this problem using dp, let's first compress values of S to have numbers in [0, N)
 * Let dp(i, len) be the number of distinct subsequences of length len in S[0..i] (0-based) where the last element is S[i]
 * Then we have the recurrence:
 *      dp(i, len) = sum of j in [0, S[i]) of dp(f(j), len - 1) where f(j) is the last index before i where j appears
 * We can do this computation efficiently by fixing len, then for i in [0, N), we maintain a BIT that holds values
 * of dp( . , len - 1) for last occurrences of elements we encountered so far. And we update it as we go, and our answer
 * will be just a prefix sum query to the BIT.
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
#else
#define db(...) true
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

const int MX_N = 10005, MX_K = 55, MOD = 5000000;
int N, K, dp[MX_N][MX_K];
bool seen[MX_N];
mii numToIdx;
vi S, idxToNum;

struct FenwickTree {
	vector<int> bit;
	int n, LOGN;
	void init(int _n) {
		n = _n;
		LOGN = log2(n) + 1;
		bit.assign(n + 1, 0);
	}
	FenwickTree(int _n) {
		init(_n);
	}
	int sum(int idx) {
		int ret = 0;
		for (; idx > 0; idx -= idx & -idx)
			ret = (ret + bit[idx] + MOD) % MOD;
		return ret;
	}
	int sum(int l, int r) {
		return (sum(r) - sum(l - 1) + MOD) % MOD;
	}
	void add(int idx, int delta) {
		for (; idx <= n; idx += idx & -idx)
			bit[idx] = (bit[idx] + delta + MOD) % MOD;
	}
};


int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> N >> K;

	vi S(N);

	si s;

	for (int i = 0; i < N; ++i) {
		cin >> S[i];
		s.insert(S[i]);
	}

	for (auto &elem : s) {
		numToIdx[elem] = sz(idxToNum);
		idxToNum.pb(elem);
	}

	for (int i = 0; i < N; ++i)
		S[i] = numToIdx[S[i]];

	ll ans = 0;

	for (int i = 0; i < N; ++i)
		dp[i][1] = 1;


	for (int len = 2; len <= K; ++len) {
		FenwickTree ft(N + 1);
		for (int i = 0; i < N; ++i) {
			if (S[i])
				dp[i][len] = ft.sum(1, S[i]);
			int val = ft.sum(S[i] + 1, S[i] + 1);
			ft.add(S[i] + 1, (-val + MOD) % MOD);
			ft.add(S[i] + 1, dp[i][len - 1]);
		}
	}

	for (int i = N - 1; i >= 0; --i) {
		if (!seen[S[i]])
			ans = (ans + dp[i][K]) % MOD;
		seen[S[i]] = 1;
	}

	cout << ans << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
