/*
 * All odd sized subsequences are included in the answer, now consider even sized subsequences
 * For that subseq. to be valid, the two middle elements need to have the same value, so we
 * can brute force over all possible pairs (i, j) s.t. Ai = Aj and find how many even sized subseq.
 * have Ai, Aj as k/2'th and k/2+1'th elements when sorting the subseq, where k is the subseq. size
 * To avoid over counting, we impose order by index when elements have the same A value, so when
 * we have a subsequence 1 1 1 we say that the middle element is that particular 1 with index
 * bigger than the index of the first one and less than the index of the last one
 * Knowing that, we can sort the input and for all i, j where i < j and Ai = Aj: if Ai, Aj were
 * to be in the middle of the subseq. then the reset of the subseq. can be constructed by drawing
 * any number of elements from A[1..i-1] along with the same number of elements in A[j+1..N]
 * In how many ways can we do that? that's sum of (i-1)Ck * (N-j)Ck for k in [0..min(i-1, N-j)]
 * That is implemented in the function f(x, y), but if we use that the solution would be too slow,
 * we can use dynamic programming to compute f(x, y) for all x and y in O(N^2)
 * And the way that works is like a game, we have two arrays A[0..x], B[0..y] and we want to know
 * in how many ways we can put 0's or 1's in them s.t. the number of ones in A and B is the same
 * We can simulate that by saying that initially it's A's turn, and whenever it chooses a 1, it becomes
 * B's turn.
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

const int MX = 1005, MOD = 1000000007;
int C[MX][MX];

void nCk() {
	for(int i = 0; i < MX; ++i)
		for(int j = 0; j <= i; ++j)
			C[i][j] = (j == 0) ? 1 : (C[i - 1][j - 1] + C[i - 1][j]) % MOD;
}

int mem[MX][MX][2];

int dp(int x, int y, int b) {
	if (x < 0) {
		if (!b)
			return 1;
		return y + 1;
	}
	if (y < 0) {
		if (!b)
			return 1;
		return 0;
	}
	int& ret = mem[x][y][b];
	if (ret != -1)
		return ret;
	ret = 0;
	if (!b)
		return ret = (dp(x - 1, y, 0) + dp(x - 1, y, 1)) % MOD;
	else
		return ret = (dp(x, y - 1, 1) + dp(x, y - 1, 0)) % MOD;
}

int f(int x, int y) {
	int ret = 0;
	for (int i = 0; i <= min(x, y); ++i) {
		ret = (ret + ((C[x][i] * 1LL * C[y][i]) % MOD)) % MOD;
	}
	return ret;
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

	nCk();
	memset(mem, -1, sizeof(mem));

	int T;
	cin >> T;
	
	for (int tc = 1; tc <= T; ++tc) {
		int N;
		cin >> N;
		vi A(N + 1);

		for (int i = 1; i <= N; ++i)
			cin >> A[i];

		int ans = 0;

		for (int i = 1; i <= N; i += 2)
			ans = (ans + C[N][i]) % MOD;

		sort(A.begin() + 1, A.end());

		for (int i = 1; i <= N; ++i) {
			for (int j = i + 1; j <= N; ++j) {
				if (A[j] != A[i])
					break;
				ans = (ans + dp(i - 1 - 1, N - j - 1, 0)) % MOD;
			}
		}

		cout << ans << '\n';
	}


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
