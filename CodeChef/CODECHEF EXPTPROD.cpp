/*
 * From the expected value definition we know that ans = sum of i = 0..N-1 of i * p(i)
 * where p(i) is probability that value at the end is i. Let's express a scenario as
 * the sequence of K values we pick, since every element has the same probability of
 * being picked we know that this will lead to every scenario having the same probability
 * Hence p(i) = (#scenarios that after K operation we get value = i) / (tot #scenarios)
 * It's clear that tot #scenarios is N^K
 * Let f_i(x) be the #scenarios after i operations that we get value = x, hence we have
 * p(i) = f_K(i) / (N^K), now we only need to efficiently compute f_K(i) for all i
 * and the rest is easy. It's clear that f_1(x) = #times the element x occurred in a.
 * We also note in important property, which is that if we have f_i(x), f_j(x), that
 * is sufficient to compute f_{i+j}(x) for all x's.
 * That is because every scenario of length i+j with result = z can be derived from
 * a scenario of length i starting from 1 and ending with x, and then followed by
 * a scenario of length j starting from 1 and ending with y, where (x * y) % N = z
 * This is because (((a * b) % N) * c) % N = (a * b * c) % N, we basically can
 * put % N's at any positions we like and as long as there's a % N at the end of the
 * scenario we'd get the correct result.
 * Now to compute f_{i+j}(z) we just check all ordered pairs (x, y) s.t. (x * y) % N = z and do
 * f_{i+j}(z) += f_i{x} +  f_j{y}.
 * We can then compute f_{2^p} for all 0 <= p <= log(K) and then f_K(x) will be the result
 * of merging f_{2^p}(x) for all p where p'th bit in K is ON.
 * Time complexity: O(N^2 * log(K))
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

ll inv(ll a, ll b) {
	return 1 < a ? b - inv(b % a, a) * b / a : 1;
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

int T, N, K;

vi merge(vi fi, vi fj) {
	vi ret(N, 0);
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			int k = (i * j) % N;
			ret[k] += (fi[i] * 1LL * fj[j]) % MOD;
			ret[k] %= MOD;
		}
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



	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		cin >> N >> K;
		vi a(N), f1(N);
		for (int i = 0; i < N; ++i) {
			cin >> a[i];
			++f1[a[i]];
		}

		vvi fpow2;

		fpow2.pb(f1);

		int tmp = K, bits = 0;

		while (tmp)
			++bits, tmp >>= 1;

		for (int i = 1; i < bits; ++i)
			fpow2.pb(merge(fpow2.back(), fpow2.back()));

		vi f = fpow2.back();

		for (int i = bits - 2; i >= 0; --i)
			if (K & (1 << i))
				f = merge(f, fpow2[i]);

		int ans = 0;

		for (int i = 0; i < N; ++i)
			ans = (ans + 1LL * i * f[i]) % MOD;

		cout << (ans * 1LL * inv(power(N, K), MOD)) % MOD << '\n';
	}




	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
