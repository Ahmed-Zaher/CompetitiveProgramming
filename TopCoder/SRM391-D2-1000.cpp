/*
 * We can model the situation as a graph, where we add an edge u->v if we
 * can move from cell u to cell v via a move. And assign scores to each node, these
 * are the scores we gain when we go through that cell. Since we have L, U cells,
 * the graph might contain cycle, so we first find the strongly connected component
 * of a graph, for every SCC, we know that we can get the score of all of its nodes
 * once we visit a node belonging to it - so we consider that to be the score of the SCC itself.
 * So we can build a DAG where each node is one of the SCCs and two nodes u, v are connected
 * if there's an edge from a node in SCC(u) to SCC(v) in the original graph.
 * And now we want to path from SCC(topLeftCell) that has maximum total score of nodes in that DAG.
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

vvi adj, adjR;
vi score;
int n, m;
int di[] = {1, 0};
int dj[] = {0, 1};

bool valid(int i, int j) {
	return i >= 0 && i < n && j >= 0 && j < m;
}

struct SCC {
	vi vis, order, comp, CCID;
	vvi CC;
	int n, nCC;
	vi mem, sccScore;
	SCC(int _n) : n(_n) {
		vis.assign(n, 0);
		for (int i = 0; i < n; ++i)
			if (!vis[i])
				dfs1(i);
		vis.assign(n, 0);
		CCID.assign(n, 0);
		reverse(order.begin(), order.end());
		for (auto& u : order)
			if (!vis[u]) {
				comp.clear();
				dfs2(u);
				for (auto& u : comp)
					CCID[u] = sz(CC);
				CC.pb(comp);
			}

	}
	void dfs1(int u) {
	    vis[u] = true;
	    for (auto& v : adj[u])
	        if (!vis[v])
	            dfs1(v);
	    order.pb(u);
	}
	void dfs2(int u) {
	    vis[u] = true;
	    comp.pb(u);
	    for (auto& v : adjR[u])
	    	if (!vis[v])
	    		dfs2(v);
	}
	int solve() {
		nCC = sz(CC);
		sccScore.assign(nCC, 0);
		for (int i = 0; i < n; ++i)
			sccScore[CCID[i]] += score[i];
		mem.assign(nCC, -1);
		return maxScore(CCID[0]);
	}
	int maxScore(int u) {
		if (mem[u] != -1)
			return mem[u];
		mem[u] = sccScore[u];
		int add = 0;
		for (int i = 0; i < n; ++i) {
			for (auto& j : adj[i]) {
				if (CCID[i] == u && CCID[j] != CCID[i])
					add = max(add, maxScore(CCID[j]));
			}
		}
		return mem[u] = mem[u] + add;
	}
};

void addEdge(int i, int j, int to_i, int to_j) {
	adj[i * m + j].pb(to_i * m + to_j);
	adjR[to_i * m + to_j].pb(i * m + j);
}

struct MarbleCollectionGame {
	int collectMarble(vector <string> board) {

		n = sz(board), m = sz(board[0]);

		score.assign(n * m, 0);
		adj.assign(n * m, vi());
		adjR.assign(n * m, vi());

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (board[i][j] == '#') {
					score[i * m + j] = -OO;
				} else {
					for (int k = 0; k < 2; ++k) {
						int to_i = i + di[k];
						int to_j = j + dj[k];
						if (valid(to_i, to_j))
							addEdge(i, j, to_i, to_j);
					}
					if (board[i][j] == 'L' && valid(i, j - 1))
						addEdge(i, j, i, j - 1);
					if (board[i][j] == 'U' && valid(i - 1, j))
						addEdge(i, j, i - 1, j);
					if (isdigit(board[i][j]))
						score[i * m + j] = board[i][j] - '0';
				}
			}
		}


		SCC scc(n * m);

		return scc.solve();
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
