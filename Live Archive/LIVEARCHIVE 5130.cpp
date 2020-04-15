/*
 * Note that each of the character has different number
 * of white connected components in it
 * So we can do the following:
 * -Find all connected components (white or black) and label them
 * -For every black connected components, find the set of white components
 * that it shares boundaries with
 * -From the size of that set, we can determine which character that
 * black connected components represents
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

const int BLACK = 1, WHITE = 0;
int di[] = {0, 0, 1, -1};
int dj[] = {1, -1, 0, 0};
int tc, H, W;
VV<bool> grid, vis;
vvi id;
si s;
mii M;

bool valid(int i, int j) {
	return i >= 0 && i <= H + 1 && j >= 0 && j <= W * 4 + 1;
}

void putID(int i, int j, int ID) {
	vis[i][j] = 1;
	id[i][j] = ID;

	for (int k = 0; k < 4; ++k) {
		int to_i = i + di[k];
		int to_j = j + dj[k];

		if (valid(to_i, to_j) && !vis[to_i][to_j] && grid[to_i][to_j] == grid[i][j])
			putID(to_i, to_j, ID);
	}
}

void getChar(int i, int j) {
	vis[i][j] = 1;

	for (int k = 0; k < 4; ++k) {
		int to_i = i + di[k];
		int to_j = j + dj[k];

		if (valid(to_i, to_j) && !vis[to_i][to_j]) {
			if (grid[to_i][to_j] == grid[i][j])
				getChar(to_i, to_j);
			else
				s.insert(id[to_i][to_j]);
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

	M[2] = 'A';
	M[4] = 'J';
	M[6] = 'D';
	M[5] = 'S';
	M[1] = 'W';
	M[3] = 'K';

	while (cin >> H >> W && (H + W)) {

		grid.assign(H + 2, V<bool>(W * 4 + 2, WHITE));
		vis.assign(H + 2, V<bool>(W * 4 + 2, 0));
		id.assign(H + 2, vi(W * 4 + 2, 0));


		for (int i = 1; i <= H; ++i) {

			int p = 1;

			for (int j = 1; j <= W; ++j) {
				char c;

				cin >> c;

				if (isdigit(c))
					c -= '0';
				else
					c = 10 + (c - 'a');

				for (int k = 0; k < 4; ++k)
					grid[i][p++] = (c & (1 << (3 - k))) >> (3 - k);
			}
		}

//		for (int i = 0; i < sz(grid); ++i) {
//			for (int j = 0; j < sz(grid[i]); ++j) {
//				cout << grid[i][j];
//			}ndl;
//		}ndl;

		int currID = 0;

		for (int i = 0; i <= H + 1; ++i)
			for (int j = 0; j <= W * 4 + 1; ++j)
				if (!vis[i][j])
					putID(i, j, ++currID);

//		for (int i = 0; i < sz(grid); ++i) {
//			for (int j = 0; j < sz(grid[i]); ++j) {
//				cout << id[i][j];
//			}ndl;
//		}ndl;

		vis.assign(H + 2, V<bool>(W * 4 + 2, 0));

		V<char> ans;

		for (int i = 0; i <= H + 1; ++i)
			for (int j = 0; j <= W * 4 + 1; ++j)
				if (!vis[i][j] && grid[i][j] == BLACK) {
					s.clear();
					getChar(i, j);
					ans.pb(M[sz(s)]);
				}

		cout << "Case " << ++tc << ": ";

		sort(ans.begin(), ans.end());

		for (auto& c : ans)
			cout << c;

		ndl;
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
