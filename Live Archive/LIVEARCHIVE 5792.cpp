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

int totLeaves;
vi f(26);
ll ans;
const int MX_ALPHABET = 26;
const char MN_CHAR = 'a';

struct Node {
	int next[MX_ALPHABET];
	bool leaf;
	Node() {
		for (int i = 0; i < MX_ALPHABET; ++i)
			next[i] = -1;
		leaf = false;
	}
};

template<class T>
struct Trie {
	vector<Node> t;
	Trie() {
		t.pb(Node());
	}
	void add(T& s) {
		int v = 0;
		for (auto& ch : s) {
			int c = ch - MN_CHAR;
			if (t[v].next[c] == -1)
				t[v].next[c] = sz(t), t.pb(Node());
			v = t[v].next[c];
		}
		t[v].leaf = 1;
	}
	bool search(T& s) {
		int v = 0;
		for (auto& ch : s) {
			int c = ch - MN_CHAR;
			if (t[v].next[c] == -1)
				return 0;
			v = t[v].next[c];
		}
		return t[v].leaf;
	}
	void dfsR(int u) {

		for (int i = 0; i < MX_ALPHABET; ++i)
			if (t[u].next[i] != -1) {
				int v = t[u].next[i];
				if (u)
					++f[i];
				dfsR(v);
			}
	}
	void dfsL(int u, int par, int dep = 0) {
//		db(path, par);
		if (dep == 1)
			ans += totLeaves;
		else if (dep >= 2)
			ans += totLeaves - f[par];


		for (int i = 0; i < MX_ALPHABET; ++i)
			if (t[u].next[i] != -1)
				dfsL(t[u].next[i], i, dep + 1);
	}
};

int brute(V<string> v) {
	V<string> pref, suff;
	for (auto& s : v) {
		for (int i = 0; i < sz(s); ++i) {
			pref.pb(s.substr(0, i + 1));
			suff.pb(s.substr(i));
		}
	}
	set<string> S;
	for (int i = 0; i < sz(pref); ++i) {
		for (int j = 0; j < sz(suff); ++j) {
			S.insert(pref[i] + suff[j]);
		}
	}
	return sz(S);
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

	int n, m;

	while (cin >> n >> m && (n + m)) {

		Trie<string> trL, trR;

		ans = 0;

		for (int i = 0; i < n; ++i) {
			string s;
			cin >> s;
			trL.add(s);
		}

		for (int j = 0; j < m; ++j) {
			string s;
			cin >> s;
			reverse(s.begin(), s.end());
			trR.add(s);
		}

		totLeaves = sz(trR.t) - 1;

		f.assign(26, 0);

//		db(totLeaves);

		trR.dfsR(0);
		trL.dfsL(0, -1);

		cout << ans << '\n';
	}




	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}
