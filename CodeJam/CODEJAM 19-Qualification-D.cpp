/*
 * Let's first make a guess consisting of
 * 16 0's | 16 1's | ...
 * This tells us for every block with range of indices [16*c, min(n, 16*(c+1)))
 * how many broken bits there's in them. Note that these blocks don't overlap and
 * each index belongs to exactly one block. Consider the size of each block with alternating values
 * we'd get as a reply for that first string.
 * We could for example get 14 0's | 16 1's | 3 0's
 * This tells us that first block's size became 14, 2nd block's size became 16 (didn't change)
 * and 3rd block's size became 3. By the change in size we can tell how many broken bits each block has
 * And because each block has size = 16 (except the special case of last block), it can never happen
 * that a block totally disappears and cause ambiguity because there's at most 15 broken bits in each
 * block. The number of broken bits also tells us the range of indices our block maps to in the 'broken' string
 *
 * And using that information we can solve the problem for each block independently.
 * Now we can just do d&c, split each block in 2 halves, make first half all 1s in original string
 * and 2nd half be all 0s, and from the reply we can tell the range of indices in broken string the
 * first half maps to and the same for the second half.
 * Starting with block of size 16 and stopping when we have a block of size at least one, we'd need
 * at most 4 more queries and hence can pass with F = 5.
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

struct Range {
	int l, r, b, lt, rt;
	Range(int _l, int _r, int _b, int _lt, int _rt)
		: l(_l), r(_r), b(_b), lt(_lt), rt(_rt) {}
};

int t;
string S;
string T;


int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> t;

	for (int tc = 1; tc <= t; ++tc) {
		int N, B, F;
		cin >> N >> B >> F;

		S.assign(N, '0');

		V<Range> ranges;

		for (int i = 0; i < N; i += 16)
			ranges.pb(Range(i, min(i + 16, N), -1, -1, -1));

		bool c = 0;

		for (auto& range : ranges) {
			for (int i = range.l; i < range.r; ++i)
				S[i] = c + '0';
			c = !c;
		}

		cout << S << endl;

		cin >> T;

		if (T == "-1")
			exit(0);

		vi arr;

		for (int i = 0; i < N - B;) {
			int j;
			for (j = i; j < N - B && T[j] == T[i]; ++j);
			arr.pb(j - i);
			i = j;
		}

		if (sz(arr) < sz(ranges))
			arr.pb(0);

		for (int i = 0; i < sz(ranges); ++i) {
			ranges[i].lt = (i ? ranges[i - 1].rt : 0);
			ranges[i].rt = ranges[i].lt + arr[i];
			ranges[i].b = (ranges[i].r - ranges[i].l) - (ranges[i].rt - ranges[i].lt);
		}

		vi ans;

		for (int I = 1;; ++I) {

			for (auto& range : ranges) {
				if (range.b == range.r - range.l) {
					for (int i = range.l; i < range.r; ++i)
						ans.pb(i);
					db(ans);
					continue;
				}
				if (range.b == 0)
					continue;

				int mid = (range.l + range.r - 1) / 2;

				for (int i = range.l; i <= mid; ++i)
					S[i] = '0';

				for (int i = mid + 1; i < range.r; ++i)
					S[i] = '1';
			}

			if (I == F)
				break;

			cout << S << endl;

			cin >> T;

			V<Range> newRanges;


			for (auto& range : ranges) {
				if (range.b == range.r - range.l)
					continue;
				if (range.b == 0)
					continue;

				int mid = (range.l + range.r - 1) / 2;

				int c0 = 0, c1 = 0;

				for (int i = range.lt; i < range.rt; ++i) {
					if (T[i] == '0')
						assert(c1 == 0), ++c0;
					else
						++c1;
				}

				newRanges.pb(Range(range.l, mid + 1, mid + 1 - range.l - c0, range.lt, range.lt + c0));
				newRanges.pb(Range(mid + 1, range.r, range.r - (mid + 1) - c1, range.lt + c0, range.rt));

			}

			ranges = newRanges;

		}


		sort(ans.begin(), ans.end());

		for (int i = 0; i < B - 1; ++i)
			cout << ans[i] << ' ';

		cout << ans.back() << endl;

		string reply;

		cin >> reply;

		if (reply == "-1")
			exit(0);
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
