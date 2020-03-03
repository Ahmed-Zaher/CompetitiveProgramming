/*
 * We get maximum balance when the sequence is 1 2 .. n, that balance
 * is sum of (i - 1) / 2 for i in [3, n]. We can first use maximal prefix 1 2 ... i
 * of that sequence in our answer, s.t. that balance gained from that prefix is <= m.
 * Now since the prefix is maximal, the remaining balance we need is < i / 2 - which
 * is guaranteed to get if we try (i+1)'th element to be in [i + 1, 2i] which is small enough
 * to be computed directly. As for the rest of the sequence, we can just put large numbers
 * that don't introduce any new balance.
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


	int n, m;

	cin >> n >> m;

	if (n <= 2) {
		if (m)
			cout << "-1\n";
		else {
			for (int i = 1; i <= n; ++i)
				cout << i << ' ';
			ndl;
		}
		return 0;
	}

	int c = 0;

	for (int i = 3; i <= n; ++i) {
		c += (i - 1) / 2;
	}

	if (m > c) {
		cout << "-1\n";
		return 0;
	}

	vi ans;
	ans.pb(1);
	ans.pb(2);

	int j = -1;

	for (int i = 3; i <= n; ++i) {
		int add = (i - 1) / 2;
		if (add > m) {
			j = i;
			break;
		}
		ans.pb(i);
		m -= add;
	}

	if (m) {
		for (;; ++j) {
			int f = 0;
			for (int k = 1; k <= sz(ans); ++k) {
				if (k < j - k && j - k <= sz(ans))
					db(j, k), ++f;
			}
			if (f == m) {
				ans.pb(j);
				break;
			}
		}
	}


	int x = 1e8;

	while (sz(ans) < n)
		ans.pb(x += (n * 2));

	for (auto& elem : ans)
		cout << elem << ' ';

	ndl;

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
