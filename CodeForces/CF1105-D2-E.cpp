/*
 * The events can be thought of as a block of 1-type then a block of 2-type events
 * then a block of 1-type and so on.. We can, for each 2-type block, chose at most one friend
 * in that block, and a friend is happy if he's chosen in all the blocks he appears in.
 * A block can be thought of as a mask of m bits, where i'th friend appearing corresponds
 * to i'th bit being ON. A subset of friends can all be happy only if for every pair of
 * friends in that subset, now two share a block. An inefficient solution would use bitmask
 * dp to try all subsets find if it's feasible to make it happy. We can speed this up by
 * a meet-in-the-middle approach, let L be the first m/2 friends and R be the rest.
 * We can trivially find if every subset of L and R is feasible. Now suppose we know that
 * a subset of L is feasible, how do we know which feasible subsets of R can be merged with it?
 * We can know from the L subset which friends in R we can't chose, so we find the complement,
 * those are the friends that can be matched with the L subset. But we need to consider all
 * subsets of thar R subset, which can also be done using dp.
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

const int MX = 21;
V<ll> t;
vvi idx;
vi events;
map<string, int> mapping;
int n, m, ID, L, R, LO, RO, ones[1 << MX];
int f[2][1 << MX], gR[1 << MX], badL[2][1 << MX], badR[1 << MX];

int main() {
#ifdef LOCAL
	auto stTime = clock();
 	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	for (int i = 0; i < (1 << MX); ++i) {
		ones[i] = __builtin_popcount(i);
	}

	cin >> n >> m;

	for (int i = 1; i <= n; ++i) {
		int t;
		cin >> t;
		if (t == 1)
			events.pb(-1);
		else {
			string s;
			cin >> s;
			if (!present(mapping, s))
				mapping[s] = ID++;
			events.pb(mapping[s]);
		}
	}
	db(mapping, events);

	if (m == 1) {
		cout << "1\n";
		return 0;
	}

	L = m / 2, R = m - L, LO = (1 << L) - 1, RO = (1 << R) - 1;

	function<pair<int, int>(ll)> getLR = [&](ll mask) {
		return mp(mask >> R, mask & RO);
	};

	db(L, R, LO, RO);

	idx.assign(m, vi());

	for (int i = 0; i < n; ++i) {
		if (events[i] == -1)
			continue;
		int j;
		ll mask = 0;
		for (j = i; j < n && events[j] != -1; ++j) {
			mask |= 1LL << events[j];
		}
		i = j - 1;
		for (int j = 0; j < m; ++j) {
			if (mask & (1LL << j))
				idx[j].pb(sz(t));
		}
		t.pb(mask);
	}

	db(t, idx);

	for (int i = 0; i < m; ++i) {
		ll bad = 0;
		for (auto &j : idx[i]) {
			bad |= t[j];
		}
		db(i, bad);
		if (i < R) {
			badR[1 << i] = (bad ^ (1 << i)) & RO;
		} else {
			pii LR = getLR(bad);
			db(LR);
			badL[0][1 << (i - R)] = LR.fs ^ (1 << (i - R));
			badL[1][1 << (i - R)] = LR.sc;
		}
	}

	db(badL[0][1], badL[1][1]);

	for (int i = 0; i < (1 << R); ++i) {
		if (ones[i] < 2)
			continue;
		badR[i] = badR[i & -i] | badR[i ^ (i & -i)];
	}

	for (int j = 0; j < 2; ++j) {
		for (int i = 0; i < (1 << L); ++i) {
			badL[j][i] = badL[j][i & -i] | badL[j][i ^ (i & -i)];
		}
	}

	for (int i = 0; i < (1 << L); ++i) {
		f[0][i] = (i & badL[0][i]) == 0;
	}

	for (int i = 0; i < (1 << R); ++i) {
		f[1][i] = (i & badR[i]) == 0;
		if (i) {
			if (f[1][i])
				gR[i] = ones[i];
			for (int j = 0; j < R; ++j) {
				if (i & (1 << j))
					gR[i] = max(gR[i], gR[i ^ (1 << j)]);
			}
		}
	}

	int ans = 0;

	for (int i = 0; i < (1 << L); ++i) {
		if (f[0][i])
			ans = max(ans, ones[i] + gR[(~badL[1][i]) & RO]);
	}

	cout << ans << '\n';


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
