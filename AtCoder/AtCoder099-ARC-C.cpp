/*
 * We want to know if it's possible to partition the vertices into
 * 2 sets A and B s.t. edges connecting 2 nodes in A form complete graph
 * of |A| nodes, and same for B. Consider the complement of our graph, if
 * there is an odd cycle in it, we know that at least 2 adjacent nodes on that cycle
 * will be in the same set, which implies there is no edge between them in
 * the original graph, would means there's no valid partitioning. Hence it is
 * necessary that the complement graph is bipartite. Now assuming it is, we can
 * for each component in that bipartite graph put it's left part in A and right in B
 * or left in B and right in A, in both cases we'd get a valid partitioning. There's
 * no other way to partition each individual component. Now we also want to minimize
 * |A| * (|A|-1) / 2 + |B| * (|B|-1) / 2, for that we use dynamic programming to try
 * all the choices for each component in our bipartite graph, since components are
 * independent.
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

const int MX = 705;
int N, M, mem[MX][MX << 1];
VV<bool> adj;
vi color;
vi moves;
int cnt[2];

int dp(int idx, int diff) {
	if (idx == sz(moves)) {
		int i = (N + diff) / 2, j = N - i;
		return i * (i - 1) / 2 + j * (j - 1) / 2;
	}
	int& ret = mem[idx][diff + N];
	if (ret != -1)
		return ret;
	return ret = min(dp(idx + 1, diff + moves[idx]), dp(idx + 1, diff - moves[idx]));
}

void dfs(int u, int clr) {
	db(u, clr);
	++cnt[clr];
	color[u] = clr;

	for (int v = 1; v <= N; ++v) {
		if (u == v || !adj[u][v])
			continue;
		if (color[v] == color[u]) {
			cout << "-1\n";
			exit(0);
		}
		if (color[v] == -1)
			dfs(v, !clr);
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

	cin >> N >> M;

	adj.assign(N + 1, V<bool>(N + 1, 1));

	for (int i = 1; i <= M; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u][v] = adj[v][u] = 0;
	}


	color.assign(N + 1, -1);

	for (int i = 1; i <= N; ++i) {
		if (color[i] == -1) {
			cnt[0] = cnt[1] = 0;
			dfs(i, 0);
			moves.pb(cnt[0] - cnt[1]);
		}
	}

	memset(mem, -1, sizeof(mem));

	cout << dp(0, 0) << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
