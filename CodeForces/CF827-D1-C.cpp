/*
 * Let s[Sz][rem][c] be an array corresponding to some characters in the string
 * with indices s[rem], s[rem+Sz], s[rem+2*Sz], ...
 * If the corresponding character is c, we store 1, otherwise we store 0
 * For each array, create a BIT, it does the updates
 * at a single position and can tell us the sum over a range of indices in the
 * array
 * Now for every character of the |e| given characters, we need to do a query
 * in just one of those arrays to calculate the answer
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

struct FenwickTree {
    vector<int> bit;
    int n, LOGN;
    void init(int _n) {
    	n = _n;
		LOGN = log2(n) + 1;
		bit.assign(n + 1, 0);
    }
    FenwickTree() {}
    FenwickTree(vector<int> a) {
    	init(sz(a));
	// replace with rangeAdd(i + 1, i + 1, a[i])
	// if you're using range adds and point queries
    	for (int i = 0; i < n; ++i)
            add(i + 1, a[i]);
    }
    int sum(int idx) {
        int ret = 0;
        for (; idx > 0; idx -= idx & -idx)
            ret += bit[idx];
        return ret;
    }
    int sum(int l, int r) {
        return sum(r) - sum(l - 1);
    }
    void add(int idx, int delta) {
        for (; idx <= n; idx += idx & -idx)
            bit[idx] += delta;
    }
};


int n;
vi s[11][10][4], s0;
FenwickTree ft[11][10][4];
map<char, int> M;

vi fix(string s) {
	vi ret;
	for (auto& c : s)
		ret.pb(M[c]);

	return ret;
}

int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	M['A'] = 0;
	M['C'] = 1;
	M['G'] = 2;
	M['T'] = 3;

	string S;

	cin >> S;

	s0 = fix(S);

	n = sz(s0);

	for (int Sz = 1; Sz <= 10; ++Sz)
		for (int i = 0; i < n; ++i) {
			for (int c = 0; c < 4; ++c)
				s[Sz][i % Sz][c].pb(0);
			s[Sz][i % Sz][s0[i]].back() = 1;
		}

	for (int Sz = 1; Sz <= 10; ++Sz)
		for (int r = 0; r < Sz; ++r)
			for (int c = 0; c < 4; ++c)
				ft[Sz][r][c] = FenwickTree(s[Sz][r][c]);


	int q;

	cin >> q;

	for (int qq = 0; qq < q; ++qq) {
		int type;
		cin >> type;

		if (type == 1) {
			int pos;
			char cc;
			cin >> pos >> cc;
			--pos;
			int c = M[cc];

			for (int Sz = 1; Sz <= 10; ++Sz) {
				ft[Sz][pos % Sz][s0[pos]].add(pos / Sz + 1, -1);
				ft[Sz][pos % Sz][c].add(pos / Sz + 1, 1);
			}

			s0[pos] = c;

		} else {
			int l, r;
			vi e;
			cin >> l >> r >> S;
			--l, --r;
			e = fix(S);

			int m = sz(e);

			int ans = 0;

			for (int i = 0; i < m; ++i) {
				int a = l + i;

				if (a > r)
					continue;

				int b = a + ((r - a) / m) * m;

				int ll = a / m, rr = b / m, rem = a % m;

				ans += ft[m][rem][e[i]].sum(ll + 1, rr + 1);
			}

			cout << ans << '\n';
		}
	}

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
