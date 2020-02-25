/*
 * Let's refer a subset with sum >= K as a K-good subset
 * Clearly for every card we need to find the min. sum of a K-good
 * subset that includes that card, and if that min. sum - a[i] >= K
 * then i'th card is unnecessary.
 * We can compute this efficiently using dynamic programming,
 * Let fp[i][x] be the min. sum of an x-good subset using elements in a[1..i],
 * and let fs[i][x] be the min. sum of an x-good subset using elements in a[i..N].
 * Now we want for i'th card a min. sum of (K-a[i])-good subset, some of that subset
 * is drawn from a[1..i-1] and the rest is from a[i+1..N], and we know that if one side
 * is x-good then the other needs to be (K-a[i]-x)-good. So we can interate over all x in [0, K-a[i]].
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


int N, K;
vi a;
VV<int> fp, fs;


int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> N >> K;

	a.assign(N + 1, 0);

	for (int i = 1; i <= N; ++i) {
		cin >> a[i];
	}

	fp.assign(N + 2, V<int>(K + 1, 1e9 + 1e6));
	fs.assign(N + 2, V<int>(K + 1, 1e9 + 1e6));

	for (int i = 0; i <= N + 1; ++i)
		fp[i][0] = fs[i][0] = 0;

	for (int i = 1; i <= N; ++i) {
		for (int x = 1; x <= K; ++x) {
			fp[i][x] = min(fp[i - 1][x], a[i] + fp[i - 1][max(x - a[i], 0)]);
		}
	}

	for (int i = N; i >= 1; --i) {
		for (int x = 1; x <= K; ++x) {
			fs[i][x] = min(fs[i + 1][x], a[i] + fs[i + 1][max(x - a[i], 0)]);
		}
	}

	int ans = 0;

	for (int i = 1; i <= N; ++i) {
		int mnSum = 1e9 + 1e6;
		int y = max(0, K - a[i]);
		for (int x = 0; x <= y; ++x)
			mnSum = min(mnSum, fp[i - 1][x] + fs[i + 1][y - x]);

		db(mnSum);

		if (mnSum >= K)
			++ans;
	}

	cout << ans << '\n';



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
