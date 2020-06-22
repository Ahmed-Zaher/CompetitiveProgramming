/*
 * The modification to the tree is equivalent to saying that
 * in any path in the tree, I can go through 1 or 2 edges in one move
 * This means that our sum is
 * sum of ceil(dist(i, j) / 2), where dist(i, j) is the distance between
 * i and j in the original tree
 * We can computer sum of dist(i, j) using dp
 * But the problem is with the division by 2 because of the ceil,
 * Note that ceil(even / 2) = even / 2, ceil(odd / 2) = floor(odd / 2) + 1
 * We can partition the tree's nodes into 2 groups, those on odd depth and those on even depth
 * And using that we compute the sum of distances from current node to each of the
 * groups, one of the groups has its nodes of even distance from current node,
 * for those we add to the answer (sum of dist(i, j)) / 2
 * The other group has its nodes of odd distance from current nodes,
 * for those we add to the answer ((sum of dist(i, j)) + (# of nodes of odd distance)) / 2
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

const int MX = 2e5 + 5;
int n, cnt[MX][2];
vi adj[MX];
ll ans, tot[2];

void dfs0(int u, int p, int lvl) {
	int clr = lvl & 1;
	++cnt[u][clr];
	tot[clr] += lvl;
	for (auto& v : adj[u]) {
		if (v != p) {
			dfs0(v, u, lvl + 1);
			cnt[u][0] += cnt[v][0];
			cnt[u][1] += cnt[v][1];
		}
	}
}

void dfs(int u, int p, int lvl) {
	int clr = lvl & 1;
	ans += tot[clr] >> 1;
	ans += (tot[!clr] + cnt[1][!clr]) >> 1;
	db(u, p, lvl, tot[0], tot[1]);
	for (auto& v : adj[u]) {
		if (v != p) {
			tot[0] -= cnt[v][0];
			tot[1] -= cnt[v][1];
			tot[0] += (cnt[1][0] - cnt[v][0]);
			tot[1] += (cnt[1][1] - cnt[v][1]);
			dfs(v, u, lvl + 1);
			tot[0] += cnt[v][0];
			tot[1] += cnt[v][1];
			tot[0] -= (cnt[1][0] - cnt[v][0]);
			tot[1] -= (cnt[1][1] - cnt[v][1]);
		}
	}
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

	for (int i = 1; i <= n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
	}


	dfs0(1, -1, 0);
	dfs(1, -1, 0);

	cout << ans / 2 << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
