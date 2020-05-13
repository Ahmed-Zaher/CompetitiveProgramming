/*
 * Let cnt[u][0] be the number ways to partition leafs in subtree of u,
 * knowing that no f(L) of that partition will have an edge that goes
 * from u to parent[u].
 * Let cnt[u][1] denote the same thing but we connect u to parent[u]
 * Denote children of u as v_0, v_1, .., v_m-1
 * To compute cnt[u][0], we can first not include u in any f(L) at all,
 * hence cnt[u][0] += product of cnt[v_i][0], otherwise if node u is contained
 * in some f(L) and we know that there's no edge from u to parent[u], hence
 * we must joint at least 2 f(L)'s belonging to 2 different children of u
 * So cnt[u][0] += (product of cnt[v_i][0]) * (product of cnt[v_j][1]),
 * where i is a number of S, j is a number of T, S union T = {0,1,..,m-1},
 * S and T are disjoint, and |T| >= 2. We can use similar analysis for cnt[u][1]
 * With a little simplification we find that
 * cnt[u][0] = sum of (product of cnt[v_i][b_i] for i in [0,m)) over all binary
 * strings b of m bits where b has #ones = 0 or 2 or more
 * cnt[u][1] is the same but the sum is over all binary strings with #ones >= 1
 * We compute this by noting that our sum of products over all binary strings
 *  = product of (cnt[v_i][0] + cnt[v_i][1]) for i in [0,m)
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

const int MOD = 998244353, MX = 2e5 + 5;
int n;
vi p;
vvi adj, cnt;

ll inv(ll a, ll b) {
	return 1 < a ? b - inv(b % a, a) * b / a : 1;
}

void dfs(int u) {
	if (sz(adj[u]) == 1 && u != 1)
		return;
	int m = 0, prod0ones = 1, totProd = 1;
	for (auto& v : adj[u]) {
		if (v != p[u]) {
			++m;
			dfs(v);
			prod0ones = (prod0ones * 1LL * cnt[v][0]) % MOD;
			totProd = (totProd * 1LL * (cnt[v][0] + cnt[v][1])) % MOD;
		}
	}
	int totProd1one = 0;
	for (auto& v : adj[u]) {
		if (v != p[u]) {
			int x = (prod0ones * 1LL * inv(cnt[v][0], MOD)) % MOD;
			x = (x * 1LL * cnt[v][1]) % MOD;
			totProd1one = (totProd1one + x) % MOD;
		}
	}
	cnt[u][0] = (totProd - totProd1one + MOD) % MOD;
	cnt[u][1] = (totProd - prod0ones + MOD) % MOD;
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
	adj.assign(n + 1, vi());
	p.assign(n + 1, -1);
	cnt.assign(n + 1, vi(2, 1));

	for (int i = 2; i <= n; ++i) {
		cin >> p[i];
		adj[i].pb(p[i]);
		adj[p[i]].pb(i);
	}

	dfs(1);

	cout << cnt[1][0] << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
