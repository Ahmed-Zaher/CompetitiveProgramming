/*
 * We can solve this in O(q + nm) time by modeling the situation as a tree
 * The tree has q+1 nodes label from 0 to q, node with label i will hold the sum of the books
 * after executing the i'th query. For every query, we create a new node corresponding to
 * that query and connect it to some parent, for queries of type 1, 2, 3 the parent will
 * be i - 1, otherwise if the type is 4, the parent will be k. Now in this tree, the sum
 * of books at any node can be computed by accumulating all the queries in the path from
 * the root of the tree to that node. So now all we need to do is DFS on the tree, whenever
 * we visit a node, we add its effect to the path from the root to the parent, and before
 * leaving a node we undo that effect.
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

struct Query {
	int t, i, j;
	Query() : t(-1), i(0), j(0) {}
};

int n, m, q;
VV<int> shelf;
V<int> flag;
vi ones, ans;
int sum;
V<Query> p;
vvi adj;


int add(Query q) {
	int i = q.i, j = q.j;
	if (q.t == 1)
		if (!(shelf[i][j] ^ flag[i])) {
			shelf[i][j] ^= 1, ++ones[i], ++sum;
			return 1;
		}
	if (q.t == 2)
		if (shelf[i][j] ^ flag[i]) {
			shelf[i][j] ^= 1, --ones[i], --sum;
			return 2;
		}
	if (q.t == 3) {
		sum -= ones[i];
		ones[i] = m - ones[i];
		sum += ones[i];
		flag[i] ^= 1;
		return 3;
	}
	return -1;

}

void dfs(int u) {
	int ch = add(p[u]);
	ans[u] = sum;

	for (auto& v : adj[u])
		dfs(v);

	int i = p[u].i, j = p[u].j;

	if (ch == 1)
		shelf[i][j] ^= 1, --ones[i], --sum;

	if (ch == 2)
		shelf[i][j] ^= 1, ++ones[i], ++sum;

	if (ch == 3) {
		flag[i] ^= 1;
		sum -= ones[i];
		ones[i] = m - ones[i];
		sum += ones[i];
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

	cin >> n >> m >> q;

	shelf.assign(n + 1, V<int>(m + 1, 0));
	adj.assign(q + 1, vi());
	p.assign(q + 1, Query());
	flag.assign(n + 1, 0);
	ones.assign(n + 1, 0);
	ans.assign(q + 1, 0);


	for (int qq = 1; qq <= q; ++qq) {
		int t;
		cin >> t;
		Query qr;
		qr.t = t;
		if (t <= 2) {
			int i, j;
			cin >> i >> j;
			qr.i = i, qr.j = j;
		}
		if (t == 3) {
			int i;
			cin >> i;
			qr.i = i;
		}
		if (t == 4) {
			int k;
			cin >> k;
			adj[k].pb(qq);
		} else
			adj[qq - 1].pb(qq);

		p[qq] = qr;
	}

	dfs(0);

	for (int i = 1; i <= q; ++i)
		cout << ans[i] << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
