/*
 * Let's find the number of paths crossing at least one black cell (let that be X),
 * then our answer is (h-1+w-1)C(h-1) - X
 * Let S_i denote the set of paths crossing the i'th black cell
 * We want |union of S_i for i in [1, n]|, here we can use inclusion exclusion
 * principle
 * And to compute the number of paths going through a subset of black cells and sum
 * that up over all possible subsets, we use dp
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


const int MOD = 1000000007, MX = 2e5 + 5, MX_N = 2005;
const long long BAD = -1e18;
int h, w, n;
ll fact[MX], factInv[MX], mem[MX_N][2];
vii x;

ll inv(ll a, ll b) {
	return 1 < a ? b - inv(b % a, a) * b / a : 1;
}

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

ll nCk(ll n, ll r) {
	if (r > n)
		return 0;
	ll ret = fact[n];
	return mul(ret, mul(factInv[r], factInv[n - r]));
}

ll dp(int last, bool parity = 1) {
	ll& ret = mem[last][parity];
	if (ret != BAD)
		return ret;
	ret = mul(parity ? 1 : -1, nCk(h - x[last].fs + w - x[last].sc, h - x[last].fs));

	for (int next = last + 1; next < n; ++next)
		if (x[next].fs >= x[last].fs && x[next].sc >= x[last].sc)
			ret = add(ret, mul(dp(next, !parity),
					nCk(x[next].fs - x[last].fs + x[next].sc - x[last].sc, x[next].fs - x[last].fs)));
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

	fact[0] = factInv[0] = 1;

	for (int i = 1; i < MX; ++i) {
		fact[i] = mul(fact[i - 1], i);
		factInv[i] = inv(fact[i], MOD);
	}

	cin >> h >> w >> n;

	x.assign(n, pii());

	for (int i = 0; i < n; ++i)
		cin >> x[i].fs >> x[i].sc;

	sort(x.begin(), x.end());

	ll ans = nCk(h + w - 2, h - 1);

	for (int i = 0; i < MX_N; ++i)
		for (int j = 0; j < 2; ++j)
			mem[i][j] = BAD;

	for (int i = 0; i < n; ++i)
		ans = add(ans, mul(nCk(x[i].fs + x[i].sc - 2, x[i].fs - 1), -dp(i, 1)));


	cout << ans << '\n';



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
