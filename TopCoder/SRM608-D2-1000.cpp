/*
 * Let C be the condition that there's a node u in the input graph s.t.
 * there is more than one cycle that has the node u
 * Let B be the fact that the number of walks is bounded,
 * we can prove that C => !B, and !C => B hence B <=> !C
 * So how do we check for C? we find a cycle, using a dfs
 * then make sure that no 2 nodes on that cycle can reach
 * each other through nodes that do not belong to the cycle
 * Then we can remove that cycle and find our next cycle
 *
 * This implementation is O(|V|^3)
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

vector<string> adj;
vi cycle, path, mark;
int N;
V<int> vis;

void dfs(int u) {

	if (vis[u] == 2)
		return;

	path.pb(u);

	if (vis[u] == 1)
		for (int i = 0; i < sz(path); ++i)
			if (path[i] == u) {
				for (int j = i; j < sz(path); ++j)
					cycle.pb(path[j]);
				return;
			}

	vis[u] = 1;

	for (int v = 0; v < N; ++v) {
		if (adj[u][v] == 'Y')
			dfs(v);
		if (!cycle.empty())
			return;
	}

	path.pop_back();
	vis[u] = 2;
}

bool dfs2(int u, int x) {
	vis[u] = true;

	bool ret = true;

	for (int v = 0; v < N; ++v) {
		if (adj[u][v] == 'Y' && mark[v] == x)
			return false;
		if (adj[u][v] == 'Y' && !vis[v])
			ret = min(ret, dfs2(v, x));
	}
	return ret;
}

class BigOEasy {
public:
	string isBounded(vector<string> graph) {
		adj = graph;
		N = sz(adj);
		mark.assign(N, -1);
		int cycleId = 0;
		do {
			vis.assign(N, 0);

			for (int i = 0; i < N; ++i)
				if (!vis[i]) {
					path.clear();
					cycle.clear();
					dfs(i);
					if (!cycle.empty()) {
						for (int k = 0; k < sz(cycle) - 1; ++k) {

							adj[cycle[k]][cycle[k + 1]] = 'N';

							if (mark[cycle[k]] != -1)
								return "Unbounded";

							mark[cycle[k]] = cycleId;
						}
						break;
					}
				}

			vis.assign(N, 0);
			for (int j = 0; j < sz(cycle) - 1; ++j)
				if (!vis[cycle[j]] && !dfs2(cycle[j], cycleId))
					return "Unbounded";

			++cycleId;

		} while (!cycle.empty());
		return "Bounded";
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



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
