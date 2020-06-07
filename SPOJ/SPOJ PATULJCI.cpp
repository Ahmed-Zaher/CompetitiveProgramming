/*
 * This is a O(N * log(N) + Q * log(N)^2) solution
 * An element that appears more than half the length of a segment is called
 * the majority element of that segment
 * The idea is that if we partition a segment into smaller non overlapping
 * segments, then the majority element of the big segment must also be
 * a majority element in at least on of the smaller segments.
 * So we can preprocess NlogN segments, for each position i and power p, find the most
 * frequent element in a[i..i + 2^p - 1], we can do that in NlogN by trying all
 * powers of 2, for a fixed power we get a sliding window and keep track of the
 * most frequent element
 * Now for a given query segment [L, R], consider binary representation
 * of its length (= R - L + 1), and use it to do 'jumps' starting from L
 * and ending at R, partitioning [L, R] into O(logN) segments and finding most frequent
 * element of those using precomputed values. Now the majority element must be one
 * of those O(logN) values, we can do binary search on each of them to find if it's
 * a majority element, which results in O(log(N)^2) query time
 */

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;
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

template<class T> using pb_set = tree<T, null_type,
	less<T>, rb_tree_tag, tree_order_statistics_node_update>;


const int MX_C = 1e5 + 5, MX_N = 3e5 + 5, MX_LG = 19;
int a[MX_N], mode[MX_LG][MX_N], freq[MX_C], freqFreq[MX_N], test[MX_LG];
vi idx[MX_C];
int N, C, M;



int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> N >> C;

	for (int i = 1; i <= N; ++i) {
		cin >> a[i];
		mode[0][i] = a[i];
		idx[a[i]].pb(i);
	}

	for (int lg = 1; lg < MX_LG; ++lg) {
		if ((1 << lg) > N)
			break;

		pb_set<int> firstWindow;
		for (int i = 1; i <= (1 << lg); ++i)
			++freq[a[i]], firstWindow.insert(a[i]);


		int mxFreq = 0, mxFreqElem = -1;

		for (auto& elem : firstWindow) {
			++freqFreq[freq[elem]];
			if (freq[elem] > mxFreq)
				mxFreq = freq[elem], mxFreqElem = elem;
		}

		for (int i = 1;; ++i) {

			mode[lg][i] = mxFreqElem;

			if (i + (1 << lg) - 1 == N) {

				pb_set<int> lastWindow;
				for (int j = i; j <= N; ++j)
					lastWindow.insert(a[j]);

				for (auto& elem : lastWindow) {
					--freqFreq[freq[elem]];
					freq[elem] = 0;
				}
				break;
			}

			int add = a[i + (1 << lg)];

			--freqFreq[freq[add]];
			++freq[add];
			++freqFreq[freq[add]];

			if (freq[add] > mxFreq)
				mxFreq = freq[add], mxFreqElem = add;

			int rmv = a[i];

			--freqFreq[freq[rmv]];

			if (mxFreq == freq[rmv] && !freqFreq[freq[rmv]])
				mxFreq = freq[rmv] - 1, mxFreqElem = rmv;

			--freq[rmv];
			++freqFreq[freq[rmv]];
		}
	}


	cin >> M;

	for (int q = 1; q <= M; ++q) {
		int l, r;

		cin >> l >> r;

		int curr = l, diff = r - l + 1;

		int p = 0;

		for (int i = 0; i < MX_LG; ++i)
			if (diff & (1 << i))
				test[p++] = mode[i][curr], curr += (1 << i);

		int ans = -1;
		for (int i = 0; i < p; ++i)
			if (upper_bound(idx[test[i]].begin(), idx[test[i]].end(), r) - lower_bound(idx[test[i]].begin(), idx[test[i]].end(), l) > diff / 2) {
				ans = test[i];
				break;
			}

		if (ans == -1)
			cout << "no\n";
		else
			cout << "yes " << ans << '\n';
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
