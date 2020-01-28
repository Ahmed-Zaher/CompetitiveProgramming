/*
 * This problem can be modeled as a graph, where each node corresponds
 * to a state (t, i, j) - the position of the thief (i, j) at time = t
 * and the edges correspond to possible valid moves that can be done
 * by the thief. Now consider a subgraph of this graph which contains
 * only nodes and edges that lie on some valid path from a node
 * (1, i1, j1) to (T, i2, j2) for some i1, i2, j1, j2. We can mark
 * the nodes that are in this subgraph using a DFS.
 * For every time step t, there is a position in which the thief must
 * be in iff there's exactly one node (t, i, j) for some i, j included
 * in the subgraph - so we check the number of marked nodes for a fixed t
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
int H, W, T;
bool good[105][105][105];
bool grid[105][105][105];
bool vis[105][105][105];
int dr[] = {0, 1, -1, 0, 0};
int dc[] = {0, 0, 0, 1, -1};

bool valid(int i, int j) {
	return i >= 1 && i <= H && j >= 1 && j <= W;
}

void dfs(int t, int i, int j) {

//	db(t, i, j);

	vis[t][i][j] = 1;

	if (t == T) {
		good[t][i][j] = 1;
		return;
	}

	for (int k = 0; k < 5; ++k) {
		int to_i = i + dr[k], to_j = j + dc[k];
		if (valid(to_i, to_j) && grid[t + 1][to_i][to_j]) {
			if (!vis[t + 1][to_i][to_j])
				dfs(t + 1, to_i, to_j);
			good[t][i][j] = max(good[t][i][j], good[t + 1][to_i][to_j]);
		}
	}

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

	int tc = 0;

	while (cin >> W >> H >> T && (W + H + T)) {

		for (int t = 1; t <= T; ++t) {
			for (int i = 1; i <= H; ++i) {
				for (int j = 1; j <= W; ++j) {
					grid[t][i][j] = 1;
					vis[t][i][j] = 0;
					good[t][i][j] = 0;
				}
			}
		}


		int n;

		cin >> n;

		for (int i = 1; i <= n; ++i) {
			int t, L, T, R, B;
			cin >> t >> L >> T >> R >> B;

			for (int i = T; i <= B; ++i) {
				for (int j = L; j <= R; ++j) {
					grid[t][i][j] = 0;
				}
			}
		}


		for (int i = 1; i <= H; ++i) {
			for (int j = 1; j <= W; ++j) {
				if (grid[1][i][j])
					dfs(1, i, j);
			}
		}

		cout << "Robbery #" << ++tc << ":\n";


		int cntT = 0;

		for (int i = 1; i <= H; ++i) {
			for (int j = 1; j <= W; ++j) {
				cntT += good[T][i][j];
			}
		}

		if (!cntT) {
			cout << "The robber has escaped.\n\n";
			continue;
		}

		V<pair<int, pii>> ans;

		for (int t = 1; t <= T; ++t) {
			bool dup = false;
			pii pos = mp(-1, -1);
			for (int i = 1; i <= H; ++i) {
				for (int j = 1; j <= W; ++j) {
					if (pos.fs != -1 && good[t][i][j])
						dup = true;
					if (pos.fs == -1 && good[t][i][j])
						pos = mp(i, j);
				}
			}
			if (!dup && pos.fs != -1)
				ans.pb(mp(t, pos));
		}


		if (ans.empty())
			cout << "Nothing known.\n";
		else
			for (auto& pr : ans) {
				cout << "Time step " << pr.fs << ": The robber has been at " << pr.sc.sc
						<< ',' << pr.sc.fs << ".\n";
			}

		ndl;

	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
