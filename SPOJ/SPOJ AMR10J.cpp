/*
 * Let's make a graph out of the given (i, C[i]), we want to find
 * in how many ways we can colors each node with one of K colors s.t.
 * no two adjacent nodes have the same color
 * We note that this graph is special, if we follow the path i, C[i], C[C[i]], ..
 * We must eventually reach a node we encountered before and hence every connected
 * component of the graph has exactly one cycle (we can show that it's impossible to have
 * more than one cycle). And that cycle can be of length 2 which is a special case.
 * If the cycle has 2 nodes then be basically have a tree, we can color it in K * (K-1)^(N-1) ways
 * where N is the number of nodes in the tree. That's because we can take an arbitrary node as root
 * and color it in K ways, then expend that coloring, for each node it can take any of the K colors
 * except its parent's color, hence it has K-1 possible colors.
 *
 * Now let's assume we have a cycle of N >= 3 nodes, we can color it in K*(K-1)^(N-1) by starting
 * at some node and coloring it in one of K colors, then the next node has K-1 possible colors
 * and so on, the only invalid colorings we get are those where the color of the start node is
 * the same as the color of the end node which are the same as the colorings of a path of N nodes
 * where first and last nodes are of same color, so we find the answer for that (let it be ansP(N))
 * and subtract that from K*(K-1)^(N-1)
 * Suppose we color a path of N nodes in the same principle, we start with the first node, color it
 * with one of K colors (this determines the color of the last node) then color the next node until
 * we reach the N-1'th node, hence we have K*(K-1)^(N-2) colorings, but there are invalid colorings
 * among those, namely those that have the color of N-1'th node the same as last (N'th) node which
 * is already the same as the color of the first node. How many such colorings are there? ansP(N-1)
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


const int MOD = 1000000007;

ll power(ll num, ll exp) {
	ll ret = 1;
	while (exp) {
		if (exp & 1)
			ret = (ret * num) % MOD;
		num = (num * num) % MOD;
		exp >>= 1;
	}
	return ret;
}
ll add(ll x, ll y) {
    x += y;
    while(x >= MOD)
        x -= MOD;
    while(x < 0)
        x += MOD;
    return x;
}

ll mul(ll x, ll y) {
    return (x * y) % MOD;
}

int T, N, K;
vvi adj;
vi path, vis;
int cycleSz;
vi CC, c;

void dfs(int u, int p = -1) {
	if (vis[u]) {
		if (!cycleSz) {
			for (int i = 0; i < sz(path); ++i) {
				if (path[i] == u)
					cycleSz = sz(path) - i;
			}
		}
		return;
	}

	path.pb(u);
	CC.pb(u);

	vis[u] = true;

	for (auto& v : adj[u])
		if (v != p)
			dfs(v, u);

	path.pop_back();
}

int P(int n) {
	if (n == 2)
		return 0;
	return add(mul(K, power(K - 1, n - 2)), -P(n - 1));
}

int C(int n) {
	return add(mul(K, power(K - 1, n - 1)), -P(n));
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

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		cin >> N >> K;
		adj.assign(N, vi());
		c.assign(N, 0);
		for (int i = 0; i < N; ++i) {
			cin >> c[i];
			adj[i].pb(c[i]);
			adj[c[i]].pb(i);
		}

		vis.assign(N, 0);

		int ans = 1;


		for (int i = 0; i < N; ++i) {
			if (!vis[i]) {
				path.clear();
				CC.clear();
				cycleSz = 0;
				dfs(i);

				bool c2 = false;
				for (auto& u : CC)
					if (c[c[u]] == u)
						c2 = true;

				if (c2)
					ans = mul(ans, mul(K, power(K - 1, sz(CC) - 1)));
				else
					ans = mul(ans, mul(C(cycleSz), power(K - 1, sz(CC) - cycleSz)));
			}
		}

		cout << ans << '\n';
	}





	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
