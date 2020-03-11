/*
 * Given p and a, we want to find if a is a primitive root of p
 * Consider the value of a^i mod p, that value can never be 0 because
 * a^i will never divide p, and it can only have p - 1 possible values [1, p - 1]
 * So we can imagine a graph of p - 1 nodes where there is an edges i->j if i * a = j mod p
 * If we start from some node and move from it to the next node we'll eventually reach
 * a visited node and have a cycle. If a is a primitive root we'd get a cycle of length p - 1
 * If not then there is a smaller cycle length c, that implies that a^c = 1 mod p, which means
 * that no matter from which node we start, after exactly c moves we'd end up where we started
 * Which implies that the graph will be partitioned into (p - 1) / c components, each of which
 * is a cycle of length c. This implies that c is a divisor of p - 1, so to find whether a
 * is a primitive root of p, just check whether a^d = 1 mod p for all divisors of p - 1
 * iff the only working d is p - 1 (and a^(p-1) = 1 mod p is always true be Fermat's little theorem)
 * then a is a primitive root.
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

int MOD;

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

vi divs;
void getDivs(int x) {
	ll i;
	for (i = 2; i * i < x; i++)
		if (x % i == 0)
			divs.pb(i), divs.pb(x / i);
	if (i * i == x)
		divs.pb(i);
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


	int p, n;

	while (cin >> p >> n && (p + n)) {

		getDivs(p - 1);
		sort(divs.begin(), divs.end());
		MOD = p;


		for (int i = 1; i <= n; ++i) {
			int a;
			cin >> a;

			bool proot = true;

			for (auto& d : divs)
				if (power(a, d) == 1) {
					proot = false;
					break;
				}

			cout << (proot ? "YES\n" : "NO\n");
		}


		divs.clear();
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
