/*
 * Let's find the frequencies for each value for the two numbers. We can show that it's optimal to all possible
 * pair of digits of value of sum =  m - 1 then pairs of value m - 2 and so on down to 0. We can imagine it as
 * a bipartite graph whose sides are A and B, where each sides has its label from 0 up to m - 1.
 * An edges between u and v has value (u + v) mod m. The thing is, this graph has O(m^2) edges, but since we are
 * looking for edges of highest value first, we will find for each node u in each side which node v from the other side
 * still has its frequency > 0 and give max value, then add an edge between u, v.
 * And then we remove edges in order of decreasing weight, since after edge removal the new edges will be of smaller
 * values, we do the updates to our edges lazily. Meaning that we remove an edge and do nothing, and later when we
 * find an invalid edge, we do updates.
 * Complexity is O(m^2*log(m)) but it gets AC - it seems that it's really difficult to break this solution.
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


int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int n, m;
	cin >> n >> m;
//	n = m = 1e5;
	vi freqA(m), freqB(m);
	set<int, greater<int>> A, B;

	for (int i = 0; i < n; ++i) {
		int x;
		cin >> x;
//		x = rand() % m;
		++freqA[x];
	}

	for (int i = 0; i < n; ++i) {
		int x;
		cin >> x;
//		x = rand() % m;
		++freqB[x];
	}

	for (int i = 0; i < m; ++i) {
		if (freqA[i])
			A.insert(i);
		if (freqB[i])
			B.insert(i);
	}

//	db(freqA, freqB);

	multiset<pair<int, pii>> edges;

	auto calc = [&](int x, set<int, greater<int>>& s) {
		int best = (m - 1 - x) % m;
		int y;
		if (s.lower_bound(best) != s.end())
			y = *s.lower_bound(best);
		else
			y = *s.begin();
		return y;
	};

	for (auto &a : A) {
		int b = calc(a, B);
		edges.insert(mp(-((a + b) % m), mp(a, b)));
	}

	for (auto &b : B) {
		int a = calc(b, A);
		edges.insert(mp(-((a + b) % m), mp(a, b)));
	}

	vi ans;

	db(A, B, edges);

	int T = 10;
	while (!A.empty()) {
		assert(!B.empty());
		assert(!edges.empty());
		auto e = *edges.begin();
		int v = -e.fs, a = e.sc.fs, b = e.sc.sc;
		edges.erase(edges.begin());
		if (!freqA[a] && !freqB[b])
			continue;
		else if (!freqA[a] && freqB[b]) {
			int aa = calc(b, A);
			edges.insert(mp(-((aa + b) % m), mp(aa, b)));
		} else if (freqA[a] && !freqB[b]) {
			int bb = calc(a, B);
			edges.insert(mp(-((a + bb) % m), mp(a, bb)));
		} else {
			int mn = min(freqA[a], freqB[b]);
			for (int i = 0; i < mn; ++i) {
				ans.pb(v);
			}
			freqA[a] -= mn, freqB[b] -= mn;
			if (!freqA[a])
				A.erase(a);
			if (!freqB[b])
				B.erase(b);
		}
		db(edges, A, B);
	}


//	assert(sz(ans) == n);

	for (auto &elem : ans) {
		cout << elem << ' ';
	}ndl;

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
