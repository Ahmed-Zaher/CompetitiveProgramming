/*
 * Assume that the resulting string from the previous step is X,
 * hence after the current (i'th) step we have AXBXC (where A, B, C
 * are the tree constant strings given)
 * Let ans(i) be the answer after applying the operation i times.
 * ans(0) = how many times F appears in S
 * ans(i) = 2 * ans(i - 1) + f(i)
 * The 2 * ans(i-1) is because we repeat X twice in
 * And f(i) is the number of occurrences of F that cover two or more
 * parts of AXBXC, these are occurrences that cover:
 * AX, XB, BX, XC, AXB, XBX, BXC, AXBXC
 * The last 5 cases only appear when |X| < 50, which happens in <= 5 steps,
 * so we can trivially simulate those steps and compute all cases.
 * The first 3 cases are simpler, they only require us to have a prefix/suffix
 * of X, so we can simulate more steps and only care about prefixes/suffixes
 * of length < 50.
 * But we also note that after 100 steps the prefix of X is AAA.., and the suffix
 * is ..CCC. And prefixes/suffixes of length < 50 remain the same after all the steps
 * to come leading to f(i) being constant. So if k > 50 we just use the above formula
 * knowing that f(i) is constant
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



struct AkariDaisukiDiv1 {
	int countF(string A, string B, string C, string X, string F, int k) {
		V<string> prefF, suffF;
		vi pref, suff;
		int ans = 0;

		auto matches = [](string s, string t) {
			// how many times t occurs in s
			int ret = 0;

			for (int i = 0; i <= sz(s) - sz(t); ++i)
				ret += (s.substr(i, sz(t)) == t);

			return ret;
		};

		ans += matches(X, F);
		int a = matches(A, F), b = matches(B, F), c = matches(C, F);

		prefF.pb("");
		suffF.pb("");

		for (int i = 0; i < sz(F); ++i) {
			prefF.pb(F.substr(0, i + 1));
			suffF.pb(F.substr(i));
		}
		reverse(suffF.begin() + 1, suffF.end());

		auto upd = [&]() {
			pref.clear();
			suff.clear();
			for (int l = 1; l <= sz(F) - 1; ++l) {
				if (l <= sz(X) && X.substr(sz(X) - l) == prefF[l])
					pref.pb(l);
				if (l <= sz(X) && X.substr(0, l) == suffF[l])
					suff.pb(l);
			}
		};

		upd();


		bool small = true;
		int add = 0;

		for (int I = 1; I <= min(k, 100); ++I) {
			ans = (ans + ans + a + b + c) % MOD;

			vi lvl;
			for (auto& elem : A)
				lvl.pb(1), elem = elem + 1 - 1;
			for (auto& elem : X)
				lvl.pb(2), elem = elem + 1 - 1;
			for (auto& elem : B)
				lvl.pb(1), elem = elem + 1 - 1;
			for (auto& elem : X)
				lvl.pb(2), elem = elem + 1 - 1;
			for (auto& elem : C)
				lvl.pb(1), elem = elem + 1 - 1;



			add = 0;

			if (small) {
				X = A + X + B + X + C;
				for (int i = 0; i <= sz(X) - sz(F); ++i) {
					int v = 0;
					for (int j = i; j < i + sz(F); ++j)
						v |= lvl[j];

					if (X.substr(i, sz(F)) == F && v == 3)
						++ans;
				}
			} else {
				for (auto& l : suff) {
					if (sz(A) >= sz(F) - l)
						if (prefF[sz(F) - l] == A.substr(sz(A) - (sz(F) - l)))
							++add;
					if (sz(B) >= sz(F) - l)
						if (prefF[sz(F) - l] == B.substr(sz(B) - (sz(F) - l)))
							++add;
				}
				for (auto& l : pref) {
					if (sz(B) >= sz(F) - l)
						if (suffF[sz(F) - l] == B.substr(0, sz(F) - l))
							++add;
					if (sz(C) >= sz(F) - l)
						if (suffF[sz(F) - l] == C.substr(0, sz(F) - l))
							++add;
					if (sz(B) < sz(F) - l && B + X.substr(0, sz(F) - l - sz(B)) == suffF[sz(F) - l])
						++add;
				}
				X = A + X + B + X + C;
			}

			ans += add;



			if (sz(X) > 200)
				X = X.substr(0, 100) + X.substr(sz(X) - 100), small = false;

			upd();
		}
		if (k <= 100)
			return ans;

		for (int i = 1; i <= k - 100; ++i)
			ans = (ans + ans + a + b + c + add) % MOD;

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
