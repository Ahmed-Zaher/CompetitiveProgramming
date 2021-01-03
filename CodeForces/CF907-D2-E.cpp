/*
 * First note that if the graph is complete the answer is 0, and if there is a node with n-1 neighbours the answer is 1
 * Apart from that, we will iterate over all subsets of nodes and check whether it is possible to make the graph
 * complete by doing the operation using only nodes in the subset, and we will report the subset of smallest size
 * If a subset is not connected (i.e. there are 2 nodes in that can't be reached through a path using only subset nodes)
 * then that subset is invalid because it will impossible to connected two separated components
 * Otherwise, it must be connected, and it also must 'cover' all nodes. That is, every node in the graph is either
 * in the subset or has a neighbour that is in the subset. If not then that node can't connect to all other nodes.
 * Since n is small, we will express subsets with masks, to compute whether a subset is connected we can try and remove
 * a node from it (and try that for all nodes), if the result is also connected then that subset is connected
 * This can be done using dp and some bit operations
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

const int MX = 22;
int ones[1 << MX], lg[1 << MX];
bool connected[1 << MX];
int adj[MX];

int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	for (int i = 0; i < (1 << MX); ++i) {
		ones[i] = __builtin_popcount(i);
	}
	for (int i = 0; i < MX; ++i) {
		lg[1 << i] = i;
	}

	int n, m;
	cin >> n >> m;

	for (int i = 1; i <= m; ++i) {
		int u, v;
		cin >> u >> v;
		--u, --v;
		adj[u] |= (1 << v);
		adj[v] |= (1 << u);
	}

	for (int i = 0; i < n; ++i) {
		db(i, adj[i]);
	}

	int cool = 0;

	for (int i = 0; i < n; ++i) {
		cool |= (1 << i) * (ones[adj[i]] == n - 1);
	}

	if (cool == (1 << n) - 1)
		cout << "0\n";
	else if (cool > 0) {
		cout << "1\n" << 1 + lg[cool & (-cool)] << '\n';
	} else {
		for (int i = 0; i < n; ++i) {
			connected[1 << i] = 1;
		}
		for (int i = 0; i < (1 << n); ++i) if (ones[i] > 1) for (int j = 0; j < n; ++j) if (i & (1 << j)) {
			connected[i] |= ((adj[j] & i) > 0) & connected[i ^ (1 << j)];
		}
		pii ans(OO, OO);
		for (int i = 1; i < (1 << n); ++i) {
			if (connected[i]) {
				int cand = i;
				for (int j = 0; j < n; ++j) {
					if (i & (1 << j))
						cand |= adj[j];
				}
				if (cand == ((1 << n) - 1))
					ans = min(ans, mp(ones[i], i));
			}
		}
		db(connected[(1 << n) - 1]);
		cout << ans.fs << '\n';
		for (int i = 0; i < n; ++i) {
			if (ans.sc & (1 << i))
				cout << i + 1 << ' ';
		}ndl;
	}

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
