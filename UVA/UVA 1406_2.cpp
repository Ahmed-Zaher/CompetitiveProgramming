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

const int M = 1 << 16;
int N, freq[2 * M], ans[16][M];

#define L(x) (x >> 8)
#define R(x) (x & 255)



int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);




	int tc = 1;

	while (cin >> N && N != -1) {


		vi a(N);

		for (int i = 0; i < N; ++i)
			cin >> a[i];

		for (int b = 0; b < 16; ++b) {

			int filter = (1 << (b + 1)) - 1, len = filter + 1;

			for (int i = 0; i < 2 * len; ++i)
				freq[i] = 0;

			for (auto ai : a)
				++freq[ai & filter];

			for (int i = 0; i < len; ++i)
				freq[i + len] = freq[i];

			for (int i = 1; i < 2 * len; ++i)
				freq[i] += freq[i - 1];

			for (int T = 0; T < len; ++T)
				ans[b][T] = freq[len + len - 1 - T] - freq[len + (len >> 1) - 1 - T];

		}

		ll tot = 0, add = 0;

		char t;

		while (cin >> t && t != 'E') {
			int T;
			cin >> T;
			if (t == 'C') {
				add += T;
				add %= M;
			} else
				tot += ans[T][add & ((1 << (T + 1)) - 1)];
		}

		cout << "Case " << tc++ << ": " << tot << '\n';

	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
