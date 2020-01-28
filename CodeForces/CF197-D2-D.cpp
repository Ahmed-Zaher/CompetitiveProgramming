/*
 * We consider 2 grids to be different if the positions (i, j) that
 * lie in them have different (i / n, j / m)
 * Consider the sequence of positions (i mod n, j mod m)
 * we'd get if we were to start walking infinitely away
 * from the start position, eventually we'd get a repeated
 * pair (and at that point the actual position lies in a grid that is different
 * from the grid from which we visited that position for the first time),
 * we e can take this part of the sequence and infinitely repeat it.
 * One way to detect if we have a cyclic sequence is to check
 * whether a cell in the current grid have been visited in a different grid.
 * Because we only want to check if 2 cells with the same (i mod n, j mod m)
 * lie in different grids and reachable from the start position and don't
 * care about the path between them, a DFS can get our answer correctly
 * This means that we only need to visit every position (i mod n, j mod m)
 * at most once before we discover that the answer is yes.
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

const int MX = 1505;
int n, m;
int di[] = {0, 0, 1, -1};
int dj[] = {-1, 1, 0, 0};
char grid[MX][MX];
pii vis[MX][MX];
map<pii, set<pii>> M;
set<pii> done;
set<pii> q;

bool valid(int i, int j) {
	return i >= 0 && i < n && j >= 0 && j < m;
}

void dfs(int i, int j, pii id) {

	db(i, j, id);


	if (vis[i][j] != mp(INT_MAX, INT_MAX) && vis[i][j] != id) {
		cout << "Yes\n";
		exit(0);
	}

	vis[i][j] = id;

	for (int k = 0; k < 4; ++k) {
		int to_i = i + di[k];
		int to_j = j + dj[k];

		if (valid(to_i, to_j)) {
			if (grid[to_i][to_j] != '#' && vis[to_i][to_j] != id)
				dfs(to_i, to_j, id);
		} else {
			pii nextId = id;
			if (to_i < 0)
				nextId.fs--;
			if (to_j < 0)
				nextId.sc--;
			if (to_i >= n)
				nextId.fs++;
			if (to_j >= m)
				nextId.sc++;


			to_i = (to_i + n) % n;
			to_j = (to_j + m) % m;
			if (grid[to_i][to_j] != '#' && vis[to_i][to_j] != nextId) {
				dfs(to_i, to_j, nextId);
			}
		}
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

	cin >> n >> m;


	int ii, jj;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			cin >> grid[i][j];
			if (grid[i][j] == 'S')
				ii = i, jj = j;
			vis[i][j] = mp(INT_MAX, INT_MAX);
		}
	}


	dfs(ii, jj, mp(0, 0));

	cout << "No\n";


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
