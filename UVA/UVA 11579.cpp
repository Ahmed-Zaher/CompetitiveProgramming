/*
 * Let's sort s in increasing order, the answer 3 sides with indices i, j, k
 * Assume that i > j > k, we note that if i is fixed, the maximal triangle
 * area is achieved when j = i - 1, j = i - 2, to see why, imagine a segment
 * of length si, and 2 circle, one centered at each point of the segment,
 * one of the circles has radius sj, the other has radius sk. The resulting triangle
 * will have the its third vertex at the intersection of the 2 circle.
 * Also, area = 1/2 * si * height, that height can only decrease by decreasing sj or sk
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
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(2);
	cin.tie(0);

	cout << fixed;

	int T;

	cin >> T;

	while (T--) {

		int n;

		cin >> n;

		V<double> s(n);

		for (int i = 0; i < n; ++i) {
			cin >> s[i];
		}

		sort(s.begin(), s.end());

		double ans = 0;

		for (int i = 2; i < n; ++i) {
			double si = s[i], sj = s[i - 1], sk = s[i - 2];
			if (sj + sk < si)
				continue;

			double x = (sk * sk + si * si - sj * sj) / (2 * si);
			double h = sqrt(sk * sk - x * x);

			ans = max(ans, 0.5 * si * h);

		}

		cout << ans << '\n';

	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
