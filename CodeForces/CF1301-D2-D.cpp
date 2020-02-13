/*
 * Let's think of every cell as a node and roads as edges.
 * There exists an Eulerian cycle that starts and ends at (1, 1) and uses all
 * the 4nm-2n-2m edges, that's because the condition for Euler's theorem is satisfied:
 * Every node has in-degree = out-degree.
 * So we have an answer if k <= 4nm-2n-2m.
 * As for the way to construct the solution, we need a pattern that satisfies the output constraints,
 * but luckily finding the tour manually in this case finds one such pattern for us.
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


int n, m, k;
int di[] = {1, -1, 0, 0};
int dj[] = {0, 0, 1, -1};
vvi adj;

bool valid(int i, int j) {
	return i >= 0 && i < n && j >= 0 && j < m;
}

map<pii, int> edgeCnt;
list<int> cyc;

void euler(list<int>::iterator it, int u) {
	for (auto& v : adj[u])
		if (edgeCnt[{u, v}]) {
			--edgeCnt[{u, v}];
			euler(cyc.insert(it, u), v);
		}
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


	cin >> n >> m >> k;

	if (k > 4 * n * m - 2 * n - 2 * m) {
		cout << "NO\n";
		return 0;
	}

	cout << "YES\n";

	adj.assign(n * m, vi());

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			for (int l = 0; l < 4; ++l) {
				int to_i = i + di[l];
				int to_j = j + dj[l];
				if (valid(to_i, to_j))
					adj[i * m + j].pb(to_i * m + to_j), ++edgeCnt[mp(i * m + j, to_i * m + to_j)];
			}
		}
	}

	euler(cyc.begin(), 0);

	vi ans;
	ans.pb(0);

	for (auto& u : cyc) {
		ans.pb(u);
	}

	V<char> moves;

	for (int i = 1; i <= sz(ans); ++i) {
		int r = ans[i] / m, c = ans[i] % m;
		int rp = ans[i - 1] / m, cp = ans[i - 1] % m;
		int dr = r - rp, dc = c - cp;
		if (mp(dr, dc) == mp(-1, 0))
			moves.pb('U');
		if (mp(dr, dc) == mp(1, 0))
			moves.pb('D');
		if (mp(dr, dc) == mp(0, -1))
			moves.pb('L');
		if (mp(dr, dc) == mp(0, 1))
			moves.pb('R');
	}

	V<pair<char, int>> ANS;

	for (int i = 0; i < k; ++i) {
		int cnt = 0;
		int j;
		for (j = i; j < k; ++j) {
			if (moves[j] == moves[i])
				++cnt;
			else
				break;
		}
		i = j - 1;
		ANS.pb(mp(moves[i], cnt));
	}

	cout << sz(ANS) << '\n';

	for (auto& pr : ANS)
		cout << pr.sc << ' ' << pr.fs << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
