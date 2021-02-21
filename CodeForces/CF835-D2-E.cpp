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
// 	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int n, x, y;
	cin >> n >> y >> x;

	vvi hasOne(10), hasZero(10);

	for (int i = 1; i <= n; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (i & (1 << j))
				hasOne[j].pb(i);
			else
				hasZero[j].pb(i);
		}
	}

	auto ask = [&](vi v, int idx = -1) {
		if (v.empty())
			return 0;
		if (idx == -1)
			idx = sz(v) - 1;
		cout << "? " << idx + 1;
		for (int i = 0; i <= idx; ++i) {
			cout << ' ' << v[i];
		}
		cout << endl;
		int ret;
		cin >> ret;
		return ret;
	};

	int pXORq = 0, diffBit = -1;
	for (int i = 0; i < 10; ++i) {
		int ret = ask(hasZero[i]);
		if (ret != 0 && ret != y)
			pXORq ^= (1 << i), diffBit = i;
	}

	assert(diffBit != -1);

	vi v = sz(hasZero[diffBit]) < sz(hasOne[diffBit]) ? hasZero[diffBit] : hasOne[diffBit];
	int lo = 0, hi = sz(v) - 1;
	db(pXORq, diffBit, v);
	while (lo < hi) {
		int mid = (lo + hi) / 2;
		int ret = ask(v, mid);
		if (ret != 0 && ret != y)
			hi = mid;
		else
			lo = mid + 1;
	}
	int p = v[lo], q = p ^ pXORq;
	if (p > q)
		swap(p, q);
	cout << "! " << ' ' << p << ' ' << q << endl;


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
