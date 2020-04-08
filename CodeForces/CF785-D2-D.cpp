/*
 * Let f(L, R) be the number of non-empty RSBS subsequences of a string
 * consisting of L '('s then R ')'s
 * We can prove that f(L, R) = [(L + R) C (L)] - 1, where nCr is 'n choose r'
 * Now to compute the answer we can iterate over the last '(' in the subsequence
 * Let L be the number of (s before current index, R be the number of )s after
 * the current index.
 * We can chose to have a subsequence of length 2, in which case we can only match
 * our closing bracket with any or the R opening brackets.
 * Otherwise, we iterate over the first closing bracket and we add to our answer
 * the sum S = sum of f(L, R - i) for 1 <= i <= R, S = sum of f(L, i) for i in [0,R)
 * S = sum of [(L)C(i)] - 1 = [sum of (L)C(i)] - R, the first term equals
 * (L+1)C(R-1) by the Hockey-stick identity
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

const int MX = 2e5 + 5, MOD = 1000000007;
int fact[MX], factInv[MX];
string s;

ll inv(ll a, ll b) {
	return 1 < a ? b - inv(b % a, a) * b / a : 1;
}


int f(int L, int R) {
	int ret = 0;
	ret = fact[L + R];
	ret = (ret * 1LL * factInv[L]) % MOD;
	ret = (ret * 1LL * factInv[R]) % MOD;
	--ret;
	if (ret < 0)
		ret += MOD;
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

	for (int i = 1; i < MX; ++i) {
		fact[i] = (fact[i - 1] * 1LL * i) % MOD;
		factInv[i] = (factInv[i - 1] * 1LL * inv(i, MOD)) % MOD;
	}

	cin >> s;

	int n = sz(s);

	int L = 0, R = 0;

	for (auto& c : s)
		R += c == ')';

	int ans = 0;

	for (int i = 0; i < n; ++i) {
		R -= s[i] == ')';

		db(i, L, R);

		if (s[i] == '(') {
			if (!L) {
				ans = (ans + R) % MOD;
			} else if (R) {
				ans = (ans + f(L + 1, R - 1) + 1) % MOD;
				ans = (ans - R + MOD) % MOD;
				ans = (ans + R + MOD) % MOD;
			}
		}
		db(ans);

		L += s[i] == '(';
	}

	cout << ans << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
