/*
 * Let's do the process in reverse,
 * we know that at the end we must bribe the strongest guy,
 * we can do that then him fight n/2 - 1 other guys, then pick
 * the next strongest guy and so on..
 * Let's number the steps from 1 to N, in 1st step we pick strongest
 * guy, then in 2nd step we pick second strongest guy and so on
 * We note to bribe a guy at j'th step, then he will fight 2^(N-j)-1
 * other guys
 * This indicates that we can'th pick a guy with low strength at an early
 * step, because he won't find enough people to fight
 * On the other hand, if we bribe a guy at j'th step, we can also bribe him
 * at a later step
 * So we can find for each step, which people can be picked at that step or
 * at a later step, and we find that at j'th step, we can pick any people with
 * strengths 2^(N-j+1)-1 or more
 * Knowing that, we can just simulate the process and at each step we pick
 * the minimum a[i] of the valid candidates we can choose from
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





int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int n;
	cin >> n;


	int N;

	for (int i = 0;; ++i)
		if ((1 << i) == n) {
			N = i;
			break;
		}

	vi a((1 << N) + 1);

	for (int i = 1; i <= (1 << N); ++i)
		cin >> a[i];

	for (int i = 1; i <= (1 << N); ++i) {
		if (a[i] != -1)
			a[i] = 0;
		else {
			for (int j = i; j <= (1 << N) - 1; ++j)
				a[j] = a[j + 1];
			break;
		}
	}

	V<multiset<int>> s(20);

	ll ans = 0;

	for (int i = 1; i <= (1 << N) - 1; ++i) {
		for (int k = 25;; --k)
			if ((1 << k) - 1 <= i) {
				s[k].insert(a[i]);
				break;
			}
	}

	for (int i = N; i >= 1; --i) {
		int mn = 2e9;
		for (int j = i; j < 20; ++j) {
			if (!s[j].empty())
				mn = min(mn, *s[j].begin());
		}
		ans += mn;
		for (int j = i; j < 20; ++j) {
			if (!s[j].empty() && *s[j].begin() == mn) {
				s[j].erase(s[j].begin());
				break;
			}
		}
	}


	cout << ans << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
