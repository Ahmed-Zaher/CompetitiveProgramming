/*
 * We want for find for all S, T, the number of edges that lie on
 * a shortest path from S to T. Let the edge we consider be (i, j),
 * if that edge is used in a path from S to T then we take the path
 * S->i->j->T, now consider edges (i1, j), (i2, j)
 * If dist(S->i1->j) < dist(S->i2->j) then we know that (i2, j) can
 * never be in any shortest path from our S to any T, so for every j we store
 * min(dist(S->i->j)) over all i's, and store the count of i's which make that
 * min dist occur. Then iterate over all T's, for a given T try all j's,
 * If dist(S->i->j) + dist(j->T) (where dist(S->i->j) is minimum for fixed S, j)
 * gives dist(S, T) then we add the count of i's to C_S,T
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

int n, m;
vvi adjMat, dist;


int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> m;

	adjMat.assign(n + 1, vi(n + 1, OO));

	for (int i = 1; i <= n; ++i)
		adjMat[i][i] = 0;

	for (int i = 1; i <= m; ++i) {
		int u, v, w;
		cin >> u >> v >> w;
		adjMat[u][v] = adjMat[v][u] = w;
	}

	dist = adjMat;

	for (int w = 1; w <= n; ++w)
		for (int u = 1; u <= n; ++u)
			for (int v = 1; v <= n; ++v)
				if (dist[u][w] + dist[w][v] < dist[u][v])
					dist[u][v] = dist[u][w] + dist[w][v];


	vvi c(n + 1, vi(n + 1, 0));


	for (int S = 1; S <= n; ++S) {
		vi best(n + 1, OO), cnt(n + 1, 0);
		for (int j = 1; j <= n; ++j)
			for (int i = 1; i <= n; ++i)
				if (i != j && dist[S][i] != OO && adjMat[i][j] != OO)
					best[j] = min(best[j], dist[S][i] + adjMat[i][j]);


		for (int j = 1; j <= n; ++j)
			for (int i = 1; i <= n; ++i)
				if (i != j && dist[S][i] != OO && adjMat[i][j] != OO)
					if (dist[S][i] + adjMat[i][j] == best[j])
						++cnt[j];

		for (int T = S + 1; T <= n; ++T)
			for (int j = 1; j <= n; ++j)
				if (dist[S][T] != OO && dist[j][T] != OO)
					if (best[j] == dist[S][T] - dist[j][T])
						c[S][T] += cnt[j];
	}

	for (int i = 1; i <= n; ++i)
		for (int j = i + 1; j <= n; ++j)
			cout << c[i][j] << ' ';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
