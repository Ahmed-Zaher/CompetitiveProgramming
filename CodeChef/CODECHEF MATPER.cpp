/*
 * We first need to match the M strings against S, which will can
 * be done efficiently using KMP
 * Then we use a meet in the middle approach, we try all permutations
 * of the M strings which consist of ceil(M / 2) strings, for each
 * one find the minimum sized prefix of S that can include the strings
 * in the permutation in the same order as the permutation, this can be
 * done greedily, find the nearest (from the left) occurrence of the first
 * string in the permutation, then find the nearest occurrence of the second
 * string looking at positions after the previous string and so on
 * Then we do the same thing again for permutations of size floor(M / 2) but
 * we deal with all the strings reversed
 * Then we combine the two solutions to find the overall answer
 * Some optimization and pre-computation will be needed due to the tight constraints
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

const int MX = 14;
int N, M, ones[1 << MX], onePos[1 << MX], SZ, filter, I;
vi  nextMask[1 << MX], occ[2][MX], _next[2][MX], mnPos[2][1 << MX];
string S, P[MX];

template<class T>
vector<int> prefFn(T& S) {
	int n = sz(S);
	vi pref(n);
	pref[0] = 0;
	int border = 0;
	for (int i = 1; i < n; ++i) {
		while (border > 0 && S[i] != S[border])
			border = pref[border - 1];
		if (S[i] == S[border])
			++border;
		else
			border = 0;
		pref[i] = border;
	}
	return pref;
}


template<class T>
vector<int> matches(T& O, T& P) {
	vector<int> result;
	T S = P + "$" + O;
	vector<int> v = prefFn(S);		// KMP
	for (int i = sz(P) + 1; i < sz(S); ++i)
		if (v[i] == sz(P))
			result.pb(i - 2 * sz(P));	// KMP
	return result;
}

void bt(int pos, int mask) {
	if (pos >= N)
		return;
	if (ones[mask] == SZ)
		mnPos[I][mask].pb(pos);
	else
		for (auto& _mask : nextMask[mask]) {
			int idx = onePos[_mask ^ mask];
			bt(_next[I][idx][pos] + sz(P[idx]), _mask);
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

	cin >> N >> M >> S;

	for (int i = 0; i < M; ++i)
		cin >> P[i];

	for (int i = 0; i < M; ++i)
		onePos[1 << i] = i;

	for (int i = 0; i < (1 << M); ++i) {
		ones[i] = __builtin_popcount(i);
		for (int j = 0; j < M; ++j)
			if (!((1 << j) & i))
				nextMask[i].pb(i ^ (1 << j));
	}

	int halves[2] = {(M + 1) / 2, M / 2};

	for (I = 0; I < 2; ++I) {

		for (int i = 0; i < M; ++i) {

			occ[I][i] = matches(S, P[i]);
			occ[I][i].pb(N);
			_next[I][i].assign(N + 1, -1);

			for (auto& idx : occ[I][i])
				_next[I][i][idx] = idx;

			for (int j = N - 1; j >= 0; --j)
				if (_next[I][i][j] == -1)
					_next[I][i][j] = _next[I][i][j + 1];
		}


		if (M == 1) {
			if (sz(occ[I][0]) > 1)
				cout << "1\n";
			else
				cout << "0\n";
			return 0;
		}

		SZ = halves[I];

		filter = (1 << SZ) - 1;

		bt(0, 0);

		reverse(S.begin(), S.end());
		for (int i = 0; i < M; ++i)
			reverse(P[i].begin(), P[i].end());
	}

	for (int I = 0; I < 2; ++I)
		for (int i = 0; i < (1 << M); ++i)
			sort(mnPos[I][i].begin(), mnPos[I][i].end());

	ll ans = 0;


	for (int mask = 0; mask < (1 << M); ++mask) {
		if (ones[mask] == halves[0]) {

			int _mask = ((1 << M) - 1) ^ mask;
			int n = sz(mnPos[0][mask]), m = sz(mnPos[1][_mask]);

			if (!n || !m)
				continue;

			for (int p1 = n - 1, p2 = 0; p1 >= 0; --p1) {
				while (p2 < m && mnPos[0][mask][p1] + mnPos[1][_mask][p2] <= N)
					++p2;
				ans += p2;
			}
		}
	}

	cout << ans << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
