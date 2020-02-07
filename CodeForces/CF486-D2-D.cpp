/*
 * Let's assume that node u is the one with min ai and do dfs from u
 * and expand the explored subtree if the new node v has:
 * 1. av >= au
 * 2. av - au <= d
 * And now the number of sets where min ai = au is the number of
 * subtrees of u that lie within the explored component
 * There is one more thing we need to do to make sure that sets with
 * multiple nodes having min ai are counted exactly once: which is to
 * to store ai as a pair: ai_new := (ai, i) and use that to expand our component
 * That way, for any S with many nodes having same min ai, it'll only
 * be counted when exploring the node with min ai_new, which is unique
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
int n, d;
vi dp;
vii a;
vvi adj;

void dfs(int u, int root, int p) {

	if (a[u] < a[root] || a[u].fs - a[root].fs > d)
		return;

	dp[u] = 1;

	for (auto& v : adj[u]) {
		if (v != p) {
			dfs(v, root, u);
			dp[u] = (dp[u] * 1LL * (dp[v] + 1)) % MOD;
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

	cin >> d >> n;

	a.assign(n + 1, mp(0, 0));
	adj.assign(n + 1, vi());

	for (int i = 1; i <= n; ++i) {
		cin >> a[i].fs;
		a[i].sc = i;
	}

	for (int i = 1; i <= n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
	}

	ll ans = 0;

	for (int i = 1; i <= n; ++i) {
		dp.assign(n + 1, 0);
		dfs(i, i, -1);
		ans = (ans + dp[i]) % MOD;
	}

	cout << ans << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
