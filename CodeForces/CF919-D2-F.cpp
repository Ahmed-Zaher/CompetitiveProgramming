/*
 * This is a games problem on a general graph, look at:
 * https://cp-algorithms.com/game_theory/games_on_graphs.html
 * At any given point the game state is completely described by:
 *  1. whose turn it is now
 *  2. the values of each player's cards
 * We can save this information in a compact way
 * A player has 8 cards with values between 0 and 4, each value occurs
 * no more than 8 times, we can represent a set of 8 cards belonging to a player
 * using a 5-digit integer, where i'th digit is how many times the number i appeared
 * in the cards, this works because order of cards is not important
 * Then we introduce a pair (a, b) where a is the current player's cards and b
 * is the other player's cards. Then we consider the transitions from one game
 * state to the next, and we note that the game graph is actually possible to traverse
 * and find for each node whether it is a win/lose/draw
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
	while (*dbStr != ',') cout << *dbStr++;
	cout << " = " << e << ',';
	_db(dbStr + 1, r...);
}

template<class S, class T>
ostream &operator<<(ostream &o, const map<S, T> &v) {
	o << "[";
	int i = 0;
	for (const pair<S, T> &pr : v) o << (!i++ ? "" : ", ")
	<< "{" << pr.fs << " : " << pr.sc << "}";
	return o << "]";
}

template<template<class, class...> class S, class T, class... L>
ostream &operator<<(ostream &o, const S<T, L...> &v) {
	o << "[";
	int i = 0;
	for (const auto &e : v) o << (!i++ ? "" : ", ") << e;
	return o << "]";
}

template<class S, class T>
ostream &operator<<(ostream &o, const pair<S, T> &pr) {
	return o << "(" << pr.fs << ", " << pr.sc << ")";
}

ostream &operator<<(ostream &o, const string &s) {
	for (const char &c : s) o << c;
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

int tens[9];
V<int> halfStates;
V<pii> states;
map<pii, int> stateID;
vvi adjR;
vector<bool> win;
vector<bool> lose;
vector<bool> vis;
vector<int> deg;

int getDigit(ll x, int d) {
	return (x / tens[d]) % 10;
}

ll changeDigit(ll x, int d, int sign) {
	return x + sign * tens[d];
}

void dfs(int u) {
	vis[u] = true;
	for (auto &v : adjR[u])
		if (!vis[v]) {
			if (lose[u])
				win[v] = true;
			else if (--deg[v] == 0)
				lose[v] = true;
			else
				continue;
			dfs(v);
		}
}

int main() {
#ifdef LOCAL
	auto stTime = clock();
//	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	tens[0] = 1;

	for (int i = 1; i < 9; ++i) {
		tens[i] = tens[i - 1] * 10;
	}

	for (short i = 0; i <= 8; ++i) {
		for (short j = 0; j <= 8; ++j) {
			for (short k = 0; k <= 8; ++k) {
				for (short l = 0; l <= 8; ++l) {
					for (short m = 0; m <= 8; ++m) {
						if (i + j + k + l + m == 8)
							halfStates.pb(i + j * 10 + k * 100 + l * 1000 + m * 10000);
					}
				}
			}
		}
	}

	db(sz(halfStates));

	for (auto halfState : halfStates) {
		for (auto &otherHalf : halfStates) {
			states.pb(mp(halfState, otherHalf));
		}
	}

	int n = sz(states);
	adjR.assign(n, vi());
	deg.assign(n, 0);
	win.assign(n, 0);
	lose.assign(n, 0);
	vis.assign(n, 0);


	for (int i = 0; i < n; ++i) {
		int L = states[i].fs, R = states[i].sc;
		if (L != 8 && R == 8)
			lose[i] = 1;
		if (R != 8 && L == 8)
			win[i] = 1;
		stateID[states[i]] = i;
	}


	for (auto &state : states) {
		int u = stateID[state];
		int L = state.fs, R = state.sc;
		for (int a = 1; a <= 4; ++a) {
			for (int b = 1; b <= 4; ++b) {
				if (getDigit(L, a) && getDigit(R, b)) {
					int LL = L, RR = R;
					int c = a + b;
					if (c >= 5)
						c -= 5;
					LL = changeDigit(LL, a, -1);
					LL = changeDigit(LL, c, 1);
					int v = stateID[mp(RR, LL)];
//					db(u, v);
					assert(u != v);
					adjR[v].pb(u);
					++deg[u];
				}
			}
		}
	}

	for (int i = 0; i < n; ++i)
		if ((win[i] || lose[i]) && !vis[i])
			dfs(i);

	int T;

	cin >> T;

	int f;
	vi a(8), b(8);
	for (int tc = 1; tc <= T; ++tc) {
		cin >> f;
		int A = 0, B = 0;
		for (int i = 0; i < 8; ++i) {
			cin >> a[i];
			A += tens[a[i]];
		}
		for (int i = 0; i < 8; ++i) {
			cin >> b[i];
			B += tens[b[i]];
		}
		db(A, B);
		if (!f) {
			int node = stateID[mp(A, B)];
			if (win[node])
				cout << "Alice\n";
			else if (lose[node])
				cout << "Bob\n";
			else
				cout << "Deal\n";
		} else {
			int node = stateID[mp(B, A)];
			if (win[node])
				cout << "Bob\n";
			else if (lose[node])
				cout << "Alice\n";
			else
				cout << "Deal\n";
		}
	}


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
