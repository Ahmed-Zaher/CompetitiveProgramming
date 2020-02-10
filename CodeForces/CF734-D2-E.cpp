/*
 * Let's first compress the input graph, for all connected
 * sub-graphs of the tree with all nodes having the same color,
 * compress them to a single node having that color - this has 0 cost.
 *
 * Now we have a new graph with the same type of operation available.
 * Consider the 2 ends of the diameter of the resulting graph,
 * the number of color changes along that path equals the diameter of the tree,
 * and can decrease by at most two in one move (like changing 010 => 000).
 * So we have a lower bound on the answer which is ceil(diameter / 2).
 * Now consider starting at the center of the tree, change its color repeatedly,
 * After the i'th change, the same-color-component including the center now has all nodes
 * that are i edges away, and after exactly ceil(diameter / 2) we'd have covered
 * the whole tree, using the minimum number of operations.
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

int n;
vi c;
vvi adj;

pii dfs(int u, int par) {
	int mxH[] = {0, 0, 0}, diam = 0;

	for (auto& v : adj[u])
		if (v != par) {
			pii pr = dfs(v, u);
			diam = max(diam, pr.fs);
			mxH[0] = pr.sc + (c[u] != c[v]);
			sort(mxH, mxH + 3);
		}

	diam = max(diam, mxH[1] + mxH[2]);

	return mp(diam, mxH[2]);
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

	cin >> n;

	c.assign(n + 1, 0);
	adj.assign(n + 1, vi());

	for (int i = 1; i <= n; ++i) {
		cin >> c[i];
	}

	for (int i = 1; i <= n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
	}


	cout << (dfs(1, -1).fs + 1) / 2 << '\n';



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
