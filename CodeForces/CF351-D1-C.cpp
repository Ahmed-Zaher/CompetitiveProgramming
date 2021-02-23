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


typedef vector<ll> row;
typedef vector<ll> col;
typedef vector<row> matrix;

matrix zero(int n, int m) {
	return matrix(n, row(m, 0));
}

matrix identity(int n) {
	matrix ret = matrix(n, row(n, 1e18));
	for (int i = 0; i < n; ++i) {
		ret[i][i] = 0;
	}
	return ret;
}

matrix multiply(const matrix& a,const matrix&b) {
	matrix ret = matrix(a.size(), row(b[0].size(), 1e18));
	db(a, b);
	for (int i = 0; i < sz(a); ++i)
		for (int k = 0; k < sz(a[0]); ++k)
			for (int j = 0; j < sz(b[0]); ++j)
				ret[i][j] = min(ret[i][j], a[i][k] + b[k][j]);
	db(ret);
	return ret;
}

matrix matPower(const matrix& a, ll k) {
	if (!k)
		return identity(sz(a));
	if (k % 2 == 1)
		return multiply(a, matPower(a, k - 1));
	return matPower(multiply(a, a), k / 2);
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

	int n, m;
	cin >> n >> m;
	vi a(n), b(n);

	for (int i = 0; i < n; ++i) {
		cin >> a[i];
	}

	for (int i = 0; i < n; ++i) {
		cin >> b[i];
	}

	matrix G(2 * n + 1, row(2 * n + 1, 1e18));

	for (int m = 0; m < (1 << n); ++m) {
		int cost = 0, mn = 0, bal = 0;
		for (int i = 0; i < n; ++i) {
			if (m & (1 << i))
				cost += b[i], --bal;
			else
				cost += a[i], ++bal;
			mn = min(mn, bal);
		}
		for (int j = -mn; j <= 2 * n && j + bal <= 2 * n; ++j) {
			G[j][j + bal] = min(G[j][j + bal], cost * 1LL);
		}
	}


	cout << matPower(G, m)[0][0] << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
