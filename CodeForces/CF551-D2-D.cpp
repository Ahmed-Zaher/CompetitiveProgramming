/*
 * Let's first catch some special cases: if l = 0 then ai can only be
 * 0, hence the answer is 1 if k = 0 and 0 otherwise.
 * If k has more than l bits, then we can't find any good ai, hence the answer is 0.
 * Now assume l > 0 and k < 2^l: note that bits can be treated independently
 * Let g(i, n) be the number of sequences of bits b1, b2, .., bn
 * s.t. (b1 & b2) | (b2 & b3) | ... | (bn-1 & b1) = i
 * Clearly the answer is product of g([is j'th bit on in k], n) for all j.
 * Let's compute g(0, n): if our sequence has no two consecutive ones then it's a good sequence
 * we can come up for a recurrence for that, note that last bit can always be 0. If, however, it is
 * one then we need to make the one to its left a 0.
 * And so we have the recurrence g(0, n) = g(0, n - 1) + g(0, n - 2), g(0, 0) = 1, g(0, 1) = 2
 * Which is the same recurrence as Fibonacci but with different base cases. We can compute g(0, n)
 * for any given n in O(logn) using matrix power.
 * To compute g(1, n), we note that g(1, n) has all the sequences where we have at least two consecutive
 * bits that are equal to 1, which is the complement of g(0, n). And hence g(1, n) = 2^n - g(0, n).
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

ll n, k;
int l, MOD;

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
			for (int j = 0; j < sz(b[0]); ++j) {
				ret[i][j] = (ret [i][j] + (a[i][k] * 1LL * b[k][j]) % MOD) % MOD;
			}
	return ret;
}

matrix matPower(const matrix& a, ll k) {
	if (!k)
		return identity(sz(a));
	if (k % 2 == 1)
		return multiply(a, matPower(a, k - 1));
	return matPower(multiply(a, a), k / 2);
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

int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);


	cin >> n >> k >> l >> MOD;

	if (!l) {
		cout << (k == 0) % MOD << '\n';
		return 0;
	}

	if (l < 64 && (unsigned long long) k >= (1ULL << l)) {
		cout << 0 << '\n';
		return 0;
	}

	int A = 0, B = 0;

	for (int i = 0; i < l; ++i)
		if (k & (1ULL << i))
			++B;
		else
			++A;

	matrix M(2, row(2, 1));

	M[1][1] = 0;

	matrix m(2, row(1, 1));
	m[0][0] = 2;

	matrix g(2, row(1, 0));

	g = multiply(matPower(M, n - 1), m);

	int G = g[0][0];

	int X = power(G, A), Y = power((power(2, n) - G + MOD) % MOD, B);

	cout << (X * 1LL * Y) % MOD << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
