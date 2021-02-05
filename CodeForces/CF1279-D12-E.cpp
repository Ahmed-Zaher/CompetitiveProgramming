/*
 * We can show that a permutation is good iff it can be partitioned into consecutive blocks that looks as follows:
 *              p_1 p_2.. p_r1 | p_l2 p_l2+1 .. p_r2 | ... | p_lm ... p_rm  for some m.
 * i.e. first block takes some elements of p from l_1 = 1 to r_1 second block goes from l_2 = r_1+1 to r_2 and so on..
 * A good permutation need to have each i'th block constituting a single permutation cycle that includes exactly
 * the numbers in the range range [l_i, r_i], furthermore, the block's maximum, which is r_i, needs to be positioned
 * in p[l_i].
 * We can first solve some simpler problems:
 *      - How many permutations of length n make exactly one permutation cycle? the answer is (n-1)!
 *      - How many good permutations of length n? Can be computed using dp and the previous point
 *      - How to find k'th lexicographically smallest permutation cycle of length n?
 *      - How to do the same as previous task but with first element = n? It is similar to finding
 *          k'th permutation, and I used DSU to make sure that we create exactly one cycle
 * The rest is straightforward. We also need to handle overflow problems appropriately.
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
using ull = unsigned long long;
using pii = pair<int, int>;
using vi = V<int>;
using vii = V<pii>;
using vvi = VV<int>;
using mii = map<int, int>;
using umii = unordered_map<int, int>;
using si = set<int>;
using usi = unordered_set<int>;

const int MX = 55;
const ull LIM = 5000000000000000000ULL;
ull dp[MX], fact[MX];

struct UnionFind {
	vi p, rank, setSize;
	int numSets;
	UnionFind(int n) {
		setSize.assign(n, 1);
		numSets = n;
		rank.assign(n, 0);
		p.assign(n, 0);
		for (int i = 0; i < n; ++i)
			p[i] = i;
	}
	int findSet(int i) {
		return (p[i] == i) ? i : (p[i] = findSet(p[i]));
	}
	bool isSameSet(int i, int j) {
		return findSet(i) == findSet(j);
	}
	void unionSet(int i, int j) {
		if (!isSameSet(i, j)) {
			--numSets;
			int x = findSet(i), y = findSet(j);
			if (rank[x] > rank[y]) {
				p[y] = x;
				setSize[x] += setSize[y];
			} else {
				p[x] = y;
				setSize[y] += setSize[x];
				if (rank[x] == rank[y])
					++rank[y];
			}
		}
	}
	int numDisjointSets() {
		return numSets;
	}
	int sizeOfSet(int i) {
		return setSize[findSet(i)];
	}
};


vi kthCycle(int l, ull k) {

	vi out(l, -1), in(l, -1);
	out[0] = l - 1, in[l - 1] = 0;

	UnionFind UF(l);
	UF.unionSet(0, l - 1);

	for (int i = l - 1; i >= 2; --i) {
		ull p = k / fact[i - 2];
		int cur = 0;

		for (int j = 0; j <= l - 1; ++j) if (in[j] == -1 && !UF.isSameSet(l - i, j)) {
			if (cur == p) {
				UF.unionSet(l - i, j);
				out[l - i] = j;
				in[j] = l - i;
			}
			++cur;
		}
		k %= fact[i - 2];
	}
	for (int i = 0; i <= l - 1; ++i) {
		if (out[i] == -1)
			for (int j = 0; j <= l - 1; ++j) {
				if (in[j] == -1)
					out[i] = j, in[j] = i;
			}
	}
	return out;
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

	fact[0] = 1;

	for (int i = 1; i <= 20; ++i) {
		fact[i] = fact[i - 1] * i;
	}

	dp[0] = 1;
	dp[1] = 1;

	for (int i = 2; i < MX; ++i) {
		for (int l = 2; l <= i; ++l) {
			if (!fact[l - 2])
				break;
			if (fact[l - 2] > LIM / dp[i - l])
				break;
			if (dp[i] + dp[i - l] * fact[l - 2] > LIM)
				break;
			dp[i] += dp[i - l] * fact[l - 2];
		}
		if (!dp[i])
			break;
		if (dp[i] + dp[i - 1] > LIM)
			break;
		dp[i] += dp[i - 1];
	}

	int T;

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		int n;
		ll k;
		cin >> n >> k;
		if (dp[n] && k > dp[n])
			cout << "-1\n";
		else {
			--k;
			int l = 1;
			vi ans;
			while (l <= n) {
				for (int cand = l; cand <= n; ++cand) {
					int remLen = n - cand;
					ull fct = (cand == l ? 1 : fact[cand - l - 1]);
					if (dp[remLen] && fct && dp[remLen] < LIM / fct
						&& dp[remLen] * fct <= k) {
						k -= dp[remLen] * fct;
					} else {
						ull div = 0;
						if (dp[remLen])
							div = k / dp[remLen];
						vi append = kthCycle(cand - l + 1, div);
						for (auto &elem : append) {
							ans.pb(elem + l);
						}
						l = cand + 1;
						if (dp[remLen])
							k %= dp[remLen];
						break;
					}
				}
			}
			for (auto &elem : ans) {
				cout << elem << ' ';
			}ndl;
		}
	}


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
