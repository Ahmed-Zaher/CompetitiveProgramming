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

int n, centroid;
vvi adj;
vi subtr, ans;

void dfs(int u, int p, bool b) {
	subtr[u] = 1;
	for (auto& v : adj[u]) {
		if (v != p) {
			dfs(v, u, b);
			subtr[u] += subtr[v];
		}
	}

	if (!b) {
		int mx = n - subtr[u];

		for (auto& v : adj[u]) {
			if (v != p)
				mx = max(mx, subtr[v]);
		}

		if (mx <= n / 2)
			centroid = u;
	}

}

void dfs2(int u, int p, int par, int mx) {
	int cc = n - subtr[u];
	if (cc - mx <= n / 2 || n - subtr[par] <= n / 2)
		ans[u] = 1;
	else
		ans[u] = 0;
	for (auto& v : adj[u]) {
		if (v != p)
			dfs2(v, u, par, mx);
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

	adj.assign(n + 1, vi());
	subtr.assign(n + 1, 0);

	for (int i = 1; i <= n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
	}

	dfs(1, -1, 0);

	db(centroid, subtr);

	dfs(centroid, -1, 1);

	db(centroid, subtr);

	ans.assign(n + 1, 1);

	set<pii> s;

	for (auto& v : adj[centroid]) {
		s.insert(mp(subtr[v], v));
	}

	for (auto& v : adj[centroid]) {
		s.erase(mp(subtr[v], v));
		dfs2(v, centroid, v, (s.empty() ? 0 : s.rbegin()->fs));
		s.insert(mp(subtr[v], v));
	}


	for (int i = 1; i <= n; ++i) {
		cout << ans[i] << " \n"[i == n];
	}

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
