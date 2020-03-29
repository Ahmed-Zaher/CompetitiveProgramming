/*
 * Note that the number of boxes must be in the range [ceil(berries / 9), berries]
 * We will simulate the process and increment the number of boxes from 1 up to berries.
 * At each position, some subset of the inspectors sees this box, we mark that in tot[i]
 * where tot[mask] = #positions so far that are seen exactly by the inspectors in that mask
 * And for every valid number of boxes we maximize the answer, knowing that each box must have
 * 1 berry. Now we need a strategy to distribute the remaining berries. Consider all subsets of
 * inspectors, if a position is seen by a certain subset wih inspectors i_1, i_2, ..., then
 * that berry will contribute to the sum of averages with value = sum(1 / seen[i_j]) for all j,
 * where seen[x] is the number of positions seen by x'th inspector.
 * Now we know that, given 2 subsets, we can tell which one has more priority and should be getting berries,
 * it's the one that has higher contribution to the sum of averages, that's because that contribution does
 * not change for a fixed number of boxes.
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

int tot[1 << 5], seen[5];
double score[1 << 5];

bool comp(int i, int j) {
	return score[i] > score[j];
}

struct BerryPacker {
	double bestPacking(vi first, vi period, int berries) {

		for (auto& elem : first)
			++elem;

		int I = sz(first), B = berries;

		double ans = 0;

		for (int i = 1; i <= B; ++i) {
			int mask = 0;
			for (int j = 0; j < I; ++j)
				if (i >= first[j] && (i - first[j]) % period[j] == 0)
					mask ^= 1 << j, ++seen[j];
			++tot[mask];

			for (int k = 1; k < (1 << I); ++k) {
				score[k] = 0;
				for (int j = 0; j < I; ++j)
					if (seen[j] && ((k >> j) & 1))
						score[k] += 1.0 / seen[j];
			}

			if (i < (B + 8) / 9)
				continue;

			vi perm((1 << I) - 1);

			for (int j = 0; j < (1 << I) - 1; ++j)
				perm[j] = j + 1;

			sort(perm.begin(), perm.end(), comp);

			int rem = B - i;

			double cand = 0;


			for (int j = 0; j < (1 << I) - 1; ++j) {
				int k = perm[j];
				int toGive = min(rem, tot[k] * 8);
				cand += (toGive + tot[k]) * score[k];
				rem -= toGive;
			}

			ans = max(ans, cand * i);
		}

		return ans / I;
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
