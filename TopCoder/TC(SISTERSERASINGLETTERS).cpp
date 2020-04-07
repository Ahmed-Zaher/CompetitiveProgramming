/*
 * The fact that the indices chosen are of increasing order makes
 * it possible to develope a dp recurrence to compute the answer
 * Let dp(i, j) be the best string we can get by playing with the string
 * word[i..n-1], the best with respect to j, where j = 0 is player 1, and
 * j = 1 is player 2.
 * If j = 0 then dp(i, j) = max(s[i..k-1] + dp(k+1, 1)) for k in [i, n)
 * If j = 1 then dp(i, j) = min(s[i..k-1] + dp(k+1, 0)) for k in [i, n)
 * Where the + denotes string concatenation
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

string mem[55][2];
string s;

string dp(int idx, bool p) {
	if (idx == sz(s))
		return "";
	string& ret = mem[idx][p];
	if (ret != "$")
		return ret;
	if (!p) {
		ret = "";
		for (int i = idx; i < sz(s); ++i) {
			ret = max(ret, s.substr(idx, i - idx) + dp(i + 1, !p));
		}
	} else {
		ret = "{";
		for (int i = idx; i < sz(s); ++i) {
			ret = min(ret, s.substr(idx, i - idx) + dp(i + 1, !p));
		}
	}

	return ret;
}

struct SistersErasingLetters {
	string whoWins(string word) {
		s = word;
		for (int i = 0; i < 55; ++i)
			mem[i][0] = mem[i][1] = "$";

		return dp(0, 0) > s ? "Camomile" : "Romashka";
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
