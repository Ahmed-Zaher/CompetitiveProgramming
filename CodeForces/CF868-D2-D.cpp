/*
 * Let's say that a string is k-good if it has all substrings of size k with value 0 to 2^k-1.
 * For a string to be k-good, its length needs to be at least 2^k & have 2^k different substrings
 * Also, a k-good string must be (k-1)-good string. We will show that it's not possible to get a 15-good
 * string and hence we can use brute force.
 * The important observation is that when we concatenate two strings a and b, we get substrings of the first
 * and those of the second, and at most k new substrings which come from the end of a and beginning of b.
 * And those overlapping substrings won't contribute in later concatenation because they are buried way
 * in the middle of the string. If all our strings are of length > 100 (we can use naive solution for smaller strings),
 * then after m <= 100 operations, each string will have at most n*m + k*m <= 100^2 + 15*100 << 2^k different substrings
 * Therefore it's not possible to get a 15-good string, and our answer is a number in [0, 15)
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
using ull = unsigned long long;
using pii = pair<int, int>;
using vi = V<int>;
using vii = V<pii>;
using vvi = VV<int>;
using mii = map<int, int>;
using umii = unordered_map<int, int>;
using si = set<int>;
using usi = unordered_set<int>;

const int MX_K = 15;
int n, m;
V<string> s;
bool good[205][MX_K][1 << MX_K];

int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);


	cin >> n;

	s.assign(n + 1, "");

	for (int i = 1; i <= n; ++i) {
		cin >> s[i];
		for (int k = 1; k < MX_K; ++k) {
			for (int j = 0; j <= sz(s[i]) - k; ++j) {
				int val = 0;
				for (int jj = j; jj < j + k; ++jj) {
					val <<= 1;
					val += s[i][jj] == '1';
				}
				good[i][k][val] = 1;
			}
		}
	}


	cin >> m;

	for (int i = n + 1; i <= n + m; ++i) {
		int a, b;
		cin >> a >> b;
		string c = s[a] + s[b];
		int ans = 0;
		for (int k = 1; k < MX_K; ++k) {
			for (int j = 0; j < sz(s[a]); ++j) if (j + k > sz(s[a]) && j + k <= sz(c)) {
					int val = 0;
					for (int jj = j; jj < j + k; ++jj) {
						val <<= 1;
						val += c[jj] == '1';
					}
					good[i][k][val] = 1;
			}
			bool goodK = true;
			for (int mask = 0; mask < (1 << k); ++mask) {
				good[i][k][mask] |= good[a][k][mask] | good[b][k][mask];
				if (!good[i][k][mask])
					goodK = false;
			}
			if (goodK)
				ans = k;
		}
		cout << ans << '\n';
		if (sz(c) > 100)
			c = c.substr(0, 50) + c.substr(sz(c) - 50);
		s.pb(c);
	}



#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
