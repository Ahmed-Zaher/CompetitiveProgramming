/*
 * Every outcome in the sample space can be thought of a sequence
 * of pairs of cells (A_1, B_1), (A_2, B_2), .. , (A_K, B_K)
 * Each outcome has probability (NMP)^(2K)
 * Let the random variable ON(c) be 1 if cell c at position (x, y, z)
 * is on after the K moves and 0 otherwise
 * Out answer = Exp(sum ON(c) over all cells) = sum of Exp(ON(c))
 * To calculate Exp(ON(c)) we want to find the probability that cell c is toggled
 * odd number of times.
 * First what is the probability that during 1 move that cell c is toggled (denote that by f_c)?
 * That's the same as (#pairs (A, B) that 'cover' cell c) / (#possible (A, B) pairs (= (NMP)^2))
 * A pair(A, B) cover cell c iff x_A, x_B include x of the cell somewhere in between
 * and the same must be satisfied for y, and z
 * Since dimensions are independent, we can find #pairs that satisfy every dimension and multiply
 * the results.
 * The number of pairs s.t. x lies between x_A, x_B is 2 * (x - 1) * (N - x) + 2 * N - 1 (1-based coordinates)
 * Now that we have f_c, to get the probability that we toggle an odd number of times we compute
 * sum of (K)C(i) * f_c^i * (1-f_c)^(K- i) for all odd valid i
 * To compute that expression efficiently we can use the trick described here:
 * 		https://math.stackexchange.com/a/2528981
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


int T, N, M, P, K;


int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(20);
	cin.tie(0);

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		cin >> N >> M >> P >> K;

		double ans = 0;


		for (int x = 1; x <= N; ++x) {
			for (int y = 1; y <= M; ++y) {
				for (int z = 1; z <= P; ++z) {
					double fc = (2.0 * ((x - 1) * (N - x) + N) - 1) / (N * N);
					fc *= (2.0 * ((y - 1) * (M - y) + M) - 1) / (M * M);
					fc *= (2.0 * ((z - 1) * (P - z) + P) - 1) / (P * P);

					assert(1 - 2 * fc >= 0);
					ans += (1 - pow(1 - 2 * fc, K)) / 2.0;
				}
			}
		}
		cout << "Case " << tc << ": ";
		cout << fixed << ans << '\n';
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
