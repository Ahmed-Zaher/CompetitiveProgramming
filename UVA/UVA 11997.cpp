/*
 * The k arrays are stored in a[i][j] where a[i] is the i'th array
 * First sort all a[i], note that min sum we can get is sum of a[i][0]
 * Let's define a sum by a sequence of k integers b1 b2 .. bk, which
 * means that the sum = sum of a[i][bi]. Note that given a sequence,
 * all the sequences c1 c2 .. ck s.t. ci <= bi for all i represents
 * a smaller sum. So we can find sums incrementally, initially starting
 * with b = 0 0 .. 0, and in every step we pick some i and try
 * b_new = b but with bi incremented. This introduces a graph, where each
 * node is defined by the sequence b, and the root is all zeros, and children
 * have the same sequence as their parent but with some element of b incremented.
 * To make sure we don't visit the same node twice, we can hash values of b
 * into ints and use them to represent the sequence.
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

const int MOD = 1000000007, P = 937;
int k;


struct Node {
	int p, incPos;
	Node(int _p = -1, int _incPos = -1) : p(_p), incPos(_incPos) {}
};

int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	while (cin >> k) {

		vvi a(k, vi(k + 1, INT_MAX));

		for (int i = 0; i < k; ++i)
			for (int j = 0; j < k; ++j)
				cin >> a[i][j];

		for (int i = 0; i < k; ++i)
			sort(a[i].begin(), a[i].end());

		priority_queue<pii, vii, greater<pii>> pq;
		pq.push(mp(0, 0));
		V<Node> nodes;
		nodes.pb(Node());

		vi ans;

		unordered_set<int> vis;

		while (sz(ans) < k) {
			pii front = pq.top();
			pq.pop();
			int d = front.fs, u = front.sc;
			vi pos(k, 0);
			int curr = u;
			while (nodes[curr].p != -1) {
				++pos[nodes[curr].incPos];
				curr = nodes[curr].p;
			}

			int hsh = 0;
			int p = P;

			for (int j = 0; j < k; ++j) {
				hsh = (hsh + p * (pos[j] + 1)) % MOD;
				p = (p * P) % MOD;
			}

			if (present(vis, hsh))
				continue;

			vis.insert(hsh);

			ans.pb(d);


			for (int j = 0; j < k; ++j) {
				int cost = a[j][pos[j] + 1] - a[j][pos[j]];
				pq.push(mp(d + cost, sz(nodes)));
				nodes.pb(Node(u, j));
			}
		}

		int mnSum = 0;

		for (int i = 0; i < k; ++i)
			mnSum += a[i][0];

		for (int i = 0; i < k; ++i)
			cout << mnSum + ans[i] << " \n"[i + 1 == k];
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
