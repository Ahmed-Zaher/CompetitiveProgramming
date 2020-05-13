/*
 * We can solve this problem by finding Grundy values:
 * https://cp-algorithms.com/game_theory/sprague-grundy-nim.html
 * Let g[i] be 0 if we'd lose a game consisting of one pile of size i,
 * and non-zero if we'd win that game. We know that g[1] = g[2] = g[3] = 0,
 * for i >= 4, g[i] = mex(g[j]) for all j s.t. i^(1/4) <= j <= i^(1/2)
 * We can compute g[i] for all i in [1, 10^6], by considering i's in
 * increasing order, and to compute g[a[i]], we use
 * g[a[i]] = mex(g[j]) for all j s.t. a[i]^(1/4) <= j <= a[i]^(1/2),
 * and the range of such j lies within [1,10^6] which we already computed,
 * we also process a[i] in increasing order
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

const int MX = 1e6;
int n;
ll a[MX];
int g[MX], freq[MX];
si S;
pair<ll, ll> ranges[MX];

pair<ll, ll> move(pair<ll, ll> range, ll x) {
	ll l = range.fs, r = range.sc;
	while (l * l * l * l < x)
		++l;
	while ((r + 1) * (r + 1) <= x)
		++r;
	return mp(l, r);
}

void upd(pair<ll, ll>& range, pair<ll, ll>& to) {
	while (range.sc < to.sc) {
		++range.sc;
		if (!freq[g[range.sc]])
			S.erase(g[range.sc]);
		++freq[g[range.sc]];
	}
	while (range.fs < to.fs) {
		--freq[g[range.fs]];
		if (!freq[g[range.fs]])
			S.insert(g[range.fs]);
		++range.fs;
	}
}


int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);


	pair<ll, ll> range = mp(1, 1);
	for (int i = 1; i < MX; ++i) {
		ranges[i] = range = move(range, i);
	}

	for (int i = 1; i < MX; ++i)
		S.insert(i);

	freq[0] = 1;

	range = mp(1, 1);

	for (int i = 4; i < MX; ++i) {
		upd(range, ranges[i]);
		g[i] = *S.begin();
	}


	cin >> n;

	for (int i = 0; i < n; ++i) {
		cin >> a[i];
		if (a[i] <= 3)
			--n, --i;
	}

	if (!n) {
		cout << "Rublo\n";
		return 0;
	}

	sort(a, a + n);


	for (int i = 0; i < MX; ++i)
		freq[i] = 0;

	++freq[0];
	range = mp(1, 1);

	S.clear();
	for (int i = 1; i < MX; ++i)
		S.insert(i);

	ll tot = 0;

	for (int i = 0; i < n; ++i) {
		auto newRange = move(range, a[i]);
		assert(range.fs <= newRange.fs && range.sc <= newRange.sc);
		upd(range, newRange);
		assert(!S.empty());
		tot ^= *S.begin();
	}

	cout << (tot ? "Furlo\n" : "Rublo\n");



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
