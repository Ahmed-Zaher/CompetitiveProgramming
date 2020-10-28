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
	while (*dbStr != ',') cout << *dbStr++;
	cout << " = " << e << ',';
	_db(dbStr + 1, r...);
}

template<class S, class T>
ostream &operator<<(ostream &o, const map<S, T> &v) {
	o << "[";
	int i = 0;
	for (const pair<S, T> &pr : v)
		o << (!i++ ? "" : ", ") << "{"
		  << pr.fs << " : " << pr.sc << "}";
	return o << "]";
}

template<template<class, class...> class S, class T, class... L>
ostream &operator<<(ostream &o, const S<T, L...> &v) {
	o << "[";
	int i = 0;
	for (const auto &e : v) o << (!i++ ? "" : ", ") << e;
	return o << "]";
}

template<class S, class T>
ostream &operator<<(ostream &o, const pair<S, T> &pr) {
	return o << "(" << pr.fs << ", " << pr.sc << ")";
}

ostream &operator<<(ostream &o, const string &s) {
	for (const char &c : s) o << c;
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

const int MX = 16;
const long long LIM = 1e18 + 5;
int n, p[MX], cnt;
ll num = 1;
V<ll> res[2];
vi nums[2];

void bt(int i, bool b) {
	if (num > LIM)
		return;
	if (i == sz(nums[b])) {
		res[b].pb(num);
	} else {
		bt(i + 1, b);
		if (num <= LIM / nums[b][i]) {
			num *= nums[b][i];
			bt(i, b);
			num /= nums[b][i];
		}
	}
}



int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);



	cin >> n;
	ll prod = 1;
	for (int i = 0; i < n; ++i) {
		cin >> p[i];
	}
	ll k;

	cin >> k;

	sort(p, p + n);

	for (int i = 0; i < n; ++i)
		nums[i & 1].pb(p[i]);

	bt(0, 0);
	bt(0, 1);

	sort(res[0].begin(), res[0].end());
	sort(res[1].begin(), res[1].end());

	db(sz(res[0]), sz(res[1]));
	db(nums[0], nums[1]);
	db(res[0], res[1]);

	ll lo = 1, hi = LIM;

	while (lo < hi) {
		ll mid = (lo + hi) / 2;
		if ([&](ll x) {
			ll ret = 0;
			for (int i = 0; i < sz(res[0]); ++i) {
				if (res[0][i] > x)
					break;
				ret += upper_bound(res[1].begin(), res[1].end(), x / res[0][i]) - res[1].begin();
			}
			return ret >= k;
		}(mid))
			hi = mid;
		else
			lo = mid + 1;

	}


	cout << lo << '\n';

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
