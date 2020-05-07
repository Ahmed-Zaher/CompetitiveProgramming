/*
 * Each state of the process can be expressed by:
 * 1. How many people of weight 50 on the left side,
 * 2. How many people of weight 50 on the right side,
 * 3. How many people of weight 100 on the left side,
 * 4. How many people of weight 100 on the right side,
 * 5. Which side the boat is on
 *
 * 2 can be deduced from 1 and 4 can be deduced from 3 since the total
 * is fixed. So we O(n^2) different states, we can model this as a graph
 * and for each state there're O(n^2) edges to other states, we can find
 * the shortest path to the destination state then use dp to count how many
 * ways we can get from source to destination.
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

const int MX = 51, MOD = 1000000007;
int n, k, C[MX][MX], adj[MX][MX][2][MX][MX][2], dist[MX][MX][2], mem[MX][MX][2];

int dp(int uOL, int uTL, bool B) {
	if (!uOL && !uTL && B)
		return 1;
	int& ret = mem[uOL][uTL][B];

	if (ret != -1)
		return ret;
	ret = 0;
	for (int vOL = 0; vOL < MX; ++vOL) {
		for (int vTL = 0; vTL < MX; ++vTL) {
			if (adj[uOL][uTL][B][vOL][vTL][!B] && dist[vOL][vTL][!B] == dist[uOL][uTL][B] + 1)
				ret = (ret + (adj[uOL][uTL][B][vOL][vTL][!B] * 1LL * dp(vOL, vTL, !B))) % MOD;
		}
	}
	return ret;
}

void nCk() {
	for(int i = 0; i < MX; ++i)
		for(int j = 0; j <= i; ++j)
			C[i][j] = (j == 0) ? 1 : (C[i - 1][j - 1] + C[i - 1][j]) % MOD;
}


int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	nCk();

	cin >> n >> k;

	k /= 50;

	int O = 0, T = 0;

	for (int i = 1; i <= n; ++i) {
		int ai;
		cin >> ai;
		if (ai == 50)
			++O;
		else
			++T;
	}

	for (int uOL = 0; uOL <= O; ++uOL) {
		for (int uTL = 0; uTL <= T; ++uTL) {
			for (int o = 0; o <= uOL; ++o) {
				for (int t = 0; t <= uTL; ++t) {
					if (o + t && o + 2 * t <= k)
						adj[uOL][uTL][0][uOL - o][uTL - t][1] = (C[uOL][o] * 1LL * C[uTL][t]) % MOD;
				}
			}
			for (int o = 0; o <= O - uOL; ++o) {
				for (int t = 0; t <= T - uTL; ++t) {
					if (o + t && o + 2 * t <= k)
						adj[uOL][uTL][1][uOL + o][uTL + t][0] = (C[O - uOL][o] * 1LL * C[T - uTL][t]) % MOD;
				}
			}
		}
	}


	for (int i = 0; i < MX; ++i) {
		for (int j = 0; j < MX; ++j) {
			for (int k = 0; k < 2; ++k) {
				dist[i][j][k] = OO;
			}
		}
	}

	queue<pair<pii, bool>> q;
	q.push(mp(mp(O, T), 0));
	dist[O][T][0] = 0;
	while (!q.empty()) {
		pair<pii, bool> u = q.front();
		q.pop();
		int uOL = u.fs.fs, uTL = u.fs.sc, B = u.sc;
		for (int vOL = 0; vOL < MX; ++vOL) {
			for (int vTL = 0; vTL < MX; ++vTL) {
				if (adj[uOL][uTL][B][vOL][vTL][!B] && dist[vOL][vTL][!B] == OO)
					dist[vOL][vTL][!B] = dist[uOL][uTL][B] + 1, q.push(mp(mp(vOL, vTL), !B));
			}
		}
	}

	memset(mem, -1, sizeof(mem));

	if (dist[0][0][1] == OO)
		cout << "-1\n0\n";
	else
		cout << dist[0][0][1] << '\n' << dp(O, T, 0) << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
