/*
 * Let v[i][j] be the j'th element in the permutation of i'th cell.
 * Let pos[i][j] be the position of element j in i'th permutation.
 * It is clear that a virus i is stable iff pos[i][i] = 1.
 * A virus is viable if there exists a j where pos[j][i] = 1.
 * Otherwise, there must be a j s.t. we can always get rid of viruses v[j][1..pos[j][i]-1]  while keeping i'th virus
 * alive. We will try all such j's.
 * Let S be the set of viruses in v[j][1..pos[j][i]-1], and T be the rest
 * If in the initial configuration we can get rid of viruses in S using viruses in T (this happens when for every cell c
 * where c'th virus is in S, there's an element in T that comes before c in c'th permutation), then the virus is viable.
 * If this is not the case, then the virus is not viable. Because that means that there's some cell that can only
 * be occupied by a virus is S. Therefore, we can never get rid of all viruses in S.
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

const int MX = 505;
int v[MX][MX], pos[MX][MX], mnT[MX][MX], S[MX];

int main() {
#ifdef LOCAL
	auto stTime = clock();
 	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int n;

	cin >> n;


	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			cin >> v[i][j];
			pos[i][v[i][j]] = j;
		}
	}

	int p;

	cin >> p;

	vi ans(n + 1);

	if (p == 1) {
		for (int i = 1; i <= n; ++i) {
			if (v[i][1] == i)
				ans[i] = 1;
		}
	} else {
		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= n; ++j) {
				if (pos[j][i] == 1)
					ans[i] = 1;
			}

			for (int j = 1; j <= n; ++j) {
				mnT[n + 1][j] = n + 1;
			}
			for (int j = n; j >= 1; --j) {
				for (int k = 1; k <= n; ++k) {
					mnT[j][k] = min(mnT[j + 1][k], pos[k][v[i][j]]);
				}
			}
			for (int j = 1; j < pos[i][i]; ++j) {
				S[v[i][j]] = 1;
				bool good = true;
				for (int k = 1; k <= n; ++k) {
					if (S[k] && pos[k][k] < mnT[j + 1][k])
						good = false;
				}
				if (good)
					ans[v[i][j + 1]] = true;
			}
		}
	}

	int cnt = 0;
	for (int i = 1; i <= n; ++i) {
		cnt += ans[i];
	}
	cout << cnt << '\n';
	for (int i = 1; i <= n; ++i) {
		if (ans[i])
			cout << i << ' ';
	}ndl;

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
