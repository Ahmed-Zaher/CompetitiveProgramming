/*
 * Solution using convex hull trick
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

const int MX = 5005;
int n, k, w[MX], x[MX];
ll prefW[MX], prefXW[MX], dp[MX][MX << 1];

struct Line {
	long long m, c;
	long long eval(long long x) { return m * x + c; }
	long double intersectX(Line l) { return (long double) (c - l.c) / (l.m - m); }
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

	cin >> n >> k;

	for (int i = 1; i <= n; ++i)
		cin >> x[i] >> w[i];

	for (int i = 1; i <= n; ++i) {
		prefW[i] = prefW[i - 1] + w[i];
		prefXW[i] = prefXW[i - 1] + x[i] * 1LL * w[i];
	}


	for (int i = 2; i <= k * 2; ++i) {
		dp[0][i] = 1e16;
	}

	for (int i = 1; i <= n; ++i) {
		dp[i][0] = 1e14;
	}

	for (int kd = 1; kd <= 2 * k; ++kd) {
		if (kd % 2 == 0) {
			deque<Line> lines;
			for (int nd = 1; nd <= n; ++nd) {
				dp[nd][kd] = 1e14;
				Line newLine = Line({-x[nd], dp[nd - 1][kd - 1] - prefXW[nd] + x[nd] * prefW[nd]});
				while (sz(lines) >= 2) {
					if (lines.back().intersectX(newLine) < lines.back().intersectX(lines[sz(lines) - 2]))
						lines.pop_back();
					else
						break;
				}
				lines.pb(newLine);

				while (sz(lines) >= 2) {
					if (lines[0].eval(prefW[nd]) > lines[1].eval(prefW[nd]))
						lines.pop_front();
					else
						break;
				}
				dp[nd][kd] = lines[0].eval(prefW[nd]) + prefXW[nd];
			}
		} else {
			dp[n][kd] = 1e16;
			deque<Line> lines;
			lines.pb(Line({prefW[0], dp[0][kd - 1] + prefXW[0]}));
			for (int nd = 1; nd < n; ++nd) {
				dp[nd][kd] = 1e16;
				Line newLine = Line({prefW[nd], dp[nd][kd - 1] + prefXW[nd]});
				while (sz(lines) >= 2) {
					if (lines.back().intersectX(newLine) > lines.back().intersectX(lines[sz(lines) - 2]))
						lines.pop_back();
					else
						break;
				}
				lines.pb(newLine);

				while (sz(lines) >= 2) {
					if (lines[0].eval(-x[nd + 1]) > lines[1].eval(-x[nd + 1]))
						lines.pop_front();
					else
						break;
				}
				dp[nd][kd] = lines[0].eval(-x[nd + 1]) + x[nd + 1] * prefW[nd] - prefXW[nd];
			}
		}
	}


	cout << dp[n][k * 2] << '\n';


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
