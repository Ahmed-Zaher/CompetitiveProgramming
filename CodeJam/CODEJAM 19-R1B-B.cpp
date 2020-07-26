/*
 * This is the solution for the case where W = 2
 * Note on day i our sum is sum of Rj * 2^(i / j) for j in [1, 6]
 * so it's a good idea to think in terms of binary representation
 * Note that R1 grows faster than R2 and R2 grows faster than R3 and so on..
 * First note that Ri <= 100, which fits in 7 bits so after day 0, the contribution
 * of each Ri occupies only bits at bit positions [0, 6]
 * After day x, Ri occupies bits at bit positions [x / i, x / i + 6]
 * So at certain days we find that some Ri's don't cause conflict with each other,
 * as they occupy a range of bit positions that no other Rj occupied, so in that case
 * we can just shift those bits and get the value for the corresponding Ri
 * Now the rest is some analysis on how the ranges of bit positions for each Ri behave
 * in the first 500 days, which is done in the commented code below
 * We find that we can a query on day 185 and another on day 54
 * The 185 query will tell us R4, R5, R6, and the leftmost 2 bits of R3
 * The 54 query has conflict between R3 and other R's, but not anymore if we assume that
 * the left most 2 bits of R3 are 0 (we will do that using our knowledge about R3 in query 185)
 * So from query 54 we can get the raining bits of R4 and get R1 and R2 as well
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





int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	/*
	vi shift(7, 1);

	int x = 100 * ((1 << 9) + (1 << 10) + (1 << 13));


	for (int i = 0; i <= 500; ++i) {
		cout << setw(5) << i << ": ";
		vi vals;
		for (int j = 1; j <= 6; ++j) {
			cout << setw(5) << i / j;
			vals.pb(i / j);
		}
		cout << " --- ";
		for (int i = 1; i < 6; ++i) {
			cout << vals[i - 1] - vals[i] << ' ';
		}ndl;

	}
	 */


	int T, W;

	cin >> T >> W;

	int filter = (1 << 7) - 1;

	for (int tc = 1; tc <= T; ++tc) {
		long long q185;
		int R[7];
		for (int i = 1; i <= 6; ++i)
			R[i] = 0;
		cout << 185 << endl;
		cin >> q185;
		assert(q185 != -1);
		R[3] = q185 >> 61;
		R[4] = (q185 >> 46) & filter;
		R[5] = (q185 >> 37) & filter;
		R[6] = (q185 >> 30) & filter;
		long long q54;
		cout << 54 << endl;
		cin >> q54;
		assert(q54 != -1);
		q54 -= (R[3] << 18LL);
		R[3] += ((q54 >> 18) ^ ((q54 >> 18) & 3)) & filter;
		R[1] = q54 >> 54;
		R[2] = (q54 >> 27) & filter;

		for (int i = 1; i <= 5; ++i)
			cout << R[i] << " ";

		cout << R[6] << endl;

		int good;

		cin >> good;

		if (good == -1)
			exit(0);

	}

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
