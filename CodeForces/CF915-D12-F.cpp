/*
 * We can solve the problem if we know how to count for each node u,
 * how many paths go through it have max a_i = a_u. To break ties, let's
 * we can consider the node with smaller index as smaller. Let's process
 * the nodes in order of increasing a_u, the desired paths either have
 * one end at u and the other reachable by taking a path from u passing through
 * one of it's neighbor, or have both of them reachable via a path from u, and in
 * that case the 2 paths must go through 2 distinct neighbors of u. And that's
 * the idea, we'll find for each neighbor of u how much nodes v s.t. a_u > a_v
 * we can reach from that neighbor without encountering a node w with a_w > a_u
 * And use that to count the paths. This can be done by using union find and merging
 * current node with the set of its neighbor if that neighbor has smaller a_i
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
vii a;
V<ll> cnt;

struct UnionFind {
	vi p, rank, setSize;
	int numSets;
	UnionFind(int n) {
		setSize.assign(n, 1);
		numSets = n;
		rank.assign(n, 0);
		p.assign(n, 0);
		for (int i = 0; i < n; ++i)
			p[i] = i;
	}
	int findSet(int i) {
		return (p[i] == i) ? i : (p[i] = findSet(p[i]));
	}
	bool isSameSet(int i, int j) {
		return findSet(i) == findSet(j);
	}
	void unionSet(int i, int j) {
		if (!isSameSet(i, j)) {
			--numSets;
			int x = findSet(i), y = findSet(j);
			if (rank[x] > rank[y]) {
				p[y] = x;
				setSize[x] += setSize[y];
			} else {
				p[x] = y;
				setSize[y] += setSize[x];
				if (rank[x] == rank[y])
					++rank[y];
			}
		}
	}
	int numDisjointSets() {
		return numSets;
	}
	int sizeOfSet(int i) {
		return setSize[findSet(i)];
	}
};


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

	a.assign(n + 1, pii());

	for (int i = 1; i <= n; ++i) {
		cin >> a[i].fs;
		a[i].sc = i;
	}

	adj.assign(n + 1, vi());

	for (int i = 1; i <= n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
	}

	sort(a.begin() + 1, a.end());

	cnt.assign(n + 1, 0);

	{
		UnionFind UF(n + 1);
		vii mx(n + 1);

		for (int i = 1; i <= n; ++i)
			mx[a[i].sc] = a[i];


		for (int i = 1; i <= n; ++i) {

			int u = a[i].sc;

			for (auto& v : adj[u]) {
				int s1 = UF.findSet(u);
				int s2 = UF.findSet(v);

				if (mx[s2] < a[i]) {
					cnt[u] += (UF.setSize[s1] - 1) * 1LL * UF.setSize[s2] + UF.setSize[s2];
					mx[s1] = mx[s2] = a[i];
					UF.unionSet(s1, s2);
				}
			}
		}
	}


	{
		UnionFind UF(n + 1);
		vii mn(n + 1);

		for (int i = 1; i <= n; ++i)
			mn[a[i].sc] = a[i];

		for (int i = n; i >= 1; --i) {

			int u = a[i].sc;

			for (auto& v : adj[u]) {
				int s1 = UF.findSet(u);
				int s2 = UF.findSet(v);

				if (mn[s2] > a[i]) {
					cnt[u] -= (UF.setSize[s1] - 1) * 1LL * UF.setSize[s2] + UF.setSize[s2];
					mn[s1] = mn[s2] = a[i];
					UF.unionSet(s1, s2);
				}
			}
		}
	}

	ll ans = 0;

	for (int i = 1; i <= n; ++i) {
		ans += cnt[a[i].sc] * 1LL * a[i].fs;
	}

	db(ans);

	cout << ans << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
