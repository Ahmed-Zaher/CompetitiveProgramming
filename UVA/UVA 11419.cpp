/*
 * This is a classical minimum vertex cover problem, which has the same
 * size as maximum matching for which we can using flows or Hopcroft Karp
 * The new thing here is that we need to build actual the vertex cover, which
 * can be done using the maximum matching
 * This tutorial explains all of that:
 * https://tryalgo.org/en/matching/2016/08/05/konig/
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


struct HopcroftKarp {
	int n, m;
	vi matchL, matchR, dist;
	vvi g;

	HopcroftKarp(int _n, int _m) :
		n(_n), m(_m), matchL(_n + 1), matchR(_m + 1), dist(_n + 1), g(_n + 1) {}

	void addEdge(int u, int v) {
		g[u].push_back(v);
	}

	bool bfs() {
		queue<int> q;
		for (int u = 1; u <= n; u++) {
			if (!matchL[u]) {
				dist[u] = 0;
				q.push(u);
			} else
				dist[u] = OO;
		}

		dist[0] = OO;

		while (!q.empty()) {
			int u = q.front();
			q.pop();
			for (auto v : g[u]) {
				if (dist[matchR[v]] == OO) {
					dist[matchR[v]] = dist[u] + 1;
					q.push(matchR[v]);
				}
			}
		}

		return dist[0] != OO;
	}

	bool dfs(int u) {
		if (!u)
			return true;
		for (auto v : g[u])
			if (dist[matchR[v]] == dist[u] + 1 && dfs(matchR[v])) {
				matchL[u] = v;
				matchR[v] = u;
				return true;
			}

		dist[u] = OO;
		return false;
	}

	int getMaxMatching() {
		int matching = 0;
		while (bfs()) {
			for (int u = 1; u <= n; u++) {
				if (!matchL[u])
					if (dfs(u))
						matching++;
			}
		}
		return matching;
	}
	// Returns nodes of MVC, assuming getMaxMatching have been called
	// ret.fs are nodes from L in MVC, ret.sc are nodes from R in MVC
	// works in O(|V| + |E|)

	V<bool> visL, visR;

	pair<vi, vi> buildMVC() {
		pair<vi, vi> ret;


		visL.assign(n + 1, 0);
		visR.assign(m + 1, 0);

		for (int i = 1; i <= n; ++i)
			if (!matchL[i] && !visL[i])
				dfsMVC(i, 0);

		for (int i = 1; i <= n; ++i)
			if (!visL[i])
				ret.fs.pb(i);

		for (int i = 1; i <= m; ++i)
			if (visR[i])
				ret.sc.pb(i);
		return ret;
	}
	void dfsMVC(int u, bool b) {
		if (!b) {
			visL[u] = true;

			for (auto& v : g[u])
				if (!visR[v] && v != matchL[u])
					dfsMVC(v, !b);
		} else {
			visR[u] = true;

			if (matchR[u] && !visL[matchR[u]])
				dfsMVC(matchR[u], !b);
		}
	}
};



int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int R, C, N;


	while (cin >> R >> C >> N && (R + C + N)) {

		HopcroftKarp hk(R, C);

		for (int i = 1; i <= N; ++i) {
			int r, c;
			cin >> r >> c;
			hk.addEdge(r, c);
		}
		cout << hk.getMaxMatching();
		auto ans = hk.buildMVC();

		for (auto& i : ans.fs)
			cout << " r" << i;

		for (auto& i : ans.sc)
			cout << " c" << i;

		ndl;
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
