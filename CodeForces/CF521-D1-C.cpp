/*
 * Let's find for each digit independently how much it contributes
 * to the answer, let f(i) be the contribution of s[i] (1-based)
 * our answer is sum of f(i) for i in [1, n]
 * First if there's no + after s[i] then we add to f(i) the value
 * s[i] * 10^(n-i) * (i-1)C(k), where nCr is 'n choose r'
 * Otherwise the next + can be after s[j] for j in [i, n)
 * For each j we add s[i] * 10^(j-i) * (n-i-j-2)C(k-1)
 * So over all, f(i) = [s[i]*10^(n-i)*(i-1)C(k)] + sum of[s[i]* 10^(j-i)*(n-i-j-2)C(k-1)] for j in [i,n)
 * Let the sum in second term of f(i) be S_i, we can shift the index of S_i to have
 * S_i = sum of [s[i]*10^z* (n-z-2)C(k-1)], for z in [0,n-i)
 * [s[i]*10^z* (n-z-2)C(k-1)] is defined by s[i] and z, so if we let
 * h(x, y) = x * 10^y * (n-y-2)C(k-1), we can precompute h(x, y) for all x and y
 * Now we have S_i as just a prefix sum of the function where x = s[i]
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



const int MX = 1e5 + 5, MOD = 1000000007;
int n, k;
int fact[MX], factInv[MX], tens[MX], h[10][MX], sum[10][MX];
string s;

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


ll inv(ll a, ll b) {
	return 1 < a ? b - inv(b % a, a) * b / a : 1;
}


int nCr(int n, int r) {
	if (n < r)
		return 0;
	int ret = 0;
	ret = fact[n];
	ret = (ret * 1LL * factInv[r]) % MOD;
	ret = (ret * 1LL * factInv[n - r]) % MOD;
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


	fact[0] = 1;
	factInv[0] = inv(1, MOD);
	tens[0] = 1;

	for (int i = 1; i < MX; ++i) {
		fact[i] = mul(fact[i - 1], i);
		factInv[i] = mul(factInv[i - 1], inv(i, MOD));
		tens[i] = mul(tens[i - 1], 10);
	}

	cin >> n >> k >> s;

	s = " " + s;

	int ans = 0;

	for (int i = 1; i <= n; ++i)
		ans = add(ans, mul(mul(s[i] - '0', tens[n - i]), nCr(i - 1, k)));

	if (!k) {
		cout << ans << '\n';
		return 0;
	}

	db(ans);

	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < MX; ++j) {
			h[i][j] = mul(mul(i, tens[j]), nCr(n - j - 2, k - 1));
			sum[i][j] = h[i][j];
			if (j)
				sum[i][j] = add(sum[i][j], sum[i][j - 1]);
		}

	for (int i = 1; i <= n - 1; ++i)
		ans = add(ans, sum[s[i] - '0'][n - 1 - i]);

	cout << ans << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
