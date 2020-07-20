/*
 * Let ans[u] be the optimal answer if we were to start at node u
 * Suppose that next move it to go to v, then:
 * ans[u] = winprob[v]/100 + [(100 - winprob[v] - looseprob[v])/100] * ans[v]
 * Note that [(100 - winprob[v] - looseprob[v])/100] <= .99
 * If we unfold the expression for a large number of steps, the last node visited
 * will be multiplied by a factor <= .99^(# steps), so after about 10000 steps,
 * the nodes we pick have negligible contribution to the answer so we can just stop
 * there. So we only simulate the process for 10000 steps, and use dynamic programming
 * to find which choices maximize the answer
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

const int STEPS = 10000;
long double mem[STEPS + 1][55];

struct GraphWalkWithProbabilities {
	double findprob(V<string> graph, vi winprob, vi looseprob, int Start) {
		int n = sz(graph);
		V<long double> w(n), l(n);
		for (int i = 0; i < n; ++i) {
			w[i] = winprob[i] / 100.;
			l[i] = looseprob[i] / 100.;
		}
		for (int u = 0; u < n; ++u) {
			mem[STEPS][u] = 1;
		}
		for (int idx = STEPS - 1; idx >= 0; --idx) {
			for (int u = 0; u < n; ++u) {
				mem[idx][u] = 0;
				for (int v = 0; v < n; ++v) {
					if (graph[u][v] == '1')
						mem[idx][u] = max(mem[idx][u], w[v] + (1 - w[v] - l[v]) * mem[idx + 1][v]);
				}
			}
		}


		return mem[0][Start];
	}
};


int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(20);
	cin.tie(0);


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
