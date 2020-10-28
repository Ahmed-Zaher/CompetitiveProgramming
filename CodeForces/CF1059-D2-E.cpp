/*
 * We can solve this problem using a greedy approach
 * Let's call the deepest node of a vertical path a good node
 * Let's find the set of good nodes by considering nodes in order of decreasing depth
 * We clearly have leaves in our answer. Once we mark a node as good, we greedily mark how much we can go
 * up the tree while abiding the S and L constraints, then for an internal node, we find whether
 * it can be reached by any good node in its subtree, if yes then we assign it to the path that can go
 * up the most. Otherwise we must mark this node as good and add it to our answer.
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
#else
#define db(...) true
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

const int MX_LG = 19;
int n, L;
ll S;
vvi adj;
vi w, lvl, y, p[MX_LG];
V<ll> s[MX_LG];


int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> L >> S;

	adj.assign(n + 1, vi());
	w.assign(n + 1, 0);
	lvl.assign(n + 1, 0);
	y.assign(n + 1, 0);

	for (int i = 0; i < MX_LG; ++i) {
		p[i].assign(n + 1, -1);
		s[i].assign(n + 1, 0);
	}

	for (int i = 1; i <= n; ++i) {
		cin >> w[i];
	}

	for (int i = 2; i <= n; ++i) {
		int pi;
		cin >> pi;
		adj[i].pb(pi);
		adj[pi].pb(i);
	}

	for (int i = 1; i <= n; ++i) {
		if (w[i] > S) {
			cout << "-1\n";
			return 0;
		}
	}

	if (n == 1) {
		cout << "1\n";
		return 0;
	}

	{
		function<void(int, int, int)> dfs = [&](int u, int par, int dep) {
			p[0][u] = par;
			s[0][u] = w[u];
			for (int i = 1; i < MX_LG; ++i) {
				s[i][u] = s[i - 1][u];
				if (p[i - 1][u] != -1) {
					p[i][u] = p[i - 1][p[i - 1][u]];
					s[i][u] += s[i - 1][p[i - 1][u]];
				}
			}
			lvl[u] = dep;
			for (auto &v : adj[u]) {
				if (v != par)
					dfs(v, u, dep + 1);
			}
		};

		dfs(1, -1, 0);
	}

	int ans = 0;

	{


		function<int(int)> get = [&](int u) {
			if (L == 1)
				return lvl[u];
			int A = u;
			for (int i = MX_LG - 1; i >= 0; --i) {
				if (A == -1)
					break;
				if ((L - 1) & (1 << i))
					A = p[i][A];
			}
			int B = u;
			ll left = S;
			for (int i = MX_LG - 1; i >= 0; --i) {
				if (B == -1)
					break;
				if (s[i][u] <= left)
					left -= s[i][u], B = p[i][B];
			}
			db(u, A, B);
			if (A == -1 && B == -1)
				return 0;
			if (A == -1)
				return lvl[B] + 1;
			if (B == -1)
				return lvl[A];
			if (lvl[A] > lvl[B] + 1)
				return lvl[A];
			return lvl[B] + 1;
		};

		function<void(int, int)> dfs = [&](int u, int par) {
			db(u, par);
			if (sz(adj[u]) == 1 && u != 1)
				++ans, y[u] = get(u);
			else {
				int mnChY = OO;
				for (auto &v : adj[u]) {
					if (v != par) {
						dfs(v, u);
						mnChY = min(mnChY, y[v]);
					}
				}
				db(mnChY);
				if (mnChY == lvl[u] + 1)
					++ans, y[u] = get(u);
				else
					y[u] = mnChY;
			}

		};

		dfs(1, -1);

		db(y);
	}

	cout << ans << '\n';


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
