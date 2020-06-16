/*
 * Let's set the rule that the loosing player would always delays
 * his loss as much as possible to maximize the number of turns played
 * This rule clearly doesn't change the result of the game
 * Now consider how the string looks when the player whose turn is now
 * loses, for that player, removing any character with index i, 1 < i < n
 * results in two characters being adjacent, where n is the length of the string
 * at that moment, and indices are 1-based
 * This implies that s1 = s3 = s5 = .., s2 = s4 = s6 = ...
 * That is, a string of characters alternating between first and last character
 * in the original string, we'll denote those as a and b respectively
 * We note that if we have 2 a/b with other characters that are neither a nor b
 * between them, we can always remove the leftmost one of those characters
 * until we either get:  ab or axa or bxa where x is any char != neither a nor b
 * So the string we get before the current player lose will always be in the form
 * abaxaxabxbaxbxb
 * The length of that string is the number of chars = a or b, plus every 2 consecutive
 * a's with no b's in between or 2 consecutive b's with no a in between
 * From that we can find how many moves it took to get there, and find which player will
 * play now and lose
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

	string s;
	cin >> s;
	int n = sz(s);
	vi v;

	for (auto& c : s) {
		if (c == s[0])
			v.pb(0);
		else if (c == s.back())
			v.pb(1);
	}

	int l = sz(v);

	for (int i = 1; i < sz(v); ++i) {
		l += v[i] == v[i - 1];
	}

	if ((n - l) % 2 == 0)
		cout << "Second\n";
	else
		cout << "First\n";



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
