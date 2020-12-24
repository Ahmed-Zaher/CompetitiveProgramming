/*
 * First note that the answer is -1 if the 2 cells lie in different components
 * Suppose we want to calculate shortest path from (r1, c1) to (r2, c2) where r1 < r2
 * Also define a 'greedy path' from 2 cells as a path does the following until it reaches c2:
 *          if c1 = c2:
 *               switch rows if necessary and terminate
 *          if the cell to the right is free:
 *              go to it
 *          else:
 *              switch row
 *          repeat
 * We can show that the greedy path path from  (r1, c1) to (r2, c2) gives an optimal answer
 * For each component, find the left most col with a free cell in it, and find the greedy path
 * starting from that cell (if both cells of the column are free, pick any)
 * For each cell, find the nearest X to its right, if that X is at col > c2, then we can go right
 * to visit that X and when c1 = c2, we stop and then switch rows if necessary
 * Otherwise, we will move right until we reach a point where X is to our immediate right, then
 * we switch row, and move right, and now we are above/below that X. And at this point we still have c2 >= c1
 * What's interesting about this position, is that it is guaranteed to be a part of the greedy path
 * we found for that component, so we can just follow that path until we reach c2.
 * How do we 'follow' it efficiently? We add up the change in columns as well as the points where the greedy path
 * switched rows, such flip points can be computed using partial sums
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

const int MX = 2e5 + 5;
char grid[2][MX];
int CC[2][MX], nxt[2][MX], ones[MX], oneSum[MX];
int dr[] = {-1, 0, 0, 1}, dc[] = {0, -1, 1, 0};
int n, m;

int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> m;

	for (int r = 0; r < 2; ++r) {
		grid[r][0] = grid[r][n + 1] = 'X';
	}

	for (int r = 0; r < 2; ++r) {
		for (int c = 1; c <= n; ++c) {
			cin >> grid[r][c];
		}
	}

	function<void(int, int, int)> dfs = [&](int r, int c, int ccID) {
		CC[r][c] = ccID;
		for (int k = 0; k < 4; ++k) {
			int rr = r + dr[k], cc = c + dc[k];
			if (rr >= 0 && rr <= 1 && cc >= 1 && cc <= n && !CC[rr][cc] && grid[rr][cc] != 'X')
				dfs(rr, cc, ccID);
		}
	};

	int id = 1;
	for (int r = 0; r < 2; ++r) {
		for (int i = 1; i <= n; ++i) {
			if (!CC[r][i] && grid[r][i] != 'X')
				dfs(r, i, id++);
		}
	}

//	for (int r = 0; r < 2; ++r) {
//		for (int i = 1; i <= n; ++i) {
//			cout << CC[r][i];
//		}
//		ndl;
//	}

	nxt[0][n + 1] = nxt[1][n + 1] = n + 1;
	for (int r = 0; r < 2; ++r) {
		for (int c = n; c >= 1; --c) {
			if (grid[r][c] == 'X')
				nxt[r][c] = c;
			else
				nxt[r][c] = nxt[r][c + 1];
		}
	}

//	for (int r = 0; r < 2; ++r) {
//		for (int i = 1; i <= n; ++i) {
//			cout << nxt[r][i] << ' ';
//		}
//		ndl;
//	}
	for (int c = 1; c <= n; ++c) {
		for (int r = 0; r < 2; ++r) {
			if (grid[r][c] != 'X') {
				int curR = r, curC = c;
				while (true) {
					if (grid[curR][curC + 1] != 'X')
						++curC;
					else if (grid[!curR][curC] != 'X' && !ones[curC])
						curR = !curR, ones[curC] = 1;
					else {
						c = curC;
						break;
					}
				}
			}
		}
	}


	for (int i = 1; i <= n; ++i) {
		oneSum[i] = ones[i] + oneSum[i - 1];
	}

	for (int q = 1; q <= m; ++q) {
		int u, v;
		cin >> u >> v;
		int r1, c1, r2, c2;
		if (u <= n)
			r1 = 0, c1 = u;
		else
			r1 = 1, c1 = u - n;
		if (v <= n)
			r2 = 0, c2 = v;
		else
			r2 = 1, c2 = v - n;

		db(r1, c1, r2, c2);

		if (CC[r1][c1] != CC[r2][c2])
			cout << "-1\n";
		else {
			if (c1 > c2)
				swap(r1, r2), swap(c1, c2);
			if (c1 == c2 || nxt[r1][c1] > c2)
				cout << abs(c1 - c2) + abs(r1 - r2) << '\n';
			else {
				int ans = nxt[r1][c1] - c1 + 1;
				c1 = nxt[r1][c1], r1 = !r1;
				db(r1, c1);
				if (c1 == c2)
					cout << ans << '\n';
				else {
					assert(!ones[c1]);
					db(ans);
					int flips = oneSum[c2 - 1] - oneSum[c1 - 1];
					db(r1, c1, r2, c2);
					ans += flips + c2 - 1 - c1;
					db(ans);
					c1 = c2 - 1;
					r1 = (r1 + flips) % 2;
					cout << ans + abs(c1 - c2) + abs(r1 - r2) << '\n';
				}
			}
		}
	}

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
