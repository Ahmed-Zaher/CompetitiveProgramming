/*
 * First note that the cost for collecting trash is constant (= NX)
 * so we can ignore it for now
 * If we were to collect a fixed subset of garbage in one trip, it'd
 * be optimal to go to most distant trash can, collect it, then on
 * collect the remaining trash cans on the way back
 * Now given a partition of trash cans into subsets, we know how to get
 * optimal answer
 * We want to find more properties about the optimal partitioning, let's fix the number
 * of subsets to be m
 * We note that for any optimal solution when we consider any suffix of trash cans,
 * the difference between the size of the largest and smallest subset (considering
 * only elements in the suffix) is minimum
 * From there we find that the optimal solution with a fixed m puts N'th can in 1st subset
 * (N-1)'th can in 2nd subset.. (N-m+1)'th can in m'th subset, (N-m)'th can in 1st subset
 * and so on...
 * We can find the cost of optimal solution for a fixed m in O(N)
 * Now to find best m, we note that when m increases by 1, the change in optimal answer
 * is increasing - hence the optimal answer is convex with respect to m
 * So we can do ternary search on m
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


int N, X;
vi x;

unsigned long long f(int m) {
	vi sizes(m);
	unsigned long long cand = N * 1LL * X;
	for (int j = N, p = 0; j >= 1; --j, ++p) {
		p %= m;
		if (!sizes[p])
			cand += X + x[j];

		++sizes[p];

		cand += (sizes[p] + 1) * 1ULL * (sizes[p] + 1) * (x[j] - x[max(j - m, 0)]);

	}
	return cand;
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


	cin >> N >> X;

	x.assign(N + 1, 0);

	for (int i = 1; i <= N; ++i)
		cin >> x[i];

	unsigned long long ans = ULLONG_MAX;

	int l = 1, r = N;
	while (r - l > 3) {
		int diff = r - l;
		int candL = l + diff / 3, candR = r - diff / 3;
		if (f(candL) < f(candR))
			r = candR;
		else
			l = candL;
	}
	for (int i = l; i <= r; ++i)
		ans = min(ans, f(i));

	cout << ans << '\n';
	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
