/*
 * Let dp(i, j, l, r) be the number of ways to color s[i..j]
 * knowing that s[i - 1] is colored l, s[j + 1] is colored r
 * 0 corresponds to no coloring
 * In every state, we find the index of the bracket matching the bracket in s[i], let it be m[i],
 * and try all possible colorings at those indices and pass the colors used to solve the problem
 * in s[i + 1..m[i - 1]], and in s[m[i] + 1..j]
 * The initial call is dp(0, |s| - 1, 0, 0), and s[0..|s| - 1] is a correct bracket sequence,
 * and because we chose ranges s[i + 1..m[i - 1]], s[m[i] + 1..j], all the ranges s[i..j]
 * in all subsequent calls will be a correct bracket sequence.
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

const int MX = 705, MOD = 1000000007;
int mem[MX][MX][3][3], m[MX];
string s;


int dp(int i, int j, int l, int r) {

	if (i > j)
		return 1;
	int& ret = mem[i][j][l][r];

	if (ret != -1)
		return ret;

	ret = 0;

	for (int ii = 0; ii < 3; ++ii) {
		for (int jj = 0; jj < 3; ++jj) {
			if ((ii && jj) || (!ii && !jj) || (l && ii == l))
				continue;
			if (m[i] == j && r && jj == r)
				continue;

			ret = (ret + (dp(i + 1, m[i] - 1, ii, jj) * 1LL * dp(m[i] + 1, j, jj, r))) % MOD;
		}
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

	cin >> s;

	stack<int> st;

	for (int i = 0; i < sz(s); ++i) {
		if (s[i] == '(')
			st.push(i);
		else
			m[st.top()] = i, st.pop();
	}


	memset(mem, -1, sizeof(mem));

	cout << dp(0, sz(s) - 1, 0, 0) << '\n';



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
