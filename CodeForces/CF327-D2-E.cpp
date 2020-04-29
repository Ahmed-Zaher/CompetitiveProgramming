/*
 * We can reduce the problem to finding the number of routes that pass through 2 points A, B where A <= B
 * To solve that problem, we can use meet in the middle approach, a route that passes through A and B
 * first has a subset of moves s1 that takes us from 0 to A, then a disjoint subset s2 that takes us from A to B
 * then the rest of the moves will take is from B to the end. If we fix those subsets, they introduce
 * number of routes = |s1|! * |s2|! * |{0,1,..,n-1} - (s1 union s2)|!
 * Note that s1 moves us distance = A, s2 moves us distance = B - A
 * We can iterate over all subsets s1 and s2 of the first n / 2 a_i, we can do that going from 0 to 3^(n/2)-1
 * Where when we present numbers in base 3, if a i'th digit is 0 then a_i belongs to s1, if it's 1 then
 * a_i belongs to s2 and if it's 2 then a_i belongs to neither s1 nor s2
 * For every s1, s2, we store |s1|, |s2| and the sum of a_i in s1 and s2 in some data structures
 * Then we iterate over subsets of the last n / 2 a_i's where we get sum of distances of s1 and sum of distances
 * of s2, by knowing that s1 adds up to A and s2 adds up to B - A we can calculate how much more we need and
 * do lookup in the data structure so we merge our last half of s1 and s2 with all valid first halves of s1 and s2
 */

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;

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


template<class T, class S> using pb_map = tree<T, S,
	less<T>, rb_tree_tag, tree_order_statistics_node_update>;

const int MX = 26, MOD = 1000000007;
int n, K, a[MX], fact[MX], k[2], L, R;
ll sumL, sumR;
pb_map<pair<ll, ll>, pb_map<pii, int>> cnt;


ll add(ll x, ll y) {
    x += y;
    while(x >= MOD)
        x -= MOD;
    while(x < 0)
        x += MOD;
    return x;
}

ll mul(ll x, ll y) {
    return (x * y) % MOD;
}

int solve(int A, int B) {

	int threePowR = 1;

	for (int i = 1; i <= R; ++i)
		threePowR *= 3;

	int ret = 0;

	for (int i = 0; i < threePowR; ++i) {
		ll sumA = 0, sumB = 0;
		int cntA = 0, cntB = 0;

		int j = i;

		vi digits;

		for (int k = 0; k < R; ++k, j /= 3)
			if (j % 3 == 0)
				++cntA, sumA += a[k];
			else if (j % 3 == 1)
				++cntB, sumB += a[k];
		if (sumA > A || sumB > B)
			continue;

		if (present(cnt, mp(A - sumA, B - sumB)))
			for (auto& pr : cnt[mp(A - sumA, B - sumB)])
				ret = add(ret, mul(pr.sc, mul(fact[pr.fs.fs + cntA], mul(fact[pr.fs.sc + cntB], fact[n - (pr.fs.fs + cntA + pr.fs.sc + cntB)]))));
	}

	db(A, B, L, R, ret);

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


	cin >> n;

	for (int i = 0; i < n; ++i)
		cin >> a[i];

	fact[0] = 1;

	for (int i = 1; i <= n; ++i)
		fact[i] = mul(fact[i - 1], i);

	cin >> K;

	for (int i = 0; i < K; ++i)
		cin >> k[i];


	if (K == 0) {
		cout << fact[n] << '\n';
		return 0;
	}

	if (n == 1) {
		if (k[0] == a[0] || k[1] == a[0])
			cout << "0\n";
		else
			cout << "1\n";
		return 0;
	}

	if (K == 1)
		k[1] = k[0];

	sort(k, k + 2);


	L = n / 2, R = n - L;

	int threePowL = 1;

	for (int i = 1; i <= L; ++i)
		threePowL *= 3;

	for (int i = 0; i < R; ++i)
		sumR += a[i];

	for (int i = R; i < R + L; ++i)
		sumL += a[i];

	for (int i = 0; i < threePowL; ++i) {
		ll sumA = 0, sumB = 0;
		int cntA = 0, cntB = 0;

		int j = i;

		vi digits;

		for (int k = 0; k < L; ++k, j /= 3)
			if (j % 3 == 0)
				++cntA, sumA += a[R + k];
			else if (j % 3 == 1)
				++cntB, sumB += a[R + k];

		cnt[mp(sumA, sumB)][mp(cntA, cntB)]++;
	}

	int ans = add(fact[n], add(-solve(k[0], 0), -solve(k[1], 0)));


	cout << add(ans, solve(k[0], k[1] - k[0])) << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
