#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
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


template<class T> using pb_set = tree<T, null_type,
		less<T>, rb_tree_tag, tree_order_statistics_node_update>;

int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	auto f = [](char c) {
		if (isdigit(c))
			return c - '0';
		if (c >= 'a' && c <= 'z')
			return c - 'a' + 10;
		return c - 'A' + 36;
	};

	const int MX = 62;

	V<si> charPoses(MX);
	pb_set<int> poses;

	int n, m;
	cin >> n >> m;
	string s;
	cin >> s;

	for (int i = 0; i < n; ++i) {
		poses.insert(i);
		charPoses[f(s[i])].insert(i);
	}


	for (int q = 0; q < m; ++q) {
		int l, r;
		char c;
		cin >> l >> r >> c;
		--l, --r;
		int v = f(c);
		int L = *poses.find_by_order(l), R = *poses.find_by_order(r);
		vi rmv;
		for (auto it = charPoses[v].lower_bound(L); it != charPoses[v].end() && *it <= R; ++it) {
			rmv.pb(*it);
		}

		for (auto &elem : rmv) {
			charPoses[v].erase(elem);
			poses.erase(elem);
		}
	}

	for (auto &elem : poses) {
		cout << s[elem];
	}ndl;


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
