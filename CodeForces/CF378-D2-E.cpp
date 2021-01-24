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

template<class T> using pb_set = tree<T, null_type,
		less<T>, rb_tree_tag, tree_order_statistics_node_update>;

int n, m, mem[1 << 20];
vi s;
vii cmds;

int dp(int mask) {
	if (!mask)
		return 0;
	int& ret = mem[mask];
	if (ret != INT_MAX)
		return ret;

	int ones = __builtin_popcount(mask);
	int zeros = m - ones;

	if (cmds[zeros].fs == 0) {  // pick
		for (int i = 0; i < m; ++i) {
			if (mask & (1 << i))
				return ret = dp(mask ^ (1 << i)) + s[i] * (cmds[zeros].sc == 0 ? 1 : -1);
		}
	} else {    // ban
		if (cmds[zeros].sc == 0) {
			ret = -1e9;
			for (int i = 0; i < m; ++i) {
				if (mask & (1 << i))
					ret = max(ret, dp(mask ^ (1 << i)));
			}
		} else {
			ret = 1e9;
			for (int i = 0; i < m; ++i) {
				if (mask & (1 << i))
					ret = min(ret, dp(mask ^ (1 << i)));
			}
		}
	}
	return ret;

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


	cin >> n;

	s.assign(n, 0);

	for (int i = 0; i < n; ++i) {
		cin >> s[i];
	}

	sort(s.rbegin(), s.rend());
	db(s);


	cin >> m;

	for (int i = 0; i < m; ++i) {
		string type;
		int t;
		cin >> type >> t;
		--t;
		if (type == "p")
			cmds.pb(mp(0, t));
		else
			cmds.pb(mp(1, t));
	}


	for (int i = 0; i < (1 << m); ++i) {
		mem[i] = INT_MAX;
	}

	cout << dp((1 << m) - 1) << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
