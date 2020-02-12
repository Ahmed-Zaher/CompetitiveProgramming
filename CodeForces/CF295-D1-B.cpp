/*
 * Let's compute the sum of d(i, u, v) in order of decreasing i
 * The shortest path between u, v using nodes in x[i..n]
 * is either a path that goes through x[i] or doesn't, if it doesn't
 * then the distance we computed when computing the answer for x[i+1..n] is already
 * correct, if however x[i] passes through that shortest path, then its
 * distance is the distance from u to x[i] in the reverse graph plus the distance
 * from x[i] to v in the original graph, so we only need to run Dijkstra 2 times from x[i]
 * And so we have d(i, u, v) computed in O(n^2 logn) for every i, so the total complexity
 * is O(n^3 logn)
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

const int MX = 505;
int n, adj[2][MX][MX], d[MX][MX];
vi pos, x;



int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n;

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			cin >> adj[0][i][j];
			d[i][j] = OO;
		}
	}

	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			adj[1][i][j] = adj[0][j][i];
		}
	}

	x.assign(n + 1, 0);
	pos.assign(n + 1, 0);

	for (int i = 1; i <= n; ++i) {
		cin >> x[i];
		pos[x[i]] = i;
	}

	V<ll> ans(n + 1, 0);

	for (int i = n - 1; i >= 1; --i) {
		vvi dist(2, vi(n + 1, OO));
		for (int j = 0; j < 2; ++j) {
			int src = x[i];
			dist[j][src] = 0;
			priority_queue<pii, vii, greater<pii>> pq;
			pq.push(mp(0, src));

			while (!pq.empty()) {
				pii front = pq.top();
				pq.pop();
				int d = front.fs, u = front.sc;
				if (d > dist[j][u])
					continue;
				for (int v = 1; v <= n; ++v) {
					if (pos[v] >= i && d + adj[j][u][v] < dist[j][v]) {
						dist[j][v] = d + adj[j][u][v];
						pq.push(mp(dist[j][v], v));
					}
				}
			}
		}
		for (int u = 1; u <= n; ++u) {
			for (int v = 1; v <= n; ++v) {
				if (u != v && pos[u] >= i && pos[v] >= i) {
					d[u][v] = min(d[u][v], dist[1][u] + dist[0][v]);
					ans[i] += d[u][v];
				}
			}
		}
	}

	for (int i = 1; i <= n; ++i) {
		cout << ans[i] << " \n"[i == n];
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
