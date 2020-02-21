/*
 * Root the tree at 1, let dp(u, pT, pS) denote the minimal #servers
 * we need to by at the subtree of u, knowing that our parent is pT and pS
 * where pT is 1 if the parent is a server and 0 if it's a client, and pS is 1
 * if the parent is covered by some server and 0 if it's not.
 * Then it's a matter of case analysis - but the tricky case is pT = 0 and pS = 1
 * which means that u can't be a server because that would cover the parent twice,
 * and so we must depend on some child of v to cover u. And to do so we need to only
 * pick one such child which must be a server to cover u, while the other children
 * see their parent (u) as covered.
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

int n;
vvi adj;
int mem[10005][2][2];

int dp(int u, bool pT, bool pS, int p = -1) {
	if (n == 1)
		return 1;
	if (sz(adj[u]) == 1 && p != -1) {
		if (pT)
			return 0;
		if (pS)
			return 1e5;
		return 1;
	}

	int& ret = mem[u][pT][pS];

	if (ret != -1)
		return ret;

	int cnt[2][2];
	cnt[0][0] = cnt[0][1] = cnt[1][0] = cnt[1][1] = 0;

	for (auto& v : adj[u])
		if (v != p)
			for (int i = 0; i < 2; ++i)
				for (int j = 0; j < 2; ++j)
					cnt[i][j] += dp(v, i, j, u);


	int g = OO;

	for (auto& v : adj[u]) {
		if (v != p)
			g = min(g, cnt[0][1] - dp(v, 0, 1, u) + dp(v, 0, 0, u));
	}

	if (u == 1) {
		return ret = min(1 + cnt[1][1], g);
	}

	if (pT)
		return ret = min(1 + cnt[1][1], cnt[0][1]);

	if (pS)
		return ret = g;

	return ret = 1 + cnt[1][1];

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

	while (cin >> n && n > 0) {
		adj.assign(n + 1, vi());
		for (int i = 1; i <= n - 1; ++i) {
			int u, v;
			cin >> u >> v;
			adj[u].pb(v);
			adj[v].pb(u);
		}

		memset(mem, -1, sizeof(mem));

		cout << dp(1, 1, 1) << '\n';

		cin >> n;
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
