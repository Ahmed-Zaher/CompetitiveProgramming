/*
 * Let's first find a greedy matching, for every edge (u, v),
 * if neither of them is an end point of some edge in the matching,
 * we add (u, v) to our matching
 * Now we can divide our 3n nodes into 2 classes,
 * the first class (let it be C1) are isolated nodes and nodes that aren't an end point
 * of any matching edge. This class of nodes forms and independent set, if
 * we assume two of them are incident we would contradict the maximality of
 * our greedy matching.
 * The other class (let it be C2) are all nodes that are an end point of some matching edge,
 * it's clear that each node in C2 is an end point of exactly one matching edge, and hence
 * the size of our matching is |C2| / 2, it's also clear that each node belongs to exactly
 * one of the two classes
 * Now if |C1| >= n then we can pick n nodes from C1 and form and independent set of n nodes,
 * otherwise, |C1| < n => |C2| = 3n - |C1| > 2n, and so our matching has |C2| / 2 > 2n/n = n
 * edges and now we have a matching of n edges
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

	int T, n, m;

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		cin >> n >> m;

		vii edges(m + 1);

		for (int i = 1; i <= m; ++i) {
			int u, v;
			cin >> u >> v;
			edges[i] = mp(u, v);
		}

		V<bool> busy(3 * n + 1, 0);

		vi M;

		for (int i = 1; i <= m; ++i) {
			int u = edges[i].fs, v = edges[i].sc;

			if (busy[u] || busy[v])
				continue;

			busy[u] = busy[v] = 1;
			M.pb(i);
		}

		vi IS;

		for (int i = 1; i <= 3 * n; ++i)
			if (!busy[i])
				IS.pb(i);

		if (sz(IS) >= n) {
			cout << "IndSet\n";
			for (int i = 0; i < n; ++i)
				cout << IS[i] << ' ';
			ndl;
		} else {
			cout << "Matching\n";
			for (int i = 0; i < n; ++i)
				cout << M[i] << ' ';
			ndl;
		}

	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
