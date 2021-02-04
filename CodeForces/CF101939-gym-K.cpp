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

int n;
vvi adj;
vi subTr, l, r, order;


struct SegmentTree {
	vi A;
	V<multiset<int>> t;
	int n;

	void build(int v, int tl, int tr) {
		if (tl == tr)
			t[v].insert(2 * subTr[order[tl]]);
		else {
			int tm = (tl + tr) >> 1;
			build(v << 1, tl, tm);
			build(v << 1 | 1, tm + 1, tr);
			auto valL = t[v << 1], valR = t[v << 1 | 1];
			for (auto &elem : t[v << 1]) {
				t[v].insert(elem);
			}
			for (auto &elem : t[v << 1 | 1]) {
				t[v].insert(elem);
			}
		}
	}
	int query(int v, int tl, int tr, int l, int r, int x) {
		if (l > r)
			return OO;
		if (l == tl && r == tr) {
			auto it = t[v].lower_bound(x);
			int ret = OO;
			if (it != t[v].end())
				ret = min(ret, abs(x - (*it)));
			if (it != t[v].begin())
				--it, ret = min(ret, abs(x - (*it)));
			return ret;
		}
		int tm = (tl + tr) >> 1;
		int valL = query(v << 1, tl, tm, l, min(r, tm), x);
		int valR = query(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r, x);
		return min(valL, valR);
	}
	void update(int v, int tl, int tr, int idx, bool b) {
		if (!b)
			t[v].erase(t[v].find(2 * subTr[order[idx]]));
		else
			t[v].insert(2 * subTr[order[idx]]);
		if (tl == tr) {

		} else {
			int tm = (tl + tr) >> 1;
			if (idx <= tm)
				update(v << 1, tl, tm, idx, b);
			else
				update(v << 1 | 1, tm + 1, tr, idx, b);
		}
	}
	void init(int _n) {
		n = _n;
		t.assign(n << 2, multiset<int>());
		build(1, 0, n - 1);
	}
	SegmentTree(int _n) {
		init(_n);
	}
	int query(int i, int j, int x) {
		return query(1, 0, n - 1, i, j, x);
	}
	void update(int idx, bool b) {
		update(1, 0, n - 1, idx, b);
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

	cin >> n;

	adj.assign(n + 1, vi());

	for (int i = 1; i <= n - 1; ++i) {
		int u, v;
		cin >> u >> v;
		adj[u].pb(v);
		adj[v].pb(u);
	}

	subTr.assign(n + 1, 0);

	l.assign(n + 1, 0);
	r.assign(n + 1, 0);
	{
		function<void(int, int)> dfs = [&](int u, int p) {
			subTr[u]++;
			l[u] = sz(order);
			order.pb(u);
			for (auto &v : adj[u]) {
				if (v != p)
					dfs(v, u), subTr[u] += subTr[v];
			}
			r[u] = sz(order) - 1;
		};
		dfs(1, -1);
	}

	SegmentTree ST(n);

	int ans = OO;

	{
		vi mnDiff(n + 1);
		function<void(int, int)> dfs = [&](int u, int p) {
			for (auto &v : adj[u]) {
				if (v != p) {
					if (subTr[v] > 1) {
						mnDiff[v] = ST.query(l[v] + 1, r[v], subTr[v]);
						int arr[3] = {n - subTr[v], (subTr[v] + mnDiff[v]) / 2, (subTr[v] - mnDiff[v]) / 2};
						sort(arr, arr + 3);
						ans = min(ans, arr[2] - arr[0]);
					}
					dfs(v, u);
				}
			}
		};
		dfs(1, -1);
	}

	{
		vi mnDiff(n + 1);
		function<void(int, int)> dfs = [&](int u, int p) {
			for (auto &v : adj[u]) {
				if (v != p) {
					ST.update(l[u], 0);
					if (n - subTr[v] > 1) {
						mnDiff[v] = OO;
						if (l[v])
							mnDiff[v] = min(mnDiff[v], ST.query(0, l[v] - 1, n - subTr[v]));
						if (r[v] < n - 1)
							mnDiff[v] = min(mnDiff[v], ST.query(r[v] + 1, n - 1, n - subTr[v]));
						int arr[3] = {subTr[v], (n - subTr[v] + mnDiff[v]) / 2, (n - subTr[v] - mnDiff[v]) / 2};
						sort(arr, arr + 3);
						ans = min(ans, arr[2] - arr[0]);
					}
					dfs(v, u);
					ST.update(l[u], 1);
				}
			}
		};
		dfs(1, -1);
	}

	cout << ans << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
