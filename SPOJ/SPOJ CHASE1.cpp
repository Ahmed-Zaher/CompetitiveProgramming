/*
 * We can show that if at the beginning of a turn A is on a node that
 * belongs to some cycle, then the answer is no
 *
 * The idea of a node belonging to a cycle relates to articulation points,
 * biconnected components, and block-cut trees
 * Note that a biconnected component of size > 1 has all nodes in it belonging to some cycle
 * Now think of a block-cut tree where the root component is the one including B
 * First note that A can reach a node belonging to a cycle if there a node u
 * that is contained in a cycle and is strictly closer to A than B
 * Otherwise, that means that A is included in a 1-node component containing
 * only node A, and in sub tree of A, there're only 1-node components
 * This means that if A goes down the tree he'll eventually be caught
 * Also if he tried to go up to reach the nearest biconnected component of size > 1,
 * he'll also be caught as B can catch A before that happens
 * So A's only options are to pick the node that maximize the number of turns,
 * That will be any 1-component node that A can reach before B
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

int n, m, a, b;
vvi adj;
vi vis, dA, dB;

vi bfs(int s) {
	vi dist(n + 1, OO);
	queue<int> q;
	q.push(s);
	dist[s] = 0;
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		for (auto& v : adj[u])
			if (dist[v] == OO)
				q.push(v), dist[v] = dist[u] + 1;
	}
	return dist;
}

void dfs(int u) {
	vis[u] = true;
	for (auto& v : adj[u]) {
		if (!vis[v])
			dfs(v);
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
		cin >> n >> m >> a >> b;
		adj.assign(n + 1, vi());

		set<pii> edges;

		for (int i = 1; i <= m; ++i) {
			int u, v;
			cin >> u >> v;
			if (u > v)
				swap(u, v);
			assert(!present(edges, mp(u, v)));
			edges.insert(mp(u, v));
			adj[u].pb(v);
			adj[v].pb(u);
		}

		dA = bfs(a);
		dB = bfs(b);

		db(dA, dB);

		V<bool> inCycle(n + 1, 0);

		for (int u = 1; u <= n; ++u) {
			vis.assign(n + 1, 0);
			vis[u] = 1;
			for (auto& v : adj[u]) {
				if (vis[v]) {
					inCycle[u] = true;
					break;
				}
				dfs(v);
			}
		}
		bool no = false;

		for (int u = 1; u <= n; ++u) {
			if (inCycle[u] && dA[u] < dB[u])
				no = true;
		}

		if (no)
			cout << "No\n";
		else {
			vis.assign(n + 1, 0);
			int ans = 0;
			for (int u = 1; u <= n; ++u) {
				if (dA[u] < dB[u])
					ans = max(ans, dB[u]);
			}
			cout << ans << '\n';
		}
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
