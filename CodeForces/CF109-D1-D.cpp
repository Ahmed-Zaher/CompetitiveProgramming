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
vi a, pos;
V<bool> vis;
vvi cycles;

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
	a.assign(n + 1, 0);
	pos.assign(n + 1, 0);
	vis.assign(n + 1, 0);

	int luckyIdx = -1;

	set<pii> s;
	map<pii, int> realToComp;

	for (int i = 1; i <= n; ++i) {
		cin >> a[i];
		s.insert(mp(a[i], i));
		int tmp = a[i];
		bool good = true;
		while (tmp) {
			if (tmp % 10 != 4 && tmp % 10 != 7)
				good = false;
			tmp /= 10;
		}
		if (good)
			luckyIdx = i;
	}


	int cur = 1;

	for (auto &elem : s) {
		realToComp[elem] = cur++;
	}

	for (int i = 1; i <= n; ++i) {
		a[i] = realToComp[mp(a[i], i)];
		pos[a[i]] = i;
	}


	if (luckyIdx == -1) {
		bool sorted = 1;
		for (int i = 1; i <= n; ++i) {
			if (a[i] != i)
				sorted = false;
		}
		if (sorted)
			cout << "0\n";
		else
			cout << "-1\n";
		return 0;
	}

	int lucky;
	vii moves;

	auto Swap = [&](int i, int j) {
		moves.pb({i, j});
		swap(a[i], a[j]);
		swap(pos[a[i]], pos[a[j]]);
	};

	for (int i = 1; i <= n; ++i) {
		if (!vis[i]) {
			vi cycle;
			int cur = pos[i];
			cycle.pb(i);
			vis[i] = 1;
			bool luckyCycle = i == luckyIdx;
			while (cur != i) {
				db(i, cur);
				cycle.pb(cur);
				if (cur == luckyIdx)
					luckyCycle = 1;
				vis[cur] = 1;
				cur = pos[cur];
			}
			db(cycle);
			if (!luckyCycle)
				cycles.pb(cycle);
			else {
				lucky = a[luckyIdx];
				int cur = luckyIdx, Next = pos[cur];
				while (pos[lucky] != lucky) {
					int nextNext = pos[Next];
					Swap(cur, Next);
					cur = Next, Next = nextNext;
				}
			}
		}
	}


	for (auto &cycle : cycles) {
		Swap(pos[lucky], cycle[0]);
		int cur = pos[lucky], Next = pos[cur];
		while (pos[lucky] != lucky) {
			int nextNext = pos[Next];
			Swap(cur, Next);
			cur = Next, Next = nextNext;
		}
	}

	assert(sz(moves) <= 2 * n);

	cout << sz(moves) << '\n';
	for (auto &pr : moves) {
		cout << pr.fs << ' ' << pr.sc << '\n';
	}


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
