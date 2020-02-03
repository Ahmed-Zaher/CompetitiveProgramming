/*
 * Let's take the given string and convert it
 * to an expression tree, and now consider the answer
 * which is all characters on the max depth from left
 * to right then next max depth and so on..
 * We can prove that when we run the queue algorithm with this answer,
 * every operator when encountered, will find exactly its appropriate operands
 * in the front of the queue
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

const int MX = 10005;
V<char> lvl[MX];
int L[MX], R[MX];
int N;
string s;

void dfs(int u, int lv) {
	lvl[lv].pb(s[u]);
	if (L[u] != -1) {
		dfs(L[u], lv + 1);
		dfs(R[u], lv + 1);
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

	int T;

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		cin >> s;
		stack<int> st;
		N = 0;
		for (auto& c : s) {
			if (isupper(c)) {
				int a, b;
				a = st.top();
				st.pop();
				b = st.top();
				st.pop();
				L[N] = a;
				R[N] = b;
			} else {
				L[N] = R[N] = -1;
			}

			st.push(N++);
		}

		for (int i = 0; i < N; ++i) {
			lvl[i].clear();
		}


		dfs(st.top(), 0);

		for (int i = N - 1; i >= 0; --i) {
			for (auto& c : lvl[i]) {
				cout << c;
			}
		}
		ndl;

	}

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
