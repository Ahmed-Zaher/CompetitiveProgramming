/*
 * Let's refer to types of burger as type A and type B.
 * An outcome is this problem can be expressed with a sequence of operations
 * each operation is either give A or give B to the next kid. The sample space
 * is the set of all such sequence which must have length = n,
 * and the # of As in it = #Bs = n / 2.
 * Note that the outcomes are not equiprobable.
 * For example if n = 4, the seq. AABB has probability (1/2)^2 because after
 * giving the second A, we can only chose B, while ABAB has probability (1/2)^3
 * Throughout the sequence, we'll eventually reach a point where we no longer
 * have a choice and can only pick one type, we want the probability that we
 * reach this exactly after giving burger to the n - 2 kids.
 * Note that after doing a prefix of operations of some sequence, we can tell
 * how many As and Bs we have left. So our answer is the probability that we
 * get 0 As, 2 Bs or 2 As and 0 Bs after doing the first n - 2 operations.
 * This implies that, one step earlier, after the prefix of length n - 3,
 * we must only accept sequences that have 1A, 2B left, or 2B, 1A left.
 * Which implies that we either already used n/2-2 As, n/2-1 Bs or n/2-2 Bs, n/2-1 As
 * This is easier to compute, because all prefixes which such counts for As, Bs are
 * equiprobable because at no point in the prefix we run out of some type.
 * The probability of that is 2 * ((n-3)C(n/2-2) * (1/2)^(n-3)), and exactly half of those
 * possibilities will have 2As, 0Bs or 0As, 2Bs in the next move, and the rest will
 * be 1A, 1B (which is rejected). So the answer is (n-3)C(n/2-2) * (1/2)^(n-3).
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

int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(4);
	cin.tie(0);

	int T;

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {

		int n;
		cin >> n;

		double ans = 1;

		int mul = n / 2 - 1, div = (n - 3) - (n / 2 - 2);
		int c2 = n - 3;

		while (mul <= n - 3) {
			if (ans < 1e9)
				ans *= mul++;
			if (ans > 1e9 && c2 > 0)
				ans /= 2, --c2;
			if (ans > 1e9 && div > 0)
				ans /= div--;
		}

		while (c2 > 0)
			ans /= 2, --c2;
		while (div > 0)
			ans /= div--;


		cout << fixed << 1 - ans << '\n';

	}

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
