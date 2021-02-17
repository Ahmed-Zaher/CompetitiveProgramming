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


const int MX = 5005, MX_AI = 1e5 + 5;
int n, m, a[MX], f[MX][MX], comp[MX_AI];

struct DS {
	V<short> f, g;
	DS() {
		f.assign(m, -10000);
		g.assign(7, -10000);
	}
	int query(int val) {
		short ret = g[val % 7];
		if (comp[val - 1] != -1)
			ret = max(ret, f[comp[val - 1]]);
		if (comp[val + 1] != -1)
			ret = max(ret, f[comp[val + 1]]);
		return ret;
	}
	void add(int val, short fVal) {
		f[comp[val]] = max(f[comp[val]], fVal);
		g[val % 7] = max(g[val % 7], f[comp[val]]);
	}
};

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

	si s;

	for (int i = 1; i <= n; ++i) {
		cin >> a[i];
		s.insert(a[i]);
	}

	{
		for (int i = 0; i < MX_AI; ++i) {
			comp[i] = -1;
		}
		for (auto &elem : s) {
			comp[elem] = m++;
		}
	}
	V<DS> rows(n + 1), cols(n + 1);

	int ans = 0;

	for (int i = 0; i <= n; ++i) {
		f[i][i] = -OO;
	}

	for (int i = 0; i <= n; ++i) {
		for (int j = 0; j <= n; ++j) {
			if (i != j) {
				f[i][j] = (i > 0) + (j > 0);
				if (i > j)
					f[i][j] = max(f[i][j], max(1 + f[0][j], 1 + cols[j].query(a[i])));
				else
					f[i][j] = max(f[i][j], max(1 + f[i][0], 1 + rows[i].query(a[j])));
				ans = max(ans, f[i][j]);
				if (j)
					rows[i].add(a[j], f[i][j]);
				if (i)
					cols[j].add(a[i], f[i][j]);
			}
		}
	}

//	for (int i = 0; i <= n; ++i) {
//		for (int j = 0; j <= n; ++j) {
//			cout << f[i][j] << ' ';
//		}ndl;
//	}


	cout << ans << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
