/*
 * Let dpmax[i][j] be the maximum product we can get by choosing exactly j
 * numbers from numbers[i...N-1], knowing that the last number we took is
 * at index i - 1. Let dpmin[i][j] be defined similarly.
 * Because numbers can have negative values, we might want to use a negative
 * number along with the minimum value we can get after then (which is hopefully negative)
 * and thus we obtain a positive value that is possibly the maximum we can get.
 * So we have dpmax[i][j > 0] = max(numbers[k] * dpmax[k+1][j-1], numbers[k] * dpmax[k+1][j-1])
 * and similarly for dpmin.
 * There is one more thing to note, is that the next state we move to might be an invalid state,
 * for example a state that needs to take j values from a part of the array that has less than
 * j values. So we only modify states in the next states are valid.
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

ll dpmax[55][11], dpmin[55][11];
const long long INF = 1e18;

class TheProduct {
public:
	ll maxProduct(vi numbers, int k, int maxDist) {
		int N = sz(numbers);


		for (int i = 0; i <= N; ++i) {
			for (int j = 0; j <= k; ++j) {
				dpmax[i][j] = -INF;	// indicates invalid states
				dpmin[i][j] = INF;
			}
		}

		for (int i = 0; i <= N; ++i)
			dpmax[i][0] = dpmin[i][0] = 1;

		for (int rem = 1; rem <= k; ++rem) {
			for (int idx = 0; idx < N; ++idx) {
				for (int j = idx; j < min(N, idx + maxDist); ++j) {
					ll nextMax = dpmax[j + 1][rem - 1];
					ll nextMin = dpmin[j + 1][rem - 1];

					if (nextMax != -INF) {
						dpmax[idx][rem] = max(dpmax[idx][rem], numbers[j] * nextMax);
						dpmin[idx][rem] = min(dpmin[idx][rem], numbers[j] * nextMax);
					}

					if (nextMin != INF) {
						dpmax[idx][rem] = max(dpmax[idx][rem], numbers[j] * nextMin);
						dpmin[idx][rem] = min(dpmin[idx][rem], numbers[j] * nextMin);
					}

				}
			}
		}

		ll ret = -1e18;
		for (int i = 0; i < N; ++i)
			if (dpmax[i][k] != -INF)
				ret = max(ret, dpmax[i][k]);

		return ret;
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
