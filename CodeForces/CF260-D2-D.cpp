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

struct UnionFind {
	vi p, rank, setSize;
	int numSets;
	UnionFind(int n) {
		setSize.assign(n, 1);
		numSets = n;
		rank.assign(n, 0);
		p.assign(n, 0);
		for (int i = 0; i < n; ++i)
			p[i] = i;
	}
	int findSet(int i) {
		return (p[i] == i) ? i : (p[i] = findSet(p[i]));
	}
	bool isSameSet(int i, int j) {
		return findSet(i) == findSet(j);
	}
	void unionSet(int i, int j) {
		if (!isSameSet(i, j)) {
			--numSets;
			int x = findSet(i), y = findSet(j);
			if (rank[x] > rank[y]) {
				p[y] = x;
				setSize[x] += setSize[y];
			} else {
				p[x] = y;
				setSize[y] += setSize[x];
				if (rank[x] == rank[y])
					++rank[y];
			}
		}
	}
	int numDisjointSets() {
		return numSets;
	}
	int sizeOfSet(int i) {
		return setSize[findSet(i)];
	}
};

int main() {
#ifdef LOCAL
	auto stTime = clock();
	freopen("../in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	int n;
	cin >> n;
	vi s(n + 1), c(n + 1);

	set<pii> L, R;
	for (int i = 1; i <= n; ++i) {
		cin >> c[i] >> s[i];
		if (!c[i] && s[i])
			L.insert(mp(s[i], i));
		if (c[i] && s[i])
			R.insert(mp(s[i], i));
	}

	V<pair<int, pii>> edges;

	if (L.empty() && R.empty()) {
		for (int i = 2; i <= n; ++i) if (c[i] != c[1]) {
				edges.pb(mp(0, mp(1, i)));
			}
		int u = edges[0].sc.sc;
		db(edges);
		db(u, c[1], c[u]);
		for (int i = 2; i <= n; ++i) if (c[i] != c[u]) {
				edges.pb(mp(0, mp(u, i)));
			}
		for (auto &e : edges) {
			cout << e.sc.fs << ' ' << e.sc.sc << ' ' << e.fs << '\n';
		}
		return 0;
	}

	UnionFind UF(n + 1);

	while (!L.empty()) {
		assert(!R.empty());
		pii pL = *L.begin(), pR = *R.begin();
		if (pL.fs < pR.fs) {
			L.erase(pL);
			R.erase(pR);
			edges.pb(mp(pL.fs, mp(pL.sc, pR.sc)));
			assert(!UF.isSameSet(pL.sc, pR.sc));
			UF.unionSet(pL.sc, pR.sc);
			pR.fs -= pL.fs;
			s[pR.sc] = pR.fs;
			if (pR.fs)
				R.insert(pR);
			s[pL.sc] = 0;
		} else {
			R.erase(pR);
			L.erase(pL);
			edges.pb(mp(pR.fs, mp(pL.sc, pR.sc)));
			assert(!UF.isSameSet(pL.sc, pR.sc));
			UF.unionSet(pL.sc, pR.sc);
			pL.fs -= pR.fs;
			s[pL.sc] = pL.fs;
			if (pL.fs)
				L.insert(pL);
			s[pR.sc] = 0;
		}
	}

	int black = -1, white = -1;

	for (int i = 1; i <= n; ++i) {
		if (!c[i] && UF.sizeOfSet(i) > 1) {
			black = i;
			for (int j = 1; j <= n; ++j) {
				if (UF.isSameSet(i, j) && c[j]) {
					white = j;
				}
			}
			break;
		}
	}

	for (int i = 1; i <= n; ++i) {
		if (!UF.isSameSet(i, black) && c[i]) {
			UF.unionSet(i, black);
			edges.pb(mp(0, mp(i, black)));
		}
	}
	for (int i = 1; i <= n; ++i) {
		if (!UF.isSameSet(i, white) && !c[i]) {
			UF.unionSet(i, white);
			edges.pb(mp(0, mp(i, white)));
		}
	}

//	assert(sz(edges) == n - 1);

	for (auto &e : edges) {
		cout << e.sc.fs << ' ' << e.sc.sc << ' ' << e.fs << '\n';
	}

#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
	     (clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	return 0;
}
