/*
 * The key idea is to think in terms of the sequence l mentioned
 * in the problem statement, it consists of consecutive segments,
 * each having a value that is 1 more than previous segment
 * We note that the l array of any valid configuration has edges only
 * between adjacent segments or between 2 nodes in the same segment
 * So we use dp to try all possible ways to partition the array into segments,
 * and when creating a new segment, we remember the size of the previous segments
 * and how many of them have degree 2 for example
 * Using the information about sizes we can make another dp that computes the number
 * of ways to merge two segments in the array
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

const int MX = 55, MOD = 1000000007;
int n, d[MX], memDP[MX][MX], memF[MX][MX][MX], g[MX], C[MX][MX];

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

void nCk() {
	for(int i = 0; i < MX; ++i)
		for(int j = 0; j <= i; ++j)
			C[i][j] = (j == 0) ? 1 : (C[i - 1][j - 1] + C[i - 1][j]) % MOD;
}

int f(int prevSz, int prevC1, int currSz) {
	if (prevSz + currSz == 0)
		return 1;

	int& ret = memF[prevSz][prevC1][currSz];

	if (ret != -1)
		return ret;

	ret = 0;

	int prevC2 = prevSz - prevC1;

	if (currSz) {
		if (prevC1)
			ret = add(ret, mul(prevC1, f(prevSz - 1, prevC1 - 1, currSz - 1)));
		if (prevC2)
			ret = add(ret, mul(prevC2, f(prevSz, prevC1 + 1, currSz - 1)));
	} else {
		if (prevC1) {
			if (prevC1 > 1)
				ret = add(ret, mul(prevC1 - 1, f(prevSz - 2, prevC1 - 2, 0)));
			if (prevC2)
				ret = add(ret, mul(prevC2, f(prevSz - 1, prevC1, 0)));
		} else {
			if (prevC2 > 2)
				ret = g[prevC2];
		}
	}

	return ret;
}


int dp(int idx, int prevSz, int prevC1) {
	if (idx == n + 1)
		return f(prevSz, prevC1, 0);
	int& ret = memDP[idx][prevSz];

	if (ret != -1)
		return ret;

	ret = 0;

	int currC2 = 0;

	for (int j = idx; j <= n; ++j) {

		currC2 += (d[j] == 2);

		int currSz = j - idx + 1;

		ret = add(ret, mul(f(prevSz, prevC1, currSz), dp(j + 1, currSz, currC2)));

	}

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

	nCk();

	// g[i] = number of ways to partition a graph of n nodes
	// into disjoint cycles

	g[0] = 1;

	for (int n = 3; n < MX; ++n) {
		for (int cycleSz = 3; cycleSz <= n; ++cycleSz) {
			int fact = 1;
			for (int i = cycleSz - 1; i >= 3; --i)
				fact = mul(fact, i);
			g[n] = add(g[n], mul(g[n - cycleSz], mul(C[n - 1][cycleSz - 1], fact)));
		}
	}

	cin >> n;

	for (int i = 1; i <= n; ++i)
		cin >> d[i];

	if (d[1] == 3 && n == 3) {
		cout << "0\n";
		return 0;
	}

	memset(memDP, -1, sizeof(memDP));
	memset(memF, -1, sizeof(memF));

	int c1 = 0;

	for (int i = 2; i <= 2 + d[1] - 1; ++i)
		c1 += (d[i] == 2);


	cout << dp(1 + d[1] + 1, d[1], c1) << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
