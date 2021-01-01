/*
 * For each query, let's binary search the answer, the answer is the first
 * number u where the following question returns true:
 * Considering only numbers in [x+1, u], are there more than or equal to k numbers
 * that are relatively prime to p?
 * This question can be asked if we can find the number of numbers in a given range
 * that are relatively prime to some integer p, which is a classical problem
 * that can be solved using inclusion-exclusion, check:
 *  https://cp-algorithms.com/combinatorics/inclusion-exclusion.html#toc-tgt-11
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
using pii = pair<int, int>;
using vi = V<int>;
using vii = V<pii>;
using vvi = VV<int>;
using mii = map<int, int>;
using umii = unordered_map<int, int>;
using si = set<int>;
using usi = unordered_set<int>;

const int MX = 1000000;
vi PFs[MX + 1];
bool prime[MX + 1];


int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	for (int i = 2; i <= MX; ++i) {
		prime[i] = true;
	}

	for (int i = 2; i <= MX; ++i) {
		if (prime[i]) {
			PFs[i].pb(i);
			for (int j = 2 * i; j <= MX; j += i)
				prime[j] = 0, PFs[j].pb(i);
		}
	}

	int T;

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		int x, p, k;
		cin >> x >> p >> k;
		int lo = x + 1, hi = 1e9;
		auto g = [&](int l, int r) {
			int ans = 0;
			for (int mask = 1; mask < (1 << (sz(PFs[p]))); ++mask) {
				int prod = 1;
				int onesCnt = 0;
				for (int i = 0; i < sz(PFs[p]); ++i) {
					if (mask & (1 << i))
						prod *= PFs[p][i], ++onesCnt;
				}
				int add = r / prod - (l - 1) / prod;
				if (onesCnt % 2 == 1)
					ans += add;
				else
					ans -= add;
			}
			return ans;
		};
		while (lo < hi) {
			int mid = (lo + hi) / 2;
			if ((mid - x - g(x + 1, mid)) >= k)
				hi = mid;
			else
				lo = mid + 1;
		}
		cout << lo << '\n';
	}


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
