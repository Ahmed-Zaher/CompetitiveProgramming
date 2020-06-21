/*
 * We can think of this problem visually, by drawing points (i, A[i])
 * on the Cartesian plan. If we pick i, j (assume i < j, A[i] < A[j]),
 * then (A[j]-A[i])/(j-i) is the slope of the line connecting the two points
 * We want to find minimum such slope
 * Now note that
 * (A[j]-A[i])/(j-i) is minimized when (j-i)/(A[j]-A[i]) is maximized, which is the
 * slope between two points if we plot points (A[i], i) instead of (i, A[i])
 * Now it's easy to see that we can always get maximum slope if we consider
 * (A[i], i), (A[j], j) where A[j] is the smallest element in A bigger than A[i]
 * So we can sort A[i]'s and find the desired slope
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

struct AqaAsadiMinimizes {
	double getMin(vector <int> P, int B0, int X, int Y, int N) {
		vi A(N);
		for (int i = 0; i < N; ++i) {
			if (i < sz(P))
				A[i] = P[i];
			else if (i == sz(P))
				A[i] = B0;
			else
				A[i] = (A[i-1] * 1LL * X + Y) % MOD;
		}
		vii v;
		for (int i = 0; i < N; ++i) {
			v.pb(mp(A[i], i));
		}

		sort(v.begin(), v.end());
		long double ans = 1e18;
		for (int i = 1; i < N; ++i) {
			if (v[i].sc == v[i - 1].sc)
				return 0;
			ans = min(ans, (v[i].fs - v[i - 1].fs) / ((long double) 1.0 * abs(v[i].sc - v[i - 1].sc)));
		}
		return ans;
	}
};



int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
