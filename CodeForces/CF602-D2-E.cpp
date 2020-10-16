/*
 * An outcome of this process is described by n permutations of length m-1
 * which describe for each competition, which of the m-1 participants get which score
 * All of these outcomes are equiprobable, we can compute the answer naively using the following
 * pseudocode:
 * ans = 0
 * for each outcome:
 *      rank = 1
 *      for each participant:
 *          if participant gets score less than sum of x's given in input:
 *              ++rank
 *      ans += rank
 *  return ans / # of outcomes
 *  Using the fact that all outcomes are equiprobable, and that the other m-1 are indistinguishable,
 *  this pseudocode can be reduced to:
 *  ans = 0
 *  for each outcome for a fixed participant:
 *      if that participant gets score less than sum of x's:
 *          ++ans
 *  return (ans / #outcomes) * (m - 1) + 1
 *  We see that in this case, (ans / #outcomes) is the probability that a certain participant
 *  gets a score less than sum of x's, and this can be computed using naive dp in O(n^2*m^2),
 *  which can be reduced to O(n^2*m) using tabulation and maintaining suffix sums of computed values
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
	while (*dbStr != ',') cout << *dbStr++;
	cout << " = " << e << ',';
	_db(dbStr + 1, r...);
}

template<class S, class T>
ostream &operator<<(ostream &o, const map<S, T> &v) {
	o << "[";
	int i = 0;
	for (const pair<S, T> &pr : v)
		o << (!i++ ? "" : ", ") << "{"
		  << pr.fs << " : " << pr.sc << "}";
	return o << "]";
}

template<template<class, class...> class S, class T, class... L>
ostream &operator<<(ostream &o, const S<T, L...> &v) {
	o << "[";
	int i = 0;
	for (const auto &e : v) o << (!i++ ? "" : ", ") << e;
	return o << "]";
}

template<class S, class T>
ostream &operator<<(ostream &o, const pair<S, T> &pr) {
	return o << "(" << pr.fs << ", " << pr.sc << ")";
}

ostream &operator<<(ostream &o, const string &s) {
	for (const char &c : s) o << c;
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

double mem[105][100005], sumSuff[105][100006];
int n, m, x[105], tot;

double dp(int i, int sum) {
	if (i == n)
		return sum < tot;
	double& ret = mem[i][sum];
	if (ret > -1)
		return ret;
	ret = 0;
	for (int j = 1; j <= m; ++j) {
		if (j != x[i])
			ret += dp(i + 1, sum + j);
	}
	ret /= m - 1;
	return ret;
}

int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(25);
	cin.tie(0);

	cin >> n >> m;

	for (int i = 0; i < n; ++i) {
		cin >> x[i];
		tot += x[i];
	}

	if (n == 1) {
		cout << x[0] << '\n';
		return 0;
	}

	if (m == 1) {
		cout << "1\n";
		return 0;
	}

	for (int i = 100004; i >= 0; --i) {
		mem[n][i] = i < tot;
		sumSuff[n][i] = mem[n][i] + sumSuff[n][i + 1];
	}

	for (int i = n - 1; i >= 0; --i) {
		for (int sum = 100004; sum >= 0; --sum) {
			mem[i][sum] = sumSuff[i + 1][sum + 1];
			if (sum + m + 1 < 100005)
				mem[i][sum] -= sumSuff[i + 1][sum + m + 1];
			if (sum + x[i] < 100005)
				mem[i][sum] -= mem[i + 1][sum + x[i]];
			mem[i][sum] /= m - 1;
			sumSuff[i][sum] = mem[i][sum] + sumSuff[i][sum + 1];
		}
	}

	cout << fixed << mem[0][0] * (m - 1) + 1 << '\n';


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
