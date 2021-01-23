/*
 * Suppose we have 2N+1 trees T[0..2N]
 * Let T[i] be X[i] for i > N, for i < N let T[i] be L - X[N + i], and T[N] = 0
 * This way we reduced the problem from a circle to a line, we start from T[N], and
 * from there we do N moves, moving left or right. An answer can be represented with a
 * string of length N consisting of L and R. At first it may seem that it's optimal to move
 * in a zig-zag fashion, that is, LRLRLR.. or RLRLRL.. After some experimentation with
 * the naive dp solution, it turns out that this is partly true. It seems that the answer
 * has the form of moving in a certain direction then doing zigzags until all trees are burnt.
 * To prove this, let's suppose that we have a string that does zigzags then moves in the same
 * direction twice, has the form LRLRLRL..LRR. We can prove that we get a better answer when
 * we shift the RR at the end to the left, and end up with RRLRLRL.. We can generalize this
 * observation to show that an optimal answer always moves in a certain direction and then
 * moves in zigzags.
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

const int MX = 2005;
int N, L;
ll mem[MX][MX][2];
vi X;

ll dp(int l, int r, bool b) {
	if (r - l + 1 == N + 1)
		return 0;
	ll& ret = mem[l][r][b];
	if (ret != -1)
		return ret;
	if (!b) {
		ret = max(ret, X[l] - X[l - 1] + dp(l - 1, r, b));
		ret = max(ret, X[r + 1] - X[l] + dp(l, r + 1, !b));
	} else {
		ret = max(ret, X[r + 1] - X[r] + dp(l, r + 1, b));
		ret = max(ret, X[r] - X[l - 1] + dp(l - 1, r, !b));
	}
	return ret;
}

string st;

void buildSol(int l, int r, bool b){
	if (r - l + 1 == N + 1)
		return;
	ll opt = dp(l, r, b);
	if (!b) {
		if (X[r + 1] - X[l] + dp(l, r + 1, !b) == opt) {
			st += "R";
			buildSol(l, r + 1, !b);
		} else{
			st += "L";
			buildSol(l - 1, r, b);
		}
	} else {
		if (X[r + 1] - X[r] + dp(l, r + 1, b) == opt) {
			st += "R";
			buildSol(l, r + 1, b);
		} else {
			st += "L";
			buildSol(l - 1, r, !b);
		}
	}
}

int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> L >> N;
	X.assign(2 * N + 1, 0);
	ll ans = 0;
	for (int I = 0; I < 2; ++I) {
		for (int i = 1; i <= N; ++i) {
			if (!I)
				cin >> X[i + N];
			else
				X[i + N] = L - X[i + N];
		}
		sort(X.begin() + 1 + N, X.end());
		ans = max(ans, X.back() * 1LL);
		for (int i = 0; i < N; ++i) {
			X[i] = X[i + N + 1] - L;
		}
		db(X);
		V<ll> sumX(2 * N + 1);
		for (int i = 0; i < sz(sumX); ++i) {
			sumX[i] = X[i];
			if (i)
				sumX[i] += sumX[i - 1];
		}
		auto s = [&](int l, int r) {
			ll ret = sumX[r];
			if (l)
				ret -= sumX[l - 1];
			return ret;
		};

		for (int r = 1; r < N; ++r) {
			ll cand = X[N + r];
			cand += 2 * (s(N + r + 1, N + r + (N - r) / 2) - s(N - (N - r) / 2, N - 1));
			if ((N - r) % 2 == 0)
				cand -= (X[N + r + (N - r) / 2] - X[N + r]);
			else
				cand += X[N + r] - X[N - (N - r + 1) / 2];
			ans = max(ans, cand);
		}
	}

	cout << ans << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
