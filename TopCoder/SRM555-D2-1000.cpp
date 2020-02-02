/*
 * Consider any given configuration,
 * let cnt[i] be the number of ways to go from the 0'th segment to the i'th segment
 * if i'th segment is a #, then cnt[i] = 0
 * otherwise, cnt[i] = cnt[i - 1] + cnt[i - 2]
 * And of course, cnt[0] = 1
 *
 * We add this configuration to our answer only if cnt[N - 2] = 0 mod 2
 * Since we only care about the number of ways mod 2, and only care about
 * the 2 previous segments, we can use dynamic programming to enumerate all
 * possible configurations and maintain cnt[i - 1], cnt[i - 2] mod 2 in our state.
 *
 * So dp(idx, cnt, p1, p2) is the number of valid configurations we can get knowing
 * that we processed positions [0, idx - 1] of the config., and have cnt muddy segments
 * left to use, and the number of ways to end at segment idx - 1 is p1, and similarly for p2
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


const int MOD = 555555555;
int mem[555][556][2][2], N;

int dp(int idx, int cnt, int p1, int p2) {
	if (idx == N - 1)
		return p1 == p2 && !cnt;
	int& ret = mem[idx][cnt][p1][p2];

	if (ret != -1)
		return ret;
	ret = 0;
	if (cnt)
		ret = dp(idx + 1, cnt - 1, 0, p1);
	ret = (ret + dp(idx + 1, cnt, (p1 + p2) % 2, p1)) % MOD;
	return ret;
}

class MuddyRoad2 {
public:
	int theCount(int N, int muddyCount) {
		::N = N;
		memset(mem, -1, sizeof(mem));
		return dp(1, muddyCount, 1, 0);
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
