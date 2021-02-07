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

int n;
vi ans;
set<pii> ys;
vii pts;
vvi adj;

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

	set<int> s;


	ans.assign(n + 1, 0);
	pts.assign(n + 1, mp(0, 0));
	adj.assign(2 * n + 1, vi());

	for (int i = 1; i <= n; ++i) {
		int x, y;
		cin >> x >> y;
		pts[i] = mp(x, y);
		s.insert(i);
	}

	if (n == 1) {
		cout << "1\n";
		return 0;
	}

	auto good = [&](ll x, ll y) {
		return x * x + y * y <= 1000000000000LL;
	};

	while (sz(s) >= 3) {
		vi idx;
		auto it = s.begin();
		for (int i = 0; i < 3; ++i, ++it) {
			idx.pb(*it);
		}
		db(idx, s);
		bool done = false;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) if (i != j) {
					ll x = pts[idx[i]].fs + pts[idx[j]].fs;
					ll y = pts[idx[i]].sc + pts[idx[j]].sc;
					pii newPt;
					if (good(x, y)) {
						ans[idx[i]] = ans[idx[j]] = 1;
						done = true;
						newPt = mp(x, y);
					} else {
						x = pts[idx[i]].fs - pts[idx[j]].fs;
						y = pts[idx[i]].sc - pts[idx[j]].sc;
						if (good(x, y)) {
							ans[idx[i]] = 1;
							ans[idx[j]] = -1;
							done = true;
							newPt = mp(x, y);
						}
					}
					if (done) {
						s.erase(idx[i]);
						s.erase(idx[j]);
						s.insert(sz(pts));
						adj[sz(pts)].pb(idx[i]);
						adj[sz(pts)].pb(idx[j]);
						ans.pb(1);
						pts.pb(newPt);
						break;
					}
				}
			if (done)
				break;
		}
		db(s, done, pts);
	}

	int i = *s.begin();
	s.erase(s.begin());
	int j = *s.begin();
	ll x = pts[i].fs + pts[j].fs, y = pts[i].sc + pts[j].sc;
	if (good(x, y))
		ans[i] = ans[j] = 1;
	else {
		x = pts[i].fs - pts[j].fs;
		y = pts[i].sc - pts[j].sc;
		ans[i] = 1, ans[j] = -1;
	}

	ans.pb(0);
	adj[sz(pts)].pb(i);
	adj[sz(pts)].pb(j);

	function<void(int, int, int)> dfs = [&](int u, int p, int sign) {
		ans[u] = sign;
		for (auto &v : adj[u]) if (v != p) {
			dfs(v, u, sign * ans[v]);
		}
	};

	dfs(sz(pts), -1, 1);


	for (int i = 1; i <= n; ++i) {
		cout << ans[i] << ' ';
	}ndl;



#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
