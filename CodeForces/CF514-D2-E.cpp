/*
 * Let ans[x] be the number of nodes with distance = x,
 * we note that ans[x] = sum of ans[x-i]*freq[i] for i in [1, 100]
 * where freq[i] = number of times the element i occurred in d
 * and we also note that sumAns[x] = sum of ans[i] for i in [0, x]
 * 								   = sumAns[x - 1] + ans[x]
 * We can model these recurrences as a matrix M where:
 * |ansSum[x]	 |        	  |ansSum[x-1]  |
 * |ans[x]	 |		  |ans[x-1]  	|
 * |    .	 |	= M * 	  |    .	|
 * |    .	 |		  |    .	|
 * |ans[x-99]	 | 		  ||ans[x-100]	|
 *
 * Then we can use matrix exponentiation to obtain the answer
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

const int MOD = 1000000007;
typedef vector<int> row;
typedef vector<int> col;
typedef vector<row> matrix;

matrix zero(int n, int m) {
	return matrix(n, row(m, 0));
}

matrix identity(int n) {
	matrix ret = zero(n, n);
	for (int i = 0; i < sz(ret); ++i)
		ret[i][i] = 1;
	return ret;
}

matrix multiply(const matrix& a,const matrix&b) {
	matrix ret = zero(a.size(), b[0].size());
	for (int i = 0; i < sz(a); ++i)
		for (int k = 0; k < sz(a[0]); ++k)
			for (int j = 0; j < sz(b[0]); ++j)
				ret[i][j] = (ret[i][j] + a[i][k] * 1LL * b[k][j]) % MOD;
	return ret;
}

matrix matPower(const matrix& a, ll k) {
	if (!k)
		return identity(sz(a));
	if (k % 2 == 1)
		return multiply(a, matPower(a, k - 1));
	return matPower(multiply(a, a), k / 2);
}

int n, x;
vi freq(101), ans(101);

int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> x;

	for (int i = 1; i <= n; ++i) {
		int di;
		cin >> di;
		++freq[di];
	}

	ans[0] = 1;

	for (int i = 1; i <= 100; ++i) {
		for (int j = 0; j < i; ++j) {
			ans[i] = (ans[i] + freq[i - j] * 1LL * ans[j]) % MOD;
		}
	}

	db(ans);

	if (x <= 100) {

		ll ret = 0;

		for (int i = 0; i <= x; ++i)
			ret = (ret + ans[i]) % MOD;

		cout << ret << '\n';
		return 0;
	}

	matrix m(101, row(1, 0));

	int s = 1;

	for (int i = 1; i <= 100; ++i) {
		m[101 - i][0] = ans[i];
		s = (s + ans[i]) % MOD;
	}

	m[0][0] = s;

	matrix M(101, row(101, 0));

	M[0][0] = 1;

	for (int j = 1; j <= 100; ++j)
		M[0][j] = M[1][j] = freq[j];

	for (int i = 2; i <= 100; ++i)
		M[i][i - 1] = 1;

	M = matPower(M, x - 100);

	cout << multiply(M, m)[0][0] << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
