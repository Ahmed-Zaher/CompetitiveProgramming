/*
 * We want a spanning tree T of the graph such that
 * (max g_e for e in T) * G + (max s_e for e in T) * S is minimum
 * Let's first multiply all g_e by G and all s_e by S
 * Now we want to minimize:
 * (max g_e for e in T) + (max s_e for e in T)
 * Now let's try all possible max g_e, if current max g is g_max, then
 * we find the subgraph with edge that gave g_e <= g_max, and set
 * the weight of each edge to be s_e. We can prove that to find minimum
 * maximum weight in that graph we can find an MST and pick its max.
 * weight edge. We can do this in O(nlogn * m) by constructing the MST
 * dynamically. If a new edge connects two nodes that weren't connected before,
 * we add it to our MST, if it connects two already connected nodes, we find
 * max weight edge on the path between those 2 nodes in MST, if its weight is
 * bigger than the weight of the new edge then we remove the old edge and replace
 * it with the new one.
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


struct Edge {
	int u, v;
	ll g, s;

	bool operator<(Edge& oth) {
		return g < oth.g;
	}

};

int n, m, G, S, cnt;
V<set<pair<int, ll>>> tree;
V<Edge> edges;
int dest;
pair<pii, ll> rmv;
ll ss;

bool dfs(int u, int p) {
	if (u == dest)
		return 1;
	int ret = 0;
	for (auto& pr : tree[u]) {
		int v = pr.fs;
		ll w = pr.sc;
		if (v != p)
			if (dfs(v, u)) {
				if (w > rmv.sc)
					rmv = mp(mp(u, v), w);
				ret = 1;
			}
	}
	return ret;
}

void getMaxW(int u, int p) {
	++cnt;

	for (auto& pr : tree[u]) {
		ss = max(ss, pr.sc);

		if (pr.fs != p)
			getMaxW(pr.fs, u);
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

	cin >> n >> m >> G >> S;

	tree.assign(n + 1, set<pair<int, ll>>());
	edges.assign(m, Edge());

	for (int i = 0; i < m; ++i) {
		cin >> edges[i].u >> edges[i].v >> edges[i].g >> edges[i].s;

		edges[i].g *= G;
		edges[i].s *= S;
	}

	sort(edges.begin(), edges.end());

	ll ans = LLONG_MAX;


	for (auto& edge : edges) {
		ll gg = edge.g;
		int u = edge.u, v = edge.v;
		ll s = edge.s;

		if (u == v)
			continue;

		rmv.sc = -1;

		dest = v;

		dfs(u, -1);

		db(u, v, rmv);

		if (rmv.sc == -1)
			tree[u].insert(mp(v, s)), tree[v].insert(mp(u, s));
		else if (rmv.sc > s){
			int uRmv = rmv.fs.fs, vRmv = rmv.fs.sc;
			ll wRmv = rmv.sc;

			tree[uRmv].erase(mp(vRmv, wRmv));
			tree[vRmv].erase(mp(uRmv, wRmv));

			tree[u].insert(mp(v, s));
			tree[v].insert(mp(u, s));

		}

		db(tree);

		cnt = 0;
		ss = -1;
		getMaxW(1, -1);

		db(gg, ss, cnt);

		if (cnt == n)
			ans = min(ans, gg + ss);

	}

	if (ans == LLONG_MAX)
		cout << "-1\n";
	else
		cout << ans << "\n";


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
