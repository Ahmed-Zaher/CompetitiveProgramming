/*
 * We can solve this problem using dynamic programming
 * Let's denote players by P1, P2, and let's first know for each
 * state of the game which player will win
 * Let win[i][j] be the index of the winning player in a game
 * that starts with i stones and it's P_j's turn
 * Clearly win[i][1] = min(win[j][2]) for all j that prime numbers
 * in [i - K, i - 1] and similarly,
 * win[i][2] = max(win[j][j]) for all j that are composite numbers
 * numbers
 * To compute win[i][1] we can record positions j s.t. if P_1 made the number of
 * stones be j at some move, then P_1 would win
 * And now win[i][1] is 1 if the max such j recorded is >= i - K
 * Similarly we can compute win[i][2]
 * And the number of moves can be done using the same technique and using std::set
 * which make it easy to find min/max number of moves
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

const int MX = 474747 + 5;
const int primeP1win = 0;
const int primeP2win = 1;
const int compP1win = 2;
const int compP2win = 3;

vvi poses(4);
vvi win, moves;
bool prime[MX];

int getMin(set<pii>& s, int mn) {
	while (!s.empty() && s.begin()->sc < mn)
		s.erase(s.begin());
	if (s.empty())
		return 0;
	return 1 + s.begin()->fs;
}

int getMax(set<pii>& s, int mn) {
	while (!s.empty() && s.rbegin()->sc < mn)
		s.erase(--s.end());
	if (s.empty())
		return 0;
	return 1 + s.rbegin()->fs;
}

struct PrimeCompositeGame {
	int theOutcome(int N, int K) {

		if (N <= 2)
			return 0;

		for (int i = 2; i < MX; ++i)
			prime[i] = 1;

		for (int i = 2; i < MX; ++i)
			if (prime[i])
				for (int j = 2 * i; j < MX; j += i)
					prime[j] = 0;

		win.assign(N + 1, vi(3, 0));
		moves.assign(N + 1, vi(3, 0));
		vi idx(N + 1);

		idx[2] = primeP1win;
		win[1][1] = win[2][1] = 2;
		win[1][2] = win[2][2] = 1;
		moves[1][1] = moves[2][1] = 0;
		moves[1][2] = moves[2][2] = 0;

		poses[primeP1win].pb(2);

		for (int i = 3; i <= N; ++i) {
			// if it's p1's turn
			if (poses[primeP1win].empty() || poses[primeP1win].back() < i - K) {	// p1 loses, p2 wins
				win[i][1] = 2;
				if (!prime[i])
					idx[i] = compP2win, poses[compP2win].pb(i);
			} else {	// p1 wins, p2 loses
				win[i][1] = 1;
				if (!prime[i])
					idx[i] = compP1win, poses[compP1win].pb(i);
			}
			// if it's p2's turn
			if (poses[compP2win].empty() || poses[compP2win].back() < i - K) {		// p2 loses, p1 wins
				win[i][2] = 1;
				if (prime[i])
					idx[i] = primeP1win, poses[primeP1win].pb(i);
			} else {	// p2 wins, p1 loses
				win[i][2] = 2;
				if (prime[i])
					idx[i] = primeP2win, poses[primeP2win].pb(i);
			}
		}


		V<set<pii>> window(4);

		window[primeP1win].insert(mp(moves[2][2], 2));


		for (int i = 3; i <= N; ++i) {

			// if it's p1's turn
			if (win[i][1] == 1)	// p1 wins
				moves[i][1] = getMin(window[primeP1win], i - K);
			else				// p1 loses
				moves[i][1] = getMax(window[primeP2win], i - K);

			// if it's p2's turn
			if (win[i][2] == 2)	// p2 wins
				moves[i][2] = getMin(window[compP2win], i - K);
			else				// p2 loses
				moves[i][2] = getMax(window[compP1win], i - K);

			int type = idx[i];
			window[type].insert(mp(moves[i][2 - (type >> 1)], i));
		}


		return win[N][1] == 1 ? moves[N][1] : -moves[N][1];
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
