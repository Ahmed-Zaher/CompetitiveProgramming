/*
 * Let's split the numbers into 2 halves, right half with first 8 bits,
 * left half with the rest of the bits
 * Let g[i][j] be the number of numbers with left half = i, right half = j
 * Also think of g[i][j] as cell in i'th row and j'th column
 * Now to answer a query about bit T will be:
 * - if T < 8, sum of columns with index x over all x s.t. 0 <= x < 2^8 and x have
 * T'th bit on
 * - if T >= 8, sum of rows with index x over all x s.t. 0 <= x < 2^8 and x have
 * T'th bit on
 * Now to modification queries: split the number T we shall add to its 2 halves,
 * let them be T_l, T_r for left and right halves
 * T_l causes a shift of all rows T_l times, where in each shift we put row i in the place
 * of row (i + 1) % 2^8
 * T_r causes a shift of each individual element in g to the right, where
 * a shift of cell with position (i, j) will be moving it to position (i + 1, j) if i + 1 < 2^8
 * and to position ((i + 1) % 2^8, 0) otherwise
 * So overall we can describe the current state as a number of shift of one of the two mentioned types
 * We note that:
 * 1. doing a shift of the first type and b shift of the seconds type in any order produces
 * the same result
 * 2. we can exchange 2^8 shifts of the second type with one shift of the first type
 * 3. 2^8 shifts of the first type have no effect and can be ignored
 * So we can always maintain 2 numbers a, b where a, b < 2^8 and reverse those shifts so that
 * we compute the sum of certain rows/columns by only looking at g we filled initially
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

const int M = 256;
int N, g[M][M], sumR[M][M], sumC[M];
vi hasBit[8];

#define L(x) ((x) >> 8)
#define R(x) ((x) & ((1 << 8) - 1))


int range(int i, int j, int k) {
	int ret = sumR[i][k];
	if (j)
		ret -= sumR[i][j - 1];
	return ret;
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


	for (int j = 0; j < 8; ++j)
		for (int i = 0; i < M; ++i)
			if (i & (1 << j))
				hasBit[j].pb(i);

	int tc = 1;

	while (cin >> N && N != -1) {

		for (int i = 0; i < M; ++i) {
			for (int j = 0; j < M; ++j)
				g[i][j] = sumR[i][j] = 0;
			sumC[i] = 0;
		}

		for (int i = 0; i < N; ++i) {
			int P;
			cin >> P;
			++g[L(P)][R(P)];
		}

		for (int i = 0; i < M; ++i) {
			for (int j = 0; j < M; ++j) {
				sumC[j] += g[i][j];
				sumR[i][j] = (j ? sumR[i][j - 1] : 0) + g[i][j];
			}
		}

		int r = 0, c = 0;
		ll ans = 0;

		char t;

		while (cin >> t && t != 'E') {
			int T;
			cin >> T;
			if (t == 'C') {
				int Tl = L(T), Tr = R(T);
				c += Tr;
				r = (r + Tl + (c / M)) % M;
				c %= M;
			} else {
				if (T < 8) {
					for (auto& n : hasBit[T])
						ans += sumC[(n - c + M) % M];
				} else {
					T -= 8;
					for (auto& n : hasBit[T]) {
						ans += range((n - r + M) % M, 0, M - c - 1);
						if (c)
							ans += range((n - r - 1 + 2 * M) % M, M - c, M - 1);
					}
				}
			}
		}

		cout << "Case " << tc++ << ": " << ans << '\n';

	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
