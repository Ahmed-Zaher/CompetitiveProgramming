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
#else
#define db(...) true
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
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

const int MX = 5005;
int N, K;
pii A[MX];
int L[MX], R[MX], subTr[MX];
ll mem[MX][MX];

int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
//	freopen("../out.txt", "wb", stdout);
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int T;

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		cin >> N >> K;

		for (int i = 1; i <= N; ++i) {
			cin >> A[i].fs;
			A[i].sc = i;
		}

		function<void(int, int)> build = [&](int l, int r) {

			pii mx = mp(-1, -1);
			for (int i = l; i <= r; ++i) {
				mx = max(mx, A[i]);
			}

			int idx = mx.sc;

			L[idx] = R[idx] = 0;
			subTr[idx] = 1;

			if (l < idx) {
				pii mxL = mp(-1, -1);
				for (int i = l; i <= idx - 1; ++i) {
					mxL = max(mxL, A[i]);
				}

				L[idx] = mxL.sc;

				build(l, idx - 1);

				subTr[idx] += subTr[mxL.sc];

			}
			if (r > idx) {
				pii mxR = mp(-1, -1);
				for (int i = idx + 1; i <= r; ++i) {
					mxR = max(mxR, A[i]);
				}

				R[idx] = mxR.sc;

				build(idx + 1, r);

				subTr[idx] += subTr[mxR.sc];
			}

		};

		build(1, N);

		for (int i = 1; i <= N; ++i) {
			for (int j = 0; j <= N; ++j) {
				mem[i][j] = -1;
			}
		}


		pii mx = mp(-1, -1);
		for (int i = 1; i <= N; ++i) {
			mx = max(mx, A[i]);
		}

		function<ll(int, int)> dp = [&](int i, int c) {

			if (i == 0)
				return c == 0 ? 0 : (ll) 1e18;

			if (c > subTr[i])
				return (ll) 1e18;

			ll& ret = mem[i][c];

			if (ret != -1)
				return ret;

			ret = 1e18;

			for (int cc = c - min(c, subTr[R[i]]); cc <= min(c, subTr[L[i]]); ++cc) {
				ret = min(ret, dp(L[i], cc) + dp(R[i], c - cc) + A[i].fs * 1LL * cc * 1LL * (c - cc));
			}

			for (int cc = c - 1 - min(c - 1, subTr[R[i]]); cc <= min(c - 1, subTr[L[i]]); ++cc) {
				ret = min(ret, dp(L[i], cc) + dp(R[i], c - 1 - cc) + A[i].fs * 1LL * (cc * 1LL * (c - 1 - cc) + c));
			}

			return ret;
		};

		cout << dp(mx.sc, K) << '\n';

//		for (int i = 1; i <= N; ++i) {
//			db(i, L[i], R[i], subTr[i]);
//		}
	}


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
