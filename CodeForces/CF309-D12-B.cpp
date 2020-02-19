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

int n, r, c;
V<string> s;
vi sum;
vvi adj;
vi path;
pii ans = mp(0, 0);

bool good(int i, int j) {
	return sum[j] - sum[i - 1] + (j - i) > c;
}

void dfs(int u, int p) {
	path.pb(u);
	int l = path[max(sz(path) - 1 - r, 0)] - u;

	if (l > ans.fs)
		ans = mp(l, u);

	for (auto& v : adj[u]) {
		if (v != p)
			dfs(v, u);
	}
	path.pop_back();
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

	cin >> n >> r >> c;
	s.assign(n + 2, "");
	sum.assign(n + 2, 0);
	adj.assign(n + 2, vi());

	for (int i = 1; i <= n; ++i) {
		cin >> s[i];
		sum[i] = sum[i - 1] + sz(s[i]);
	}

	sum.back() = OO;

	vi next(n + 2, n + 1);

	for (int i = 1; i <= n; ++i) {
		int lo = i, hi = n + 1;

		while (lo < hi) {
			int mid = (lo + hi) / 2;

			if (good(i, mid))
				hi = mid;
			else
				lo = mid + 1;
		}

		next[i] = lo;
		adj[next[i]].pb(i);
	}

	for (int i = 1; i <= n + 1; ++i) {
		if (next[i] == i)
			dfs(i, i);
	}

	if (!ans.fs)
		ndl;
	else {
		for (int l = ans.sc, r = ans.sc + ans.fs, Sz = sz(s[l]); l < r; ++l) {
			cout << s[l];
			if (l + 1 == r || (l + 1 < r && Sz + 1 + sz(s[l + 1]) > c)) {
				ndl;
				Sz = sz(s[l + 1]);
			} else {
				cout << ' ';
				Sz += 1 + sz(s[l + 1]);
			}
		}
	}


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
