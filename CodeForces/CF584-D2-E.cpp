/*
 * Let's try to find a lower bound on the minimum cost needed to make
 * the 2 permutations the same, let m[x] be j - i, where i is where x
 * occurs in p, and j is where x occurs in s.
 * Clearly we need x to move |m[x]| steps to the left if m[x] < 0 and to
 * the right if |m[x]| > 0, and if m[x] = 0 then x is in the right position
 * Any valid sequence of swaps make m[x] = 0 for all x at the end
 * Now let's consider the swap move, if we say p_i, p_j where i < j, that
 * implies that we do the updates: m[p_i] -= j - i, m[p_j] += j - i
 * So a swap with cost c causes a change in the m values with 2 * c
 * Since we want all m[x]=0, we need a change of S = sum of m[x]
 * This implies that the cost of the optimal answer is S / 2, we'll show
 * a solution with exactly that cost.
 * Consider a string T of length n, which is made of L, R, N
 * T[i] = L/R means that p_i needs to move left/right, and N means no moves needed
 * Let idx be the index of the element with maximum index that has T[i] = R
 * if T[idx+1] = L, swap p_idx, p_idx+1, if T[idx+1] = N, then we skip that element
 * untill p[idx] is placed in its appropriate position, then we repeat this process
 * We'll eventually reach the permutation s, with cost = S / 2, and that's because
 * every move with cost c contributes to m's with exactly 2*c in the direction that
 * makes it all zeros.
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
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int n;

	cin >> n;

	vi p(n + 1), s(n + 1);

	for (int i = 1; i <= n; ++i) {
		cin >> p[i];
	}

	for (int i = 1; i <= n; ++i) {
		cin >> s[i];
	}

	vi m(n + 1);
	ll tot = 0;

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			if (p[i] == s[j])
				m[p[i]] = j - i;
		}
		tot += abs(m[p[i]]);
	}

	vii ans;

	for (int i = n; i >= 1; --i) {
		if (m[p[i]] > 0) {

			int currIdx = i;

			for (int j = i + 1; m[p[currIdx]]; ++j) {
				assert(m[p[j]] <= 0);
				if (m[p[j]] < 0) {
					m[p[currIdx]] -= j - currIdx;
					m[p[j]] += j - currIdx;
					ans.pb(mp(currIdx, j));
					swap(p[currIdx], p[j]);
					currIdx = j;
				}
			}
		}
	}

	for (auto& elem : m)
		assert(elem == 0);

	cout << tot / 2 << '\n' << sz(ans) << '\n';

	for (auto& pr : ans)
		cout << pr.fs << ' ' << pr.sc << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}
