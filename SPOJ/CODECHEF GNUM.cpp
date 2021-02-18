/*
 * This is a matching problem, we want to match pairs (i, j) with pairs (p, q)
 * Consider the bipartite graph where L is set of all valid pairs (i, j) (as described in the problem)
 * and R is the set of all valid (p, q). We then connect each node in L with a node in R if the gcd
 * of the 2 nodes is > 1, and after that we find maximum matching.
 * We can optimize this by 'flattening' all the primes within a number, that is, if a number has a prime factor
 * p raised to a power x, we decrease that power to be 1. This is valid because for gcd of 2 numbers being > 1
 * the 2 numbers need to share only one prime factor.
 * Another optimization is compress each side so that its size is equal to the number of distinct values of gcd.
 *
 * Solutions with this level of optimization seem to pass, but there's one more optimization we can do
 * We will have a central layer of all the prime factors we encounter (their count is ~ 9 * 2 N because each number
 * can have at most 9 prime factors). And for each gcd of (i, j), we will connect it to only primes that divide it
 * (which is again <= 9) with infinite edge capacity. We do the same for gcd's of (p, q) but reversing the direction.
 * Now the number of edges in our graph is roughly 9 * #gcd's we can get ~= 10*N^2
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
	for (const char& c : s) o << c; return o;
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

const int MX = 32000;
vi primes;
bool prime[MX];
VV<pii> adj;

struct Dinic {
	int n, src, snk, visCnt;
	vi vis, cap, d;
	V<vii> adjAug;
	void addAugEdge(int u, int v, int c, int cRev = 0) {
		adjAug[u].pb(mp(v, sz(cap)));
		cap.pb(c);
		adjAug[v].pb(mp(u, sz(cap)));
		cap.pb(cRev);	// c if graph is undirected
	}
	void init() {
		visCnt = 0;
		vis.assign(n + 1, 0);
		d.assign(n + 1, OO);
		cap.clear();
		adjAug.assign(n + 1, vii());
		for (int u = 0; u < n; ++u) // make sure u, n are appropriate values
			for (auto& pr : adj[u]) {
				int v = pr.fs, c = pr.sc;
				addAugEdge(u, v, c);
			}
	}
	Dinic(int _n) : n(_n)  {}
	int getFlow(int _src, int _snk) {
		init();
		src = _src, snk = _snk;
		int flow = 0, fAdd;
		while (bfs()) {
			do {
				++visCnt;
				fAdd = dfs(src, OO);
				flow += fAdd;
			} while (fAdd);
		}
		return flow;
	}
	int dfs(int u, int mn) {
		vis[u] = visCnt;
		if (u == snk)
			return mn;
		for (auto& pr : adjAug[u]) {
			int v = pr.fs, edge = pr.sc, edgeCap = cap[edge];
			if (vis[v] != visCnt && edgeCap && d[v] == d[u] + 1) {
				int f = dfs(v, min(mn, edgeCap));
				if (f) {
					cap[edge] -= f;
					cap[edge ^ 1] += f;
					return f;
				}
			}
		}
		return 0;
	}
	bool bfs() {
		vis[src] = ++visCnt;
		queue<int> q;
		q.push(src);
		d[src] = 0;
		while (!q.empty()) {
			int u = q.front();
			if (u == snk)
				return 1;
			q.pop();
			for (auto& pr : adjAug[u]) {
				int v = pr.fs, edge = pr.sc, edgeCap = cap[edge];
				if (edgeCap)
					if (vis[v] != visCnt)
						q.push(v), d[v] = d[u] + 1, vis[v] = visCnt;
			}
		}
		return 0;
	}
};


int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	for (int i = 2; i < MX; ++i) {
		prime[i] = 1;
	}

	for (int i = 2; i < MX; ++i) {
		if (prime[i]) {
			for (int j = 2 * i; j < MX; j += i)
				prime[j] = 0;
			primes.pb(i);
		}
	}



	db(sz(primes));

	int T;

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		int N;

		cin >> N;

		vi A(N), B(N);

		for (int i = 0; i < N; ++i) {
			cin >> A[i];
		}

		for (int i = 0; i < N; ++i) {
			cin >> B[i];
		}

		auto Atmp = A, Btmp = B;

		si P;

		vvi pA(N), pB(N);

		for (auto &p : primes) {
			for (int i = 0; i < N; ++i) {
				if (A[i] % p == 0) {
					P.insert(p);
					while (A[i] % p == 0)
						A[i] /= p;
					pA[i].pb(p);
				}
				if (B[i] % p == 0) {
					P.insert(p);
					while (B[i] % p == 0)
						B[i] /= p;
					pB[i].pb(p);
				}
			}
		}

		for (int i = 0; i < N; ++i) {
			if (A[i] > 1)
				P.insert(A[i]), pA[i].pb(A[i]), A[i] = 1;
			if (B[i] > 1)
				P.insert(B[i]), pB[i].pb(B[i]), B[i] = 1;
			for (auto &p : pA[i]) {
				A[i] *= p;
			}
			for (auto &p : pB[i]) {
				B[i] *= p;
			}
		}

		db(A, B);
		db(Atmp, Btmp);

		mii pToInt;

		int m = 0;

		for (auto &elem : P) {
			pToInt[elem] = m++;
		}

		mii L, R;
		map<int, vi> gP;



		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				int g = __gcd(A[i], B[j]);
				if (g > 1 && Atmp[i] != Btmp[j]) {
					if (Btmp[j] > Atmp[i])
						++L[g];
					if (Btmp[j] < Atmp[i])
						++R[g];
					if (!present(gP, g))
						for (auto &p : pA[i]) {
							if (B[j] % p == 0)
								gP[g].pb(pToInt[p]);
						}
				}
			}
		}

		db(L, R);



		int n = m;
		int src = n++, dest = n++;

		adj.assign(n, vii());

		for (auto &pr : L) {
			int u = n++;
			adj.pb(vii());
			for (auto &elem : gP[pr.fs]) {
				adj[u].pb(mp(elem, 400 * 400));
			}
			adj[src].pb(mp(u, pr.sc));
		}
		for (auto &pr : R) {
			int u = n++;
			adj.pb(vii());
			for (auto &elem : gP[pr.fs]) {
				adj[elem].pb(mp(u, 400 * 400));
			}
			adj[u].pb(mp(dest, pr.sc));
		}

		Dinic dinic(n);

		cout << dinic.getFlow(src, dest) << '\n';

	}

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
