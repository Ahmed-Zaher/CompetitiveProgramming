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

V<ll> x, y, z;

ll sum(ll mid) {
	ll tot = 0;
	for (int k = 0; k < sz(x); ++k) {
		ll xx = x[k], yy = y[k], zz = z[k];
		if (mid < xx)
			continue;
		tot += 1 + min((mid - xx) / zz, (yy - xx) / zz);
	}
	return tot;
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

	V<string> in;

	string s;

	while (getline(cin, s))
		in.pb(s);


	for (int i = 0; i < sz(in); ++i) {
		if (in[i].empty() || !isdigit(in[i][0]))
			continue;
		x.clear();
		y.clear();
		z.clear();
		for (int j = i; j <= sz(in); ++j) {
			if (j == sz(in) || (in[j].empty() || !isdigit(in[j][0]))) {
				ll lo = 0, hi = 1e11;
				while (lo < hi) {
					ll mid = (lo + hi) >> 1;

					ll tot = sum(mid);

					if (tot & 1)
						hi = mid;
					else
						lo = mid + 1;
				}

				if (lo > 1e10)
					cout << "no corruption\n";
				else {
					cout << lo << ' ' << sum(lo) - sum(lo - 1) << '\n';
				}
				i = j;
				break;
			} else {
				stringstream ss(in[j]);
				ll xx, yy, zz;
				ss >> xx >> yy >> zz;
				if (yy >= xx)
					x.pb(xx), y.pb(yy), z.pb(zz);
			}
		}
	}


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
