/*
 * Let's first compress all integers we see into integers from 0 to MX
 * The thing to note here is that the number of segments having one point in
 * common with a range [l, r] = total number of segments (that aren't cleared)
 *              - (# segs ending at r_seg < l + # segs starting at l_seg > r)
 * So this reduces to the task of finding the number of l's/r's in a given range,
 * which can be easily solved using two BITs.
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

struct FenwickTree {
	vector<int> bit;
	int n, LOGN;
	void init(int _n) {
		n = _n;
		LOGN = log2(n) + 1;
		bit.assign(n + 1, 0);
	}
	FenwickTree(int _n) {
		init(_n);
	}
	FenwickTree(vector<int> a) {
		init(sz(a));
		// replace with rangeAdd(i + 1, i + 1, a[i])
		// if you're using range adds and point queries
		for (int i = 0; i < n; ++i)
			add(i + 1, a[i]);
	}
private:
	int sum(int idx) {	// returns prefix sum
		int ret = 0;
		for (; idx > 0; idx -= idx & -idx)
			ret += bit[idx];
		return ret;
	}
public:
	int sum(int l, int r) {
		++l, ++r;
		return sum(r) - sum(l - 1);
	}
	void add(int idx, int delta) {
		++idx;
		for (; idx <= n; idx += idx & -idx)
			bit[idx] += delta;
	}
};

struct Query {
	char c;
	int l, r;

	friend ostream &operator<<(ostream &os, const Query &query) {
		os << "c: " << query.c << " l: " << query.l << " r: " << query.r;
		return os;
	}
};

const int MX = 1e6 + 5;
int N;
V<Query> queries;
si saw;
mii realToComp;
vi compToReal;

int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> N;

	queries.assign(N, Query());

	saw.insert(0);
	saw.insert(1e9 + 1);

	for (int i = 0; i < N; ++i) {
		cin >> queries[i].c >> queries[i].l;
		if (queries[i].c != 'C') {
			cin >> queries[i].r;
			saw.insert(queries[i].l);
			saw.insert(queries[i].r);
		}
	}

	db(saw, queries);

	for (auto &elem : saw) {
		realToComp[elem] = sz(compToReal);
		compToReal.pb(elem);
	}

	db(realToComp);

	FenwickTree ftLs(MX), ftRs(MX);

	vi seenSegs;
	int active = 0;

	for (int i = 0; i < N; ++i) {
		if (queries[i].c == 'D') {
			++active;
			seenSegs.pb(i);
			int l = realToComp[queries[i].l];
			int r = realToComp[queries[i].r];
			ftLs.add(l, 1);
			ftRs.add(r, 1);
		} else if (queries[i].c == 'C') {
			--active;
			int toRmv = seenSegs[queries[i].l - 1];
			int l = realToComp[queries[toRmv].l];
			int r = realToComp[queries[toRmv].r];
			ftLs.add(l, -1);
			ftRs.add(r, -1);
		} else {
			int l = realToComp[queries[i].l];
			int r = realToComp[queries[i].r];
			db(l, r);
			cout << active - ftRs.sum(0, l - 1) - ftLs.sum(r + 1, MX - 1) << '\n';
		}
	}

	db(seenSegs);

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
