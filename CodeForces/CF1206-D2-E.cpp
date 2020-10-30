/*
 * Suppose that we want to know whether the values at 2 cells (x1, y1), (x2, y2) are the same
 * or not, where where the 2 cells are legit to use in a query, and the manhattan distance between
 * them is 2. In this case, any path between them is a string of length 3, and it is palindrome
 * iff we have valueOf(x1, y1) = valueOf(x2, y2), so we can know that by sending a query about those 2 cells
 * Let's also color the cells like on a chess board, where the top left corner is white.
 * Using the method described above we can find value at all the white cells, and find the value of the black
 * cells with respect to each other, i.e. if we assume a black cell, say cell (1, 2), has the value X we can
 * tell for the other black cells whether they * have X or !X.
 * This can be done in n^2 - 3 queries, what is left is to know what is the value of X.
 *
 * Consider any 2x3 sub-rectangle, lets say it starts at (x0, y0) and ends at (x0 + 1, y0 + 2),
 * we note that if the values at (x0, y0 + 1) and (x0 + 1, y0) are different, then if we query that sub-rectangle
 * and get a zero back, that means that this is because values at (x0, y0) and (x0 + 1, y0 + 2) are different (and
 * those 2 cells will be of opposite color, so we can find X) - this is because we can always find a path where second
 * and third steps in the path in that subrect. are the same.
 * Same happens if (x0, y0 + 2) and (x0 + 1, y0 + 1) are different.
 *
 * If this never occur, then we can always find a 3x3 subrect that has a 1 at top left corner and 0 at bottom right
 * corner, and the diagonal in the middle going from bottom left to top right cells  has only 1's or only 0s.
 * We will ask 2 queries about the top and bottom 2x3 subrects inside of it, and with more analysis we can infer
 * from those 2 queries what X is.
 * So we ask at most n^2 - 1 queries
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

int main() {
#ifdef LOCAL
	auto stTime = clock();
// 	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int n;

	cin >> n;

	vvi a(n + 1, vi(n + 1));

	int q = 0;

	auto ask = [&](int x, int y, int xx, int yy) {
		++q;
		if (mp(x, y) > mp(xx, yy))
			swap(x, xx), swap(y, yy);
		cout << "? " << x << ' ' << y << ' ' << xx << ' ' << yy << endl;
		int ret;
		cin >> ret;
		return ret;
	};

	a[1][1] = 1;

	for (int i = 3; i <= n; i += 2) {
		a[1][i] = a[1][i - 2] ^ (!ask(1, i - 2, 1, i));
		a[i][1] = a[i - 2][1] ^ (!ask(i - 2, 1, i, 1));
	}
	for (int i = 2; i <= n; ++i) {
		for (int j = 2; j <= n; ++j) {
			if (i == n && j == n)
				break;
			if ((i + j) % 2 == 0)
				a[i][j] = a[i - 1][j - 1] ^ (!ask(i - 1, j - 1, i, j));
		}
	}

	a[2][3] = a[1][2] ^ (!ask(1, 2, 2, 3));
	a[2][1] = a[2][3] ^ (!ask(2, 1, 2, 3));

	for (int i = 4; i <= n; i += 2) {
		a[1][i] = a[1][i - 2] ^ (!ask(1, i - 2, 1, i));
		a[i][1] = a[i - 2][1] ^ (!ask(i - 2, 1, i, 1));
	}
	for (int i = 2; i <= n; ++i) {
		for (int j = 2; j <= n; ++j) {
			if (i == n && j == n)
				break;
			if ((i + j) % 2 == 1 && mp(i, j) != mp(2, 3))
				a[i][j] = a[i - 1][j - 1] ^ (!ask(i - 1, j - 1, i, j));
		}
	}

	assert(q == n * n - 3);

	for (int i = 1; i <= n - 2; ++i) {
		for (int j = 1; j <= n - 2; ++j) {
			if ((i + j) % 2 == 0 && a[i][j] && !a[i + 2][j + 2]) {
				int c = a[i + 1][j + 1];
				int q1 = ask(i, j, i + 1, j + 2), q2 = ask(i + 1, j, i + 2, j + 2);
				bool flip = 0;
				if (a[i][j + 2] != a[i + 1][j + 1] || a[i][j + 1] != a[i + 1][j]) {
					int valOfB = q1;
					flip = (valOfB != a[i + 1][j + 2]);
				} else if (a[i + 1][j + 1] != a[i + 2][j] || a[i + 1][j + 2] != a[i + 2][j + 1]) {
					int valOfA = !q2;
					flip = (valOfA != a[i + 1][j]);
				} else {
					if (q1 == 1) {
						int valOfA = c;
						flip = (valOfA != a[i][j + 1]);
					} else if (q2 == 1) {
						int valOfA = 0;
						flip = (valOfA != a[i][j + 1]);
					} else {
						if (!c) {
							int valOfA = 1;
							flip = (valOfA != a[i][j + 1]);
						} else {
							int valOfB = 0;
							flip = (valOfB != a[i + 1][j + 2]);
						}
					}
				}
				cout << "!" << endl;
				for (int i = 1; i <= n; ++i) {
					for (int j = 1; j <= n; ++j) {
						if (flip && (i + j) % 2 == 1)
							a[i][j] = !a[i][j];
						cout << a[i][j];
					}ndl;
				}
				return 0;
			}
		}
	}


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
