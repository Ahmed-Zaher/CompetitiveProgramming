/*
 * Ask about the first character of all 119 strings, we will receive each of
 * the characters as a response 24 times if we have all permutations,
 * but since we are missing one one of the characters will be received as response
 * 23 times. That must be the first character in the missing string, so we add it to
 * our answer and do the same for the 23 strings that start with that character
 * So in total we do about 119 + 23 + 5 + 1 = 148 queries
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


	int T, F;

	cin >> T >> F;

	V<char> chars({'A', 'B', 'C', 'D', 'E'});
	vi want({23, 5, 1, 0});

	for (int tc = 1; tc <= T; ++tc) {

		vi cands;

		for (int i = 1; i <= 119; ++i)
			cands.pb(i);

		string ans;

		for (int I = 0; I < 4; ++I) {


			vvi res(5);

			for (auto& cand : cands) {
				cout << (cand - 1) * 5 + 1 + I << endl;
				char c;
				cin >> c;
				int x = c - 'A';
				res[x].pb(cand);
			}

			for (int i = 0; i < 5; ++i) {
				bool used = false;
				char c = i + 'A';
				for (auto& cc : ans) {
					if (cc == c)
						used = true;
				}
				if (used)
					continue;
				if (sz(res[i]) == want[I]) {
					cands = res[i];
					ans += c;
					break;
				}
			}


		}

		for (int i = 0; i < 5; ++i) {
			bool used = false;
			char c = i + 'A';
			for (auto& cc : ans) {
				if (cc == c)
					used = true;
			}
			if (used)
				continue;
			ans += char(i + 'A');
			break;
		}

		cout << ans << endl;

		char good;

		cin >> good;

		if (good == 'N')
			exit(0);

	}

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
