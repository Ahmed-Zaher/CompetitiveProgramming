/*
 * Since the number of ASK queries is small, we can, for each query, iterate over all the garlands.
 * So eventually each garland has a contribution that it adds to each query, we will compute
 * that contribution for one garland at a time. First we will express a rectangle query as 4 rectangle
 * queries where the top left corner is (1, 1). For each garland, we will move from top down, adding
 * bulbs along the way, and maintaining a BIT that tells us for every column how much total value we have
 * from bulbs so far. While doing that, we check queries in the appropriate order and the contribution
 * of this particular garland to that query will be a prefix sum (in the BIT) that goes from 1 to the right side
 * of the rectangle
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


int N, M, K, Q;
V<bool> on;

struct FenwickTree {
	vector<ll> bit;
	int n, LOGN;
	void init(int _n) {
		n = _n;
		LOGN = log2(n) + 1;
		bit.assign(n + 1, 0);
	}
	FenwickTree(int _n) {
		init(_n);
	}
	FenwickTree() {}
private:
	ll sum(int idx) {	// returns prefix sum
		ll ret = 0;
		for (; idx > 0; idx -= idx & -idx)
			ret += bit[idx];
		return ret;
	}
public:
	ll sum(int l, int r) {
		return sum(r) - sum(l - 1);
	}
	void add(int idx, ll delta) {
		for (; idx <= n; idx += idx & -idx)
			bit[idx] += delta;
	}
};


int I;

V<ll> queries;
V<pair<pair<short, short>, int>> bulbs[2001];
FenwickTree ft[2001];
V<pair<pair<short, short>, pair<short, bool>>> toAnswer[2001];


struct Garland {

	void go() {
		ft[I] = FenwickTree(M);
		bulbs[I].pb(mp(mp(N + 1, 1), 0));
		sort(bulbs[I].begin(), bulbs[I].end());
		sort(toAnswer[I].begin(), toAnswer[I].end());
		int ptr = 0;
		for (auto &bulb : bulbs[I]) {
			while (ptr < sz(toAnswer[I]) && toAnswer[I][ptr].fs < bulb.fs) {
				queries[toAnswer[I][ptr].sc.fs] += (toAnswer[I][ptr].sc.sc * 2 - 1) * ft[I].sum(1, toAnswer[I][ptr].fs.sc);
				++ptr;
			}
			ft[I].add(bulb.fs.sc, bulb.sc);
		}
		ft[I].bit.clear();
		bulbs[I].clear();
		toAnswer[I].clear();
	}

};

Garland garlands[2001];

int main() {
#ifdef LOCAL
	auto stTime = clock();
 	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> N >> M >> K;

	on.assign(K, 1);

	for (int i = 0; i < K; ++i) {
		int len;
		cin >> len;
		for (int j = 0; j < len; ++j) {
			int x, y, w;
			cin >> x >> y >> w;
			bulbs[i].pb(mp(mp(x, y), w));
		}
	}

	cin >> Q;


	for (int i = 0; i < Q; ++i) {
		string s;
		cin >> s;
		if (s == "SWITCH") {
			int idx;
			cin >> idx;
			--idx;
			on[idx] = !on[idx];
		} else {
			int x, y, xx, yy;
			cin >> x >> y >> xx >> yy;
			for (int j = 0; j < K; ++j) {
				if (on[j]) {
					toAnswer[j].pb(mp(mp(xx, yy), mp(sz(queries), 1)));
					if (x > 1)
						toAnswer[j].pb(mp(mp(x - 1, yy), mp(sz(queries), 0)));
					if (y > 1)
						toAnswer[j].pb(mp(mp(xx, y - 1), mp(sz(queries), 0)));
					if (x > 1 && y > 1)
						toAnswer[j].pb(mp(mp(x - 1, y - 1), mp(sz(queries), 1)));
				}
			}
			queries.pb(0);
		}
	}

	for (int i = 0; i < K; ++i) {
		I = i;
		garlands[i].go();
	}

	for (int i = 0; i < sz(queries); ++i) {
		cout << queries[i] << '\n';
	}


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
