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

const int MX_LG = 30;
int T, N, K;
vvi sp;
VVV<ll> f(MX_LG, VV<ll>());


int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> T;

	for (int tc = 1; tc <= T; ++tc) {
		cin >> K >> N;

		f[0].assign(N, V<ll>(N, 0));
		sp.assign(N, vi(N, 1e9));

		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				cin >> f[0][i][j];
				sp[i][j] = f[0][i][j];
			}
		}


		for (int w = 0; w < N; ++w) {
			for (int u = 0; u < N; ++u) {
				for (int v = 0; v < N; ++v) {
					sp[u][v] = min(sp[u][v], sp[u][w] + sp[w][v]);
				}
			}
		}

		for (int lg = 1; lg < MX_LG; ++lg) {
			f[lg].assign(N, V<ll>(N, 2e18));
			for (int w = 0; w < N; ++w) {
				for (int u = 0; u < N; ++u) {
					for (int v = 0; v < N; ++v) {
						f[lg][u][v] = min(f[lg][u][v], f[lg - 1][u][w] + f[lg - 1][w][v]);
					}
				}
			}
		}

		VV<ll> ans(N, V<ll>(N, 0));

		for (int lg = 0; lg < MX_LG; ++lg) {
			if (K & (1 << lg)) {
				K ^= (1 << lg);
				ans = f[lg];
				break;
			}
		}


		for (int lg = 0; lg < MX_LG; ++lg) if (K & (1 << lg)) {
			VV<ll> newAns(N, V<ll>(N, 2e18));
			for (int w = 0; w < N; ++w) {
				for (int u = 0; u < N; ++u) {
					for (int v = 0; v < N; ++v) {
						newAns[u][v] = min(newAns[u][v], ans[u][w] + f[lg][w][v]);
					}
				}
			}
			ans = newAns;
		}

		VV<ll> finalAns = ans;

		for (int w = 0; w < N; ++w) {
			for (int u = 0; u < N; ++u) {
				for (int v = 0; v < N; ++v) {
					finalAns[u][v] = min(finalAns[u][v], ans[u][w] + sp[w][v]);
				}
			}
		}

		cout << "Region #" << tc << ":\n";
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				cout << finalAns[i][j] << ' ';
			}ndl;
		}ndl;

	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
