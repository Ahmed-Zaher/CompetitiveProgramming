/*
 * We note that:
 * 1. We can't have 2 adjacent hills on which we can build houses
 * 2. The positions where houses will end up don't need to decrease
 * 3. If a position is chosen for a house, we only need to decrease the adjacent hills
 * to be at most the height of the current hill  - 1
 * 4. If 2 houses at positions i, j, and |i - j| > 2 then their costs are independent
 * So we can use dynamic programming to try all possible configurations of houses' positions,
 * and remember for every state whether the hill at current position - 2 is chosen for a house
 * and use that to decrease the cost if possible
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

const int MX = 5005;
int n, a[MX];
int mem[MX][MX >> 1][2][2];

int dp(int idx, int k, bool p1, bool p2) {
	if (!k)
		return 0;
	if (idx == n + 1)
		return 1e9;
	int& ret = mem[idx][k][p1][p2];

	if (ret != -1)
		return ret;

	ret = dp(idx + 1, k, 0, p1);

	if (p1)
		return ret;

	int cost = max(0, a[idx + 1] - a[idx] + 1);

	if (p2)
		cost += max(0, min(a[idx - 1], a[idx - 2] - 1) - a[idx] + 1);
	else
		cost += max(0, a[idx - 1] - a[idx] + 1);

	return ret = min(ret, cost + dp(idx + 1, k - 1, 1, p1));

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

	for (int i = 1; i <= n; ++i) {
		cin >> a[i];
	}

	memset(mem, -1, sizeof(mem));

	for (int k = 1; k <= (n + 1) / 2; ++k) {
		cout << dp(1, k, 0, 0) << ' ';
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
