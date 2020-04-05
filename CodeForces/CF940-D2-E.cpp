/*
 * Consider a subarray a[l..r], let len = r - l + 1
 * The key observation in this problem is that, we can always
 * partition the array into len / c smaller subarrays (call them pieces) of length c, and
 * ignore the rest of a[l..r] and get the same or a better answer
 * First note that this way the number of elements excluded didn't change, it's len / c
 * We can do our partitioning so every minimum considered is included in one of the pieces.
 * Let g(i) = the number of elements that were considered minimum in
 * a[l..r] that lies in i'th piece
 * If g(i) = 1 then the contribution of that piece didn't change because
 * after paritioning we'd still pick the same elements from that piece
 * If however g(i) > 1, then it means for some j's we have g(j) = 0
 * Now for the maximum g(i) - 1 in i'th piece that were included in a[l..r],
 * they will be replaced by elements in the empty pieces, and those element
 * will be always of larger value, because if they weren't they would have
 * been selected as min elements of a[l..r].
 * So we can make a dp[i] where we try ignoring i'th element (putting it
 * in a subarray of size 1) or take the subarray a[i..i+c-1]
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

const int MX = 1e5 + 5;
int n, c;
vi a, f;
ll mem[MX];

ll dp(int i) {
	if (i == n)
		return 0;
	ll& ret = mem[i];
	if (ret != -1)
		return ret;
	return ret = max(dp(i + 1), (i + c <= n ? f[i] + dp(i + c) : 0));
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

	cin >> n >> c;

	a.assign(n, 0);
	ll sum = 0;

	for (int i = 0; i < n; ++i) {
		cin >> a[i];
		sum += a[i];
	}

	if (c > n) {
		cout << sum << '\n';
		return 0;
	}

	set<pii> s;

	for (int i = 0; i < c; ++i)
		s.insert(mp(a[i], i));

	f.assign(n, 0);

	for (int i = 0;; ++i) {

		f[i] = s.begin()->fs;

		if (i + c == n)
			break;
		s.erase(mp(a[i], i));
		s.insert(mp(a[i + c], i + c));
	}

	for (int i = 0; i < MX; ++i)
		mem[i] = -1;

	cout << sum - dp(0) << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
